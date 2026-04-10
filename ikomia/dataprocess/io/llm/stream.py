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

"""LLM streaming event classes."""

from dataclasses import dataclass
from typing import Literal

from ikomia.dataprocess.io.llm.base import LlmBaseEvent, LlmBaseItem
from ikomia.dataprocess.io.llm.content import LlmSummaryTextContent


@dataclass(kw_only=True)
class LlmOutputItemAddedEvent(LlmBaseEvent):
    """
    Emitted when a new output item is added to the response.

    ``item`` is the (potentially partial) output item echoed by the API.
    """

    type: Literal["response.output_item.added"] = "response.output_item.added"
    item: LlmBaseItem


@dataclass(kw_only=True)
class LlmOutputTextDeltaEvent(LlmBaseEvent):
    """Emitted when a text delta is appended to an output message."""

    type: Literal["response.output_text.delta"] = "response.output_text.delta"
    item_id: str
    content_index: int
    delta: str


@dataclass(kw_only=True)
class LlmReasoningTextDeltaEvent(LlmBaseEvent):
    """Emitted when a reasoning text delta is appended."""

    type: Literal["response.reasoning_text.delta"] = "response.reasoning_text.delta"
    item_id: str
    content_index: int
    delta: str


@dataclass(kw_only=True)
class LlmReasoningSummaryPartAddedEvent(LlmBaseEvent):
    """Emitted when a new reasoning summary part is added."""

    type: Literal["response.reasoning_summary_part.added"] = (
        "response.reasoning_summary_part.added"
    )
    item_id: str
    summary_index: int
    part: LlmSummaryTextContent


@dataclass(kw_only=True)
class LlmReasoningSummaryTextDeltaEvent(LlmBaseEvent):
    """Emitted when a reasoning summary text delta is appended."""

    type: Literal["response.reasoning_summary_text.delta"] = (
        "response.reasoning_summary_text.delta"
    )
    item_id: str
    summary_index: int
    delta: str


@dataclass(kw_only=True)
class LlmFunctionCallArgumentsDeltaEvent(LlmBaseEvent):
    """Emitted when a function-call arguments delta is appended."""

    type: Literal["response.function_call_arguments.delta"] = (
        "response.function_call_arguments.delta"
    )
    item_id: str
    delta: str
