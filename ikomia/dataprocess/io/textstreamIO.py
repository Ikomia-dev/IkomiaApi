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

"""
Module providing Pythonic wrapper for text stream I/O using generators.
"""
import queue
import threading
from typing import Generator

from ikomia.core import CWorkflowTaskIO, IODataType  # pylint: disable=E0611
from ikomia.dataprocess.pydataprocess import CTextStreamIO


class TextStreamIO(CWorkflowTaskIO):
    """
    Pythonic wrapper for text stream I/O that hides callback complexity behind generators.
    
    This class provides a more Pythonic interface for working with text streams by using
    generators to iterate through incoming text chunks, eliminating the need for manual
    callback management and thread coordination.
    
    Inherits from :py:class:`~ikomia.core.pycore.CWorkflowTaskIO`.
    
    :ivar _cpp_io: the underlying C++ CTextStreamIO instance
    """
    
    def __init__(self):
        """
        Initialize the TextStreamIO wrapper.
        """
        CWorkflowTaskIO.__init__(self, IODataType.TEXT)
        self._cpp_io = CTextStreamIO()

    def __str__(self) -> str:
        """
        String representation of the TextStreamIO object.

        Returns:
            str: object description
        """
        return f"TextStreamIO(data_available={self.is_data_available()}, read_finished={self.is_read_finished()})"

    def __repr__(self) -> str:
        """
        Detailed string representation of the TextStreamIO object.

        Returns:
            str: detailed object description
        """
        return (f"TextStreamIO("
                f"data_available={self.is_data_available()}, "
                f"read_finished={self.is_read_finished()}, "
                f"chunk_queue_size={self._chunk_queue.qsize()})")

    def is_data_available(self) -> bool:
        """
        Check if data is available in the stream.

        Returns:
            bool: True if data is available, False otherwise
        """
        return self._cpp_io.is_data_available()

    def is_read_finished(self) -> bool:
        """
        Check if reading from the stream is finished.

        Returns:
            bool: True if reading is complete, False otherwise
        """
        return self._cpp_io.is_read_finished()
        
    def feed(self, text: str):
        """
        Feed text data into the stream.
        
        Args:
            text (str): text chunk to feed into the stream
        """
        self._cpp_io.feed(text)
            
    def close(self):
        """
        Close the text stream, signaling that no more data will be fed.
        """
        self._cpp_io.close()
            
    def read_full(self) -> str:
        """
        Read the full text content synchronously.
        
        Returns:
            str: complete text content
        """
        return self._cpp_io.read_full()
            
    def stream(self, min_text_size: int = 1, timeout: int = 60) -> Generator[str, None, None]:
        """
        Generator that yields text chunks as they become available.
        
        This is the main Pythonic interface that hides the callback complexity.
        
        Args:
            min_text_size (int): minimum text size to read
            timeout (int): read timeout in seconds
            
        Yields:
            str: text chunks as they arrive
        """
        q = queue.Queue()
        sentinel = object()

        # Start async reading if not already started
        def reader():
            try:
                while True:
                    chunk = self._cpp_io.read_next(min_text_size, 0.001)
                    if chunk:
                        q.put(chunk)
                    elif self.is_read_finished():
                        break
            finally:
                self._cpp_io.shutdown()
                q.put(sentinel)

        threading.Thread(target=reader, daemon=True).start()

        while True:
            item = q.get(block=True, timeout=timeout)
            if item is sentinel:
                break

            yield item
                
    def clear_data(self):
        """
        Clear all internal data and reset state.
        """
        self._cpp_io.clear_data()
    
    def cancel_pending_operations(self):
        """
        Cancel all pending asynchronous operations.
        
        This method should be called before destroying the object
        to prevent memory corruption from pending callbacks.
        """
        # Clear all pending operations
        self._cpp_io.shutdown()
