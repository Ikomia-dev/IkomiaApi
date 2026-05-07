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

"""LLM workflow input/output package."""

from ikomia.dataprocess.io.llm.base import LlmBaseEvent, LlmBaseItem, LlmObject
from ikomia.dataprocess.io.llm.content import (
    LlmInputImageContent,
    LlmInputTextContent,
    LlmOutputTextContent,
    LlmReasoningTextContent,
    LlmSummaryTextContent,
)
from ikomia.dataprocess.io.llm.items import (
    LlmAssistantMessage,
    LlmBaseMessage,
    LlmFunctionCall,
    LlmFunctionCallOutput,
    LlmReasoningItem,
    LlmSystemMessage,
    LlmUserMessage,
)
from ikomia.dataprocess.io.llm.llmcontextIO import LlmContextIO
from ikomia.dataprocess.io.llm.llmstreamingresponseIO import LlmStreamingResponseIO
from ikomia.dataprocess.io.llm.stream import (
    LlmFunctionCallArgumentsDeltaEvent,
    LlmOutputItemAddedEvent,
    LlmOutputTextDeltaEvent,
    LlmReasoningSummaryPartAddedEvent,
    LlmReasoningSummaryTextDeltaEvent,
    LlmReasoningTextDeltaEvent,
)

__all__ = [
    "LlmBaseItem",
    "LlmObject",
    "LlmBaseEvent",
    "LlmInputImageContent",
    "LlmInputTextContent",
    "LlmOutputTextContent",
    "LlmReasoningTextContent",
    "LlmSummaryTextContent",
    "LlmBaseMessage",
    "LlmSystemMessage",
    "LlmUserMessage",
    "LlmAssistantMessage",
    "LlmFunctionCall",
    "LlmFunctionCallOutput",
    "LlmReasoningItem",
    "LlmContextIO",
    "LlmStreamingResponseIO",
    "LlmOutputItemAddedEvent",
    "LlmOutputTextDeltaEvent",
    "LlmReasoningTextDeltaEvent",
    "LlmReasoningSummaryPartAddedEvent",
    "LlmReasoningSummaryTextDeltaEvent",
    "LlmFunctionCallArgumentsDeltaEvent",
]
