import argparse
import logging
from ikomia import core, dataprocess

logger = logging.getLogger(__name__)


def _create_graphics_list():
    graphics = []
    pt = core.CGraphicsPoint(core.CPointF(150, 80))
    graphics.append(pt)

    ellipse = core.CGraphicsEllipse(0, 0, 100, 50)
    graphics.append(ellipse)

    rect = core.CGraphicsRectangle(0, 60, 100, 50)
    graphics.append(rect)

    polyline = core.CGraphicsPolyline([core.CPointF(0, 0),
                                       core.CPointF(30, 30),
                                       core.CPointF(100, 30),
                                       core.CPointF(130, 100)])
    graphics.append(polyline)

    polygon = core.CGraphicsPolygon([core.CPointF(50, 10),
                                     core.CPointF(100, 100),
                                     core.CPointF(80, 150),
                                     core.CPointF(20, 120),
                                     core.CPointF(20, 60)])
    graphics.append(polygon)

    text = core.CGraphicsText("This is a text", 200, 10)
    graphics.append(text)

    complex_polygon = core.CGraphicsComplexPolygon(
        [
            core.CPointF(250, 10),
            core.CPointF(300, 100),
            core.CPointF(280, 150),
            core.CPointF(220, 120),
            core.CPointF(220, 60)],
        [
            [core.CPointF(250, 100), core.CPointF(270, 70), core.CPointF(240, 40)]
        ]
    )
    graphics.append(complex_polygon)
    return graphics


def test_base():
    logger.info("===== Test: base class =====")
    # Base class
    item = core.CGraphicsItem(core.GraphicsItem.LAYER)
    logger.info(f"Item ID: {item.get_id()}")
    logger.info(f"Is text item: {item.is_text_item()}")
    assert item.get_type() == core.GraphicsItem.LAYER


def test_complex_polygon():
    logger.info("===== Test: complex polygon =====")
    # Default ctor
    item = core.CGraphicsComplexPolygon()
    logger.info(f"Item ID: {item.get_id()}")
    logger.info(f"Category: {item.get_category()}")
    logger.info(f"Is text item: {item.is_text_item()}")
    assert item.get_type() == core.GraphicsItem.COMPLEX_POLYGON
    assert len(item.outer) == 0
    assert len(item.inners) == 0
    assert type(item.property) == core.GraphicsPolygonProperty
    item.set_category("custom")
    assert(item.get_category() == "custom")
    rc = item.get_bounding_rect()
    assert len(rc) == 4

    # Ctor with outer and inners polygons
    outer = [core.CPointF(0.0, 0.0),
             core.CPointF(100.0, 0.0),
             core.CPointF(100.0, 100.0),
             core.CPointF(50.0, 150.0),
             core.CPointF(0.0, 100.0)]
    inners = [
        [core.CPointF(0.0, 0.0), core.CPointF(30.0, 0.0), core.CPointF(15.0, 50.0)],
        [core.CPointF(50.0, 50.0), core.CPointF(80.0, 0.0), core.CPointF(65.0, 100.0)]
    ]
    item = core.CGraphicsComplexPolygon(outer, inners)
    assert len(item.outer) == 5
    assert len(item.inners) == 2

    # Ctor with outer, inners polygons and property
    prop = core.GraphicsPolygonProperty()
    prop.pen_color = [0, 0, 255, 255]
    prop.brush_color = [0, 0, 128, 255]
    prop.line_size = 2
    prop.category = "Pentagon"
    item = core.CGraphicsComplexPolygon(outer, inners, prop)
    assert item.property.pen_color == prop.pen_color
    assert item.property.brush_color == prop.brush_color
    assert item.property.line_size == prop.line_size
    assert item.property.category == prop.category


def test_ellipse():
    logger.info("===== Test: ellipse =====")
    # Default ctor
    item = core.CGraphicsEllipse()
    logger.info(f"Item ID: {item.get_id()}")
    logger.info(f"Category: {item.get_category()}")
    logger.info(f"Is text item: {item.is_text_item()}")
    assert item.get_type() == core.GraphicsItem.ELLIPSE
    assert type(item.property) == core.GraphicsEllipseProperty
    item.set_category("custom")
    assert (item.get_category() == "custom")
    rc = item.get_bounding_rect()
    assert len(rc) == 4

    # Ctor with coordinates
    left = 0.0
    top = 0.0
    width = 200.0
    height = 100.0
    item = core.CGraphicsEllipse(left, top, width, height)
    assert item.x == left
    assert item.y == top
    assert item.width == width
    assert item.height == height

    # Ctor with coordinates and property
    prop = core.GraphicsEllipseProperty()
    prop.pen_color = [0, 0, 255, 255]
    prop.brush_color = [0, 0, 128, 255]
    prop.line_size = 2
    prop.category = "Ellipse"
    item = core.CGraphicsEllipse(left, top, width, height, prop)
    assert item.property.pen_color == prop.pen_color
    assert item.property.brush_color == prop.brush_color
    assert item.property.line_size == prop.line_size
    assert item.property.category == prop.category


