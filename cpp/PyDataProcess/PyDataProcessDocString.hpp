// Copyright (C) 2021 Ikomia SAS
// Contact: https://www.ikomia.com
//
// This file is part of the Ikomia API libraries.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#ifndef PYDATAPROCESSDOCSTRING_HPP
#define PYDATAPROCESSDOCSTRING_HPP

constexpr auto _moduleDocString =
        "Module offering all features to implement image processing plugins that can be used in the Ikomia workflow. "
        "The system uses factory design pattern to allow integration of third-party plugins. "
        "So when you want to add your own plugin, you have to implement (override) three factory classes derived from the followings:\n\n"
        "- :py:class:`~ikomia.dataprocess.pydataprocess.CPluginProcessInterface`: abstract base class exposing the two factories required by the plugin engine (task and widget)\n"
        "- :py:class:`~ikomia.dataprocess.pydataprocess.CTaskFactory`: abstract base class for process instanciation\n"
        "- :py:class:`~ikomia.dataprocess.pydataprocess.CWidgetFactory`: abstract base class for widget instanciation\n\n"
        "This module provides class specialization for several types of usual inputs/outputs.\n"
        "It also provides class specialization for common processing task.\n"
        "You will find details about implementation below.\n\n";

//---------------------//
//----- CTaskInfo -----//
//---------------------//
constexpr auto _processInfoDocString =
        "Manage metadata associated with a task. "
        "Information are then available for consulting in the software. "
        "These metadata are also used by the system search engine (process library and store).\n";

//------------------------//
//----- CTaskFactory -----//
//------------------------//
constexpr auto _processFactoryDocString =
        "Abstract class defining the core structure of the process factory. "
        "The system extensibility for the process library is based on the factory design pattern. "
        "Each process task must implement a factory class derived from this class. "
        "Then the system is able to instantiate dynamically a process object (even user-defined).\n\n";

constexpr auto _processFactoryInfoDocString =
        "Metadata structure associated with the task. "
        "Some fields are mandatory to allow plugin publication. "
        "See :py:class:`~ikomia.dataprocess.pydataprocess.CTaskInfo` for details.\n";

constexpr auto _create1DocString =
        "Pure virtual method to create new process task instance with default task parameters. "
        "Must be implemented.\n\n"
        "Returns:\n"
        "   :py:class:`~ikomia.core.pycore.CWorkflowTask` based object\n";

constexpr auto _create2DocString =
        "Pure virtual method to create new process task instance with the given task parameters. "
        "Must be implemented.\n\n"
        "Args:\n"
        "   param: :py:class:`~ikomia.core.pycore.CWorkflowTaskParam` based object\n\n"
        "Returns:\n"
        "   :py:class:`~ikomia.core.pycore.CWorkflowTask` based object\n";

//--------------------------//
//----- CWidgetFactory -----//
//--------------------------//
constexpr auto _widgetFactoryDocString =
        "Abstract class defining the core structure of the process widget factory. "
        "The system extensibility for the process library is based on the factory design pattern. "
        "Each process task must implement a widget factory class derived from this class. "
        "Then the system is able to instantiate dynamically a process widget object (even user-defined).\n\n";

constexpr auto _widgetFactoryNameDocString =
        "Name of the associated task. This name must be the same of the one set in the task class.\n";

constexpr auto _createWidgetDocString =
        "Pure virtual method to create new task widget instance with the given task parameters. "
        "Must be implemented.\n\n"
        "Args:\n"
        "   param: :py:class:`~ikomia.core.pycore.CWorkflowTaskParam` based object\n\n"
        "Returns:\n"
        "   :py:class:`~ikomia.core.pycore.CWorkflowTaskWidget` based object\n";

//-----------------------------------//
//----- CPluginProcessInterface -----//
//-----------------------------------//
constexpr auto _pluginInterfaceDocString =
        "Abstract class defining core structure of a plugin interface. "
        "A plugin is a user-defined task that can be integrated automatically into the system. "
        "For such, it must respect this architecture based on factory design pattern. "
        "A plugin must provide and implement two factory classes: one for the task and one for its widget. "
        "It should also override this CPluginProcessInterface class and provides an implementation for the two factory getters.\n\n";

constexpr auto _getProcessFactoryDocString =
        "Pure virtual method that returns instance of a task factory object. "
        "Must be implemented.\n\n"
        "Returns:\n"
        "   factory: :py:class:`CTaskFactory` based object\n";

constexpr auto _getWidgetFactoryDocString =
        "Pure virtual method that returns instance of a task factory object. "
        "Must be implemented.\n\n"
        "Returns:\n"
        "   factory: :py:class:`CWidgetFactory` based object\n";

//--------------------------//
//----- CObjectMeasure -----//
//--------------------------//
constexpr auto _objectMeasureDocString =
        "Store values of a given measure computed on a single blob of an image. "
        "Available measures are listed here: :py:class:`~ikomia.core.pycore.MeasureId. "
        "It can also maps this measure with the associated graphics item (by its identifier). "
        "Store grahics item in a :py:class:`~ikomia.dataprocess.pycdataprocess.CGraphicsOutput` object "
        "is the classical way to handle graphics items.\n";

constexpr auto _ctor1ObjMeasureDocString =
        "Construct a CObjectMeasure instance with a single value.\n"
        "Args:\n"
        "   measure (:py:class:`~ikomia.core.pycore.CMeasure`): basic information about the computed measure.\n\n"
        "   value (double): value of the measure\n\n"
        "   graphicsId (int): graphics item identifier, -1 if no item is associated\n\n"
        "   label (str)\n";

constexpr auto _ctor2ObjMeasureDocString =
        "Construct a CObjectMeasure instance with multiple values.\n"
        "Args:\n"
        "   measure (:py:class:`~ikomia.core.pycore.CMeasure`): basic information about the computed measure.\n\n"
        "   values (list of double): values of the measure (example Bbox needs 4 values)\n\n"
        "   graphicsId (int): graphics item identifier, -1 if no item is associated\n\n"
        "   label (str)\n";

constexpr auto _getMeasureInfoDocString =
        "Get measure information (identifier + name). See :py:class:`~ikomia.core.pycore.CMeasure`.\n";

//----------------------//
//----- CMeasureIO -----//
//----------------------//
constexpr auto _measureIODocString =
        "Define input or output for a task dedicated to manage blob measures (ie connected components). "
        "It is possible to compute and store several measures for a single blob. A CMeasureIO instance "
        "stores a list of measures for each blob of an image, so you have a list of :py:class:`~ikomia.dataprocess.pycdataprocess.CObjectMeasure` list. "
        "Please note that it is possible to map each blob with its associated graphics item stored in a :py:class:`~ikomia.dataprocess.pycdataprocess.CGraphicsOutput` instance. "
        "You just need to pass the graphics item identifier to the object measure\n. "
        "Blob measures can also be handled by :py:class:`~ikomia.dataprocess.pycdataprocess.CDblFeatureIO`. "
        "Although CDblFeatureIO is more generic, it can't map measure values with graphics item, which can be useful "
        "to give visual information from object measures to users.\n";

constexpr auto _setObjMeasureDocString =
        "Set measure for the blob specified by the given index.\n"
        "Args:\n"
        "   index (int): zeo-based index in the blob list\n\n"
        "   measure (:py:class:`~ikomia.dataprocess.pycdataprocess.CObjectMeasure`)\n";

constexpr auto _getMeasuresDocString =
        "Get measures for all blobs.\n"
        "Returns:\n"
        "   measures (list of :py:class:`~ikomia.dataprocess.pycdataprocess.CObjectMeasure` list)\n";

