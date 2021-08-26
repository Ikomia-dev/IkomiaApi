import logging
import ikomia
import cv2
from ikomia import core, dataprocess
from ikomia.dataprocess import displayIO, workflow
from ikomia.utils import tests

logger = logging.getLogger(__name__)


def test_display_image():
    logger.info("===== Test::display image IO =====")
    img_path = tests.get_test_image_directory() + "/Lena.png"
    img = cv2.imread(img_path)
    io = dataprocess.CImageIO(core.IODataType.IMAGE, img)
    displayIO.display(io, label="Lena.png")


def test_display_graphics():
    logger.info("===== Test::display graphics IO =====")
    # graphics input
    items = []
    io = dataprocess.CGraphicsInput()
    graphics_pt = core.CGraphicsPoint(core.CPointF(150, 80))
    graphics_ellipse = core.CGraphicsEllipse(0, 0, 100, 50)
    graphics_rect = core.CGraphicsRectangle(0, 60, 100, 50)
    graphics_polyline = core.CGraphicsPolyline([core.CPointF(0, 0),
                                                core.CPointF(30, 30),
                                                core.CPointF(100, 30),
                                                core.CPointF(130, 100)])
    graphics_polygon = core.CGraphicsPolygon([core.CPointF(50, 10),
                                              core.CPointF(100, 100),
                                              core.CPointF(80, 150),
                                              core.CPointF(20, 120),
                                              core.CPointF(20, 60)])
    graphics_text = core.CGraphicsText("This is a text", 200, 10)
    graphics_complexpoly = core.CGraphicsComplexPolygon(
        [core.CPointF(250, 10), core.CPointF(300, 100), core.CPointF(280, 150), core.CPointF(220, 120), core.CPointF(220, 60)],
        [[core.CPointF(250, 100), core.CPointF(270, 70), core.CPointF(240, 40)]])

    items.append(graphics_pt)
    items.append(graphics_ellipse)
    items.append(graphics_rect)
    items.append(graphics_polyline)
    items.append(graphics_polygon)
    items.append(graphics_text)
    items.append(graphics_complexpoly)
    io.setItems(items)
    displayIO.display(io, label="Input graphics scene")

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
    displayIO.display(io, label="Output graphics scene")


def test_display_table():
    logger.info("===== Test::display measure IO =====")
    # load image
    img_path = tests.get_test_image_directory() + "/example_05.jpg"
    img = cv2.imread(img_path)
    # run ResNet classification
    algo = ikomia.ik_registry.create_algorithm("ResNet")
    input_img = algo.getInput(0)
    input_img.setImage(img)
    algo.run()
    # display table output
    displayIO.display(algo.getOutput(2), label="ResNet classification")

    # run YoloV4 detection
    algo = ikomia.ik_registry.create_algorithm("YoloV4")
    input_img = algo.getInput(0)
    input_img.setImage(img)
    algo.run()
    # display table output
    displayIO.display(algo.getOutput(2), label="YoloV4 detection")


def test_display_plot():
    logger.info("===== Test::display numeric IO =====")
    # load image
    img_path = tests.get_test_image_directory() + "/example_05.jpg"
    img = cv2.imread(img_path)
    # run CalcHist
    algo = ikomia.ik_registry.create_algorithm("CalcHist")
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
    logger.info("===== Test::display task =====")
    # load image
    img_path = tests.get_test_image_directory() + "/example_05.jpg"
    img = cv2.imread(img_path)

    # run MaskRCNN
    algo = ikomia.ik_registry.create_algorithm("Mask RCNN")
    input_img = algo.getInput(0)
    input_img.setImage(img)
    algo.run()
    # display task I/O
    displayIO.display(algo, "Mask RCNN")


def test_display_workflow():
    logger.info("===== Test::display workflow =====")
    wf_path = tests.get_test_workflow_directory() + "/WorkflowTest1.json"
    wf = workflow.Workflow("Test Workflow", ikomia.ik_registry)
    wf.load(wf_path)
    displayIO.display(wf, wf.name)


if __name__ == "__main__":
    test_display_image()
    test_display_graphics()
    test_display_table()
    test_display_plot()
    test_display_task()
    test_display_workflow()
