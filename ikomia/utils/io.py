"""
The module io provides helper functions to manage workflow I/O of Ikomia API.
"""
from ikomia.core import IODataType, CWorkflowTaskIO  # pylint: disable=E0611
from ikomia.dnn.datasetio import IkDatasetIO
from ikomia.dataprocess import (  # pylint: disable=E0611
    CImageIO, CVideoIO, CGraphicsInput, CGraphicsOutput, CNumericIO, CBlobMeasureIO, CWidgetOutput, CPathIO,
    CArrayIO, CObjectDetectionIO, CInstanceSegmentationIO, CSemanticSegmentationIO, CKeypointsIO, CTextIO
)
from ikomia.dataprocess.datadictIO import DataDictIO


def get_io_type_from_string(type_str: str) -> IODataType:
    """
    Helper function to get :py:class:`~ikomia.core.pycore.IODataType` enum value from string.

    Args:
        type_str (str): string representation of enum field

    Returns:
        IODataType enum value (:py:class:`~ikomia.core.pycore.IODataType`)
    """
    if type_str == "IODataType.IMAGE":
        data_type = IODataType.IMAGE
    elif type_str == "IODataType.IMAGE_BINARY":
        data_type = IODataType.IMAGE_BINARY
    elif type_str == "IODataType.IMAGE_LABEL":
        data_type = IODataType.IMAGE_LABEL
    elif type_str == "IODataType.VOLUME":
        data_type = IODataType.VOLUME
    elif type_str == "IODataType.VOLUME_BINARY":
        data_type = IODataType.VOLUME_BINARY
    elif type_str == "IODataType.VOLUME_LABEL":
        data_type = IODataType.VOLUME_LABEL
    elif type_str == "IODataType.VIDEO":
        data_type = IODataType.VIDEO
    elif type_str == "IODataType.VIDEO_BINARY":
        data_type = IODataType.VIDEO_BINARY
    elif type_str == "IODataType.VIDEO_LABEL":
        data_type = IODataType.VIDEO_LABEL
    elif type_str == "IODataType.LIVE_STREAM":
        data_type = IODataType.LIVE_STREAM
    elif type_str == "IODataType.LIVE_STREAM_BINARY":
        data_type = IODataType.LIVE_STREAM_BINARY
    elif type_str == "IODataType.LIVE_STREAM_LABEL":
        data_type = IODataType.LIVE_STREAM_LABEL
    elif type_str == "IODataType.INPUT_GRAPHICS":
        data_type = IODataType.INPUT_GRAPHICS
    elif type_str == "IODataType.OUTPUT_GRAPHICS":
        data_type = IODataType.OUTPUT_GRAPHICS
    elif type_str == "IODataType.NUMERIC_VALUES":
        data_type = IODataType.NUMERIC_VALUES
    elif type_str == "IODataType.BLOB_VALUES":
        data_type = IODataType.BLOB_VALUES
    elif type_str == "IODataType.DESCRIPTORS":
        data_type = IODataType.DESCRIPTORS
    elif type_str == "IODataType.WIDGET":
        data_type = IODataType.WIDGET
    elif type_str == "IODataType.FOLDER_PATH":
        data_type = IODataType.FOLDER_PATH
    elif type_str == "IODataType.FILE_PATH":
        data_type = IODataType.FILE_PATH
    elif type_str == "IODataType.DNN_DATASET":
        data_type = IODataType.DNN_DATASET
    elif type_str == "IODataType.ARRAY":
        data_type = IODataType.ARRAY
    elif type_str == "IODataType.DATA_DICT":
        data_type = IODataType.DATA_DICT
    elif type_str == "IODataType.OBJECT_DETECTION":
        data_type = IODataType.OBJECT_DETECTION
    elif type_str == "IODataType.INSTANCE_SEGMENTATION":
        data_type = IODataType.INSTANCE_SEGMENTATION
    elif type_str == "IODataType.SEMANTIC_SEGMENTATION":
        data_type = IODataType.SEMANTIC_SEGMENTATION
    elif type_str == "IODataType.KEYPOINTS":
        data_type = IODataType.KEYPOINTS
    elif type_str == "IODataType.TEXT":
        data_type = IODataType.TEXT
    else:
        data_type = IODataType.NONE

    return data_type