constexpr auto _isMeasureDataAvailableDocString =
        "Return True if there is at least one measure for one blob, False otherwise.\n";

constexpr auto _addObjMeasureDocString =
        "Add a new blob measure. Use this method if only one measure is computed for a blob.\n"
        "Args:\n"
        "   measure (:py:class:`~ikomia.dataprocess.pycdataprocess.CObjectMeasure`)\n";

constexpr auto _addObjMeasuresDocString =
        "Add a new list of measures for a blob.\n"
        "Args:\n"
        "measures (list of :py:class:`~ikomia.dataprocess.pycdataprocess.CObjectMeasure`)\n";

//----------------------//
//----- CFeatureIO -----//
//----------------------//
constexpr auto _featureProcessIODocString =
        "Define input or output for a task dedicated to manage numeric values. "
        "C++ implementation uses template to handle various data types. In Python, Ikomia API exposes only the double precision (float) specialization. "
        "The class is designed to handle numeric values, "
        "it consists on a list of float values, a list of associated labels and a display type (see :py:class:`NumericOutputType`). "
        "For the specific case of plot display, a plot type property is available (see :py:class:`PlotType`). "
        "Derived from :py:class:`~ikomia.core.pycore.CWorkflowTaskIO`\n\n";

constexpr auto _addValueList1DocString =
        "Append a new value list.\n\n"
        "Args:\n"
        "   values (list): list of float values\n";

constexpr auto _addValueList2DocString =
        "Append a new value list with the corresponding header label. A header label can be seen as a column name in a table representation.\n\n"
        "Args:\n"
        "   values (list): list of float values\n\n"
        "   header_label (str): header label\n";

constexpr auto _addValueList3DocString =
        "Append a new value list with a label for each value.\n\n"
        "Args:\n"
        "   values (list): list of float values\n\n"
        "   labels (list of str): store label for each numeric value\n";

constexpr auto _addValueList4DocString =
        "Append a new value list with the corresponding header label and a label for each value.\n\n"
        "Args:\n"
        "   values (list): list of float values\n\n"
        "   header_label (str): header label\n\n"
        "   labels (list of str): store label for each numeric value\n";

constexpr auto _clearDataDerivedDocString =
        "See :py:meth:`~ikomia.core.pycore.CWorkflowTaskIO.clearData`.\n";

constexpr auto _copyStaticDataDerivedDocString =
        "See :py:meth:`~ikomia.core.pycore.CWorkflowTaskIO.copyStaticData`.\n";

constexpr auto _getAllLabelListDocString =
        "Get all label lists associated with value lists.\n\n"
        "Returns:\n"
        "   list of list of str: all labels\n";

constexpr auto _getAllHeaderLabelsDocString =
        "Get all header labels (column labels).\n\n"
        "Returns:\n"
        "   list of str\n";

constexpr auto _getAllValueListDocString =
        "Get all value lists.\n\n"
        "Returns:\n"
        "   list of list of float: all values\n";

constexpr auto _getOutputTypeDocString =
        "Get output display type. Can be one of the values defined in :py:class:`NumericOutputType`.\n\n"
        "Returns:\n"
        "   int: display type\n";

constexpr auto _getPlotTypeDocString =
        "Get plot type. Can be one of the values defined in :py:class:`PlotType`. "
        "Meaningful only if output display type is set to PLOT.\n\n"
        "Returns:\n"
        "   PlotType: plot type\n";

constexpr auto _getUnitEltCountDerivedDocString =
        "See :py:meth:`~ikomia.core.pycore.CWorkflowTaskIO.getUnitElementCount`.\n";

constexpr auto _getValueListDocString =
        "Get value list at position index.\n\n"
        "Args:\n"
        "   index (int): index of value list\n\n"
        "Returns:\n"
        "   list: float values\n";

constexpr auto _isDataAvailableDerivedDocString =
        "See :py:meth:`~ikomia.core.pycore.CWorkflowTaskIO.isDataAvailable`.\n";

constexpr auto _setOutputTypeDocString =
        "Set output display type. See :py:class:`NumericOutputType` for possible values.\n\n"
        "Args:\n"
        "   type (int): display type\n";

constexpr auto _setPlotTypeDocString =
        "Set plot type. See :py:class:`PlotType` for possible values. "
        "Only used if output display type is set to PLOT (see :py:meth:`setOutputType`).\n\n"
        "Args:\n"
        "   type (PlotType): plot type\n";

//--------------------//
//----- CImageIO -----//
//--------------------//
constexpr auto _imageProcessIODocString =
        "Define input or output for a task dedicated to image processing (2D or 3D). "
        "This class is designed to handle image as input or output. "
        "A CImageIO instance can be added as input or output to a :py:class:`~ikomia.core.pycore.CWorkflowTask` or derived object. "
        "It is the base class to define input or output of an image processing task. "
        "Several image data type can be defined according to the nature of the algorithm:\n\n"
        "- binary image\n"
        "- labelled image\n"
        "- standard image\n\n"
        "The internal image data structure is a numpy array that can be either 2D or 3D.\n"
        "Derived from :py:class:`~ikomia.core.pycore.CWorkflowTaskIO`.\n\n";

constexpr auto _ctor1imageProcessIODocString =
        "Construct a CImageIO instance with the given image. "
        "By default, the image output type is set to the standard one (IODataType.IMAGE). "
        "See :py:class:`~ikomia.core.pycore.IODataType` for details.\n\n"
        "Args:\n"
        "   arg1: self\n\n"
        "   arg2 (Numpy array): 2D/3D image\n";

constexpr auto _ctor2imageProcessIODocString =
        "Construct a CImageIO instance with the given data type. The data type must be one of these values:\n\n"
        "- IODataType.IMAGE\n"
        "- IODataType.IMAGE_BINARY\n"
        "- IODataType.IMAGE_LABEL\n"
        "- IODataType.VOLUME\n"
        "- IODataType.VOLUME_BINARY\n"
        "- IODataType.VOLUME_LABEL\n\n"
        "Please note that internal image structure is empty\n\n"
        "Args:\n"
        "   arg1: self\n\n"
        "   arg2 (:py:class:`~ikomia.core.pycore.IODataType`): data type\n";

constexpr auto _ctor3imageProcessIODocString =
        "Construct a CImageIO instance with the given data type and the given image. "
        "The data type must be one of these values:\n\n"
        "- IODataType.IMAGE\n"
        "- IODataType.IMAGE_BINARY\n"
        "- IODataType.IMAGE_LABEL\n"
        "- IODataType.VOLUME\n"
        "- IODataType.VOLUME_BINARY\n"
        "- IODataType.VOLUME_LABEL\n\n"
        "Args:\n"
        "   arg1: self\n\n"
        "   arg2 (:py:class:`~ikomia.core.pycore.IODataType`): data type\n\n"
        "   arg3 (Numpy array): 2D/3D image\n";

constexpr auto _ctor4imageProcessIODocString =
        "Construct a CImageIO instance with the default image data type (:py:class:`~ikomia.core.pycore.IODataType.IMAGE)"
        "and the image loaded from the given path.\n\n"
        "Args:\n"
        "   arg1: self\n\n"
        "   arg2 (str): image path\n";

