# Copyright (C) 2021 Ikomia SAS
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""
Module dedicated to the visualization of workflow components. The idea is to propose a list of basic visualization
functions for each type of components: workflow, task, input and output (for debug purpose essentially).
"""
import sys
import os
import logging
import numpy as np
from functools import singledispatch
from ikomia import core, dataprocess
import matplotlib
import matplotlib.patches as patches
from matplotlib.cbook import flatten

# Matplotlib backend choice
_backend_name = None
_valid_backend = True

try:
    matplotlib.use("Qt5Agg")
    import matplotlib.pyplot as plt
    _backend_name = "Qt5Agg"
except ImportError:
    try:
        matplotlib.use("GTK3Agg")
        import matplotlib.pyplot as plt
        _backend_name = "GTK3Agg"
    except ImportError:
        try:
            matplotlib.use("TkAgg")
            import matplotlib.pyplot as plt
            _backend_name = "TkAgg"
        except ImportError:
            try:
                import matplotlib.pyplot as plt
                _backend_name = plt.get_backend()
            except ImportError:
                _valid_backend = False

logger = logging.getLogger(__name__)


def _check_backend():
    if not _valid_backend:
        raise RuntimeError("No valid Matplotlib backend found.")

    if plt.get_backend() != _backend_name:
        matplotlib.use(_backend_name)


def _to_plot_color(color):
    plot_color = [val / 255.0 for val in color]
    return plot_color


@singledispatch
def display(obj, label="", fig=None, **kwargs):
    """
    Display function for various workflow components.
    """
    raise NotImplementedError("Unsupported type")


@display.register
def _(obj: dataprocess.CImageIO, label="", fig=None, **kwargs):
    """
    Display image input or output (:py:class:`~ikomia.dataprocess.pydataprocess.CImageIO`) in a Matplotlib figure.
    """
    _check_backend()

    if not obj.isDataAvailable:
        return

    if fig is not None:
        child = True
        ax = fig.subplots(1, 1)
    else:
        child = False
        fig, ax = plt.subplots(1, 1)

    ax.set_title(label)
    ax.imshow(obj.getImage())
    ax.axis("off")

    if not child:
        fig.tight_layout()
        plt.show()


@display.register
def _(obj: dataprocess.CGraphicsInput, label="", fig=None, **kwargs):
    """
    Display the scene of a graphics input (:py:class:`~ikomia.dataprocess.pydataprocess.CGraphicsInput`)
    in a Matplotlib figure.
    """
    _check_backend()

    if not obj.isDataAvailable:
        return

    if fig is not None:
        child = True
        ax = fig.subplots(1, 1)
    else:
        child = False
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

    if not child:
        fig.tight_layout()
        plt.show()


@display.register
def _(obj: dataprocess.CGraphicsOutput, label="", fig=None, **kwargs):
    """
    Display the scene of a graphics output (:py:class:`~ikomia.dataprocess.pydataprocess.CGraphicsOutput`)
    in a Matplotlib figure.
    """
    _check_backend()

    if not obj.isDataAvailable:
        return

    if fig is not None:
        child = True
        ax = fig.subplots(1, 1)
    else:
        child = False
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
            rect = patches.Rectangle(xy=[item.x, item.y], width=item.width, height=item.height,
                                     edgecolor=_to_plot_color(item.property.pen_color),
                                     facecolor=_to_plot_color(item.property.brush_color), **kwargs)
            ax.add_patch(rect)
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

    if not child:
        fig.tight_layout()
        plt.show()


@display.register
def _(obj: dataprocess.CNumericIO, label="", fig=None, **kwargs):
    """
    Display numeric values input or output (:py:class:`~ikomia.dataprocess.pydataprocess.CNumericIO`)
    as a table in a Matplotlib figure.
    """
    _check_backend()

    if not obj.isDataAvailable:
        return

    if fig is None:
        child = False
    else:
        child = True

    labels = obj.getAllLabelList()
    col_labels = obj.getAllHeaderLabels()
    values = obj.getAllValueList()
    out_type = obj.getOutputType()

    if out_type == dataprocess.NumericOutputType.TABLE:
        if fig is None:
            fig, ax = plt.subplots(1, 1)
        else:
            ax = fig.subplots(1, 1)

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
        table = ax.table(cellText=np_values, colLabels=col_labels, rowLabels=row_labels,
                         cellLoc="left", loc="upper right", colWidths=col_width)
        table.auto_set_font_size(False)
        table.set_fontsize(10)
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

    if not child:
        fig.tight_layout()
        plt.show()


@display.register
def _(obj: dataprocess.CBlobMeasureIO, label="", fig=None, **kwargs):
    """
    Display object measures input or output (:py:class:`~ikomia.dataprocess.pydataprocess.CBlobMeasureIO`)
    as a table in a Matplotlib figure.
    """
    _check_backend()

    if not obj.isDataAvailable:
        return

    if fig is not None:
        child = True
        ax = fig.subplots(1, 1)
    else:
        child = False
        fig, ax = plt.subplots(1, 1)

    measures = obj.getMeasures()
    row_labels = list(range(len(measures)))
    col_labels = ["Graphics ID", "Label"]
    max_value_length = 9

    for blob_measures in measures:
        for measure in blob_measures:
            measure_info = measure.getMeasureInfo()

            if measure_info.name not in col_labels:
                col_labels.append(measure_info.name)

    cols_count = len(col_labels)
    values = [["No data" for _ in range(cols_count)] for _ in range(len(measures))]

    if len(values) == 0:
        values = [["No data" for _ in range(cols_count)]]
        row_labels = ["No data"]

    for i, blob_measures in enumerate(measures):
        for measure in blob_measures:
            measure_info = measure.getMeasureInfo()
            values[i][0] = str(measure.graphicsId)
            values[i][1] = str(measure.label)

            try:
                j = col_labels.index(measure_info.name)
                if len(measure.values) == 1:
                    str_val = str(measure.values[0])
                    values[i][j] = (str_val[:max_value_length] + '...') if len(str_val) > max_value_length else str_val
                else:
                    measure_val = ""
                    for value in measure.values:
                        str_val = str(value)
                        str_val = (str_val[:max_value_length] + '...') if len(str_val) > max_value_length else str_val
                        measure_val += str_val + ";"
                    values[i][j] = measure_val
            except ValueError:
                continue

    col_width = [1 / (cols_count + 1)] * cols_count
    table = ax.table(cellText=values, colLabels=col_labels, rowLabels=row_labels,
                     cellLoc="left", loc="upper left", colWidths=col_width)
    table.auto_set_font_size(False)
    table.set_fontsize(10)

    ax.axis("off")
    fig.suptitle(label)

    if not child:
        fig.tight_layout()
        plt.show()


@display.register
def _(obj: dataprocess.CWorkflowTask, label="", **kwargs):
    """
    Display task inputs and outputs (:py:class:`~ikomia.core.pycore.CWorkflowTask`) in two separate Matplotlib figures.
    """
    _check_backend()

    # inputs
    inputs = obj.getInputs()
    input_count = len(inputs)
    in_fig = plt.figure(num=1, constrained_layout=True)
    in_fig.suptitle(label + " inputs")
    in_sub_figs = in_fig.subfigures(input_count, 1, wspace=0.07)

    if input_count == 1:
        try:
            display(inputs[0], type(inputs[0]).__name__, fig=in_sub_figs, **kwargs)
        except NotImplementedError:
            logger.error("No display function available for input 1")
    else:
        for i, task_input in enumerate(inputs):
            try:
                display(task_input, type(task_input).__name__, fig=in_sub_figs[i], **kwargs)
            except NotImplementedError:
                logger.error("No display function available for input " + str(i))

    # outputs
    outputs = obj.getOutputs()
    output_count = len(outputs)
    out_fig = plt.figure(num=2, constrained_layout=True)
    out_fig.suptitle(label + " outputs")
    out_sub_figs = out_fig.subfigures(output_count, 1, wspace=0.07)

    if output_count == 1:
        try:
            display(outputs[0], type(outputs[0]).__name__, fig=out_sub_figs, **kwargs)
        except NotImplementedError:
            logger.error("No display function available for output 1")
    else:
        for i, task_output in enumerate(outputs):
            try:
                display(task_output, type(task_output).__name__, fig=out_sub_figs[i], **kwargs)
            except NotImplementedError:
                logger.error("No display function available for output " + str(i))

    plt.show()


@display.register
def _(obj: dataprocess.CWorkflow, label="", **kwargs):
    """
    Display Graphviz representation of a workflow (:py:class:`~ikomia.dataprocess.pydataprocess.CWorkflow`).
    """
    from graphviz import Source
    dot_file_name = obj.name + ".dot"
    path = os.path.join(core.config.main_cfg["data"]["path"], dot_file_name)
    obj.exportGraphviz(path)
    s = Source.from_file(path)
    s.view()

