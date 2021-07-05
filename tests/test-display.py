import os
import cv2
import ikomia
from ikomia import core, dataprocess
from ikomia.dataprocess import registry, displayIO


def get_test_image_directory():
    test_dir = os.path.dirname(os.path.abspath(__file__))
    return test_dir + "/../cpp/UnitTests/Data/Images"


def test_display_image():
    img_path = get_test_image_directory() + "/Lena.png"
    img = cv2.imread(img_path)
    io = dataprocess.CImageIO(core.IODataType.IMAGE, img)
    displayIO.display(io, label="Lena.png")


def test_display_graphics():
    # graphics output
    io = dataprocess.CGraphicsOutput()
    io.addPoint(core.CPointF(150, 80))
    io.addEllipse(0, 0, 100, 50)
    io.addRectangle(0, 60, 100, 50)
    points = [core.CPointF(0, 0), core.CPointF(30, 30), core.CPointF(100, 30), core.CPointF(130, 100)]
    io.addPolyline(points)
    pts_poly = [core.CPointF(50, 10), core.CPointF(100, 100), core.CPointF(80, 150), core.CPointF(20, 120), core.CPointF(20, 60)]
    io.addPolygon(pts_poly)
    io.addText("This is a text", 200, 10)
    pts_poly = [core.CPointF(250, 10), core.CPointF(300, 100), core.CPointF(280, 150), core.CPointF(220, 120), core.CPointF(220, 60)]
    pts_inner = [[core.CPointF(250, 100), core.CPointF(270, 70), core.CPointF(240, 40)]]
    io.addComplexPolygon(pts_poly, pts_inner)
    displayIO.display(io, label="Graphics scene")


def test_display_table():
    # load image
    img_path = get_test_image_directory() + "/example_05.jpg"
    img = cv2.imread(img_path)
    # initialize Ikomia registry
    reg = registry.IkomiaRegistry()
    # run ResNet classification
    algo = reg.create_algorithm("ResNet")
    input_img = algo.getInput(0)
    input_img.setImage(img)
    algo.run()
    # display table output
    displayIO.display(algo.getOutput(2), label="ResNet classification")

    # run YoloV4 detection
    algo = reg.create_algorithm("YoloV4")
    input_img = algo.getInput(0)
    input_img.setImage(img)
    algo.run()
    # display table output
    displayIO.display(algo.getOutput(2), label="YoloV4 detection")


def test_display_plot():
    # load image
    img_path = get_test_image_directory() + "/example_05.jpg"
    img = cv2.imread(img_path)
    # initialize Ikomia registry
    reg = registry.IkomiaRegistry()
    # run CalcHist
    algo = reg.create_algorithm("CalcHist")
    input_img = algo.getInput(0)
    input_img.setImage(img)
    algo.run()
    # display plot output
    feature_io = algo.getOutput(1)
    displayIO.display(feature_io, label="CalcHist Histogram")
    feature_io.setPlotType(dataprocess.PlotType.CURVE)
    displayIO.display(feature_io, label="CalcHist Curves")
    feature_io.setPlotType(dataprocess.PlotType.MULTIBAR)
    displayIO.display(feature_io, label="CalcHist Multi-bar")
    feature_io.setPlotType(dataprocess.PlotType.PIE)
    displayIO.display(feature_io, label="CalcHist Pie chart")


def test_display_task():
    # load image
    img_path = get_test_image_directory() + "/example_05.jpg"
    img = cv2.imread(img_path)
    # initialize Ikomia registry
    reg = registry.IkomiaRegistry()
    # run MaskRCNN
    algo = reg.create_algorithm("Mask RCNN")
    input_img = algo.getInput(0)
    input_img.setImage(img)
    algo.run()
    # display task I/O
    displayIO.display(algo, "Mask RCNN")


if __name__ == "__main__":
    ikomia.initialize("Ludo", "ludo?imageez")
    # test_display_image()
    # test_display_graphics()
    test_display_table()
    # test_display_plot()
    # test_display_task()
