import os
import cv2
import ikomia
from ikomia import core, dataprocess
from ikomia.dataprocess import displayIO


def get_test_image_directory():
    test_dir = os.path.dirname(os.path.abspath(__file__))
    return test_dir + "/../cpp/UnitTests/Data/Images"


def test_display_image():
    img_path = get_test_image_directory() + "/Lena.png"
    img = cv2.imread(img_path)
    io = dataprocess.CImageIO(core.IODataType.IMAGE, img)
    displayIO.display(label="Lena.png", imageio=io)
    cv2.waitKey(0)


def test_display_graphics():
    # graphics input

    # graphics output
    io = dataprocess.CGraphicsOutput()
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
    displayIO.display(label="Graphics scene", graphicsio=io, fill=False)


if __name__ == "__main__":
    ikomia.initialize("Ludo", "ludo?imageez")
    # test_display_image()
    test_display_graphics()
