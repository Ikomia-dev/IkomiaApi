import sys
import logging
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
    matplotlib.use("TkAgg")
    fig, ax = plt.subplots(1, 1)
    ax.set_title(label)
    ax.imshow(obj.getImage())
    ax.axis("off")
    fig.tight_layout()
    plt.show()


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
    if "parent" in kwargs:
        fig = kwargs["parent"]
    else:
        fig = None

    labels = obj.getAllLabelList()
    col_labels = obj.getAllHeaderLabels()
    values = obj.getAllValueList()
    out_type = obj.getOutputType()

    if out_type == dataprocess.NumericOutputType.TABLE:
        if fig is None:
            fig, ax = plt.subplots(1, 1)
        else:
            ax = fig.subplots(1, 1)

        ax.set_title(label)
        cell_length_limit = 50
        rows_count = len(values[0])
        cols_count = len(values)
        np_values = np.asarray(values)
        np_values = np_values.reshape((rows_count, cols_count))

        if len(labels) == 1:
            row_labels = list(flatten(labels))
        else:
            row_labels = labels[0]

        for i, label in enumerate(row_labels):
            label = (label[:cell_length_limit] + '..') if len(label) > cell_length_limit else label
            row_labels[i] = label

        col_width = [1 / (cols_count + 1)] * cols_count
        ax.table(cellText=np_values, colLabels=col_labels, rowLabels=row_labels, cellLoc="center", loc="upper right",
                 colWidths=col_width)
        ax.axis("off")

    elif out_type == dataprocess.NumericOutputType.PLOT:
        plot_type = obj.getPlotType()

        # HISTOGRAM
        if plot_type == dataprocess.PlotType.HISTOGRAM or plot_type == dataprocess.PlotType.BAR:
            if fig is None:
                fig, ax = plt.subplots(1, 1)
            else:
                ax = fig.subplots(1, 1)

            has_x_values = len(labels) > 0

            for i in range(len(values)):
                if len(col_labels) == 0 or len(col_labels) != len(values):
                    name = "serie" + str(i+1)
                else:
                    name = col_labels[i]

                if not has_x_values:
                    x = []
                    for j in range(len(values[i])):
                        x.append(j)

                    ax.bar(x, values[i], label=name)
                elif i >= len(labels):
                    ax.bar(labels[0], values[i], label=name)
                else:
                    ax.bar(labels[i], values[i], label=name)
            ax.legend()
        # CURVE
        elif plot_type == dataprocess.PlotType.CURVE:
            if fig is None:
                fig, ax = plt.subplots(1, 1)
            else:
                ax = fig.subplots(1, 1)

            has_x_values = len(labels) > 0

            for i in range(len(values)):
                if len(col_labels) == 0 or len(col_labels) != len(values):
                    name = "serie" + str(i + 1)
                else:
                    name = col_labels[i]

                if not has_x_values:
                    x = []
                    for j in range(len(values[i])):
                        x.append(j)

                    ax.plot(x, values[i], label=name)
                elif i >= len(labels):
                    ax.plot(labels[0], values[i], label=name)
                else:
                    ax.plot(labels[i], values[i], label=name)
            ax.legend()
        # MULTI-BAR
        elif plot_type == dataprocess.PlotType.MULTIBAR:
            if fig is None:
                fig, ax = plt.subplots(1, len(values))
            else:
                ax = fig.subplots(1, len(values))

            has_x_values = len(labels) > 0

            for i in range(len(values)):
                if len(col_labels) == 0 or len(col_labels) != len(values):
                    name = "serie" + str(i + 1)
                else:
                    name = col_labels[i]

                ax[i].set_title(name)

                if not has_x_values:
                    x = []
                    for j in range(len(values[i])):
                        x.append(j)

                    ax[i].bar(x, values[i])
                elif i >= len(labels):
                    ax[i].bar(labels[0], values[i])
                else:
                    ax[i].bar(labels[i], values[i])
        # PIE CHART
        elif plot_type == dataprocess.PlotType.PIE:
            if fig is None:
                fig, ax = plt.subplots(1, len(values))
            else:
                ax = fig.subplots(1, len(values))

            has_labels = len(labels) > 0

            for i in range(len(values)):
                if not has_labels or i >= len(labels):
                    ax[i].pie(values[i], shadow=True)
                else:
                    ax[i].pie(x=values[i], labels=labels[i], shadow=True)

    fig.suptitle(label)
    fig.tight_layout()
    plt.show()