constexpr auto _ctor5imageProcessIODocString =
        "Construct a CImageIO instance with the given data type and the image loaded from the given path. "
        "The data type must be one of these values:\n\n"
        "- IODataType.IMAGE\n"
        "- IODataType.IMAGE_BINARY\n"
        "- IODataType.IMAGE_LABEL\n"
        "- IODataType.VOLUME\n"
        "- IODataType.VOLUME_BINARY\n"
        "- IODataType.VOLUME_LABEL\n\n"
        "Args:\n"
        "   arg1: self\n\n"
        "   arg2 (:py:class:`~ikomia.core.pycore.IODataType`): data type\n\n"
        "   arg3 (str): image path\n";

constexpr auto _clearImageDataDocString =
        "Clear image and overlay mask so that they become empty.\n";

constexpr auto _copyImageStaticDataDocString =
        "Set the static information from the given input or ouput. "
        "For this class, the channel count is the only static data.\n\n"
        "Args:\n"
        "   io (:py:class:`~ikomia.core.pycore.CWorkflowTaskIO`): input or ouput instance from which data is copied.\n";

constexpr auto _getChannelCountDocString =
        "Get the static channel count information. "
        "The method does not get the channel count property from the image data structure\n\n"
        "Returns:\n"
        "   int: number of channel required for the input/output\n";

constexpr auto _getDataDocString =
        "Get the image data.\n\n"
        "Returns:\n"
        "   Numpy array: either 2D or 3D image buffer\n";

constexpr auto _getImageDocString =
        "Get the 2D image data only. "
        "In case of volume, the current image index is used to get the desired 2D plane (see :py:meth:`setCurrentImage`).\n\n"
        "Returns:\n"
        "   Numpy array: 2D image buffer\n";

constexpr auto _getOverlayMaskDocString =
        "Get the overlay mask. See :py:meth:`setOverlayMask` for more information.\n\n"
        "Returns:\n"
        "   Numpy array: 2D image buffer (8 bits - 1 channel)\n";

constexpr auto _getImageUnitElementCountDocString =
        "Get the number of unit elements when the data is processed. "
        "The number of unit elements is used to calculate the number of progress steps needed to perform a task. "
        "In case of 2D image, we won't update progress bar every pixel, so the return value should be 1 (1 update per image). "
        "In case of 3D image, update progress every 2D plane can make sense for task processing volume on 2D planes independently. "
        "So user have to define the number of unit elements through the progress bar update perspective.\n\n"
        "Returns:\n"
        "   int: number of unit element to process.\n";

constexpr auto _isImageDataAvailableDocString =
        "Check whether the input/output have valid image or not.\n\n"
        "Returns:\n"
        "   bool: True if image is not empty, False otherwise.\n";

constexpr auto _isOverlayAvailableDocString =
        "Check whether the input/output have valid overlay mask or not\n\n"
        "Returns:\n"
        "   bool: True if overlay mask is not empty, False otherwise.\n";

constexpr auto _setChannelCountDocString =
        "Set the channel count of the image data."
        "This property holds the required channel count as a static information. "
        "Such information can be useful when one designs a workflow and wants to inform about specific image structure towards future connected tasks.\n\n"
        "Args:\n"
        "   nb (int): channel count, 1 for monochrome image, 3 for color image\n";

constexpr auto _setCurrentImageDocString =
        "Set the index of the current image (2D plane) from a volume (3D image data structure).\n\n"
        "Args:\n"
        "   index (int): zero-based index of the 2D plane\n";

constexpr auto _setImageDocString =
        "Set the image\n\n"
        "Args:\n"
        "   image (Numpy array): image buffer\n";

constexpr auto _setOverlayMaskDocString =
        "Set the associated overlay mask. Ikomia software is able to display overlay mask on top of image. "
        "This method sets this mask, it will be displayed automatically according to a predefined color map. "
        "Zero pixels of the mask will be completely transparent, non-zero will be displayed according to the corresponding color in the color map. "
        "The color map must be defined in the task implementation. "
        "See :py:meth:`~C2dImageTask.setOutputColorMap` for details.\n\n"
        "Args:\n"
        "   image (Numpy array): image buffer (8 bits - 1 channel)\n";

constexpr auto _drawGraphicsDocString =
        "Draw given graphics items (input or output) in the image.\n\n"
        "Args:\n"
        "   graphics (:py:class:`~ikomia.dataprocess.pycdataprocess.CGraphicsInput` or "
        "   :py:class:`~ikomia.dataprocess.pycdataprocess.CGraphicsOutput`)\n";

//--------------------------//
//----- CGraphicsInput -----//
//--------------------------//
constexpr auto _graphicsInputDocString =
        "Define task input containing graphics items. Consult :py:class:`~ikomia.core.pycore.GraphicsItem` to see the list of possible graphics types. "
        "Instance can be added as input of a :py:class:`~ikomia.core.pycore.CWorkflowTask` or derived. "
        "This kind of input is used to manage user-defined ROI or to forward graphics items generated by a previous task in your workflow. "
        "Derived from :py:class:`~ikomia.core.pycore.CWorkflowTaskIO`.\n\n";

constexpr auto _setItemsDocString =
        "Fill input with the given graphics item list.\n\n"
        "Args:\n"
        "   items (list): list of :py:class:`~ikomia.core.pycore.CGraphicsItem` based objects\n";

constexpr auto _getItemsDocString =
        "Get list of input graphics items.\n\n"
        "Returns:\n"
        "   list: list of :py:class:`~ikomia.core.pycore.CGraphicsItem` based objects\n";

constexpr auto _isGraphicsDataAvailableDocString =
        "Check whether the input contains data.\n\n"
        "Returns:\n"
        "   bool: True if input contains data, False otherwise\n";

constexpr auto _clearGraphicsDataDocString =
        "Clear input data.\n";

//---------------------------//
//----- CGraphicsOutput -----//
//---------------------------//
constexpr auto _graphicsOutputDocString =
        "Define task output containing graphics items. Consult :py:class:`~ikomia.core.pycore.GraphicsItem` to see the list of possible graphics types. "
        "Instance can be added as output of a :py:class:`~ikomia.core.pycore.CWorkflowTask` or derived. "
        "This kind of output is used to manage graphics items generated in a task. "
        "Ikomia software displays it as an overlay layer on top of images or videos. "
        "Graphics items can then be forwarded as input of workflow's following tasks. "
        "Derived from :py:class:`~ikomia.core.pycore.CWorkflowTaskIO`.\n\n";

constexpr auto _setNewLayerDocString =
        "Initiate a new graphics layer for the output with the given name. The method clears all graphics items that was previously generated.\n\n"
        "Args:\n"
        "   name (str): name of the associated graphics layer\n";

constexpr auto _setImageIndexDocString =
        "Set the output index on which graphics items will be displayed (in overlay layer). "
        "Index must refer to an existing output managing image data such as :py:class:`CImageIO` or derived.\n\n"
        "Args:\n"
        "   index (int): index of image-based output\n";

constexpr auto _addItemDocString =
        "Add graphics item to this output. Object class must be derived from :py:class:`~ikomia.core.pycore.CGraphicsItem`. "
        "Use this method only if you need to add user-defined graphics items. "
        "Consult :py:class:`~ikomia.core.pycore.GraphicsItem` to see the list of built-in graphics items.\n\n"
        "Args:\n"
        "   item (:py:class:`~ikomia.core.pycore.CGraphicsItem` based object)\n";

constexpr auto _addPoint1DocString =
        "Add point item to this output. Item properties (color, size...) come from the global preferences of Ikomia software. "
        "They can be adjusted interactively from the option button of the graphics toolbar.\n\n"
        "Args:\n"
        "   point (:py:class:`~ikomia.core.pycore.CPointF`): position in image coordinates\n";

