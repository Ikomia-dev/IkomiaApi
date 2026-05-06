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
import os
import tempfile

from ikomia.core import IODataType
from ikomia.dataprocess.io.datadictIO import DataDictIO, DataDictIOFactory

logger = logging.getLogger(__name__)


def test_data_dict_io():
    logger.info("===== Testing DataDictIO =====")

    def test_initial_state():
        logger.info("Testing initial state...")

        io = DataDictIO()
        assert io.data == {}
        assert not io.is_data_available()
        assert io.data_type == IODataType.DATA_DICT

        logger.info("✓ Initial state test passed")

    def test_is_data_available():
        logger.info("Testing is_data_available...")

        io = DataDictIO()
        assert not io.is_data_available()

        io.data["key"] = "value"
        assert io.is_data_available()

        io.data = {"a": 1, "b": 2}
        assert io.is_data_available()

        logger.info("✓ is_data_available test passed")

    def test_clear_data():
        logger.info("Testing clear_data...")

        io = DataDictIO()
        io.data = {"name": "Alice", "age": 30}
        assert io.is_data_available()

        io.clear_data()
        assert not io.is_data_available()
        assert io.data == {}

        logger.info("✓ clear_data test passed")

    def test_to_json_compact():
        logger.info("Testing to_json compact format...")

        io = DataDictIO()
        io.data = {"model": "gpt-4", "temperature": 0.7}

        json_str = io.to_json()
        parsed = json.loads(json_str)
        assert parsed == io.data
        assert "\n" not in json_str

        logger.info("✓ to_json compact test passed")

    def test_to_json_indented():
        logger.info("Testing to_json indented format...")

        io = DataDictIO()
        io.data = {"model": "gpt-4", "temperature": 0.7}

        json_str = io.to_json(["json_format", "indented"])
        assert "\n" in json_str
        parsed = json.loads(json_str)
        assert parsed == io.data

        logger.info("✓ to_json indented test passed")

    def test_to_json_explicit_compact():
        logger.info("Testing to_json with explicit compact option...")

        io = DataDictIO()
        io.data = {"result": [1, 2, 3]}

        json_str = io.to_json(["json_format", "compact"])
        assert "\n" not in json_str
        parsed = json.loads(json_str)
        assert parsed == io.data

        logger.info("✓ to_json explicit compact test passed")

    def test_to_json_no_options():
        logger.info("Testing to_json with None options...")

        io = DataDictIO()
        io.data = {"x": 42}

        json_str = io.to_json(None)
        parsed = json.loads(json_str)
        assert parsed == io.data

        logger.info("✓ to_json no options test passed")

    def test_from_json():
        logger.info("Testing from_json...")

        io = DataDictIO()
        data = {"label": "cat", "confidence": 0.95, "box": [10, 20, 100, 80]}
        json_str = json.dumps(data)

        io.from_json(json_str)
        assert io.data == data
        assert io.is_data_available()

        logger.info("✓ from_json test passed")

    def test_from_json_roundtrip():
        logger.info("Testing to_json / from_json round-trip...")

        io = DataDictIO()
        io.data = {
            "name": "detector",
            "version": "1.0",
            "classes": ["car", "bus", "truck"],
            "threshold": 0.5,
        }

        json_str = io.to_json()

        io2 = DataDictIO()
        io2.from_json(json_str)
        assert io2.data == io.data

        logger.info("✓ from_json round-trip test passed")

    def test_save_and_load():
        logger.info("Testing save and load...")

        io = DataDictIO()
        io.data = {"task": "object_detection", "score": 0.88}

        with tempfile.NamedTemporaryFile(suffix=".json", delete=False) as tmp:
            path = tmp.name

        try:
            io.save(path)
            assert os.path.exists(path)

            io2 = DataDictIO()
            io2.load(path)
            assert io2.data == io.data
            assert io2.is_data_available()
        finally:
            os.unlink(path)

        logger.info("✓ save/load test passed")

    def test_save_and_load_nested():
        logger.info("Testing save and load with nested data...")

        io = DataDictIO()
        io.data = {
            "metadata": {"author": "test", "version": 2},
            "results": [{"id": 0, "label": "dog"}, {"id": 1, "label": "cat"}],
        }

        with tempfile.NamedTemporaryFile(suffix=".json", delete=False) as tmp:
            path = tmp.name

        try:
            io.save(path)
            io2 = DataDictIO()
            io2.load(path)
            assert io2.data == io.data
        finally:
            os.unlink(path)

        logger.info("✓ save/load nested data test passed")

    def test_unicode_content():
        logger.info("Testing Unicode content...")

        io = DataDictIO()
        io.data = {"message": "Bonjour 🌍 こんにちは Привет", "emoji": "✓"}

        json_str = io.to_json()
        io2 = DataDictIO()
        io2.from_json(json_str)
        assert io2.data == io.data

        logger.info("✓ Unicode content test passed")

    def test_empty_dict_serialization():
        logger.info("Testing serialization of empty dict...")

        io = DataDictIO()
        json_str = io.to_json()
        assert json.loads(json_str) == {}

        io2 = DataDictIO()
        io2.from_json(json_str)
        assert not io2.is_data_available()
        assert io2.data == {}

        logger.info("✓ Empty dict serialization test passed")

    def test_factory():
        logger.info("Testing DataDictIOFactory...")

        factory = DataDictIOFactory()

        valid_types = factory.get_valid_data_types()
        assert IODataType.DATA_DICT in valid_types

        created = factory.create(IODataType.DATA_DICT)
        assert isinstance(created, DataDictIO)
        assert not created.is_data_available()

        logger.info("✓ Factory test passed")

    # Run all tests
    test_initial_state()
    test_is_data_available()
    test_clear_data()
    test_to_json_compact()
    test_to_json_indented()
    test_to_json_explicit_compact()
    test_to_json_no_options()
    test_from_json()
    test_from_json_roundtrip()
    test_save_and_load()
    test_save_and_load_nested()
    test_unicode_content()
    test_empty_dict_serialization()
    test_factory()

    logger.info("\n✅ All DataDictIO tests passed!")


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

    test_data_dict_io()