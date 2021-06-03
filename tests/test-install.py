import logging
import numpy as np
from ikomia import utils, core, dataprocess, dnn


def test_utils():
    print("----- Utils module -----")
    try:
        cpp_exc = utils.CException()

        memory = utils.CMemoryInfo()
        print("Total memory:", memory.totalMemory())
        print("Available memory:", memory.availableMemory())
        print("Memory load:", memory.memoryLoad())

        timer = utils.CTimer()
        timer.start()
        timer.printElapsedTime_ms("Elapsed time")
        timer.printTotalElapsedTime_ms("Total elapsed time")
    except Exception as e:
        logging.error(e.message)


def test_core():
    print("----- Core module -----")
    try:
        io = core.CProtocolTaskIO()
        task = core.CProtocolTask("NewTask")
        print("Task name:", task.name)
        param = core.CProtocolTaskParam()
        point2d = core.CPointF(0.0, 0.0)
        complex_poly = core.CGraphicsComplexPolygon()
        ellipse = core.CGraphicsEllipse()
        point = core.CGraphicsPoint()
        polygon = core.CGraphicsPolygon()
        polyline = core.CGraphicsPolyline()
        rect = core.CGraphicsRectangle()
        graphics_conv = core.CGraphicsConversion(512, 512)
    except Exception as e:
        logging.error(e.message)


def test_dataprocess():
    print("----- DataProcess module -----")
    try:
        image_io = dataprocess.CImageProcessIO(core.IODataType.IMAGE)
        video_io = dataprocess.CVideoProcessIO(core.IODataType.VIDEO)
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

        image_task = dataprocess.CImageProcess2d("Image-task")
        print("Task name:", image_task.name)
        video_task = dataprocess.CVideoProcess("Video-task")
        print("Task name:", video_task.name)
        video_of_task = dataprocess.CVideoProcess("Video-optical-flow-task")
        print("Task name:", video_of_task.name)
        video_tracking = dataprocess.CVideoProcessTracking("Video-tracking-task")
        print("Task name:", video_tracking.name)

        info = dataprocess.CProcessInfo()
    except Exception as e:
        logging.error(e.message)


def test_dnn():
    print("----- DNN module -----")
    try:
        train_param = dnn.TrainParam()
        train_task = dnn.TrainProcess("Train-task", train_param)
        print("Task name:", train_task.name)

        datasetio = dnn.IkDatasetIO("new-format")
    except Exception as e:
        logging.error(e.message)


if __name__ == "__main__":
    test_utils()
    test_core()
    test_dataprocess()
    test_dnn()
    print("Install test run successfully")