constexpr auto _addPoint2DocString =
        "Add point item to this output with the given display properties (color, size...)\n\n"
        "Args:\n"
        "   point (:py:class:`~ikomia.core.pycore.CPointF`): position in image coordinates\n\n"
        "   properties (:py:class:`~ikomia.core.pycore.GraphicsPointProperty`): display properties\n";

constexpr auto _addRectangle1DocString =
        "Add rectangle or square item to this output. "
        "Item properties (color, size...) come from the global preferences of Ikomia software. "
        "They can be adjusted interactively from the option button of the graphics toolbar.\n\n"
        "Args:\n"
        "   x (float): x-coordinate of the top-left point.\n\n"
        "   y (float): y-coordinate of the top-left point.\n\n"
        "   width (float): width of the rectangle\n\n"
        "   height (float): height of the rectangle\n";

constexpr auto _addRectangle2DocString =
        "Add rectangle or square item to this output with the given display properties (color, size...).\n\n"
        "Args:\n"
        "   x (float): x-coordinate of the top-left point.\n\n"
        "   y (float): y-coordinate of the top-left point.\n\n"
        "   width (float): width of the rectangle\n\n"
        "   height (float): height of the rectangle\n\n"
        "   properties (:py:class:`~ikomia.core.pycore.GraphicsRectProperty`): display properties\n";

constexpr auto _addEllipse1DocString =
        "Add ellipse or circle item to this output. "
        "Item properties (color, size...) come from the global preferences of Ikomia software. "
        "They can be adjusted interactively from the option button of the graphics toolbar.\n\n"
        "Args:\n"
        "   x (float): x-coordinate of the top-left point.\n\n"
        "   y (float): y-coordinate of the top-left point.\n\n"
        "   width (float): width of the ellipse\n\n"
        "   height (float): height of the ellipse\n";

constexpr auto _addEllipse2DocString =
        "Add ellipse or circle item to this output with the given display properties (color, size...).\n\n"
        "Args:\n"
        "   x (float): x-coordinate of the top-left point.\n\n"
        "   y (float): y-coordinate of the top-left point.\n\n"
        "   width (float): width of the ellipse\n\n"
        "   height (float): height of the ellipse\n\n"
        "   properties (:py:class:`~ikomia.core.pycore.GraphicsEllipseProperty`): display properties\n";

constexpr auto _addPolygon1DocString =
        "Add polygon item to this output. The polygon is closed automatically. "
        "Item properties (color, size...) come from the global preferences of Ikomia software. "
        "They can be adjusted interactively from the option button of the graphics toolbar.\n\n"
        "Args:\n"
        "   points (:py:class:`~ikomia.core.pycore.CPointF` list): list of polygon vertices (x,y)\n";

constexpr auto _addPolygon2DocString =
        "Add polygon item to this output with the given display properties (color, size...).\n\n"
        "Args:\n"
        "   points (:py:class:`~ikomia.core.pycore.CPointF` list): list of polygon vertices (x,y)\n\n"
        "   properties (:py:class:`~ikomia.core.pycore.GraphicsPolygonProperty`): display properties\n";

constexpr auto _addPolyline1DocString =
        "Add polyline item to this output. "
        "Item properties (color, size...) come from the global preferences of Ikomia software. "
        "They can be adjusted interactively from the option button of the graphics toolbar.\n\n"
        "Args:\n"
        "   points (:py:class:`~ikomia.core.pycore.CPointF` list): list of polygon vertices (x,y)\n";

constexpr auto _addPolyline2DocString =
        "Add polyline item to this output with the given display properties (color, size...).\n\n"
        "Args:\n"
        "   points (:py:class:`~ikomia.core.pycore.CPointF` list): list of polygon vertices (x,y)\n\n"
        "   properties (:py:class:`~ikomia.core.pycore.GraphicsPolylineProperty`): display properties\n";

constexpr auto _addComplexPolygon1DocString =
        "Add complex polygon item to this output. A complex polygon means a polygon with one or several holes inside. "
        "Item properties (color, size...) come from the global preferences of Ikomia software. "
        "They can be adjusted interactively from the option button of the graphics toolbar.\n\n"
        "Args:\n"
        "   outer (:py:class:`~ikomia.core.pycore.CPointF` list): list of polygon vertices (x,y)\n\n"
        "   inners (list of :py:class:`~ikomia.core.pycore.CPointF` list): list of inner polygons (holes)\n";

constexpr auto _addComplexPolygon2DocString =
        "Add complex polygon item to this output with the given display properties (color, size...). "
        "A complex polygon means a polygon with one or several holes inside.\n\n"
        "Args:\n"
        "   outer (:py:class:`~ikomia.core.pycore.CPointF` list): list of polygon vertices (x,y)\n\n"
        "   inners (list of :py:class:`~ikomia.core.pycore.CPointF` list): list of inner polygons (holes)\n\n"
        "   properties (:py:class:`~ikomia.core.pycore.GraphicsPolygonProperty`): display properties\n";

constexpr auto _addText1DocString =
        "Add text item to this output. "
        "Item properties (color, size...) come from the global preferences of Ikomia software. "
        "They can be adjusted interactively from the option button of the graphics toolbar.\n\n"
        "Args:\n"
        "   text (str): text to display\n\n"
        "   x (float): x-coordinate of the top-left point.\n\n"
        "   y (float): y-coordinate of the top-left point.\n";

constexpr auto _addText2DocString =
        "Add text item to this output with the given display properties (color, size...).\n\n"
        "Args:\n"
        "   text (str): text to display\n\n"
        "   x (float): x-coordinate of the top-left point.\n\n"
        "   y (float): y-coordinate of the top-left point.\n\n"
        "   properties (:py:class:`~ikomia.core.pycore.GraphicsTextProperty`): display properties\n";

//--------------------//
//----- CVideoIO -----//
//--------------------//
constexpr auto _videoProcessIODocString =
        "Define an input or output for a task dedicated to video management. "
        "This class is designed to handle video and instance can be added as input or output to a :py:class:`~ikomia.core.pycore.CWorkflowTask` or derived object. "
        "It is the base class to define input or output of a video processing task. "
        "Several video output type can be defined according to the nature of the algorithm:\n\n"
        "- binary video\n"
        "- labelled video (graylevel connected components)\n"
        "- standard video\n\n"
        "Source video can be either a file, an image sequence or a stream. Image data is stored as a numpy array.\n"
        "Derived from :py:class:`~ikomia.dataprocess.pydataprocess.CImageIO`.\n\n";

constexpr auto _ctor1VideoProcessIODocString =
        "Construct a CVideoIO instance with the given image. "
        "By default, the image output type is set to the standard one (IODataType.IMAGE). "
        "See :py:class:`~ikomia.core.pycore.IODataType` for details.\n\n"
        "Args:\n"
        "   arg1: self\n\n"
        "   arg2 (Numpy array): 2D image\n";

constexpr auto _ctor2VideoProcessIODocString =
        "Construct a CVideoIO instance with the given data type. The data type must be one of these values:\n\n"
        "- IODataType.VIDEO\n"
        "- IODataType.VIDEO_BINARY\n"
        "- IODataType.VIDEO_LABEL\n"
        "- IODataType.LIVE_STREAM\n"
        "- IODataType.LIVE_STREAM_BINARY\n"
        "- IODataType.LIVE_STREAM_LABEL\n\n"
        "Please note that internal image structure is empty\n\n"
        "Args:\n"
        "   arg1: self\n\n"
        "   arg2 (:py:class:`~ikomia.core.pycore.IODataType`): data type\n";

