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

"""LLM item content classes."""

from dataclasses import dataclass
from typing import Literal

from ikomia.dataprocess.io.llm.base import LlmObject


@dataclass(kw_only=True)
class LlmInputTextContent(LlmObject):
    """Text content part."""

    type: Literal["input_text"] = "input_text"
    text: str


@dataclass(kw_only=True)
class LlmInputImageContent(LlmObject):
    """Image content part."""

    type: Literal["input_image"] = "input_image"
    image_url: str


@dataclass(kw_only=True)
class LlmOutputTextContent(LlmObject):
    """Text content part for assistant messages."""

    type: Literal["output_text"] = "output_text"
    text: str


@dataclass(kw_only=True)
class LlmSummaryTextContent(LlmObject):
    """Summary text content part for reasoning items."""

    type: Literal["summary_text"] = "summary_text"
    text: str


@dataclass(kw_only=True)
class LlmReasoningTextContent(LlmObject):
    """Reasoning text content part for reasoning items."""

    type: Literal["reasoning_text"] = "reasoning_text"
    text: str
