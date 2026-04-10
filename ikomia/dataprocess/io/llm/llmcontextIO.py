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

"""Module providing workflow I/O for LLM conversation contexts."""

import copy
import json
from typing import Any, Iterable, Type

from ikomia.core import CWorkflowTaskIO, IODataType
from ikomia.dataprocess.io.llm.base import LlmBaseItem
from ikomia.dataprocess.io.llm.items import (
    LlmAssistantMessage,
    LlmBaseMessage,
    LlmFunctionCall,
    LlmFunctionCallOutput,
    LlmMessageRole,
    LlmReasoningItem,
    LlmSystemMessage,
    LlmUserMessage,
)


class LlmContextIO(CWorkflowTaskIO):
    """Workflow input/output object for LLM conversations."""

    def __init__(self):
        """Initialize I/O with empty list of items."""
        # TODO: change to IODataType.LLM_CONTEXT or something like that
        CWorkflowTaskIO.__init__(
            self, IODataType.DATA_DICT
        )
        self._raw_items: list[dict[str, Any]] = []
        self._response_start_index = 0

    def clear_data(self):
        """Clear whole list of items."""
        self._raw_items = []
        self._response_start_index = 0

    def is_data_available(self) -> bool:
        """
        Check whether the list of items is not empty.

        Returns:
            bool: True if the list of items is not empty, False otherwise.
        """
        return len(self._raw_items) > 0

    def reset(self, context = None):
        """
        Reset the context to the given context.

        Args:
            context (LlmContextIO): The context to reset to.
        """
        if context is not None:
            self._raw_items = copy.deepcopy(context._raw_items)
            self._response_start_index = context._response_start_index
        else:
            self.clear_data()

    def start_response(self):
        """
        Mark next added items as part of the response.
        """
        self._response_start_index = len(self._raw_items)

    def add_item(self, item: LlmBaseItem):
        """
        Add an item to the context.

        Args:
            item (LlmBaseItem): The item to add.
        """
        self._raw_items.append(item.to_dict())

    def add_message(self, role: LlmMessageRole, text: str) -> LlmBaseMessage:
        """
        Create and append a text-only message item.

        Args:
            role (LlmMessageRole): The role of the message.
            text (str): The text of the message.

        Returns:
            LlmBaseMessage: The added message item.
        """
        match role:
            case "system":
                item = LlmSystemMessage(content=text)
            case "user":
                item = LlmUserMessage(content=text)
            case "assistant":
                item = LlmAssistantMessage(content=text)
            case _:
                raise ValueError(f"Unsupported message role: {role}")

        self.add_item(item)
        return item

    def _get_classes(self, custom_item_classes: Iterable[Type[LlmBaseItem]] | None = None) -> Iterable[Type[LlmBaseItem]]:
        classes = (
            LlmSystemMessage,
            LlmUserMessage,
            LlmAssistantMessage,
            LlmFunctionCall,
            LlmFunctionCallOutput,
            LlmReasoningItem,
        )
        if custom_item_classes:
            classes = (*custom_item_classes, *classes)
        return classes

    def get_items(
        self,
        *,
        custom_item_classes: Iterable[Type[LlmBaseItem]] | None = None,
        include_unknown_items: bool = True,
    ) -> list[LlmBaseItem]:
        """
        Get items from the context.

        Args:
            custom_item_classes (Iterable[Type[LlmBaseItem]] | None): Additional item classes to attempt resolution against.
            include_unknown_items (bool): If False, unresolved base items are excluded.

        Returns:
            list[LlmBaseItem]: The list of items.
        """
        classes = self._get_classes(custom_item_classes)

        items: list[LlmBaseItem] = []
        for raw in self._raw_items:
            matched = next((cls for cls in classes if cls.matches_raw(raw)), None)
            if matched is not None:
                items.append(matched.from_dict(raw))
            elif include_unknown_items:
                items.append(LlmBaseItem.from_dict(raw))

        return items

    def get_response_items(self, *, custom_item_classes: Iterable[Type[LlmBaseItem]] | None = None, include_unknown_items: bool = True) -> list[LlmBaseItem]:
        """
        Get response items from the context.

        Args:
            custom_item_classes (Iterable[Type[LlmBaseItem]] | None): Additional item classes to attempt resolution against.
            include_unknown_items (bool): If False, unresolved base items are excluded.
        """
        classes = self._get_classes(custom_item_classes)

        items: list[LlmBaseItem] = []
        for raw in self._raw_items[self._response_start_index:]:
            matched = next((cls for cls in classes if cls.matches_raw(raw)), None)
            if matched is not None:
                items.append(matched.from_dict(raw))
            elif include_unknown_items:
                items.append(LlmBaseItem.from_dict(raw))

        return items

    def get_pending_function_calls(self) -> list[LlmFunctionCall]:
        """
        Get all function calls that have not yet an associated function call output.

        Returns:
            list[LlmFunctionCall]: The list of pending function calls.
        """
        resolved_call_ids = set[str]()
        pending_function_calls: list[LlmFunctionCall] = []

        for item in reversed(self.get_items()):
            match item:
                case LlmFunctionCallOutput(call_id=call_id):
                    resolved_call_ids.add(call_id)

                case LlmFunctionCall(call_id=call_id):
                    if call_id not in resolved_call_ids:
                        pending_function_calls.append(item)

        return pending_function_calls

    def to_json(self, options: list = None) -> str:
        """
        Convert context items to a JSON string.

        Args:
            options: JSON format options ['json_format', 'compact'] or ['json_format', 'indented']
        """
        indent = None
        if options and "json_format" in options:
            if options[options.index("json_format") + 1] == "indented":
                indent = 4
        return json.dumps(self._raw_items, indent=indent)

    def from_json(self, json_string: str):
        """
        Set internal items from a JSON string.

        Args:
            json_string (str): The JSON string to set the internal items from.
        """
        self._raw_items = json.loads(json_string)

    def save(self, path: str):
        """Save context items as a JSON file."""
        with open(path, "w", encoding="utf-8") as fh:
            fh.write(self.to_json())

    def load(self, path: str):
        """Load context items from a JSON file."""
        with open(path, "r", encoding="utf-8") as fh:
            self.from_json(fh.read())