constexpr auto _ctor3VideoProcessIODocString =
        "Construct a CVideoIO instance with the given data type and the given image. "
        "The data type must be one of these values:\n\n"
        "- IODataType.VIDEO\n"
        "- IODataType.VIDEO_BINARY\n"
        "- IODataType.VIDEO_LABEL\n"
        "- IODataType.LIVE_STREAM\n"
        "- IODataType.LIVE_STREAM_BINARY\n"
        "- IODataType.LIVE_STREAM_LABEL\n\n"
        "Args:\n"
        "   arg1: self\n\n"
        "   arg2 (:py:class:`~ikomia.core.pycore.IODataType`): data type\n\n"
        "   arg3 (Numpy array): 2D image\n";

constexpr auto _setVideoPathDocString =
        "Set the source path of the video.\n\n"
        "Args:\n"
        "   path (str): can be a file path, a device ID (internal webcam) or an url (IP camera)\n\n";

constexpr auto _setVideoPosDocString =
        "Set the current frame of the video.\n\n"
        "Args:\n"
        "   position (int): index of the frame\n";

constexpr auto _getVideoFrameCountDocString =
        "Get the total frames number of the video.\n\n"
        "Returns:\n"
        "   int: frames number\n";

constexpr auto _getVideoImagesDocString =
        "Get all image frames extracted from the video.\n\n"
        "Returns:\n"
        "   Numpy array list: list of 2D images\n";

constexpr auto _getVideoPathDocString =
        "Get the path to the source video. It can be a file path, a device ID or an url.\n\n"
        "Returns:\n"
        "   str: source video path\n";

constexpr auto _getSnapshotDocString =
        "Get image of a single frame.\n\n"
        "Args:\n"
        "   position (int): index of the frame\n\n"
        "Returns:\n"
        "   Numpy array: 2D image buffer\n";

constexpr auto _getCurrentPosDocString =
        "Get the current frame index of the video or stream.\n\n"
        "Returns:\n"
        "   int: current frame index\n";

constexpr auto _startVideoDocString =
        "Start playing the video.\n\n";

constexpr auto _stopVideoDocString =
        "Stop playing the video.\n\n";

constexpr auto _startVideoWriteDocString =
        "Write the video to disk.\n\n";

constexpr auto _stopVideoWriteDocString =
        "Stop writting to disk.\n";

constexpr auto _addVideoImageDocString =
        "Append image frame to the video.\n\n"
        "Args:\n"
        "   image (Numpy array): 2D image\n";

constexpr auto _writeImageDocString =
        "Append a new image to the list of images to write to disk. The write process is launched using :py:meth:`startVideoWrite`.\n\n"
        "Args:\n"
        "   image (Numpy array): 2D image\n";

constexpr auto _hasVideoDocString =
        "Check whether the input or output has a video source.\n\n"
        "Returns:\n"
        "   bool: True if the source is valid, False otherwise\n";

constexpr auto _getVideoImageDocString =
        "Get the image at the current frame index.\n\n"
        "Returns:\n"
        "   Numpy array: 2D image buffer\n";

constexpr auto _getVideoUnitElementCountDocString =
        "Get the number of unit elements to process, ie the number of frames for a video. "
        "Used to determine the number of steps for progress bar.\n\n"
        "Returns:\n"
        "   int: frames number of the video\n";

constexpr auto _isVideoDataAvailableDocString =
        "Check whether the video contains valid data.\n\n"
        "Returns:\n"
        "   bool: True if there is some valid data, False otherwise\n";

constexpr auto _clearVideoDataDocString =
        "Clear the current image at the current frame index.\n";

//-------------------------//
//----- CWidgetOutput -----//
//-------------------------//
constexpr auto _widgetOutputDocString =
        "Define a widget output for a task. "
        "This class is designed to handle widget as output of a workflow task. "
        "A widget must be a derived object from QWidget of the Qt framework (PyQt5 or PySide2).\n"
        "Derived from :py:class:`~ikomia.core.pycore.CWorkflowTaskIO`.\n\n";

constexpr auto _ctorWidgetOutputDocString =
        "Construct a CWidgetOutput instance with the given data type."
        "By default, the widget data type is IODataType.WIDGET (see :py:class:`~ikomia.core.pycore.IODataType`).\n";

constexpr auto _setWidgetDocString =
        "Set the widget instance: use :py:mod:`~ikomia.utils.qtconversion` module to get C++ handle from Python Qt-based framework.\n\n"
        "Args:\n"
        "   widget: compatible C++ widget handle\n";

constexpr auto _isWidgetDataAvailableDocString =
        "Check whether the output contains a valid widget.\n\n"
        "Returns:\n"
        "   bool: True if the widget handle is valid, False otherwise\n";

constexpr auto _clearWidgetDataDocString =
        "Clear the output data: sets the widget handle to null.\n\n";

//-------------------//
//----- CPathIO -----//
//-------------------//
constexpr auto _pathIODocString =
        "Define input or output for a task to handle file or folder path. "
        "Derived from :py:class:`~ikomia.core.pycore.CWorkflowTaskIO`.\n\n";

constexpr auto _ctor1PathIODocString =
        "Construct a CPathIO instance with the given data type. "
        "Data type can be either a file or folder path (see :py:class:`~ikomia.core.pycore.IODataType`).\n\n";

constexpr auto _ctor2PathIODocString =
        "Construct a CPathIO instance with the given data type and the given path. "
        "Data type can be either a file or folder path (see :py:class:`~ikomia.core.pycore.IODataType`).\n\n";

constexpr auto _setPathDocString =
        "Set the path of the input/output. The path must exist.\n\n";

constexpr auto _getPathDocString =
        "Get the path of the input/output.\n\n";

constexpr auto _clearDataDocString =
        "Clear the data stored in the object. Should be overriden for custom input or output.\n\n";

//----------------------//
//----- CDatasetIO -----//
//----------------------//
constexpr auto _datasetIODocString =
        "Virtual base classe to define task input or output containing deep learning dataset structure. "
        "Derived from :py:class:`~ikomia.core.pycore.CWorkflowTaskIO`.\n"
        "Instances can be added as input or output of a :py:class:`~ikomia.core.pycore.CWorkflowTask` or "
        "derived object. Such input or output is required for deep learning training "
        "task. Custom dataset loader must inherit this class and implements required virtual methods. \n\n"
        "Dataset structure is composed of a dict for each image and some common "
        "metadata with the following specifications (mandatory fields may vary "
        "depending on the training goal).\n\n"
        "   - images (list[dict]): image information and corresponding annotations:\n"
        "       - filename (str): full path of the image file.\n"
        "       - height, width (int): size of the image.\n"
        "       - image_id (int): unique image identifier.\n"
        "       - annotations (list[dict]): each dict corresponds to annotations of one instance in this image.\n"
        "           - bbox (list[float]): x, y, width, height of the bounding box.\n"
        "           - category_id (int): integer representing the category label.\n"
        "           - segmentation_poly (list[list[float]]): list of polygons, one for each connected component.\n"
        "           - keypoints (list[float]).\n"
        "           - iscrowd (boolean): whether the instance is labelled as a crowd region (COCO).\n"
        "       - segmentation_masks (numpy array [N, H, W]).\n"
        "       - instance_seg_masks_file: full path of the ground truth instance segmentation image file.\n"
        "       - semantic_seg_masks_file: full path of the ground truth semantic segmentation image file.\n"
        "   - metadata (dict): key-value mapping that contains information that's shared among the entire dataset:\n"
        "       - category_names (list[str]).\n"
        "       - category_colors (list[tuple(r,g,b)]).\n"
        "       - keypoint_names (list[str]).\n"
        "       - keypoint_connection_rules (list[tuple(str, str, (r,g,b))]): each tuple specifies a pair of keypoints that "
        "are connected and the color to use for the line between them.\n\n";

