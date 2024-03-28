from ikomia import dataprocess
from ikomia.core import IODataType
from ikomia.dnn.datasetio import IkDatasetIO
from ikomia.dataprocess.datadictIO import DataDictIO


def get_io_type_from_string(type_str: str):
    """
    Helper function to get :py:class:`~ikomia.core.pycore.IODataType` enum value from string.

    Args:
        type_str (str): string representation of enum field

    Return:
        IODataType enum value (:py:class:`~ikomia.core.pycore.IODataType`)
    """
    if type_str == "IODataType.IMAGE":
        return IODataType.IMAGE
    elif type_str == "IODataType.IMAGE_BINARY":
        return IODataType.IMAGE_BINARY
    elif type_str == "IODataType.IMAGE_LABEL":
        return IODataType.IMAGE_LABEL
    elif type_str == "IODataType.VOLUME":
        return IODataType.VOLUME
    elif type_str == "IODataType.VOLUME_BINARY":
        return IODataType.VOLUME_BINARY
    elif type_str == "IODataType.VOLUME_LABEL":
        return IODataType.VOLUME_LABEL
    elif type_str == "IODataType.VIDEO":
        return IODataType.VIDEO
    elif type_str == "IODataType.VIDEO_BINARY":
        return IODataType.VIDEO_BINARY
    elif type_str == "IODataType.VIDEO_LABEL":
        return IODataType.VIDEO_LABEL
    elif type_str == "IODataType.LIVE_STREAM":
        return IODataType.LIVE_STREAM
    elif type_str == "IODataType.LIVE_STREAM_BINARY":
        return IODataType.LIVE_STREAM_BINARY
    elif type_str == "IODataType.LIVE_STREAM_LABEL":
        return IODataType.LIVE_STREAM_LABEL
    elif type_str == "IODataType.INPUT_GRAPHICS":
        return IODataType.INPUT_GRAPHICS
    elif type_str == "IODataType.OUTPUT_GRAPHICS":
        return IODataType.OUTPUT_GRAPHICS
    elif type_str == "IODataType.NUMERIC_VALUES":
        return IODataType.NUMERIC_VALUES
    elif type_str == "IODataType.BLOB_VALUES":
        return IODataType.BLOB_VALUES
    elif type_str == "IODataType.DESCRIPTORS":
        return IODataType.DESCRIPTORS
    elif type_str == "IODataType.WIDGET":
        return IODataType.WIDGET
    elif type_str == "IODataType.FOLDER_PATH":
        return IODataType.FOLDER_PATH
    elif type_str == "IODataType.FILE_PATH":
        return IODataType.FILE_PATH
    elif type_str == "IODataType.DNN_DATASET":
        return IODataType.DNN_DATASET
    elif type_str == "IODataType.ARRAY":
        return IODataType.ARRAY
    elif type_str == "IODataType.DATA_DICT":
        return IODataType.DATA_DICT
    elif type_str == "IODataType.OBJECT_DETECTION":
        return IODataType.OBJECT_DETECTION
    elif type_str == "IODataType.INSTANCE_SEGMENTATION":
        return IODataType.INSTANCE_SEGMENTATION
    elif type_str == "IODataType.SEMANTIC_SEGMENTATION":
        return IODataType.SEMANTIC_SEGMENTATION
    elif type_str == "IODataType.KEYPOINTS":
        return IODataType.KEYPOINTS
    elif type_str == "IODataType.TEXT":
        return IODataType.TEXT
    else:
        return IODataType.NONE


def create_task_io(io_type: IODataType):
    """
    Helper function to create task I/O object from the given data type.

    Args:
        io_type (:py:class:`~ikomia.core.pycore.IODataType`): I/O data type

    Returns:
        Task I/O object (base class: :py:class:`~ikomia.core.pycore.CWorkflowTaskIO`)
    """
    # TODO: use match syntax when minimum Python version will be 3.10
    if io_type == IODataType.IMAGE:
        return dataprocess.CImageIO(IODataType.IMAGE)
    elif io_type == IODataType.IMAGE_BINARY:
        return dataprocess.CImageIO(IODataType.IMAGE_BINARY)
    elif io_type == IODataType.IMAGE_LABEL:
        return dataprocess.CImageIO(IODataType.IMAGE_LABEL)
    elif io_type == IODataType.VOLUME:
        return dataprocess.CImageIO(IODataType.VOLUME)
    elif io_type == IODataType.VOLUME_BINARY:
        return dataprocess.CImageIO(IODataType.VOLUME_BINARY)
    elif io_type == IODataType.VOLUME_LABEL:
        return dataprocess.CImageIO(IODataType.VOLUME_LABEL)
    elif io_type == IODataType.VIDEO:
        return dataprocess.CVideoIO(IODataType.VIDEO)
    elif io_type == IODataType.VIDEO_BINARY:
        return dataprocess.CVideoIO(IODataType.VIDEO_BINARY)
    elif io_type == IODataType.VIDEO_LABEL:
        return dataprocess.CVideoIO(IODataType.VIDEO_LABEL)
    elif io_type == IODataType.LIVE_STREAM:
        return dataprocess.CVideoIO(IODataType.LIVE_STREAM)
    elif io_type == IODataType.LIVE_STREAM_BINARY:
        return dataprocess.CVideoIO(IODataType.LIVE_STREAM_BINARY)
    elif io_type == IODataType.LIVE_STREAM_LABEL:
        return dataprocess.CVideoIO(IODataType.LIVE_STREAM_LABEL)
    elif io_type == IODataType.INPUT_GRAPHICS:
        return dataprocess.CGraphicsInput()
    elif io_type == IODataType.OUTPUT_GRAPHICS:
        return dataprocess.CGraphicsOutput()
    elif io_type == IODataType.NUMERIC_VALUES:
        return dataprocess.CNumericIO()
    elif io_type == IODataType.BLOB_VALUES:
        return dataprocess.CBlobMeasureIO()
    elif io_type == IODataType.DESCRIPTORS:
        return dataprocess.CImageIO(IODataType.DESCRIPTORS)
    elif io_type == IODataType.WIDGET:
        return dataprocess.CWidgetOutput()
    elif io_type == IODataType.FOLDER_PATH:
        return dataprocess.CPathIO(IODataType.FOLDER_PATH)
    elif io_type == IODataType.FILE_PATH:
        return dataprocess.CPathIO(IODataType.FILE_PATH)
    elif io_type == IODataType.DNN_DATASET:
        return IkDatasetIO()
    elif io_type == IODataType.ARRAY:
        return dataprocess.CArrayIO()
    elif io_type == IODataType.DATA_DICT:
        return DataDictIO()
    elif io_type == IODataType.OBJECT_DETECTION:
        return dataprocess.CObjectDetectionIO()
    elif io_type == IODataType.INSTANCE_SEGMENTATION:
        return dataprocess.CInstanceSegmentationIO()
    elif io_type == IODataType.SEMANTIC_SEGMENTATION:
        return dataprocess.CSemanticSegmentationIO()
    elif io_type == IODataType.KEYPOINTS:
        return dataprocess.CKeypointDetectionIO()
    elif io_type == IODataType.TEXT:
        return dataprocess.CTextIO()
    else:
        raise TypeError(f"Unable to create I/O object for the given type {io_type}.")
