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
import logging
import queue
import threading
from typing import Generator

from ikomia.core import CWorkflowTaskIO, IODataType  # pylint: disable=E0611
from ikomia.dataprocess.pydataprocess import CTextStreamIO

logger = logging.getLogger(__name__)


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
        self._chunk_queue = queue.Queue()
        self._stop_event = threading.Event()
        self._reader_thread = None
        self._lock = threading.Lock()

    def __del__(self):
        """
        Clean up resources when the object is destroyed.
        """
        # Clear all pending operations
        self._cpp_io.shutdown()
        self._clear_queue()

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
                f"read_finished={self.is_read_finished()}")

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

    def _clear_queue(self):
        try:
            while True:
                self._chunk_queue.get_nowait()
        except queue.Empty:
            pass

    def _reader_loop(self):
        """
        Background thread that reads chunks from C++ and puts them in the queue.
        """
        # Start async reading if not already started
        try:
            while not self._stop_event.is_set():
                chunk = self._cpp_io.read_next(0.001)
                if chunk:
                    self._chunk_queue.put(chunk)
                elif self.is_read_finished():
                    break
        finally:
            self._chunk_queue.put(None)

    def _start_reader_thread(self):
        """
        Start the background reader thread if not already running.
        """
        with self._lock:
            if (self._reader_thread is None or not self._reader_thread.is_alive()) and not self._stop_event.is_set():
                self._reader_thread = threading.Thread(
                    target=self._reader_loop,
                    daemon=True,
                    name="TextStreamQueueReader"
                )
                self._reader_thread.start()
        
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
            
    def stream(self, timeout: int = 60) -> Generator[str, None, None]:
        """
        Generator that yields text chunks as they become available.
        
        This is the main Pythonic interface that hides the callback complexity.
        
        Args:
            timeout (int): read timeout in seconds
            
        Yields:
            str: text chunks as they arrive
        """
        # Start the reader thread
        self._start_reader_thread()

        while True:
            try:
                chunk = self._chunk_queue.get(block=True, timeout=timeout)
                if chunk is None:
                    break

                yield chunk
            except queue.Empty:
                break

        self._stop_streaming()

    def _stop_streaming(self):
        """
        Stop the streaming and clean up resources.
        """
        with self._lock:
            if not self._stop_event.is_set():
                self._stop_event.set()
                self._reader_thread.join(timeout=1.0)
                self._stop_event.clear()
                # Cancel pending operations in C++ layer
                self._cpp_io.close()
                self._cpp_io.shutdown()

    def clear_data(self):
        """
        Clear all internal data and reset state.
        """
        self._clear_queue()
        self._cpp_io.clear_data()
