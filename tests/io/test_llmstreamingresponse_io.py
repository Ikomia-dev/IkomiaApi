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

import argparse
import json
import logging
import threading
import time
from dataclasses import dataclass
from typing import Literal

from ikomia.dataprocess.io.llm.llmstreamingresponseIO import LlmStreamingResponseIO
from ikomia.dataprocess.io.llm.base import LlmBaseEvent, LlmBaseItem
from ikomia.dataprocess.io.llm.stream import (
    LlmFunctionCallArgumentsDeltaEvent,
    LlmOutputItemAddedEvent,
    LlmOutputTextDeltaEvent,
    LlmReasoningSummaryPartAddedEvent,
    LlmReasoningSummaryTextDeltaEvent,
    LlmReasoningTextDeltaEvent,
)
from ikomia.dataprocess.io.llm.content import LlmSummaryTextContent

logger = logging.getLogger(__name__)


def test_llm_streaming_response_io():
    logger.info("===== Testing LlmStreamingResponseIO =====")

    def _feed_and_collect(io, **stream_kwargs):
        """Close the stream then collect all events synchronously."""
        io.close()
        return list(io.stream_events(**stream_kwargs))

    # ------------------------------------------------------------------
    # __init__ / initial state
    # ------------------------------------------------------------------
    def test_initial_state():
        logger.info("Testing initial state...")

        io = LlmStreamingResponseIO()
        assert io._auto_sequence_number == 0
        assert io._next_item_idx == 0
        assert not io.is_data_available()

        logger.info("✓ Initial state test passed")

    # ------------------------------------------------------------------
    # clear_data
    # ------------------------------------------------------------------
    def test_clear_data():
        logger.info("Testing clear_data resets all internal state...")

        io = LlmStreamingResponseIO()
        io.feed_event(LlmOutputTextDeltaEvent(output_index=0, item_id="i1", content_index=0, delta="hi"))
        io._next_item_idx = 3
        assert io._auto_sequence_number == 1

        io.clear_data()
        assert io._auto_sequence_number == 0
        assert io._next_item_idx == 0

        logger.info("✓ clear_data test passed")

    def test_clear_data_then_feed_restarts_numbering():
        logger.info("Testing clear_data then feed restarts sequence at 0...")

        io = LlmStreamingResponseIO()
        io.feed_event(LlmOutputTextDeltaEvent(output_index=0, item_id="i1", content_index=0, delta="a"))
        io.feed_event(LlmOutputTextDeltaEvent(output_index=0, item_id="i1", content_index=1, delta="b"))
        assert io._auto_sequence_number == 2

        io.clear_data()

        e = LlmOutputTextDeltaEvent(output_index=0, item_id="i1", content_index=0, delta="c")
        io.feed_event(e)
        assert e.sequence_number == 0
        assert io._auto_sequence_number == 1

        logger.info("✓ clear_data then feed restarts numbering test passed")

    # ------------------------------------------------------------------
    # allocate_item
    # ------------------------------------------------------------------
    def test_allocate_item():
        logger.info("Testing allocate_item returns monotonically increasing indices...")

        io = LlmStreamingResponseIO()
        assert io.allocate_item() == 0
        assert io.allocate_item() == 1
        assert io.allocate_item() == 2
        assert io._next_item_idx == 3

        logger.info("✓ allocate_item test passed")

    # ------------------------------------------------------------------
    # feed_event — sequence number management
    # ------------------------------------------------------------------
    def test_feed_event_auto_sequence():
        logger.info("Testing feed_event assigns auto sequence numbers when None...")

        io = LlmStreamingResponseIO()

        e1 = LlmOutputTextDeltaEvent(output_index=0, item_id="i1", content_index=0, delta="Hello")
        e2 = LlmOutputTextDeltaEvent(output_index=0, item_id="i1", content_index=1, delta=" world")
        assert e1.sequence_number is None
        assert e2.sequence_number is None

        io.feed_event(e1)
        assert e1.sequence_number == 0
        assert io._auto_sequence_number == 1

        io.feed_event(e2)
        assert e2.sequence_number == 1
        assert io._auto_sequence_number == 2

        logger.info("✓ feed_event auto sequence test passed")

    def test_feed_event_explicit_sequence_advances_counter():
        logger.info("Testing feed_event with explicit sequence number advances counter past it...")

        io = LlmStreamingResponseIO()
        e = LlmOutputTextDeltaEvent(output_index=0, item_id="i1", content_index=0, delta="Hi", sequence_number=5)
        io.feed_event(e)

        assert e.sequence_number == 5
        assert io._auto_sequence_number == 6

        logger.info("✓ feed_event explicit sequence advances counter test passed")

    def test_feed_event_explicit_lower_sequence_does_not_regress_counter():
        logger.info("Testing feed_event with lower explicit sequence does not regress counter...")

        io = LlmStreamingResponseIO()
        e1 = LlmOutputTextDeltaEvent(output_index=0, item_id="i1", content_index=0, delta="a")
        io.feed_event(e1)
        assert io._auto_sequence_number == 1

        e2 = LlmOutputTextDeltaEvent(output_index=0, item_id="i1", content_index=1, delta="b", sequence_number=0)
        io.feed_event(e2)
        # max(1, 0+1) = 1, counter does not go below 1
        assert io._auto_sequence_number == 1

        logger.info("✓ feed_event lower explicit sequence test passed")

    def test_feed_event_produces_valid_ndjson():
        logger.info("Testing feed_event produces single-line compact NDJSON...")

        io = LlmStreamingResponseIO()
        e = LlmOutputTextDeltaEvent(output_index=0, item_id="i1", content_index=0, delta="test_value")
        io.feed_event(e)
        io.close()

        raw = io.read_full()
        lines = [l for l in raw.split("\n") if l.strip()]
        assert len(lines) == 1, f"Expected 1 line, got {len(lines)}"
        parsed = json.loads(lines[0])
        assert parsed["type"] == "response.output_text.delta"
        assert parsed["delta"] == "test_value"
        assert parsed["output_index"] == 0

        logger.info("✓ feed_event produces valid NDJSON test passed")

    # ------------------------------------------------------------------
    # stream_events — all built-in event types
    # ------------------------------------------------------------------
    def test_stream_events_output_text_delta():
        logger.info("Testing stream_events: LlmOutputTextDeltaEvent...")

        io = LlmStreamingResponseIO()
        io.feed_event(LlmOutputTextDeltaEvent(output_index=0, item_id="i1", content_index=0, delta="Hello"))
        events = _feed_and_collect(io)

        assert len(events) == 1
        assert isinstance(events[0], LlmOutputTextDeltaEvent)
        assert events[0].delta == "Hello"
        assert events[0].item_id == "i1"
        assert events[0].content_index == 0

        logger.info("✓ LlmOutputTextDeltaEvent test passed")

    def test_stream_events_reasoning_text_delta():
        logger.info("Testing stream_events: LlmReasoningTextDeltaEvent...")

        io = LlmStreamingResponseIO()
        io.feed_event(LlmReasoningTextDeltaEvent(output_index=0, item_id="i1", content_index=0, delta="Reasoning..."))
        events = _feed_and_collect(io)

        assert len(events) == 1
        assert isinstance(events[0], LlmReasoningTextDeltaEvent)
        assert events[0].delta == "Reasoning..."

        logger.info("✓ LlmReasoningTextDeltaEvent test passed")

    def test_stream_events_output_item_added():
        logger.info("Testing stream_events: LlmOutputItemAddedEvent...")

        io = LlmStreamingResponseIO()
        item = LlmBaseItem(type="message")
        io.feed_event(LlmOutputItemAddedEvent(output_index=0, item=item))
        events = _feed_and_collect(io)

        assert len(events) == 1
        assert isinstance(events[0], LlmOutputItemAddedEvent)
        assert events[0].item.type == "message"

        logger.info("✓ LlmOutputItemAddedEvent test passed")

    def test_stream_events_reasoning_summary_part_added():
        logger.info("Testing stream_events: LlmReasoningSummaryPartAddedEvent...")

        io = LlmStreamingResponseIO()
        part = LlmSummaryTextContent(text="Summary text")
        io.feed_event(LlmReasoningSummaryPartAddedEvent(output_index=0, item_id="i1", summary_index=0, part=part))
        events = _feed_and_collect(io)

        assert len(events) == 1
        assert isinstance(events[0], LlmReasoningSummaryPartAddedEvent)
        assert events[0].part.text == "Summary text"
        assert events[0].summary_index == 0

        logger.info("✓ LlmReasoningSummaryPartAddedEvent test passed")

    def test_stream_events_reasoning_summary_text_delta():
        logger.info("Testing stream_events: LlmReasoningSummaryTextDeltaEvent...")

        io = LlmStreamingResponseIO()
        io.feed_event(LlmReasoningSummaryTextDeltaEvent(output_index=0, item_id="i1", summary_index=0, delta="delta text"))
        events = _feed_and_collect(io)

        assert len(events) == 1
        assert isinstance(events[0], LlmReasoningSummaryTextDeltaEvent)
        assert events[0].delta == "delta text"
        assert events[0].summary_index == 0

        logger.info("✓ LlmReasoningSummaryTextDeltaEvent test passed")

    def test_stream_events_function_call_arguments_delta():
        logger.info("Testing stream_events: LlmFunctionCallArgumentsDeltaEvent...")

        io = LlmStreamingResponseIO()
        io.feed_event(LlmFunctionCallArgumentsDeltaEvent(output_index=0, item_id="i1", delta='{"city": "Paris"}'))
        events = _feed_and_collect(io)

        assert len(events) == 1
        assert isinstance(events[0], LlmFunctionCallArgumentsDeltaEvent)
        assert events[0].delta == '{"city": "Paris"}'

        logger.info("✓ LlmFunctionCallArgumentsDeltaEvent test passed")

    def test_stream_events_multiple_types_in_sequence():
        logger.info("Testing stream_events: multiple event types preserve order...")

        io = LlmStreamingResponseIO()
        io.feed_event(LlmOutputTextDeltaEvent(output_index=0, item_id="i1", content_index=0, delta="Hello"))
        io.feed_event(LlmReasoningTextDeltaEvent(output_index=0, item_id="i1", content_index=0, delta="Thinking"))
        io.feed_event(LlmFunctionCallArgumentsDeltaEvent(output_index=1, item_id="i2", delta='{"x": 1}'))
        events = _feed_and_collect(io)

        assert len(events) == 3
        assert isinstance(events[0], LlmOutputTextDeltaEvent)
        assert isinstance(events[1], LlmReasoningTextDeltaEvent)
        assert isinstance(events[2], LlmFunctionCallArgumentsDeltaEvent)

        logger.info("✓ multiple event types in sequence test passed")

    # ------------------------------------------------------------------
    # stream_events — unknown events and custom classes
    # ------------------------------------------------------------------
    def test_stream_events_unknown_excluded_by_default():
        logger.info("Testing stream_events: unknown events excluded when include_unknown_events=False (default)...")

        io = LlmStreamingResponseIO()
        io.feed_event(LlmOutputTextDeltaEvent(output_index=0, item_id="i1", content_index=0, delta="known"))
        unknown_raw = {"type": "response.totally_unknown_xyz", "output_index": 0}
        io.feed(json.dumps(unknown_raw) + "\n")
        events = _feed_and_collect(io)

        assert len(events) == 1
        assert isinstance(events[0], LlmOutputTextDeltaEvent)

        logger.info("✓ unknown events excluded by default test passed")

    def test_stream_events_include_unknown():
        logger.info("Testing stream_events with include_unknown_events=True yields LlmBaseEvent...")

        io = LlmStreamingResponseIO()
        unknown_raw = {"type": "response.custom_event", "output_index": 0}
        io.feed(json.dumps(unknown_raw) + "\n")
        io.close()
        events = list(io.stream_events(include_unknown_events=True))

        assert len(events) == 1
        assert type(events[0]) is LlmBaseEvent
        assert events[0].type == "response.custom_event"

        logger.info("✓ include_unknown_events=True test passed")

    def test_stream_events_custom_event_classes_resolved_first():
        logger.info("Testing stream_events: custom_event_classes take priority over built-ins...")

        @dataclass(kw_only=True)
        class CustomEvent(LlmBaseEvent):
            type: Literal["response.custom"] = "response.custom"
            payload: str = ""

        io = LlmStreamingResponseIO()
        custom_raw = {"type": "response.custom", "output_index": 0, "payload": "my_data"}
        io.feed(json.dumps(custom_raw) + "\n")
        io.close()
        events = list(io.stream_events(custom_event_classes=[CustomEvent]))

        assert len(events) == 1
        assert isinstance(events[0], CustomEvent)
        assert events[0].payload == "my_data"

        logger.info("✓ custom_event_classes test passed")

    def test_stream_events_custom_event_classes_fallback_to_builtin():
        logger.info("Testing stream_events: built-in events still resolved when custom_event_classes provided...")

        @dataclass(kw_only=True)
        class OtherCustomEvent(LlmBaseEvent):
            type: Literal["response.other_custom"] = "response.other_custom"

        io = LlmStreamingResponseIO()
        io.feed_event(LlmOutputTextDeltaEvent(output_index=0, item_id="i1", content_index=0, delta="built-in"))
        events = _feed_and_collect(io, custom_event_classes=[OtherCustomEvent])

        assert len(events) == 1
        assert isinstance(events[0], LlmOutputTextDeltaEvent)
        assert events[0].delta == "built-in"

        logger.info("✓ custom_event_classes fallback to built-in test passed")

    # ------------------------------------------------------------------
    # stream_events — NDJSON buffering (split chunks)
    # ------------------------------------------------------------------
    def test_stream_events_handles_split_chunks():
        logger.info("Testing stream_events handles NDJSON lines split across chunks...")

        io = LlmStreamingResponseIO()
        e = LlmOutputTextDeltaEvent(output_index=0, item_id="i1", content_index=0, delta="hi")
        line = json.dumps(e.to_dict(), separators=(",", ":")) + "\n"

        # Split the line into two halves to simulate partial reads
        mid = len(line) // 2
        io.feed(line[:mid])
        io.feed(line[mid:])
        io.close()

        events = list(io.stream_events())
        assert len(events) == 1
        assert isinstance(events[0], LlmOutputTextDeltaEvent)
        assert events[0].delta == "hi"

        logger.info("✓ split-chunk NDJSON buffering test passed")

    def test_stream_events_multiple_lines_in_one_chunk():
        logger.info("Testing stream_events handles multiple NDJSON lines in one chunk...")

        io = LlmStreamingResponseIO()
        e1 = LlmOutputTextDeltaEvent(output_index=0, item_id="i1", content_index=0, delta="first")
        e2 = LlmOutputTextDeltaEvent(output_index=0, item_id="i1", content_index=1, delta="second")
        combined = (
            json.dumps(e1.to_dict(), separators=(",", ":")) + "\n"
            + json.dumps(e2.to_dict(), separators=(",", ":")) + "\n"
        )
        io.feed(combined)
        io.close()

        events = list(io.stream_events())
        assert len(events) == 2
        assert events[0].delta == "first"
        assert events[1].delta == "second"

        logger.info("✓ multiple lines in one chunk test passed")

    # ------------------------------------------------------------------
    # stream_text
    # ------------------------------------------------------------------
    def test_stream_text_yields_only_output_text_deltas():
        logger.info("Testing stream_text: only LlmOutputTextDeltaEvent deltas are yielded...")

        io = LlmStreamingResponseIO()
        io.feed_event(LlmOutputTextDeltaEvent(output_index=0, item_id="i1", content_index=0, delta="Hello"))
        io.feed_event(LlmReasoningTextDeltaEvent(output_index=0, item_id="i1", content_index=0, delta="<thinking>"))
        io.feed_event(LlmOutputTextDeltaEvent(output_index=0, item_id="i1", content_index=1, delta=" world"))
        io.close()

        text_chunks = list(io.stream_text())
        assert text_chunks == ["Hello", " world"]

        logger.info("✓ stream_text yields only output text deltas test passed")

    def test_stream_text_empty_when_no_output_text_events():
        logger.info("Testing stream_text returns nothing when no LlmOutputTextDeltaEvent fed...")

        io = LlmStreamingResponseIO()
        io.feed_event(LlmReasoningTextDeltaEvent(output_index=0, item_id="i1", content_index=0, delta="only reasoning"))
        io.close()

        text_chunks = list(io.stream_text())
        assert text_chunks == []

        logger.info("✓ stream_text empty when no output text events test passed")

    def test_stream_text_empty_stream():
        logger.info("Testing stream_text on an empty (immediately closed) stream...")

        io = LlmStreamingResponseIO()
        io.close()

        text_chunks = list(io.stream_text())
        assert text_chunks == []

        logger.info("✓ stream_text empty stream test passed")

    # ------------------------------------------------------------------
    # Asynchronous producer / consumer
    # ------------------------------------------------------------------
    def test_stream_events_async_producer():
        logger.info("Testing stream_events with asynchronous producer thread...")

        io = LlmStreamingResponseIO()
        collected = []

        def producer():
            for i in range(3):
                io.feed_event(
                    LlmOutputTextDeltaEvent(output_index=0, item_id="i1", content_index=i, delta=f"chunk{i}")
                )
                time.sleep(0.05)
            io.close()

        t = threading.Thread(target=producer)
        t.start()

        for event in io.stream_events(timeout=5):
            collected.append(event)

        t.join()

        assert len(collected) == 3
        assert all(isinstance(e, LlmOutputTextDeltaEvent) for e in collected)
        assert [e.delta for e in collected] == ["chunk0", "chunk1", "chunk2"]

        logger.info("✓ async producer test passed")

    def test_stream_text_async_producer():
        logger.info("Testing stream_text with asynchronous producer thread...")

        io = LlmStreamingResponseIO()
        collected = []

        def producer():
            deltas = ["The ", "answer ", "is ", "42."]
            for delta in deltas:
                io.feed_event(LlmOutputTextDeltaEvent(output_index=0, item_id="i1", content_index=0, delta=delta))
                time.sleep(0.03)
            io.close()

        t = threading.Thread(target=producer)
        t.start()

        for chunk in io.stream_text(timeout=5):
            collected.append(chunk)

        t.join()

        assert "".join(collected) == "The answer is 42."

        logger.info("✓ async stream_text producer test passed")

    # Run all tests
    test_initial_state()
    test_clear_data()
    test_clear_data_then_feed_restarts_numbering()
    test_allocate_item()
    test_feed_event_auto_sequence()
    test_feed_event_explicit_sequence_advances_counter()
    test_feed_event_explicit_lower_sequence_does_not_regress_counter()
    test_feed_event_produces_valid_ndjson()
    test_stream_events_output_text_delta()
    test_stream_events_reasoning_text_delta()
    test_stream_events_output_item_added()
    test_stream_events_reasoning_summary_part_added()
    test_stream_events_reasoning_summary_text_delta()
    test_stream_events_function_call_arguments_delta()
    test_stream_events_multiple_types_in_sequence()
    test_stream_events_unknown_excluded_by_default()
    test_stream_events_include_unknown()
    test_stream_events_custom_event_classes_resolved_first()
    test_stream_events_custom_event_classes_fallback_to_builtin()
    test_stream_events_handles_split_chunks()
    test_stream_events_multiple_lines_in_one_chunk()
    test_stream_text_yields_only_output_text_deltas()
    test_stream_text_empty_when_no_output_text_events()
    test_stream_text_empty_stream()
    test_stream_events_async_producer()
    test_stream_text_async_producer()

    logger.info("\n✅ All LlmStreamingResponseIO tests passed!")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--tests",
        type=str,
        default="all",
        help="List of tests to execute (comma-separated string, default=all)",
    )

    opt = parser.parse_args()

    logging.basicConfig(
        level=logging.INFO,
        format="%(asctime)s - %(levelname)s - %(message)s",
    )

    test_llm_streaming_response_io()