constexpr auto _ctor1DatasetIODocString =
        "Construct a CDatasetIO object specifying the source format.\n\n"
        "Args:\n"
        "   str: unique string identifier.\n\n";

constexpr auto _getImagePathsDocStr =
        "Virtual method to reimplement, return the file path list of all images contained in the dataset.\n\n"
        "Returns:\n"
        "   str[]: path list\n";

constexpr auto _getCategoriesDocStr =
        "Virtual method to reimplement, return the categories of the dataset.\n\n"
        "Returns:\n"
        "   :py:class:`~ikomia.dataprocess.pydataprocess.MapIntStr`: list of categories\n";

constexpr auto _getCategoryCountDocStr =
        "Virtual method to reimplement, return the number of instance categories (ie classes) in the dataset.\n\n"
        "Returns:\n"
        "   int: number of categories\n";

constexpr auto _getMaskPathDocStr =
        "Virtual method to reimplement, return the file path list of segmentation masks contained in the dataset.\n\n"
        "Returns:\n"
        "   str[]: path list\n";

constexpr auto _getGraphicsAnnotationsDocStr =
        "Virtual method to reimplement, return the list of graphics items corresponding to dataset annotations.\n\n"
        "Returns:\n"
        "   list of :py:class:`~ikomia.core.pycore.CGraphicsItem` or derived: graphics items\n";

constexpr auto _getSourceFormatDocStr =
        "Get the source format of the dataset.\n\n"
        "Return:\n"
        "   str: source format string identifier (lowercase)\n";

constexpr auto _saveDocStr =
        "Virtual method to reimplement, save dataset structure as JSON.\n\n"
        "Args:\n"
        "   str: file path\n";

//--------------------//
//----- CArrayIO -----//
//--------------------//
constexpr auto _arrayIODocString =
        "Define multi-dimensional array as input or output of a task. "
        "A CArrayIO instance can be added as input or output to a :py:class:`~ikomia.core.pycore.CWorkflowTask` or derived object. "
        "The internal image data structure is a numpy array.\n"
        "Derived from :py:class:`~ikomia.core.pycore.CWorkflowTaskIO`.\n\n";

constexpr auto _ctor1ArrayIODocString =
        "Construct a CArrayIO instance with the given array. "
        "Args:\n"
        "   arg1: self\n\n"
        "   arg2 (Numpy array): nd array\n";

constexpr auto _clearArrayDataDocString =
        "Clear array so that it becomes empty.\n";

constexpr auto _getArrayDocString =
        "Get the array data.\n\n"
        "Returns:\n"
        "   Numpy array: nd array\n";

constexpr auto _getArrayUnitElementCountDocString =
        "Get the number of unit elements when the data is processed. "
        "The number of unit elements is used to calculate the number of progress steps needed to perform a task. "
        "Ikomia Studio consider array as a whole entity.\n\n"
        "Returns:\n"
        "   int: number of unit element to process (always 1).\n";

constexpr auto _isArrayDataAvailableDocString =
        "Check whether the input/output has valid array.\n\n"
        "Returns:\n"
        "   bool: True if array is not empty, False otherwise.\n";

constexpr auto _setArrayDocString =
        "Set the array data\n\n"
        "Args:\n"
        "   array (Numpy array): nd array\n";

//------------------------//
//----- C2dImageTask -----//
//------------------------//
constexpr auto _imageProcess2dDocString =
        "Implement all basic features used in a task dedicated to 2D image processing. "
        "This class defines a task with the following properties:\n\n"
        "Inputs:\n\n"
        "- image (:py:class:`CImageIO`)\n"
        "- graphics layer (not accessible yet in Python API)\n\n"
        "Outputs:\n\n"
        "- image (:py:class:`CImageIO`)\n\n"
        "It is a good starting point to use it as a base class for all task dedicated to 2D images.\n\n"
        "Derived from :py:class:`~ikomia.core.pycore.CWorkflowTask`.\n";

constexpr auto _ctor1ImageProcess2dDocString =
        "Construct C2dImageTask task with or without graphics input.\n\n"
        "Args:\n"
        "   arg1 (self)\n\n"
        "   arg2 (bool): True, the task manage graphics input. False, the task does not have any graphics input\n";

constexpr auto _ctor2ImageProcess2dDocString =
        "Construct C2dImageTask task with the given name. Default inputs and outputs.\n\n"
        "Args:\n"
        "   arg1 (self)\n\n"
        "   arg2 (str): task name, must be unique\n";

constexpr auto _ctor3ImageProcess2dDocString =
        "Construct C2dImageTask task with the given name, with or without graphics input.\n\n"
        "Args:\n"
        "   arg1 (self)\n\n"
        "   arg2 (str): task name, must be unique\n\n"
        "   arg3 (bool): True, the task manage graphics input. False, the task does not have any graphics input\n";

constexpr auto _setActiveDocString =
        "See :py:meth:`~ikomia.core.pycore.CWorkflowTask.setActive`.\n";

constexpr auto _setOutputColorMapDocString =
        "Bind a display color map to an image output. "
        "The color mask is generated from an output mask generated by the task itself (binary or labelled image).\n\n"
        "Args:\n"
        "   index (int): zero-based index of the output to be displayed with the color map. The output must be a :py:class:`~ikomia.dataprocess.pydataprocess.CImageIO` or derived.\n"
        "   mask_index (int): zero-based index of the output representing the mask used to generate the color overlay.\n"
        "   colors (list): list of tuples (r,g,b values) for the color map. If empty, the system generates random colors.\n";

constexpr auto _updateStaticOutputsDocString =
        "Determine output data type automatically from input data types. "
        "Don't forget to call this method in overriden methods. See :py:meth:`~ikomia.core.pycore.CWorkflowTask.updateStaticOutputs`.\n";

constexpr auto _beginTaskRunDocString =
        "See :py:meth:`~ikomia.core.pycore.CWorkflowTask.beginTaskRun`.\n";

constexpr auto _endTaskRunDocString =
        "See :py:meth:`~ikomia.core.pycore.CWorkflowTask.endTaskRun`.\n";

constexpr auto _graphicsChangedDocString =
        "See :py:meth:`~ikomia.core.pycore.CWorkflowTask.graphicsChanged`.\n";

constexpr auto _globalInputChangedDocString =
        "See :py:meth:`~ikomia.core.pycore.CWorkflowTask.globalInputChanged`.\n";

constexpr auto _createGraphicsMaskDocString =
        "Generate a binary mask image from the given graphics input. "
        "The new mask is appended to the internal mask list.\n\n"
        "Args:\n"
        "   width (int): mask width (should be the width of the source image)\n"
        "   height (int): mask height (should be the height of the source image)\n"
        "   graphics (:py:class:`~ikomia.dataprocess.pydataprocess.CGraphicsInput`): graphics items become white blobs in the black background mask\n";

constexpr auto _applyGraphicsMaskDocString =
        "Apply the mask generated from graphics input to the result image so that only masked areas seems to be processed.\n\n"
        "Args:\n"
        "   src (Numpy array): source image of the task\n"
        "   dst (Numpy array): result image\n"
        "   index (int): zero-based index of the mask\n";

constexpr auto _applyGraphicsMaskToBinaryDocString =
        "Apply the mask generated from graphics input to the binary source image. "
        "Only white areas on both image and mask are kept in the result image.\n\n"
        "Args:\n"
        "   src (Numpy array): source image of the task\n"
        "   dst (Numpy array): result image\n"
        "   index (int): zero-based index of the mask\n";

