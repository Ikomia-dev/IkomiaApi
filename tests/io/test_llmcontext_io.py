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
import tempfile
import os

from ikomia.dataprocess.io.llm.llmcontextIO import LlmContextIO
from ikomia.dataprocess.io.llm.base import LlmBaseItem
from ikomia.dataprocess.io.llm.items import (
    LlmAssistantMessage,
    LlmFunctionCall,
    LlmFunctionCallOutput,
    LlmReasoningItem,
    LlmSystemMessage,
    LlmUserMessage,
)
from ikomia.dataprocess.io.llm.content import (
    LlmInputTextContent,
    LlmOutputTextContent,
    LlmSummaryTextContent,
)

logger = logging.getLogger(__name__)


def test_llm_context_io():
    logger.info("===== Testing LlmContextIO =====")

    def test_initial_state():
        logger.info("Testing initial state...")

        ctx = LlmContextIO()
        assert not ctx.is_data_available()
        assert ctx.get_items() == []
        assert ctx.get_response_items() == []

        logger.info("✓ Initial state test passed")

    def test_add_message_roles():
        logger.info("Testing add_message for all roles...")

        ctx = LlmContextIO()

        sys_msg = ctx.add_message("system", "You are a helpful assistant.")
        assert isinstance(sys_msg, LlmSystemMessage)

        user_msg = ctx.add_message("user", "Hello!")
        assert isinstance(user_msg, LlmUserMessage)

        asst_msg = ctx.add_message("assistant", "Hi there!")
        assert isinstance(asst_msg, LlmAssistantMessage)

        assert ctx.is_data_available()
        items = ctx.get_items()
        assert len(items) == 3

        logger.info("✓ add_message roles test passed")

    def test_add_message_invalid_role():
        logger.info("Testing add_message with invalid role...")

        ctx = LlmContextIO()
        try:
            ctx.add_message("unknown_role", "text")
            assert False, "Expected ValueError"
        except ValueError:
            pass

        logger.info("✓ Invalid role test passed")

    def test_add_item():
        logger.info("Testing add_item with LlmBaseItem subclasses...")

        ctx = LlmContextIO()

        func_call = LlmFunctionCall(call_id="call_1", name="get_weather", arguments='{"city": "Paris"}')
        ctx.add_item(func_call)

        func_output = LlmFunctionCallOutput(
            call_id="call_1",
            output=[LlmInputTextContent(text="Sunny, 22°C")],
        )
        ctx.add_item(func_output)

        items = ctx.get_items()
        assert len(items) == 2
        assert isinstance(items[0], LlmFunctionCall)
        assert items[0].call_id == "call_1"
        assert items[0].name == "get_weather"
        assert isinstance(items[1], LlmFunctionCallOutput)

        logger.info("✓ add_item test passed")

    def test_clear_data():
        logger.info("Testing clear_data...")

        ctx = LlmContextIO()
        ctx.add_message("user", "Hello")
        assert ctx.is_data_available()

        ctx.clear_data()
        assert not ctx.is_data_available()
        assert ctx.get_items() == []

        logger.info("✓ clear_data test passed")

    def test_reset_with_context():
        logger.info("Testing reset with another context...")

        src = LlmContextIO()
        src.add_message("system", "Be concise.")
        src.add_message("user", "What is AI?")

        dst = LlmContextIO()
        dst.reset(src)

        assert dst.is_data_available()
        items = dst.get_items()
        assert len(items) == 2
        assert isinstance(items[0], LlmSystemMessage)
        assert isinstance(items[1], LlmUserMessage)

        # Ensure it is a deep copy — mutating src does not affect dst
        src.add_message("assistant", "AI stands for Artificial Intelligence.")
        assert len(dst.get_items()) == 2

        logger.info("✓ reset with context test passed")

    def test_reset_without_context():
        logger.info("Testing reset without context (clears data)...")

        ctx = LlmContextIO()
        ctx.add_message("user", "Hello")
        ctx.reset()

        assert not ctx.is_data_available()

        logger.info("✓ reset without context test passed")

    def test_start_response_and_get_response_items():
        logger.info("Testing start_response / get_response_items...")

        ctx = LlmContextIO()
        ctx.add_message("system", "Be helpful.")
        ctx.add_message("user", "Tell me a joke.")

        ctx.start_response()
        ctx.add_message("assistant", "Why don't scientists trust atoms? Because they make up everything!")

        all_items = ctx.get_items()
        assert len(all_items) == 3

        response_items = ctx.get_response_items()
        assert len(response_items) == 1
        assert isinstance(response_items[0], LlmAssistantMessage)

        logger.info("✓ start_response / get_response_items test passed")

    def test_get_response_items_empty_before_start():
        logger.info("Testing get_response_items returns all items when start_response not called...")

        ctx = LlmContextIO()
        ctx.add_message("user", "Hello")
        ctx.add_message("assistant", "Hi")

        # _response_start_index starts at 0, so all items are "response items"
        assert len(ctx.get_response_items()) == 2

        logger.info("✓ get_response_items before start_response test passed")

    def test_get_pending_function_calls():
        logger.info("Testing get_pending_function_calls...")

        ctx = LlmContextIO()
        ctx.add_item(LlmFunctionCall(call_id="call_1", name="fn_a", arguments="{}"))
        ctx.add_item(LlmFunctionCall(call_id="call_2", name="fn_b", arguments="{}"))
        # Resolve call_1
        ctx.add_item(LlmFunctionCallOutput(call_id="call_1", output=[LlmInputTextContent(text="result_a")]))

        pending = ctx.get_pending_function_calls()
        assert len(pending) == 1
        assert pending[0].call_id == "call_2"

        # Resolve call_2 as well
        ctx.add_item(LlmFunctionCallOutput(call_id="call_2", output=[LlmInputTextContent(text="result_b")]))
        assert ctx.get_pending_function_calls() == []

        logger.info("✓ get_pending_function_calls test passed")

    def test_get_pending_function_calls_empty():
        logger.info("Testing get_pending_function_calls with no calls...")

        ctx = LlmContextIO()
        ctx.add_message("user", "Hello")
        assert ctx.get_pending_function_calls() == []

        logger.info("✓ get_pending_function_calls empty test passed")

    def test_reasoning_item():
        logger.info("Testing LlmReasoningItem round-trip...")

        ctx = LlmContextIO()
        reasoning = LlmReasoningItem(
            summary=[LlmSummaryTextContent(text="I thought about this carefully.")],
            encrypted_content="enc_abc123",
        )
        ctx.add_item(reasoning)

        items = ctx.get_items()
        assert len(items) == 1
        assert isinstance(items[0], LlmReasoningItem)
        assert items[0].encrypted_content == "enc_abc123"
        assert items[0].summary[0].text == "I thought about this carefully."

        logger.info("✓ LlmReasoningItem round-trip test passed")

    def test_custom_item_classes():
        logger.info("Testing get_items with custom_item_classes...")

        from dataclasses import dataclass
        from typing import Literal

        @dataclass(kw_only=True)
        class CustomItem(LlmBaseItem):
            type: Literal["custom"] = "custom"
            payload: str = ""

        ctx = LlmContextIO()
        ctx._raw_items.append({"type": "custom", "payload": "hello"})

        # Without custom class: resolved as LlmBaseItem
        items_default = ctx.get_items(include_unknown_items=True)
        assert len(items_default) == 1
        assert type(items_default[0]) is LlmBaseItem

        # With custom class: resolved correctly
        items_custom = ctx.get_items(custom_item_classes=[CustomItem])
        assert len(items_custom) == 1
        assert isinstance(items_custom[0], CustomItem)
        assert items_custom[0].payload == "hello"

        logger.info("✓ custom_item_classes test passed")

    def test_include_unknown_items_false():
        logger.info("Testing get_items with include_unknown_items=False...")

        ctx = LlmContextIO()
        ctx.add_message("user", "Hello")
        ctx._raw_items.append({"type": "totally_unknown_item_xyz"})

        items_all = ctx.get_items(include_unknown_items=True)
        assert len(items_all) == 2

        items_known_only = ctx.get_items(include_unknown_items=False)
        assert len(items_known_only) == 1
        assert isinstance(items_known_only[0], LlmUserMessage)

        logger.info("✓ include_unknown_items=False test passed")

    def test_to_json_compact():
        logger.info("Testing to_json compact...")

        ctx = LlmContextIO()
        ctx.add_message("user", "Hi")

        json_str = ctx.to_json()
        data = json.loads(json_str)
        assert isinstance(data, list)
        assert len(data) == 1
        assert data[0]["role"] == "user"

        logger.info("✓ to_json compact test passed")

    def test_to_json_indented():
        logger.info("Testing to_json indented...")

        ctx = LlmContextIO()
        ctx.add_message("user", "Hi")

        json_str = ctx.to_json(["json_format", "indented"])
        assert "\n" in json_str
        data = json.loads(json_str)
        assert isinstance(data, list)

        logger.info("✓ to_json indented test passed")

    def test_from_json_roundtrip():
        logger.info("Testing from_json round-trip...")

        ctx = LlmContextIO()
        ctx.add_message("system", "Be brief.")
        ctx.add_message("user", "What is 2+2?")
        ctx.add_message("assistant", "4.")

        json_str = ctx.to_json()

        ctx2 = LlmContextIO()
        ctx2.from_json(json_str)

        items = ctx2.get_items()
        assert len(items) == 3
        assert isinstance(items[0], LlmSystemMessage)
        assert isinstance(items[1], LlmUserMessage)
        assert isinstance(items[2], LlmAssistantMessage)

        logger.info("✓ from_json round-trip test passed")

    def test_from_json_invalid():
        logger.info("Testing from_json with invalid input...")

        ctx = LlmContextIO()

        for bad_input in ['{"key": "value"}', '"just a string"', "42", "[1, 2, 3]"]:
            try:
                ctx.from_json(bad_input)
                assert False, f"Expected ValueError for: {bad_input}"
            except ValueError:
                pass

        logger.info("✓ from_json invalid input test passed")

    def test_save_and_load():
        logger.info("Testing save and load...")

        ctx = LlmContextIO()
        ctx.add_message("user", "Save me.")
        ctx.add_message("assistant", "Saved.")

        with tempfile.NamedTemporaryFile(suffix=".json", delete=False) as tmp:
            path = tmp.name

        try:
            ctx.save(path)
            assert os.path.exists(path)

            ctx2 = LlmContextIO()
            ctx2.load(path)

            items = ctx2.get_items()
            assert len(items) == 2
            assert isinstance(items[0], LlmUserMessage)
            assert isinstance(items[1], LlmAssistantMessage)
        finally:
            os.unlink(path)

        logger.info("✓ save/load test passed")

    def test_message_content_normalization():
        logger.info("Testing message content normalization...")

        ctx = LlmContextIO()
        ctx.add_message("user", "Hello, world!")
        ctx.add_message("assistant", "Hello back!")

        items = ctx.get_items()
        user_msg = items[0]
        asst_msg = items[1]

        # String content is normalized to a list of content objects
        assert isinstance(user_msg.content, list)
        assert len(user_msg.content) == 1
        assert isinstance(user_msg.content[0], LlmInputTextContent)
        assert user_msg.content[0].text == "Hello, world!"

        assert isinstance(asst_msg.content, list)
        assert len(asst_msg.content) == 1
        assert isinstance(asst_msg.content[0], LlmOutputTextContent)
        assert asst_msg.content[0].text == "Hello back!"

        logger.info("✓ message content normalization test passed")

    def test_unicode_content():
        logger.info("Testing Unicode content...")

        ctx = LlmContextIO()
        unicode_text = "Bonjour 🌍 こんにちは Привет"
        ctx.add_message("user", unicode_text)

        json_str = ctx.to_json()
        ctx2 = LlmContextIO()
        ctx2.from_json(json_str)

        items = ctx2.get_items()
        assert items[0].content[0].text == unicode_text

        logger.info("✓ Unicode content test passed")

    def test_multiple_turns():
        logger.info("Testing multi-turn conversation...")

        ctx = LlmContextIO()
        ctx.add_message("system", "You are a helpful assistant.")

        turns = [
            ("user", "What is the capital of France?"),
            ("assistant", "Paris."),
            ("user", "And Germany?"),
            ("assistant", "Berlin."),
        ]
        for role, text in turns:
            ctx.add_message(role, text)

        items = ctx.get_items()
        assert len(items) == 5  # system + 4 turns

        logger.info("✓ Multi-turn conversation test passed")

    def test_start_response_resets_correctly():
        logger.info("Testing start_response index is updated on repeated calls...")

        ctx = LlmContextIO()
        ctx.add_message("user", "First question")
        ctx.add_message("assistant", "First answer")

        ctx.start_response()
        ctx.add_message("user", "Second question")
        ctx.add_message("assistant", "Second answer")

        # Response items from second start_response
        response_items = ctx.get_response_items()
        assert len(response_items) == 2

        # Call start_response again
        ctx.start_response()
        ctx.add_message("assistant", "Third answer")

        response_items2 = ctx.get_response_items()
        assert len(response_items2) == 1
        assert isinstance(response_items2[0], LlmAssistantMessage)

        logger.info("✓ start_response repeated calls test passed")

    # Run all tests
    test_initial_state()
    test_add_message_roles()
    test_add_message_invalid_role()
    test_add_item()
    test_clear_data()
    test_reset_with_context()
    test_reset_without_context()
    test_start_response_and_get_response_items()
    test_get_response_items_empty_before_start()
    test_get_pending_function_calls()
    test_get_pending_function_calls_empty()
    test_reasoning_item()
    test_custom_item_classes()
    test_include_unknown_items_false()
    test_to_json_compact()
    test_to_json_indented()
    test_from_json_roundtrip()
    test_from_json_invalid()
    test_save_and_load()
    test_message_content_normalization()
    test_unicode_content()
    test_multiple_turns()
    test_start_response_resets_correctly()

    logger.info("\n✅ All LlmContextIO tests passed!")


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

    test_llm_context_io()