def test_point():
    logger.info("===== Test: point =====")
    # Default ctor
    item = core.CGraphicsPoint()
    logger.info(f"Item ID: {item.get_id()}")
    logger.info(f"Category: {item.get_category()}")
    logger.info(f"Is text item: {item.is_text_item()}")
    assert item.get_type() == core.GraphicsItem.POINT
    assert type(item.property) == core.GraphicsPointProperty
    item.set_category("custom")
    assert (item.get_category() == "custom")
    rc = item.get_bounding_rect()
    assert len(rc) == 4

    # Ctor with coordinates
    pt = core.CPointF(10.0, 15.0)
    item = core.CGraphicsPoint(pt)
    assert item.point.x == pt.x
    assert item.point.y == pt.y

    # Ctor with coordinates and property
    prop = core.GraphicsPointProperty()
    prop.pen_color = [0, 0, 255, 255]
    prop.brush_color = [0, 0, 128, 255]
    prop.size = 2
    prop.category = "Point"
    item = core.CGraphicsPoint(pt, prop)
    assert item.property.pen_color == prop.pen_color
    assert item.property.brush_color == prop.brush_color
    assert item.property.size == prop.size
    assert item.property.category == prop.category


def test_polygon():
    logger.info("===== Test: polygon =====")
    # Default ctor
    item = core.CGraphicsPolygon()
    logger.info(f"Item ID: {item.get_id()}")
    logger.info(f"Category: {item.get_category()}")
    logger.info(f"Is text item: {item.is_text_item()}")
    assert item.get_type() == core.GraphicsItem.POLYGON
    assert len(item.points) == 0
    assert type(item.property) == core.GraphicsPolygonProperty
    item.set_category("custom")
    assert(item.get_category() == "custom")
    rc = item.get_bounding_rect()
    assert len(rc) == 4

    # Ctor with points
    pts = [core.CPointF(0.0, 0.0),
           core.CPointF(100.0, 0.0),
           core.CPointF(100.0, 100.0),
           core.CPointF(50.0, 150.0),
           core.CPointF(0.0, 100.0)]
    item = core.CGraphicsPolygon(pts)
    assert len(item.points) == len(pts) + 1

    # Ctor with outer, inners polygons and property
    prop = core.GraphicsPolygonProperty()
    prop.pen_color = [0, 0, 255, 255]
    prop.brush_color = [0, 0, 128, 255]
    prop.line_size = 2
    prop.category = "Pentagon"
    item = core.CGraphicsPolygon(pts, prop)
    assert item.property.pen_color == prop.pen_color
    assert item.property.brush_color == prop.brush_color
    assert item.property.line_size == prop.line_size
    assert item.property.category == prop.category


def test_polyline():
    logger.info("===== Test: polyline =====")
    # Default ctor
    item = core.CGraphicsPolyline()
    logger.info(f"Item ID: {item.get_id()}")
    logger.info(f"Category: {item.get_category()}")
    logger.info(f"Is text item: {item.is_text_item()}")
    assert item.get_type() == core.GraphicsItem.POLYLINE
    assert len(item.points) == 0
    assert type(item.property) == core.GraphicsPolylineProperty
    item.set_category("custom")
    assert(item.get_category() == "custom")
    rc = item.get_bounding_rect()
    assert len(rc) == 4

    # Ctor with points
    pts = [core.CPointF(0.0, 0.0),
           core.CPointF(100.0, 0.0),
           core.CPointF(100.0, 100.0),
           core.CPointF(50.0, 150.0),
           core.CPointF(0.0, 100.0)]
    item = core.CGraphicsPolyline(pts)
    assert len(item.points) == len(pts)

    # Ctor with outer, inners polygons and property
    prop = core.GraphicsPolylineProperty()
    prop.pen_color = [0, 0, 255, 255]
    prop.line_size = 2
    prop.category = "Polyline"
    item = core.CGraphicsPolyline(pts, prop)
    assert item.property.pen_color == prop.pen_color
    assert item.property.line_size == prop.line_size
    assert item.property.category == prop.category


