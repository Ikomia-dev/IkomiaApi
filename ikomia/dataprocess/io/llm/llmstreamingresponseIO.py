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

"""Workflow I/O for LLM streaming responses."""

import json
from typing import Generator, Iterable, Type

from ikomia.dataprocess.io.llm.base import LlmBaseEvent
from ikomia.dataprocess.io.llm.stream import (
    LlmFunctionCallArgumentsDeltaEvent,
    LlmOutputItemAddedEvent,
    LlmOutputTextDeltaEvent,
    LlmReasoningSummaryPartAddedEvent,
    LlmReasoningSummaryTextDeltaEvent,
    LlmReasoningTextDeltaEvent,
)
from ikomia.dataprocess.io.textstreamIO import TextStreamIO


class LlmStreamingResponseIO(TextStreamIO):
    """
    Text-stream I/O for LLM streaming responses.

    Transports :class:`LlmBaseEvent` objects serialized as newline-delimited JSON (NDJSON).
    """

    def __init__(self):
        super().__init__()
        # TODO: change to IODataType.LLM_STREAMING_RESPONSE or something like that
        self._auto_sequence_number = 0
        self._next_item_idx = 0

    def clear_data(self):
        """Clear all internal data."""
        super().clear_data()
        self._auto_sequence_number = 0
        self._next_item_idx = 0

    def allocate_item(self) -> int:
        """
        Allocate a new output index.

        Returns:
            The allocated output index.
        """
        idx = self._next_item_idx
        self._next_item_idx += 1
        return idx

    def feed_event(self, event: LlmBaseEvent):
        """
        Feed a streaming event into the stream.

        Args:
            event (LlmBaseEvent): The event to feed into the stream.
        """
        if event.sequence_number is None:
            event.sequence_number = self._auto_sequence_number
            self._auto_sequence_number += 1
        else:
            self._auto_sequence_number = max(
                self._auto_sequence_number, event.sequence_number + 1
            )

        # Each event must stay on a single line so the stream remains valid NDJSON.
        self.feed(json.dumps(event.to_dict(), separators=(",", ":")) + "\n")

    def stream_events(
        self,
        *,
        timeout: int = 60,
        custom_event_classes: Iterable[Type[LlmBaseEvent]] | None = None,
        include_unknown_events: bool = False,
    ) -> Generator[LlmBaseEvent, None, None]:
        """
        Generator that yields streaming events as they arrive.

        Args:
            timeout (int): Read timeout in seconds.
            custom_event_classes (Iterable[Type[LlmBaseEvent]] | None): Additional event classes to attempt resolution against.
            include_unknown_events (bool): If False, unresolved base are excluded.

        Yields:
            LlmBaseEvent: The streaming event.
        """
        classes = (
            LlmOutputItemAddedEvent,
            LlmOutputTextDeltaEvent,
            LlmReasoningTextDeltaEvent,
            LlmReasoningSummaryPartAddedEvent,
            LlmReasoningSummaryTextDeltaEvent,
            LlmFunctionCallArgumentsDeltaEvent,
        )
        if custom_event_classes:
            classes = (*custom_event_classes, *classes)

        buffer = ""
        for chunk in self.stream(timeout=timeout):
            buffer += chunk
            while "\n" in buffer:
                line, buffer = buffer.split("\n", 1)
                line = line.strip()
                if line:
                    raw = json.loads(line)

                    matched = next(
                        (cls for cls in classes if cls.matches_raw(raw)), None
                    )
                    if matched is not None:
                        yield matched.from_dict(raw)
                    elif include_unknown_events:
                        yield LlmBaseEvent.from_dict(raw)

    def stream_text(self, *, timeout: int = 60) -> Generator[str, None, None]:
        """
        Generator that yields output text deltas as they arrive.

        Args:
            timeout (int): Read timeout in seconds.

        Yields:
            str: The output text delta.
        """
        for event in self.stream_events(timeout=timeout):
            if isinstance(event, LlmOutputTextDeltaEvent):
                yield event.delta