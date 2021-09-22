import logging
import numpy as np
import ikomia
from ikomia import utils, core, dataprocess, dnn
from ikomia.core.task import TaskParam

logger = logging.getLogger(__name__)


def test_utils():
    logger.info("===== Test::Utils module =====")
    try:
        cpp_exc = utils.CException()
        assert cpp_exc

        memory = utils.CMemoryInfo()
        logger.info("Total memory:" + str(memory.totalMemory()))
        logger.info("Available memory:" + str(memory.availableMemory()))
        logger.info("Memory load:" + str(memory.memoryLoad()))

        timer = utils.CTimer()
        timer.start()
        timer.printElapsedTime_ms("Elapsed time")
        timer.printTotalElapsedTime_ms("Total elapsed time")
    except Exception as e:
        logger.error(e)


def test_core():
    logger.info("===== Test::Core module =====")
    try:
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
    except Exception as e:
        logger.error(e)


def test_dataprocess():
    logger.info("===== Test::DataProcess module =====")
    try:
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

        categories = dataprocess.MapIntStr()
        categories[0] = "boat"
        categories[1] = "kyte"
        categories[2] = "windsurf"
        categories[3] = "surf"

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

        logger.info("Installed algorithms : " + str(len(ikomia.ik_registry.getAlgorithms())))
    except Exception as e:
        logger.error(e)


def test_dnn():
    logger.info("===== Test::DNN module =====")
    try:
        train_param = TaskParam()
        train_task = dnn.TrainProcess("Train-task", train_param)
        logger.info("Task name:" + train_task.name)

        datasetio = dnn.IkDatasetIO("new-format")
        assert datasetio
    except Exception as e:
        logger.error(e)


if __name__ == "__main__":
    ikomia.authenticate()
    test_utils()
    test_core()
    test_dataprocess()
    test_dnn()
    logger.info("Install test run successfully")
