import logging
import argparse
import numpy as np
from ikomia import utils, core, dataprocess, dnn, authenticate
from ikomia.core.task import TaskParam
from ikomia.dataprocess.registry import ik_registry

logger = logging.getLogger(__name__)


def test_utils():
    logger.info("===== Test::Utils module =====")
    cpp_exc = utils.CException()
    assert cpp_exc

    memory = utils.CMemoryInfo()
    total_mem = memory.get_total_memory()
    assert total_mem
    available_mem = memory.get_available_memory()
    assert available_mem
    mem_load = memory.get_memory_load()
    assert mem_load

    timer = utils.CTimer()
    timer.start()
    timer.get_elapsed_ns()
    timer.get_elapsed_us()
    timer.get_elapsed_ms()
    timer.get_total_elapsed_ns()
    timer.get_total_elapsed_us()
    timer.get_total_elapsed_ms()
    timer.print_elapsed_time_ms("Elapsed time")
    timer.print_total_elapsed_time_ms("Total elapsed time")


def test_core():
    logger.info("===== Test::Core module =====")
    io = core.CWorkflowTaskIO()
    assert io
    task = core.CWorkflowTask("NewTask")
    logger.info("Task name:" + task.name)
    param = core.CWorkflowTaskParam()
    assert param
    point2d = core.CPointF(0.0, 0.0)
    assert point2d
    complex_poly = core.CGraphicsComplexPolygon()
    assert complex_poly
    ellipse = core.CGraphicsEllipse()
    assert ellipse
    point = core.CGraphicsPoint()
    assert point
    polygon = core.CGraphicsPolygon()
    assert polygon
    polyline = core.CGraphicsPolyline()
    assert polyline
    rect = core.CGraphicsRectangle()
    assert rect
    graphics_conv = core.CGraphicsConversion(512, 512)
    assert graphics_conv


def test_dataprocess():
    logger.info("===== Test::DataProcess module =====")
    image_io = dataprocess.CImageIO(core.IODataType.IMAGE)
    assert image_io
    video_io = dataprocess.CVideoIO(core.IODataType.VIDEO)
    assert video_io
    numeric_io = dataprocess.CNumericIO()
    assert numeric_io
    graphics_in = dataprocess.CGraphicsInput()
    assert graphics_in
    graphics_out = dataprocess.CGraphicsOutput()
    assert graphics_out
    path_io = dataprocess.CPathIO(core.IODataType.FOLDER_PATH, "/home")
    assert path_io
    array_io = dataprocess.CArrayIO(np.arange(20), "numpy array")
    assert array_io

    image_task = dataprocess.C2dImageTask("Image-task")
    logger.info("Task name:" + image_task.name)
    video_task = dataprocess.CVideoTask("Video-task")
    logger.info("Task name:" + video_task.name)
    video_of_task = dataprocess.CVideoOFTask("Video-optical-flow-task")
    logger.info("Task name:" + video_of_task.name)
    video_tracking = dataprocess.CVideoTrackingTask("Video-tracking-task")
    logger.info("Task name:" + video_tracking.name)

    info = dataprocess.CTaskInfo()
    assert info

    logger.info("Installed algorithms : " + str(len(ik_registry.get_algorithms())))


def test_dnn():
    logger.info("===== Test::DNN module =====")
    train_param = TaskParam()
    train_task = dnn.TrainProcess("Train-task", train_param)
    logger.info("Task name:" + train_task.name)

    datasetio = dnn.IkDatasetIO("new-format")
    assert datasetio


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--tests",
                        type=str,
                        default='all',
                        help="List of tests to execute (comma-separated string, default=all)")
    opt = parser.parse_args()
    running_tests = opt.tests.split(',')

    authenticate()

    if 'all' in running_tests or 'utils' in running_tests:
        test_utils()
    if 'all' in running_tests or 'core' in running_tests:
        test_core()
    if 'all' in running_tests or 'dataprocess' in running_tests:
        test_dataprocess()
    if 'all' in running_tests or 'dnn' in running_tests:
        test_dnn()

    logger.info("Install test run successfully")
