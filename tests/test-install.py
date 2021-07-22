import logging
import numpy as np
from ikomia import utils, core, dataprocess, dnn
from ikomia.core.task import TaskParam

logger = logging.getLogger(__name__)


def test_utils():
    print("----- Utils module -----")
    try:
        cpp_exc = utils.CException()

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
    print("----- Core module -----")
    try:
        io = core.CWorkflowTaskIO()
        task = core.CWorkflowTask("NewTask")
        logger.info("Task name:" + task.name)
        param = core.CWorkflowTaskParam()
        point2d = core.CPointF(0.0, 0.0)
        complex_poly = core.CGraphicsComplexPolygon()
        ellipse = core.CGraphicsEllipse()
        point = core.CGraphicsPoint()
        polygon = core.CGraphicsPolygon()
        polyline = core.CGraphicsPolyline()
        rect = core.CGraphicsRectangle()
        graphics_conv = core.CGraphicsConversion(512, 512)
    except Exception as e:
        logger.error(e)


def test_dataprocess():
    print("----- DataProcess module -----")
    try:
        image_io = dataprocess.CImageIO(core.IODataType.IMAGE)
        video_io = dataprocess.CVideoIO(core.IODataType.VIDEO)
        feat_io = dataprocess.CDblFeatureIO()
        graphics_in = dataprocess.CGraphicsInput()
        graphics_out = dataprocess.CGraphicsOutput()
        path_io = dataprocess.CPathIO(core.IODataType.FOLDER_PATH, "/home")
        array_io = dataprocess.CArrayIO(np.arange(20))

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
    except Exception as e:
        logger.error(e)


def test_dnn():
    print("----- DNN module -----")
    try:
        train_param = TaskParam()
        train_task = dnn.TrainProcess("Train-task", train_param)
        logger.info("Task name:" + train_task.name)

        datasetio = dnn.IkDatasetIO("new-format")
    except Exception as e:
        logger.error(e)


if __name__ == "__main__":
    test_utils()
    test_core()
    test_dataprocess()
    test_dnn()
    logger.info("Install test run successfully")
