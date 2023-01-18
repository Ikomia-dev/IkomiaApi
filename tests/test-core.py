import argparse
import logging
from ikomia import core

logger = logging.getLogger(__name__)


def test_cpp_enums():
    # Graphics shape
    shape = core.GraphicsShape.ELLIPSE
    shape = core.GraphicsShape.FREEHAND_POLYGON
    shape = core.GraphicsShape.FREEHAND_POLYLINE
    shape = core.GraphicsShape.POINT
    shape = core.GraphicsShape.POLYGON
    shape = core.GraphicsShape.POLYLINE
    shape = core.GraphicsShape.RECTANGLE
    shape = core.GraphicsShape.SELECTION
    shape = core.GraphicsShape.TEXT

    # Graphics item type
    graphics_type = core.GraphicsItem.LAYER
    graphics_type = core.GraphicsItem.ELLIPSE
    graphics_type = core.GraphicsItem.RECTANGLE
    graphics_type = core.GraphicsItem.POINT
    graphics_type = core.GraphicsItem.POLYGON
    graphics_type = core.GraphicsItem.COMPLEX_POLYGON
    graphics_type = core.GraphicsItem.POLYLINE
    graphics_type = core.GraphicsItem.TEXT

    # IO data type
    io_type = core.IODataType.NONE
    io_type = core.IODataType.IMAGE
    io_type = core.IODataType.IMAGE_BINARY
    io_type = core.IODataType.IMAGE_LABEL
    io_type = core.IODataType.VOLUME
    io_type = core.IODataType.VOLUME_BINARY
    io_type = core.IODataType.VOLUME_LABEL
    io_type = core.IODataType.VIDEO
    io_type = core.IODataType.VIDEO_BINARY
    io_type = core.IODataType.VIDEO_LABEL
    io_type = core.IODataType.LIVE_STREAM
    io_type = core.IODataType.LIVE_STREAM_BINARY
    io_type = core.IODataType.LIVE_STREAM_LABEL
    io_type = core.IODataType.INPUT_GRAPHICS
    io_type = core.IODataType.OUTPUT_GRAPHICS
    io_type = core.IODataType.NUMERIC_VALUES
    io_type = core.IODataType.BLOB_VALUES
    io_type = core.IODataType.DESCRIPTORS
    io_type = core.IODataType.WIDGET
    io_type = core.IODataType.FOLDER_PATH
    io_type = core.IODataType.FILE_PATH
    io_type = core.IODataType.DNN_DATASET
    io_type = core.IODataType.ARRAY
    io_type = core.IODataType.DATA_DICT
    io_type = core.IODataType.OBJECT_DETECTION
    io_type = core.IODataType.INSTANCE_SEGMENTATION
    io_type = core.IODataType.SEMANTIC_SEGMENTATION

    # Task type
    task_type = core.TaskType.GENERIC
    task_type = core.TaskType.IMAGE_PROCESS_2D
    task_type = core.TaskType.IMAGE_PROCESS_3D
    task_type = core.TaskType.VIDEO

    # Action flag
    action_flag = core.ActionFlag.APPLY_VOLUME
    action_flag = core.ActionFlag.OUTPUT_AUTO_EXPORT

    # Measures
    measure = core.MeasureId.SURFACE
    measure = core.MeasureId.PERIMETER
    measure = core.MeasureId.CENTROID
    measure = core.MeasureId.BBOX
    measure = core.MeasureId.ORIENTED_BBOX
    measure = core.MeasureId.EQUIVALENT_DIAMETER
    measure = core.MeasureId.ELONGATION
    measure = core.MeasureId.CIRCULARITY
    measure = core.MeasureId.SOLIDITY
    measure = core.MeasureId.CUSTOM


def test_cpp_point():
    pt = core.CPointF()
    assert type(pt.x) == float
    pt = core.CPointF(5.0, 10.0)
    assert pt.x == 5.0
    assert pt.y == 10.0


def test_cpp_measure():
    # Default ctor
    measure = core.CMeasure()
    measure_list = measure.get_available_measures()
    assert len(measure_list) == 9

    # Static method
    measure_list = core.CMeasure.get_available_measures()
    assert len(measure_list) == 9
    measure_name = core.CMeasure.get_name(core.MeasureId.SURFACE)
    logger.info(f"Measure name: {measure_name}")

    # Ctor with id
    measure = core.CMeasure(core.MeasureId.SURFACE)
    assert measure.id == core.MeasureId.SURFACE

    # Ctor with id and name
    custom_name = "Volume"
    measure = core.CMeasure(core.MeasureId.CUSTOM, custom_name)
    assert measure.id == core.MeasureId.CUSTOM
    assert measure.name == custom_name


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--tests",
                        type=str,
                        default='all',
                        help="List of tests to execute (comma-separated string, default=all)")

    opt = parser.parse_args()
    running_tests = opt.tests.split(',')

    if "all" in running_tests or "cpp_enums" in running_tests:
        test_cpp_enums()
    if "all" in running_tests or "cpp_point" in running_tests:
        test_cpp_point()
    if "all" in running_tests or "cpp_measure" in running_tests:
        test_cpp_measure()
