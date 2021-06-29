import sys
import logging
import cv2
from ikomia import core
import matplotlib.pyplot as plt
import matplotlib.patches as patches

logger = logging.getLogger(__name__)


def display(label="", imageio=None, graphicsio=None, **kwargs):
    if imageio is not None:
        _display_image(label, imageio, **kwargs)

    if graphicsio is not None:
        _display_graphics(label, graphicsio, **kwargs)


def _display_image(label, imageio, **kwargs):
    img = imageio.getImage()

    if img is None:
        logger.error("Image display error: image is empty.")
    else:
        cv2.imshow(label, img)


def _display_graphics(label, io, **kwargs):
    fig, ax = plt.subplots()
    ax.set_title(label)
    items = io.getItems()
    x_min = y_min = sys.maxsize
    x_max = y_max = 0

    def update_min_max(x, y):
        nonlocal x_min, y_min, x_max, y_max
        x_min = min(x_min, x)
        y_min = min(y_min, y)
        x_max = max(x_max, x)
        y_max = max(y_max, y)

    for item in items:
        item_type = item.getType()
        prop = core.GraphicsRectProperty()

        if item_type == core.GraphicsItem.ELLIPSE:
            xc = item.x + item.width/2
            yc = item.x + item.height / 2
            ellipse = patches.Ellipse(xy=[xc, yc], width=item.width, height=item.height, **kwargs)
            ax.add_patch(ellipse)
            update_min_max(item.x, item.y)
            update_min_max(item.x + item.width, item.y + item.height)
        elif item_type == core.GraphicsItem.RECTANGLE:
            ellipse = patches.Rectangle(xy=[item.x, item.y], width=item.width, height=item.height, **kwargs)
            ax.add_patch(ellipse)
            update_min_max(item.x, item.y)
            update_min_max(item.x + item.width, item.y + item.height)
        elif item_type == core.GraphicsItem.POLYLINE:
            pts = []
            for pt in item.points:
                pts.append((pt.x, pt.y))
                update_min_max(pt.x, pt.y)

            poly = patches.Polygon(pts, closed=False, **kwargs)
            ax.add_patch(poly)
        elif item_type == core.GraphicsItem.POLYGON:
            pts = []
            for pt in item.points:
                pts.append((pt.x, pt.y))
                update_min_max(pt.x, pt.y)

            poly = patches.Polygon(pts, closed=True, **kwargs)
            ax.add_patch(poly)
        elif item_type == core.GraphicsItem.COMPLEX_POLYGON:
            pts = []
            for pt in item.outer:
                pts.append((pt.x, pt.y))
                update_min_max(pt.x, pt.y)

            outer = patches.Polygon(pts, closed=True, **kwargs)
            ax.add_patch(outer)

            for inner in item.inners:
                pts = []
                for pt in inner:
                    pts.append((pt.x, pt.y))
                    update_min_max(pt.x, pt.y)

                inner = patches.Polygon(pts, closed=True, **kwargs)
                ax.add_patch(inner)

    ax.set_xlim(x_min, x_max)
    ax.set_ylim(y_min, y_max)
    ax.invert_yaxis()
    plt.show()