constexpr auto _getProgressStepsDocString =
        "See :py:meth:`~ikomia.core.pycore.CWorkflowTask.getProgressSteps`.\n";

constexpr auto _getGraphicsMaskDocString =
        "Get the binary mask generated from graphics input at position index.\n\n"
        "Args:\n"
        "   index (int): zero-based index of the mask\n\n"
        "Returns:\n"
        "   Numpy array: binary mask (8 bits - 1 channel)\n";

constexpr auto _isMaskAvailableDocString =
        "Check whether a binary mask from graphics input is available at position index.\n\n"
        "Args:\n"
        "   index (int): zero-based index of the mask\n\n"
        "Returns:\n"
        "   bool: True if mask is available, False otherwise\n";

constexpr auto _runDocString =
        "See :py:meth:`~ikomia.core.pycore.CWorkflowTask.run`.\n";

constexpr auto _stopDocString =
        "See :py:meth:`~ikomia.core.pycore.CWorkflowTask.stop`.\n";

constexpr auto _forwardInputImageDocString =
        "Forward input image at position input_index to output at position output_index.\n\n"
        "Args:\n"
        "   input_index (int): zero-based index of the input\n"
        "   output_index (int): zero-based index of the output\n";

constexpr auto _emitAddSubProgressSteps =
        "See :py:meth:`~ikomia.core.pycore.CWorkflowTask.emitAddSubProgressSteps`.\n";

constexpr auto _emitStepProgressDocString =
        "See :py:meth:`~ikomia.core.pycore.CWorkflowTask.emitStepProgress`.\n";

constexpr auto _emitGraphicsContextChangedDocString =
        "See :py:meth:`~ikomia.core.pycore.CWorkflowTask.emitGraphicsContextChanged`.\n";

constexpr auto _emitOutputChangedDocString =
        "See :py:meth:`~ikomia.core.pycore.CWorkflowTask.emitOutputChanged`.\n";

//-----------------------------------//
//----- C2dImageInteractiveTask -----//
//-----------------------------------//
constexpr auto _interactiveImageProcess2d =
        "Add user interactions capability to a 2D image process task. "
        "The class implements a user interaction mechanism through the use of dedicated graphics layer. "
        "When a C2dImageInteractiveTask instance is active, the system automatically activates this "
        "internal graphics layer on which the user can interact by drawing items (points, lines, polygons...). "
        "Every changes on this layer are then notified to this class, and actions can be implemented accordingly. "
        "The class could be used for example to handle interactive segmentation with color picker.\n\n"
        "Derived from :py:class:`C2dImageTask`.\n";

constexpr auto _ctorInteractiveImageProcessDocString =
        "Construct C2dImageTask task with the given name. Same inputs and outputs as :py:class:`C2dImageTask`.\n\n"
        "Args:\n"
        "   arg1 (self)\n\n"
        "   arg2 (str): task name, must be unique\n";

constexpr auto _setActiveInteractiveDocString =
        "Make the task and its graphics interaction layer active or inactive.\n\n"
        "Args:\n"
        "   is_active (bool): True or False\n";

constexpr auto _graphicsChangedInteractiveDocString =
        "Notify that the graphics interaction layer has changed.\n";

constexpr auto _globalInputChangedInteractiveDocString =
        "Notify that the workflow input has changed. "
        "The graphics interaction layer is updated. "
        "Don't forger to call this method if you override it in a derived class.\n\n"
        "Args:\n"
        "   is_new_sequence (bool): indicate if new input is a new sequence (ex: new frame of the same video is not a new sequence).";

constexpr auto _getInteractionMaskDocString =
        "Get the binary mask generated from the graphics interaction layer.\n\n"
        "Returns:\n"
        "   Numpy array: binary mask (8 bits - 1 channel)\n";

constexpr auto _getBlobsDocString =
        "Get the list of connected components extracted from the binary interaction mask.\n\n"
        "Returns:\n"
        "   List of 2D point list\n";

constexpr auto _createInteractionMaskDocString =
        "Generate a binary mask (stored internally) from the graphics interaction layer.\n\n"
        "Args:\n"
        "   width (int): mask width (should be the width of the source image)\n"
        "   height (int): mask height (should be the height of the source image)\n";

constexpr auto _computeBlobsDocString =
        "Generate the list of connected components from the binary mask. "
        "Use :py:meth:`getBlobs` to retrieve it.\n\n";

constexpr auto _clearInteractionLayerDocString =
        "Clear all graphics items in the interaction layer.\n";

//----------------------//
//----- CVideoTask -----//
//----------------------//
constexpr auto _videoProcessDocString =
        "Add video specific features. "
        "This class defines a task with the following properties:\n\n"
        "Inputs:\n\n"
        "- video (:py:class:`CVideoIO`)\n"
        "- graphics layer (not accessible yet in Python API)\n\n"
        "Outputs:\n\n"
        "- video (:py:class:`CVideoIO`)\n\n"
        "It should be the base class for all task dedicated to video processing.\n\n"
        "Derived from :py:class:`C2dImageTask`.\n";

constexpr auto _ctorVideoProcessDocString =
        "Construct CVideoTask object with the given name.\n\n"
        "Args:\n"
        "   arg1 (self)\n\n"
        "   arg2 (str): task name, must be unique\n";

constexpr auto _notifyVideoStartDocString =
        "Notify that the video has started.\n\n"
        "Args:\n"
        "   frame_count (int): total number of frames\n";

constexpr auto _notifyVideoEndDocString =
        "Notify that the end of the video is reached.\n";

constexpr auto _beginTaskRunVideoDocString =
        "Perform video specific checks before the process is started.\n";

//------------------------//
//----- CVideoOFTask -----//
//------------------------//
constexpr auto _videoProcessOFDocString =
        "Add optical flow specific features for methods based on OpenCV framework. "
        "This class handles the persistent data required to compute classical optical flow algorithms. "
        "It also offers a way to display the computed flow as a color map.\n\n"
        "Derived from :py:class:`CVideoTask`.\n";

constexpr auto _ctorVideoProcessOFDocString =
        "Construct CVideoOFTask object with the given name. "
        "Same inputs and outputs as :py:class:`CVideoTask`.\n\n"
        "Args:\n"
        "   arg1 (self)\n\n"
        "   arg2 (str): task name, must be unique\n";

constexpr auto _beginTaskRunVideoOFDocString =
        "See :py:meth:`CVideoTask.beginTaskRun`.\n";

constexpr auto _drawOptFlowMapDocString =
        "Add optical flow vectors to optical flow map (image).\n\n"
        "Args:\n"
        "   flow (Numpy array): source optical flow color map\n"
        "   vectors (Numpy array): optical flow vectors. Each pixel store vector coordinates of the optical flow.\n"
        "   step (int): sampling step\n";

constexpr auto _flowToDisplayDocString =
        "Generate a displayable image of the optical flow color map.\n\n"
        "Args:\n"
        "   flow (Numpy array): source optical flow color map\n\n"
        "Returns:\n"
        "   Numpy array: displayable color image (3 channels)\n";

//------------------------------//
//----- CVideoTrackingTask -----//
//------------------------------//
constexpr auto _videoProcessTrackingDocString =
        "Add specific features for tracking task. "
        "This class handles the graphics input to extract the region of interest to track. "
        "This region is limited to rectangle region. "
        "It also generates the tracking results as a binary mask and a dedicated graphics layer.\n\n"
        "Derived from :py:class:`CVideoTask`.\n";

