# Copyright (C) 2021 Ikomia SAS
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""
The module `utils.llm` provides utilities for LLM-based workflows.
"""

import inspect
import json
import re
from base64 import b64encode
from collections.abc import Generator
from contextlib import AbstractContextManager
from io import BytesIO
from types import UnionType
from typing import Any, Callable, Literal, Union, get_args, get_origin

from PIL import Image

from ikomia.core.pycore import CWorkflowTaskIO
from ikomia.dataprocess import CImageIO
from ikomia.dataprocess.io.llm import (
    LlmContextIO,
    LlmFunctionCallOutput,
    LlmInputImageContent,
    LlmInputTextContent,
)
from ikomia.dataprocess.workflow import Workflow


class LlmToolkit:
    """
    Set of functions a LLM can use as tools.
    """

    def __init__(self):
        self.functions: dict[str, Callable] = {}

    @staticmethod
    def _annotation_to_schema(annotation: Any) -> dict:
        """
        Convert a Python type annotation to a JSON schema.
        Only supports compositions of core Python types.

        Args:
            annotation (Any): The type annotation to convert.

        Returns:
            dict: The JSON schema.
        """
        TYPE_MAP = {
            str: "string",
            int: "integer",
            float: "number",
            bool: "boolean",
            list: "array",
            tuple: "array",
            set: "array",
            dict: "object",
        }

        if annotation is inspect._empty or annotation is Any:
            return {}

        if annotation is type(None):
            return {"type": "null"}

        origin = get_origin(annotation)

        if origin in (list, tuple, set):
            args = get_args(annotation)
            schema = {"type": "array"}

            # Define item type if provided
            if args:
                schema["items"] = LlmToolkit._annotation_to_schema(args[0])

            return schema

        if origin is dict:
            args = get_args(annotation)
            schema = {"type": "object"}

            # Define key type if provided
            if len(args) == 2:
                schema["additionalProperties"] = LlmToolkit._annotation_to_schema(
                    args[1]
                )

            return schema

        if origin in (Union, UnionType):
            return {
                "anyOf": [
                    LlmToolkit._annotation_to_schema(arg) or {}
                    for arg in get_args(annotation)
                ]
            }

        json_type = TYPE_MAP.get(annotation)
        return {"type": json_type} if json_type else {}

    @staticmethod
    def _get_function_name_and_description(fn: Callable) -> tuple[str, str]:
        """
        Retrieve a function name and its top-level docstring description.

        Args:
            fn (Callable): The function to inspect.

        Returns:
            tuple[str, str]: The function name and description.
        """
        doc = inspect.getdoc(fn) or ""
        description_lines = []
        section_headers = {
            "Args:",
            "Arguments:",
            "Parameters:",
            "Returns:",
            "Raises:",
            "Yields:",
            "Examples:",
        }

        for line in doc.splitlines():
            stripped = line.strip()
            if stripped in section_headers or stripped.startswith(":param "):
                break
            description_lines.append(stripped)

        return fn.__name__, "\n".join(
            line for line in description_lines if line
        ).strip()

    @staticmethod
    def _get_parameters_schema(fn: Callable) -> tuple[dict[str, dict], list[str]]:
        """
        Retrieve parameter schemas and required fields from a function signature and docstring.

        Args:
            fn (Callable): The function to inspect.

        Returns:
            tuple[dict[str, dict], list[str]]: Schema properties and required parameter names.
        """
        doc = inspect.getdoc(fn) or ""
        signature = inspect.signature(fn)

        param_descriptions = {}
        properties = {}
        required = []

        # Line-by-line parsing of the docstring
        current_param = None
        in_params_section = False

        for line in doc.splitlines():
            stripped = line.strip()

            # Support reST-style docstring format ":param name: description"
            # These are not in a specific section, so we parse them directly.
            if stripped.startswith(":param "):
                match = re.match(r":param\s+(\w+)\s*:\s*(.*)", stripped)
                if match:
                    current_param = match.group(1)
                    param_descriptions[current_param] = match.group(2).strip()
                continue

            # Start of Google-style parameters section
            # Args:
            #     name: description
            if stripped in {"Args:", "Arguments:", "Parameters:"}:
                in_params_section = True
                current_param = None
                continue

            if not in_params_section:
                continue

            # Exit Google-style parameters section
            if stripped in {"Returns:", "Raises:", "Yields:", "Examples:"}:
                in_params_section = False
                current_param = None
                continue

            # Match Google-style docstring format "name (type): description" or "name: description"
            match = re.match(
                r"\s*(\w+)(?:\s*\([^)]*\))?\s*:\s*(.*)", line
            )  # match string like "name: description"
            if match:
                current_param = match.group(1)
                param_descriptions[current_param] = match.group(2).strip()
                continue

            # Concatenate multi-line descriptions
            if current_param and stripped:
                param_descriptions[current_param] = (
                    f"{param_descriptions[current_param]} {stripped}".strip()
                )

        # Convert function signature to JSON schema properties and required fields
        for name, param in signature.parameters.items():
            if param.kind not in (
                inspect.Parameter.POSITIONAL_OR_KEYWORD,
                inspect.Parameter.KEYWORD_ONLY,
            ):
                continue

            schema = LlmToolkit._annotation_to_schema(param.annotation)
            if name in param_descriptions:
                schema["description"] = param_descriptions[name]
            if param.default is not inspect._empty:
                schema["default"] = param.default
            else:
                required.append(name)

            properties[name] = schema

        return properties, required

    @staticmethod
    def _get_tool_schema(
        fn: Callable,
        format: Literal[
            "responses",
            "openai_chat_completions",
            "anthropic_messages",
        ],
    ) -> dict:
        """
        Convert a registered function to a tool schema for the target LLM API.

        Args:
            fn (Callable): The function to inspect.
            format (Literal["responses", "openai_chat_completions", "anthropic_messages"]):
                The target API format.

        Returns:
            dict: The formatted tool schema.
        """
        fn_name, description = LlmToolkit._get_function_name_and_description(fn)
        properties, required = LlmToolkit._get_parameters_schema(fn)
        parameters = {
            "type": "object",
            "properties": properties,
            "required": required,
        }

        match format:
            case "responses":
                return {
                    "type": "function",
                    "name": fn_name,
                    "description": description,
                    "parameters": parameters,
                }
            case "openai_chat_completions":
                return {
                    "type": "function",
                    "function": {
                        "name": fn_name,
                        "description": description,
                        "parameters": parameters,
                    },
                }
            case "anthropic_messages":
                return {
                    "name": fn_name,
                    "description": description,
                    "input_schema": parameters,
                }

    @staticmethod
    def _image_to_content(image: Image.Image) -> LlmInputImageContent:
        """
        Convert an image to a content item.
        """
        if image.mode not in {"RGB", "RGBA"}:
            image = image.convert("RGBA" if "A" in image.getbands() else "RGB")
        buffer = BytesIO()
        image.save(buffer, format="PNG")
        return LlmInputImageContent(
            image_url="data:image/png;base64,"
            + b64encode(buffer.getvalue()).decode("ascii")
        )

    @staticmethod
    def _default_format_output(
        output: Any,
    ) -> list[LlmInputTextContent | LlmInputImageContent]:
        """
        Default formatter for tool output.

        Accepts strings, LlmInputTextContent, LlmInputImageContent, CImageIO, CWorkflowTaskIO, Image.Image
        Other types will be JSON-serialized.
        Also support list and tuple of those items.

        Args:
            output (Any): The output to format.

        Returns:
            list[LlmInputTextContent | LlmInputImageContent]: The formatted output.
        """
        # Return string as is
        if isinstance(output, str):
            return [LlmInputTextContent(text=output)]

        is_output_list_like = isinstance(output, (list, tuple))
        has_special_items = is_output_list_like and any(
            isinstance(
                item,
                (
                    LlmInputTextContent,
                    LlmInputImageContent,
                    CImageIO,
                    CWorkflowTaskIO,
                    Image.Image,
                ),
            )
            for item in output
        )

        items = []
        content = []

        # List of non-special items are directly JSON-serialized
        if is_output_list_like:
            if not has_special_items:
                return [
                    LlmInputTextContent(
                        text=json.dumps(
                            output, ensure_ascii=False, indent=2, default=repr
                        )
                    )
                ]
            # Otherwise, each item is serialized one-by-one
            else:
                items.extend(output)
        else:
            items.append(output)

        for item in items:
            match item:
                # Append string as is
                case _ if isinstance(item, str):
                    content.append(LlmInputTextContent(text=item))
                    continue
                # Append input content items directly
                case LlmInputTextContent() | LlmInputImageContent():
                    content.append(item)
                # Send images to vision-enabled LLMs
                case CImageIO():
                    image = Image.fromarray(item.get_image())
                    content.append(LlmToolkit._image_to_content(image))
                case Image.Image():
                    content.append(LlmToolkit._image_to_content(item))
                # Other task I/O are JSON-serialized
                case CWorkflowTaskIO():
                    content.append(
                        LlmInputTextContent(
                            text=item.to_json(options=["json_format", "indented"])
                        )
                    )
                # Anything else is JSON-serialized
                case _:
                    content.append(
                        LlmInputTextContent(
                            text=json.dumps(
                                item, ensure_ascii=False, indent=2, default=repr
                            )
                        )
                    )

        return content

    @staticmethod
    def _default_format_error(error: Exception) -> list[LlmInputTextContent]:
        """
        Default formatter for tool error.
        """
        return [LlmInputTextContent(text=str(error))]

    def tool(self, fn: Callable):
        """
        Decorator to register a function as a tool.
        """
        self.functions[fn.__name__] = fn
        return fn

    def get_schemas(
        self,
        format: Literal[
            "responses",
            "openai_chat_completions",
            "anthropic_messages",
        ] = "responses",
    ) -> list[dict]:
        """
        Get the tool schemas for the registered functions.

        Args:
            format (Literal["responses", "openai_chat_completions", "anthropic_messages"]):
                The target API format (default: "responses").

        Returns:
            list[dict]: The tool schemas.
        """
        return [self._get_tool_schema(fn, format) for fn in self.functions.values()]

    def get_schemas_json(
        self,
        format: Literal[
            "responses",
            "openai_chat_completions",
            "anthropic_messages",
        ] = "responses",
    ) -> str:
        """
        Get the tool schemas for the registered functions as a JSON string.

        Args:
            format (Literal["responses", "openai_chat_completions", "anthropic_messages"]):
                The target API format (default: "responses").

        Returns:
            str: The tool schemas as a JSON string.
        """
        return json.dumps(
            [self._get_tool_schema(fn, format) for fn in self.functions.values()],
            ensure_ascii=False,
            indent=2,
        )

    def execute_tools(
        self,
        context: LlmContextIO,
        format_output: Callable[[Any], list[LlmInputTextContent | LlmInputImageContent]]
        | None = None,
        format_error: Callable[
            [Exception], list[LlmInputTextContent | LlmInputImageContent]
        ]
        | None = None,
    ) -> LlmContextIO:
        """
        Execute the tools for the pending function calls.

        Args:
            context (LlmContextIO): The context to execute the tools in.
            format_output (Callable[[Any], list[LlmInputTextContent | LlmInputImageContent]] | None):
                Custom formatter for tool output (optional).
            format_error (Callable[[Exception], list[LlmInputTextContent | LlmInputImageContent]] | None):
                Custom formatter in case of tool error (optional).

        Returns:
            LlmContextIO: The context with the executed tools.
        """
        output_formatter = format_output or self._default_format_output
        error_formatter = format_error or self._default_format_error

        pending_function_calls = context.get_pending_function_calls()
        for function_call in pending_function_calls:
            function = self.functions.get(function_call.name)
            if not function:
                continue

            try:
                output = function(**json.loads(function_call.arguments))
                context.add_item(
                    LlmFunctionCallOutput(
                        call_id=function_call.call_id,
                        output=output_formatter(output),
                    )
                )
            except Exception as error:
                context.add_item(
                    LlmFunctionCallOutput(
                        call_id=function_call.call_id,
                        output=error_formatter(error),
                    )
                )

        return context


class LlmAgentLoop:
    """
    Iterable session for agent workflow execution.
    """

    def __init__(
        self,
        workflow: Workflow,
        toolkit: LlmToolkit | None = None,
        context_input_index: int = 0,
        context_output_index: int | None = None,
        context_output_task_id: int | None = None,
        context_output_task_index: int = 0,
    ):
        self.workflow = workflow
        self.toolkit = toolkit
        self.context_input_index = context_input_index
        self.context_output_index = context_output_index
        self.context_output_task_id = context_output_task_id
        self.context_output_task_index = context_output_task_index

        self._input_context: LlmContextIO = workflow.get_input(context_input_index)
        self._final_context: LlmContextIO | None = None
        self._thread_manager: AbstractContextManager | None = None
        self._turn_iterator: (
            Generator[tuple[CWorkflowTaskIO, ...], None, None] | None
        ) = None

    def _start_workflow_thread(self):
        self._thread_manager = self.workflow.run_in_thread()
        self._thread_manager.__enter__()

    def wait_workflow_end(self):
        """
        Block until the current workflow execution ends.
        """
        if self._thread_manager is None:
            return

        thread_manager = self._thread_manager
        self._thread_manager = None
        thread_manager.__exit__(None, None, None)

    @staticmethod
    def _copy_context(context: LlmContextIO) -> LlmContextIO:
        snapshot = LlmContextIO()
        snapshot.reset(context)
        return snapshot

    def _get_turn_context(self) -> LlmContextIO:
        match self.context_output_index, self.context_output_task_id:
            # No output index and no task id (default): use the first output
            case None, None:
                return self.workflow.get_output(0)
            # No output index, but task id: use the output of the task (default to 0)
            case None, task_id:
                return self.workflow.get_task(task_id).get_output(
                    self.context_output_task_index
                )
            # Output index: use the output at the given index
            case index, _:
                return self.workflow.get_output(index)

    def _get_turn_outputs(self) -> tuple[CWorkflowTaskIO, ...]:
        if self.context_output_task_id is not None:
            return tuple(
                self.workflow.get_task(self.context_output_task_id).get_outputs()
            )

        return tuple(self.workflow.get_outputs())

    def _start_turn(self) -> Generator[tuple[CWorkflowTaskIO, ...], None, None]:
        has_tools = self.toolkit is not None
        is_agent_turn = True

        while is_agent_turn:
            try:
                self._start_workflow_thread()
                yield self._get_turn_outputs()
            finally:
                self.wait_workflow_end()

            turn_context = self._get_turn_context()
            has_generated_response_items = len(turn_context.get_response_items()) > 0
            has_pending_tool_calls = len(turn_context.get_pending_function_calls()) > 0

            is_agent_turn = not has_generated_response_items or (
                has_tools and has_pending_tool_calls
            )

            if has_tools and has_pending_tool_calls:
                turn_context = self.toolkit.execute_tools(turn_context)

            self._input_context.reset(turn_context)

        self._final_context = self._copy_context(self._input_context)

    def turn(self):
        """
        Start a new agent turn.
        """
        self._input_context = self.workflow.get_input(self.context_input_index)
        self._final_context = None
        self._turn_iterator = self._start_turn()

        yield from self._turn_iterator

    def get_final_context(self) -> LlmContextIO:
        """
        Retrieve final context after the last turn.
        """
        if self._turn_iterator is None:
            raise ValueError("No turn iterator started")

        for _ in self._turn_iterator:
            pass

        if self._final_context is None:
            self._final_context = self._copy_context(self._input_context)

        return self._copy_context(self._final_context)
