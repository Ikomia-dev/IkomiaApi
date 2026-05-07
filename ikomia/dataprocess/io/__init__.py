"""Workflow input/output module."""

from ikomia.core import IODataTypeEx
from ikomia.dataprocess.io.datadictIO import DataDictIO, DataDictIOFactory
from ikomia.dataprocess.io.textstreamIO import TextStreamIO, TextStreamIOFactory

# Register Python-only I/O types
LLM_CONTEXT = IODataTypeEx.register("LLM_CONTEXT", 101, "LLM context I/O")
LLM_STREAMING_RESPONSE = IODataTypeEx.register("LLM_STREAMING_RESPONSE", 102, "LLM streaming response (output)")
