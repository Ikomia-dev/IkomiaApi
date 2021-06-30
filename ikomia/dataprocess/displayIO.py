import sys
import logging
import cv2
import numpy as np
from functools import singledispatch
from ikomia import core, dataprocess
import matplotlib
import matplotlib.pyplot as plt
import matplotlib.patches as patches
from matplotlib.cbook import flatten

logger = logging.getLogger(__name__)


def _to_plot_color(color):
    plot_color = [val / 255.0 for val in color]
    return plot_color


@singledispatch
def display(obj, label="", **kwargs):
    raise NotImplementedError("Unsupported type")


@display.register
def _(obj: dataprocess.CImageIO, label="", **kwargs):
    img = obj.getImage()

    if img is None:
        logger.error("Image display error: image is empty.")
    else:
        cv2.imshow(label, img)

    cv2.waitKey(0)


@display.register
def _(obj: dataprocess.CGraphicsOutput, label="", **kwargs):
    matplotlib.use("TkAgg")
    fig, ax = plt.subplots(1, 1)
    ax.set_title(label)
    items = obj.getItems()
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

        if item_type == core.GraphicsItem.ELLIPSE:
            xc = item.x + item.width / 2
            yc = item.x + item.height / 2
            ellipse = patches.Ellipse(xy=[xc, yc], width=item.width, height=item.height,
                                      edgecolor=_to_plot_color(item.property.pen_color),
                                      facecolor=_to_plot_color(item.property.brush_color), **kwargs)
            ax.add_patch(ellipse)
            update_min_max(item.x, item.y)
            update_min_max(item.x + item.width, item.y + item.height)
        elif item_type == core.GraphicsItem.POINT:
            circle = patches.Circle(xy=[item.point.x, item.point.y], radius=item.property.size / 2,
                                    edgecolor=_to_plot_color(item.property.pen_color),
                                    facecolor=_to_plot_color(item.property.brush_color), **kwargs)
            ax.add_patch(circle)
            update_min_max(item.point.x, item.point.y)
        elif item_type == core.GraphicsItem.RECTANGLE:
            ellipse = patches.Rectangle(xy=[item.x, item.y], width=item.width, height=item.height,
                                        edgecolor=_to_plot_color(item.property.pen_color),
                                        facecolor=_to_plot_color(item.property.brush_color), **kwargs)
            ax.add_patch(ellipse)
            update_min_max(item.x, item.y)
            update_min_max(item.x + item.width, item.y + item.height)
        elif item_type == core.GraphicsItem.POLYLINE:
            pts = []
            for pt in item.points:
                pts.append((pt.x, pt.y))
                update_min_max(pt.x, pt.y)

            poly = patches.Polygon(pts, closed=False, fill=False,
                                   edgecolor=_to_plot_color(item.property.pen_color), **kwargs)
            ax.add_patch(poly)
        elif item_type == core.GraphicsItem.POLYGON:
            pts = []
            for pt in item.points:
                pts.append((pt.x, pt.y))
                update_min_max(pt.x, pt.y)

            poly = patches.Polygon(pts, closed=True,
                                   edgecolor=_to_plot_color(item.property.pen_color),
                                   facecolor=_to_plot_color(item.property.brush_color), **kwargs)
            ax.add_patch(poly)
        elif item_type == core.GraphicsItem.COMPLEX_POLYGON:
            pts = []
            for pt in item.outer:
                pts.append((pt.x, pt.y))
                update_min_max(pt.x, pt.y)

            outer = patches.Polygon(pts, closed=True,
                                    edgecolor=_to_plot_color(item.property.pen_color),
                                    facecolor=_to_plot_color(item.property.brush_color), **kwargs)
            ax.add_patch(outer)

            for inner in item.inners:
                pts = []
                for pt in inner:
                    pts.append((pt.x, pt.y))
                    update_min_max(pt.x, pt.y)

                inner = patches.Polygon(pts, closed=True, fill=False,
                                        edgecolor=_to_plot_color(item.property.pen_color),
                                        facecolor=_to_plot_color(item.property.brush_color), **kwargs)
                ax.add_patch(inner)
        elif item_type == core.GraphicsItem.TEXT:
            ax.text(item.x, item.y, item.text, size=item.property.font_size, color=_to_plot_color(item.property.color))
            update_min_max(item.x, item.y)

    if x_min == x_max:
        x_min = 0
        x_max = 2*x_max

    if y_min == y_max:
        y_min = 0
        y_max = 2*y_max

    ax.set_xlim(x_min, x_max)
    ax.set_ylim(y_min, y_max)
    ax.invert_yaxis()
    plt.gca().set_aspect('equal', adjustable='box')
    plt.show()


@display.register
def _(obj: dataprocess.CDblFeatureIO, label="", **kwargs):
    matplotlib.use("TkAgg")
    fig, ax = plt.subplots(1, 1)
    ax.set_title(label)
    out_type = obj.getOutputType()

    if out_type == dataprocess.NumericOutputType.TABLE:
        labels = obj.getAllLabelList()
        col_labels = obj.getAllHeaderLabels()
        values = obj.getAllValueList()
        rows_count = len(values[0])
        cols_count = len(values)
        np_values = np.asarray(values)
        np_values = np_values.reshape((rows_count, cols_count))

        if len(labels) == 1:
            row_labels = list(flatten(labels))
        else:
            row_labels = labels[0]

        ax.set_axis_off()
        ax.table(cellText=np_values, colLabels=col_labels, rowLabels=row_labels, cellLoc="center", loc="upper left")
        plt.box(on=None)
    elif out_type == dataprocess.NumericOutputType.PLOT:
        pass

    plt.show()