def test_rectangle():
    logger.info("===== Test: rectangle =====")
    # Default ctor
    item = core.CGraphicsRectangle()
    logger.info(f"Item ID: {item.get_id()}")
    logger.info(f"Category: {item.get_category()}")
    logger.info(f"Is text item: {item.is_text_item()}")
    assert item.get_type() == core.GraphicsItem.RECTANGLE
    assert type(item.property) == core.GraphicsRectProperty
    item.set_category("custom")
    assert (item.get_category() == "custom")
    rc = item.get_bounding_rect()
    assert len(rc) == 4

    # Ctor with coordinates
    left = 0.0
    top = 0.0
    width = 200.0
    height = 100.0
    item = core.CGraphicsRectangle(left, top, width, height)
    assert item.x == left
    assert item.y == top
    assert item.width == width
    assert item.height == height

    # Ctor with coordinates and property
    prop = core.GraphicsRectProperty()
    prop.pen_color = [0, 0, 255, 255]
    prop.brush_color = [0, 0, 128, 255]
    prop.line_size = 2
    prop.category = "Rectangle"
    item = core.CGraphicsRectangle(left, top, width, height, prop)
    assert item.property.pen_color == prop.pen_color
    assert item.property.brush_color == prop.brush_color
    assert item.property.line_size == prop.line_size
    assert item.property.category == prop.category


def test_text():
    logger.info("===== Test: text =====")
    # Default ctor
    item = core.CGraphicsText()
    logger.info(f"Item ID: {item.get_id()}")
    logger.info(f"Category: {item.get_category()}")
    logger.info(f"Is text item: {item.is_text_item()}")
    assert item.get_type() == core.GraphicsItem.TEXT
    assert type(item.property) == core.GraphicsTextProperty
    item.set_category("custom")
    assert (item.get_category() == "custom")
    rc = item.get_bounding_rect()
    assert len(rc) == 4

    # Ctor with label
    label = "Hello"
    item = core.CGraphicsText(label)
    assert item.text == label
    rc = item.get_bounding_rect()
    assert len(rc) == 4

    # Ctor with label and position
    x = 10.0
    y = 15.0
    item = core.CGraphicsText(label, x, y)
    assert item.x == x
    assert item.y == y

    # Ctor with label, position and property
    prop = core.GraphicsTextProperty()
    prop.color = [0, 0, 255, 255]
    prop.font_name = "Arial"
    prop.font_size = 12
    prop.bold = False
    prop.italic = False
    prop.underline = False
    prop.strikeout = False
    item = core.CGraphicsText(label, x, y, prop)
    assert item.property.color == prop.color
    assert item.property.font_name == prop.font_name
    assert item.property.font_size == prop.font_size
    assert item.property.bold == prop.bold
    assert item.property.italic == prop.italic
    assert item.property.underline == prop.underline
    assert item.property.strikeout == prop.strikeout


def test_image_conversion():
    logger.info("===== Test: graphics to image =====")
    graphics = _create_graphics_list()
    conv = core.CGraphicsConversion(512, 512)
    mask = conv.graphics_to_binary_mask(graphics)
    assert mask is not None
    logger.info(mask.shape)


def test_polymorphism():
    logger.info("===== Test: polymorphism =====")

    class MyGraphicsItem(core.CGraphicsItem):
        def __init__(self):
            core.CGraphicsItem.__init__(self, core.GraphicsItem.LAYER)
            self.category = "layer"
            self.rc = [0.0, 0.0, 0.0, 0.0]
            self.text = "layer"

        def get_bounding_rect(self):
            return self.rc

        def get_category(self):
            return self.category

        def insert_to_image(self):
            pass

        def is_text_item(self):
            return self.text != ""

        def set_category(self, category):
            self.category = category

    items = _create_graphics_list()
    items.append(MyGraphicsItem())
    io = dataprocess.CGraphicsInput()
    io.set_items(items)

    inserted_items = io.get_items()
    for item in inserted_items:
        logger.info(f"Item ID: {item.get_id()}")
        logger.info(f"Item type: {item.get_type()}")
        logger.info(f"Category: {item.get_category()}")
        logger.info(f"Is text item: {item.is_text_item()}")
        item.set_category("custom")
        assert (item.get_category() == "custom")
        rc = item.get_bounding_rect()
        assert len(rc) == 4


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--tests",
                        type=str,
                        default='all',
                        help="List of tests to execute (comma-separated string, default=all)")

    opt = parser.parse_args()
    running_tests = opt.tests.split(',')

    if "all" in running_tests or "base" in running_tests:
        test_base()
    if "all" in running_tests or "complex_poly" in running_tests:
        test_complex_polygon()
    if "all" in running_tests or "ellipse" in running_tests:
        test_ellipse()
    if "all" in running_tests or "point" in running_tests:
        test_point()
    if "all" in running_tests or "polygon" in running_tests:
        test_polygon()
    if "all" in running_tests or "polyline" in running_tests:
        test_polyline()
    if "all" in running_tests or "rectangle" in running_tests:
        test_rectangle()
    if "all" in running_tests or "text" in running_tests:
        test_text()
    if "all" in running_tests or "image_conversion" in running_tests:
        test_image_conversion()
    if "all" in running_tests or "polymorphism" in running_tests:
        test_polymorphism()
