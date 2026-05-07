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

"""LLM item classes."""

from dataclasses import dataclass, field
from typing import Any, Literal

from ikomia.dataprocess.io.llm.base import LlmBaseItem
from ikomia.dataprocess.io.llm.content import (
    LlmInputImageContent,
    LlmInputTextContent,
    LlmOutputTextContent,
    LlmReasoningTextContent,
    LlmSummaryTextContent,
)

LlmMessageRole = Literal["system", "user", "assistant"]


@dataclass(kw_only=True)
class LlmBaseMessage(LlmBaseItem):
    """Base conversation message item."""

    type: Literal["message"] = "message"
    role: LlmMessageRole
    content: str | list[Any]

    def __post_init__(self):
        self.content = self._normalize_content(self.content, self.role)

    @classmethod
    def from_dict(cls, data: dict[str, Any]):
        clean = dict(data)
        if "content" in clean:
            clean["content"] = cls._normalize_content(
                clean["content"],
                clean.get("role", getattr(cls, "role", None)),
            )
        return super().from_dict(clean)

    @staticmethod
    def _normalize_content(content: Any, role: LlmMessageRole | None) -> Any:
        # Make content always a list of content items
        if not isinstance(content, str):
            return content

        content_cls = (
            LlmOutputTextContent if role == "assistant" else LlmInputTextContent
        )
        return [content_cls(text=content)]


@dataclass(kw_only=True)
class LlmSystemMessage(LlmBaseMessage):
    """System message item."""

    role: Literal["system"] = "system"
    content: str | list[LlmInputTextContent]


@dataclass(kw_only=True)
class LlmUserMessage(LlmBaseMessage):
    """User message item."""

    role: Literal["user"] = "user"
    content: str | list[LlmInputTextContent | LlmInputImageContent]


@dataclass(kw_only=True)
class LlmAssistantMessage(LlmBaseMessage):
    """Assistant message item."""

    role: Literal["assistant"] = "assistant"
    content: str | list[LlmOutputTextContent]


@dataclass(kw_only=True)
class LlmFunctionCall(LlmBaseItem):
    """Function call item."""

    type: Literal["function_call"] = "function_call"
    call_id: str
    name: str
    arguments: str


@dataclass(kw_only=True)
class LlmFunctionCallOutput(LlmBaseItem):
    """Function call output item."""

    type: Literal["function_call_output"] = "function_call_output"
    call_id: str
    output: list[LlmInputTextContent | LlmInputImageContent]


@dataclass(kw_only=True)
class LlmReasoningItem(LlmBaseItem):
    """Reasoning item."""

    type: Literal["reasoning"] = "reasoning"
    content: list[LlmReasoningTextContent] | None = None
    summary: list[LlmSummaryTextContent] = field(default_factory=list)
    encrypted_content: str | None = None
