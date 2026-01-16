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
import logging
import threading
import time

from ikomia.dataprocess.io.textstreamIO import TextStreamIO

logger = logging.getLogger(__name__)


def test_pythonic_text_stream_io():
    """
    Comprehensive test suite for the Pythonic TextStreamIO wrapper.
    Tests both synchronous and asynchronous streaming functionality.
    """
    logger.info("===== Testing Pythonic TextStreamIO Wrapper =====")
    
    # Test data
    words = ["Il ", "était ", "une ", "fois ", "dans ", "l'", "Ouest.\n", "Ici ", "c'est ", "La ", "Rochelle", "!"]
    text_src = ''.join(words)
    
    def test_basic_functionality():
        """Test basic feed and read operations"""
        logger.info("Testing basic functionality...")
        
        text_io = TextStreamIO()
        
        # Feed data
        for word in words:
            text_io.feed(word)
        
        # Close to signal end of data
        text_io.close()
        
        # Read full text
        full_text = text_io.read_full()
        assert full_text == text_src, f"Expected '{text_src}', got '{full_text}'"
        
        # Test state methods
        assert text_io.is_data_available()
        assert text_io.is_read_finished()
        
        logger.info("✓ Basic functionality test passed")
        
    def test_streaming():
        """Test streaming with generator"""
        logger.info("Testing synchronous streaming...")
        
        text_io = TextStreamIO()
        
        # Feed data in background (simulate async source)
        def feed_data():
            for word in words:
                text_io.feed(word)
                time.sleep(0.05)  # Simulate network delay
            text_io.close()
        
        feed_thread = threading.Thread(target=feed_data)
        feed_thread.start()
        
        # Stream data
        received_chunks = []
        for chunk in text_io.stream(timeout=5):
            received_chunks.append(chunk)
            logger.debug(f"Received chunk: '{chunk.strip()}'")
        
        received_text = ''.join(received_chunks)
        assert received_text == text_src, f"Expected '{text_src}', got '{received_text}'"
        
        feed_thread.join()
        logger.info("✓ Synchronous streaming test passed")
        
    def test_empty_stream():
        """Test behavior with empty stream"""
        logger.info("Testing empty stream...")
        
        text_io = TextStreamIO()
        text_io.close()  # Close without feeding data
        
        full_text = text_io.read_full()
        assert full_text == "", f"Expected empty string, got '{full_text}'"
        
        # Test streaming from empty stream
        chunks = list(text_io.stream(timeout=5))
        assert len(chunks) == 0, f"Expected no chunks, got {len(chunks)}"
        
        logger.info("✓ Empty stream test passed")
        
    def test_large_text():
        """Test with larger text to stress test buffer handling"""
        logger.info("Testing large text handling...")
        
        text_io = TextStreamIO()
        
        # Create large text
        large_text = text_src * 100
        
        # Feed in chunks
        chunk_size = 100
        for i in range(0, len(large_text), chunk_size):
            text_io.feed(large_text[i:i+chunk_size])
        
        text_io.close()
        
        # Read full text
        result = text_io.read_full()
        assert result == large_text, f"Large text mismatch: expected length {len(large_text)}, got {len(result)}"
        
        logger.info("✓ Large text test passed")
        
    def test_unicode_handling():
        """Test Unicode character handling"""
        logger.info("Testing Unicode handling...")
        
        unicode_text = "Hello 世界 🌍 Café naïve "
        
        text_io = TextStreamIO()
        text_io.feed(unicode_text)
        text_io.close()
        
        result = text_io.read_full()
        assert result == unicode_text, f"Unicode handling failed: expected '{unicode_text}', got '{result}'"
        
        logger.info("✓ Unicode handling test passed")
        
    def test_cleanup_and_memory_management():
        """Test proper cleanup and resource management"""
        logger.info("Testing cleanup and memory management...")
        
        # Test explicit cleanup
        text_io = TextStreamIO()
        text_io.feed("Test data for cleanup")
        
        # Test clear data
        text_io.clear_data()
        assert not text_io.is_data_available()
        
        # Test cleanup on deletion
        del text_io
        
        logger.info("✓ Cleanup and memory management test passed")
        
    def test_timeout_behavior():
        """Test timeout behavior in streaming"""
        logger.info("Testing timeout behavior...")
        
        # Test 1: Empty stream with timeout (should return quickly)
        text_io = TextStreamIO()
        text_io.close()  # Close immediately to signal no data

        start_time = time.time()
        chunks = list(text_io.stream(timeout=1))  # Short timeout
        elapsed_time = time.time() - start_time

        # Should return empty list since stream is closed and no data
        assert len(chunks) == 0, f"Expected no chunks from closed empty stream, got {len(chunks)}"

        # Should complete quickly
        assert elapsed_time < 2, f"Empty stream read took too long: {elapsed_time}s"
        
        # Test 2: Stream with data but short timeout between chunks
        text_io2 = TextStreamIO()
        
        def feed_with_delays():
            """Feed data with delays to test timeout between chunks"""
            text_io2.feed("First")
            time.sleep(0.3)  # Delay before next chunk
            text_io2.feed("Second")
            text_io2.close()
        
        feed_thread = threading.Thread(target=feed_with_delays)
        feed_thread.start()
        
        start_time = time.time()
        chunks2 = []
        for chunk in text_io2.stream(timeout=5):  # 5s timeout
            chunks2.append(chunk)

        elapsed_time2 = time.time() - start_time

        # Should get both chunks despite the delay
        assert len(chunks2) == 2, f"Expected 2 chunks, got {len(chunks2)}"
        assert ''.join(chunks2) == "FirstSecond", f"Expected 'FirstSecond', got '{''.join(chunks2)}'"
        
        # Should complete within reasonable time
        assert elapsed_time2 < 5, f"Delayed read took too long: {elapsed_time2}s"
        
        feed_thread.join()
        
        # Test 3: Verify timeout parameter doesn't cause hangs
        text_io3 = TextStreamIO()
        text_io3.feed("Test data")
        text_io3.close()

        start_time = time.time()
        chunks3 = list(text_io3.stream(timeout=1))
        elapsed_time3 = time.time() - start_time

        # Should still get the data quickly
        assert len(chunks3) == 1, f"Expected 1 chunk, got {len(chunks3)}"
        assert chunks3[0] == "Test data", f"Expected 'Test data', got '{chunks3[0]}'"
        assert elapsed_time3 < 1, f"Fast read with short timeout took too long: {elapsed_time3}s"

        logger.info("✓ Timeout behavior test passed")
        
    def test_multiple_streams():
        """Test multiple sequential streaming operations"""
        logger.info("Testing multiple sequential streams...")
        
        text_io = TextStreamIO()
        
        # First stream
        text_io.feed("First ")
        text_io.feed("message")
        text_io.close()
        
        chunks1 = list(text_io.stream())
        assert ''.join(chunks1) == "First message"
        
        # Clear and do second stream
        text_io.clear_data()
        text_io.feed("Second ")
        text_io.feed("message")
        text_io.close()
        
        chunks2 = list(text_io.stream())
        assert ''.join(chunks2) == "Second message"
        
        logger.info("✓ Multiple streams test passed")
        
    # Run all tests
    test_basic_functionality()
    test_streaming()
    test_empty_stream()
    test_large_text()
    test_unicode_handling()
    test_cleanup_and_memory_management()
    test_timeout_behavior()
    test_multiple_streams()
    
    logger.info("\n✅ All TextStreamIO tests passed!")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--tests",
                        type=str,
                        default='all',
                        help="List of tests to execute (comma-separated string, default=all)")

    opt = parser.parse_args()
    running_tests = opt.tests.split(',')
    
    # Configure logging
    logging.basicConfig(
        level=logging.INFO,
        format='%(asctime)s - %(levelname)s - %(message)s'
    )
    
    test_pythonic_text_stream_io()