def create_task_io(io_type: IODataType) -> CWorkflowTaskIO:
    """
    Helper function to create task I/O object from the given data type.

    Args:
        io_type (:py:class:`~ikomia.core.pycore.IODataType`): I/O data type

    Returns:
        Task I/O object (base class: :py:class:`~ikomia.core.pycore.CWorkflowTaskIO`)
    """
    # TODO: use match syntax when minimum Python version will be 3.10
    if io_type == IODataType.IMAGE:
        io = CImageIO(IODataType.IMAGE)
    elif io_type == IODataType.IMAGE_BINARY:
        io = CImageIO(IODataType.IMAGE_BINARY)
    elif io_type == IODataType.IMAGE_LABEL:
        io = CImageIO(IODataType.IMAGE_LABEL)
    elif io_type == IODataType.VOLUME:
        io = CImageIO(IODataType.VOLUME)
    elif io_type == IODataType.VOLUME_BINARY:
        io = CImageIO(IODataType.VOLUME_BINARY)
    elif io_type == IODataType.VOLUME_LABEL:
        io = CImageIO(IODataType.VOLUME_LABEL)
    elif io_type == IODataType.VIDEO:
        io = CVideoIO(IODataType.VIDEO)
    elif io_type == IODataType.VIDEO_BINARY:
        io = CVideoIO(IODataType.VIDEO_BINARY)
    elif io_type == IODataType.VIDEO_LABEL:
        io = CVideoIO(IODataType.VIDEO_LABEL)
    elif io_type == IODataType.LIVE_STREAM:
        io = CVideoIO(IODataType.LIVE_STREAM)
    elif io_type == IODataType.LIVE_STREAM_BINARY:
        io = CVideoIO(IODataType.LIVE_STREAM_BINARY)
    elif io_type == IODataType.LIVE_STREAM_LABEL:
        io = CVideoIO(IODataType.LIVE_STREAM_LABEL)
    elif io_type == IODataType.INPUT_GRAPHICS:
        io = CGraphicsInput()
    elif io_type == IODataType.OUTPUT_GRAPHICS:
        io = CGraphicsOutput()
    elif io_type == IODataType.NUMERIC_VALUES:
        io = CNumericIO()
    elif io_type == IODataType.BLOB_VALUES:
        io = CBlobMeasureIO()
    elif io_type == IODataType.DESCRIPTORS:
        io = CImageIO(IODataType.DESCRIPTORS)
    elif io_type == IODataType.WIDGET:
        io = CWidgetOutput()
    elif io_type == IODataType.FOLDER_PATH:
        io = CPathIO(IODataType.FOLDER_PATH)
    elif io_type == IODataType.FILE_PATH:
        io = CPathIO(IODataType.FILE_PATH)
    elif io_type == IODataType.DNN_DATASET:
        io = IkDatasetIO()
    elif io_type == IODataType.ARRAY:
        io = CArrayIO()
    elif io_type == IODataType.DATA_DICT:
        io = DataDictIO()
    elif io_type == IODataType.OBJECT_DETECTION:
        io = CObjectDetectionIO()
    elif io_type == IODataType.INSTANCE_SEGMENTATION:
        io = CInstanceSegmentationIO()
    elif io_type == IODataType.SEMANTIC_SEGMENTATION:
        io = CSemanticSegmentationIO()
    elif io_type == IODataType.KEYPOINTS:
        io = CKeypointsIO()
    elif io_type == IODataType.TEXT:
        io = CTextIO()
    else:
        raise TypeError(f"Unable to create I/O object for the given type {io_type}.")

    return io