constexpr auto _ctorVideoTrackingDocString =
        "Construct CVideoTrackingTask object with the given name.\n\n"
        "Inputs: same as :py:class:`CVideoTask`.\n\n"
        "Outputs:\n\n"
        "- binary mask (video frame)\n"
        "- original image (video frame)\n"
        "- graphics layer\n"
        "- dedicated measures\n\n"
        "Args:\n"
        "   arg1 (self)\n\n"
        "   arg2 (str): task name, must be unique\n";

constexpr auto _setRoiToTrackDocString =
        "Extract region of interest to track from graphics input.\n";

constexpr auto _manageOutputsDocString =
        "Fill all the outputs generated by tracking algorithm.\n\n"
        "- Output #1: binary mask of the tracked region\n"
        "- Output #2: original frame with tracked graphics item (overlay)\n"
        "- Output #3: graphics layer (tracked item)\n"
        "- Output #4: tracked region coordinates\n";

//-------------------------//
//----- CDnnTrainTask -----//
//-------------------------//
constexpr auto _dnnTrainProcessDocString =
        "Internal use only";

constexpr auto _ctor1DnnTrainProcessDocString =
        "Internal use only";

constexpr auto _ctor2DnnTrainProcessDocString =
        "Internal use only";

constexpr auto _getTensorboardLogDirDocString =
        "Return Ikomia default logging directory for Tensorboard.\n\n"
        "Returns:\n"
        "   str: path of Tensorboard log directory\n";

constexpr auto _enableMlflowDocString =
        "Enable or disable automatic launch of MLflow dashboard when training starts. "
        "Dashboard is opened just once except if a new call to this function is made before "
        "a new training job starts.\n"
        "Default: enable.\n\n"
        "Args:"
        "   enable (boolean): True or False\n";

constexpr auto _enableTensorboardDocString =
        "Enable or disable automatic launch of Tensorboard dashboard when training starts. "
        "Dashboard is opened just once except if a new call to this function is made before "
        "a new training job starts.\n"
        "Default: enable.\n\n"
        "Args:"
        "   enable (boolean): True or False\n";

//------------------------------//
//----- CDnnTrainTaskParam -----//
//------------------------------//
constexpr auto _dnnTrainProcessParamDocString =
        "Base class defining common deep learning hyper-parameters.\n\n"
        "Derived from :py:class:`~ikomia.core.pycore.CWorkflowTaskParam`.";

constexpr auto _setParamMapDocString =
        "Set task parameter names and values from the given :py:class:`~ikomia.core.pycore.ParamMap` object (same use as Python dict).\n\n"
        "Args:\n"
        "   params (:py:class:`~ikomia.core.pycore.ParamMap`): pairs of string (parameter name, parameter value)\n"
        "       Numeric values must be converted from str to the desired numeric type before use\n\n";

constexpr auto _getParamMapDocString =
        "Get task parameter names and values.\n\n"
        "Returns:\n"
        "   :py:class:`ParamMap`: dict-like structure storing pairs of string (parameter name, parameter value)\n";

constexpr auto _getHashValueDocString =
        "Get hash value from parameters values. "
        "The system uses this method to determine if task configuration has changed. \n\n"
        "Returns:\n"
        "   int: hash value\n";

//---------------------------//
//----- CIkomiaRegistry -----//
//---------------------------//
constexpr auto _ikomiaRegistryDocString =
        "Algorithms registry of the Ikomia platform.";

constexpr auto _setPluginsDirDocString =
        "Set directory where Ikomia plugins are stored.\n\n"
        "Args:\n"
        "   directory (str)\n";

constexpr auto _getPluginsDirDocString =
        "Get the current Ikomia plugins directory.\n\n"
        "Returns:\n"
        "   str: full path to Ikomia plugins directory\n";

constexpr auto _getAlgorithmsDocString =
        "Get all available algorithms from the Ikomia registry. Before using an algorithm, you must instanciate it "
        "from its name using :py:method:`~ikomia.dataprocess.pydataprocess.CIkomiaRegistry.createInstance`."
        "You can only instanciate algorithms whose name is in the returned list.\n\n"
        "Returns:\n"
        "   string: list of algorithm names\n";

constexpr auto _getAlgorithmInfoDocString =
        "Get algorithm informations such as description, authors, documentation link...\n\n"
        "Args:\n"
        "   name (str): algorithm name\n"
        "Returns:\n"
        "   algorithm information (:py:class:`~ikomia.dataprocess.pydataprocess.CTaskInfo`)\n";

constexpr auto _createInstance1DocString =
        "Instanciate algorithm of the Ikomia registry from its name with default parameters. The full list of available algorithms can be"
        "retrieved using :py:method:`~ikomia.dataprocess.pydataprocess.CIkomiaRegistry.getAlgorithms`.\n\n"
        "Args:\n"
        "   algorithm name(str): unique name\n\n"
        "Returns:\n"
        "   :py:class:`~ikomia.core.pycore.CWorkflowTask`-based object: algorithm instance\n";

constexpr auto _createInstance2DocString =
        "Instanciate algorithm of the Ikomia registry from its name with the given parameters. The full list of available algorithms can be"
        "retrieved using :py:method:`~ikomia.dataprocess.pydataprocess.CIkomiaRegistry.getAlgorithms`.\n\n"
        "Args:\n"
        "   algorithm name(str): unique name\n\n"
        "   parameters object(:py:class:`~ikomia.core.pycore.CWorkflowTaskParam`): associated parameters\n\n"
        "Returns:\n"
        "   :py:class:`~ikomia.core.pycore.CWorkflowTask`-based object: algorithm instance\n";

constexpr auto _registerTaskDocString =
        "Add new algorithm factory to Ikomia registry. Once registered, you are able to instanciate algorithm object "
        "and use it inside a workflow.\n\n"
        "Args:\n"
        "   task factory (:py:class:`~ikomia.dataprocess.pydataprocess.CTaskFactory` based object)\n";

constexpr auto _registerIODocString =
        "Add new input/output factory to Ikomia registry. Once registered, you are able to instanciate input or ouput "
        "and use it inside a task.\n\n"
        "Args:\n"
        "   I/O factory (:py:class:`~ikomia.core.pycore.CTaskIOFactory` based object)\n";

constexpr auto _loadCppPluginDocString =
        "Load C++ algorithm to Ikomia registry.\n\n"
        "Args:\n"
        "   path (str): path to shared library of the plugin\n";

//---------------------//
//----- CWorkflow -----//
//---------------------//
constexpr auto _workflowDocString =
        "";

constexpr auto _ctor1WorkflowDocString =
        "";

constexpr auto _ctor2WorkflowDocString =
        "";

constexpr auto _wfSetInputDocString =
        "";

constexpr auto _wfSetOutputFolderDocString =
        "";

constexpr auto _wfSetAutoSaveDocString =
        "";

constexpr auto _wfGetTaskCountDocString =
        "";

constexpr auto _wfGetRootTargetTypesDocString =
        "";

constexpr auto _wfAddInputDocString =
        "";

constexpr auto _wfClearInputsDocString =
        "";

constexpr auto _wfRunDocString =
        "";

constexpr auto _wfStopDocString =
        "";

constexpr auto _wfLoadDocString =
        "";

constexpr auto _wfUpdateStartTimeDocString =
        "";

constexpr auto _wfSaveDocString =
        "";

constexpr auto _wfExportGraphvizDocString =
        "";


#endif // PYDATAPROCESSDOCSTRING_HPP
