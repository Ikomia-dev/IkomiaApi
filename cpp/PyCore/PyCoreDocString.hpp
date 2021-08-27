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

#ifndef PYCOREDOCSTRING_HPP
#define PYCOREDOCSTRING_HPP

constexpr auto _moduleDocString =
        "Module offering core features to handle **tasks**, **I/O**, **parameters** and **widgets**.\n\n"
        "The extension capability of Ikomia platform is based on inheritance and this module offers the main base classes. "
        "The system uses the concept of worflow to process images. "
        "A worflow is a graph composed of processing tasks, each task comes with its parameters, its inputs and its outputs. "
        "Additionnally, a widget (for Graphical User Interface) is associated with the task to ensure user interaction. "
        "Through this API we provide base classes that you could override for each component.\n\n";

//---------------------//
//----- CPoint<T> -----//
//---------------------//
constexpr auto _ctorCPointDocString =
        "Construct 2D point with the given coordinates.\n\n"
        "Args:\n"
        "   arg1 : self\n\n"
        "   arg2 : x-coordinate\n\n"
        "   arg3 : y-coordinate\n";

//-------------------------//
//----- CGraphicsItem -----//
//-------------------------//
constexpr auto _graphicsItemDocString =
        "Base class for all graphics items that aim to be displayed on top of images. "
        "Such graphics items are inserted into an overlay layer associated with image. "
        "Image content is not modified so that it can be forwarded directly as input of another task. "
        "Moreover, graphics layer can also be forwarded as input of another task. "
        "One can imagine an example where we want to apply specific process to tracked object. "
        "Each graphics item has a type (see :py:class:`~ikomia.core.pycore.GraphicsItem` for possible values) and a category (label).\n\n";

constexpr auto _setGraphicsCategoryDocString =
        "Set the graphics item category. It's just a generic way to identify the item.\n\n"
        "Args:\n"
        "   category (str): identification label\n";

constexpr auto _getGraphicsIdDocString =
        "Get graphics item unique identifier.\n\n"
        "Returns:\n"
        "   int: identifier\n";

constexpr auto _getGraphicsTypeDocString =
        "Get graphics item type. See :py:class:`GraphicsItem` for possible values.\n\n"
        "Returns:\n"
        "   :py:class:`~ikomia.core.pycore.GraphicsItem`: type\n";

constexpr auto _getGraphicsCategoryDocString =
        "Get graphics item category.\n\n"
        "Returns:\n"
        "   str: category (label)\n";

constexpr auto _isGraphicsTextItemDocString =
        "Check whether item type is TEXT. "
        "In case of object detection workflow, text graphics items should not be useful as input for another task. "
        "This method enables to filter only shape-based items (rectangle, polygon...) when processing graphics input.\n\n"
        "Returns:\n"
        "   bool: True or False\n";

//-----------------------------------//
//----- CGraphicsComplexPolygon -----//
//-----------------------------------//
constexpr auto _graphicsComplexPolyDocString =
        "Graphics item to display complex polygon (with holes) in the overlay layer of an image. "
        "Such graphics item is defined by an outer polygon and a list of inner polygons (holes). "
        "Each polygon is a list of vertices defined by 2D point (x,y).\n"
        "Derived from :py:class:`~ikomia.core.pycore.CGraphicsItem`.\n\n";

constexpr auto _ctor1GraphicsComplexPoly =
        "Construct a complex polygon item.\n\n"
        "Args:\n"
        "   arg1: self\n\n"
        "   arg2 (list): list of :py:class:`~ikomia.core.pycore.CPointF` corresponding to vertices of outer polygon\n\n"
        "   arg3 (list of list): list of :py:class:`~ikomia.core.pycore.CPointF` lists. Each list corresponds to the vertices of one inner polygon (hole)\n";

constexpr auto _ctor2GraphicsComplexPoly =
        "Construct a complex polygon item.\n\n"
        "Args:\n"
        "   arg1: self\n\n"
        "   arg2 (list): list of :py:class:`~ikomia.core.pycore.CPointF` corresponding to vertices of outer polygon\n\n"
        "   arg3 (list of list): list of :py:class:`~ikomia.core.pycore.CPointF` lists. Each list corresponds to the vertices of one inner polygon (hole)\n\n"
        "   arg4 (:py:class:`~ikomia.core.pycore.GraphicsPolygonProperty`): information and visual properties.\n\n";

//----------------------------//
//----- CGraphicsEllipse -----//
//----------------------------//
constexpr auto _graphicsEllipseDocString =
        "Graphics item to display ellipse or circle in the overlay layer of an image. "
        "Such graphics item is defined by:\n\n"
        "- top-left point of the bounding box (x,y)\n"
        "- width (float)\n"
        "- height (float)\n"
        "Derived from :py:class:`~ikomia.core.pycore.CGraphicsItem`.\n\n";

constexpr auto _ctor1GraphicsEllipse =
        "Construct an ellipse item with the given dimensions.\n\n"
        "Args:\n"
        "   arg1: self\n\n"
        "   arg2 (float): x coordinate of the top-left point\n\n"
        "   arg3 (float): y coordinate of the top-left point\n\n"
        "   arg4 (float): width\n\n"
        "   arg5 (float): height\n";

constexpr auto _ctor2GraphicsEllipse =
        "Construct an ellipse item with the given dimensions.\n\n"
        "Args:\n"
        "   arg1: self\n\n"
        "   arg2 (float): x coordinate of the top-left point\n\n"
        "   arg3 (float): y coordinate of the top-left point\n\n"
        "   arg4 (float): width\n\n"
        "   arg5 (float): height\n\n"
        "   arg6 (:py:class:`~ikomia.core.pycore.GraphicsEllipseProperty`): information and visual properties.\n\n";

//--------------------------//
//----- CGraphicsPoint -----//
//--------------------------//
constexpr auto _graphicsPointDocString =
        "Graphics item to display point in the overlay layer of an image. "
        "Such graphics is defined by a 2D point (x,y)\n"
        "Derived from :py:class:`~ikomia.core.pycore.CGraphicsItem`.\n\n";

constexpr auto _ctor1GraphicsPoint =
        "Construct a point graphics with the given 2D point.\n\n"
        "Args:\n"
        "   arg1: self\n\n"
        "   arg2 (:py:class:`~ikomia.core.pycore.CPointF`): 2D points (float coordinates)\n";

constexpr auto _ctor2GraphicsPoint =
        "Construct a point graphics with the given 2D point.\n\n"
        "Args:\n"
        "   arg1: self\n\n"
        "   arg2 (:py:class:`~ikomia.core.pycore.CPointF`): 2D points (float coordinates)\n\n"
        "   arg3 (:py:class:`~ikomia.core.pycore.GraphicsPointProperty`): information and visual properties.\n\n";

//----------------------------//
//----- CGraphicsPolygon -----//
//----------------------------//
constexpr auto _graphicsPolygonDocString =
        "Graphics item to display polygon in the overlay layer of an image. "
        "Such graphics is defined by a list of 2D points (x,y) corresponding to vertices.\n"
        "Derived from :py:class:`~ikomia.core.pycore.CGraphicsItem`.\n\n";

constexpr auto _ctor1GraphicsPolygon =
        "Construct a polygon graphics with the given vertices.\n\n"
        "Args:\n"
        "   arg1: self\n\n"
        "   arg2 (list): list of :py:class:`~ikomia.core.pycore.CPointF` corresponding to vertices\n";

constexpr auto _ctor2GraphicsPolygon =
        "Construct a polygon graphics with the given vertices.\n\n"
        "Args:\n"
        "   arg1: self\n\n"
        "   arg2 (list): list of :py:class:`~ikomia.core.pycore.CPointF` corresponding to vertices\n"
        "   arg3 (:py:class:`~ikomia.core.pycore.GraphicsPolygonProperty`): information and visual properties.\n\n";

//------------------------------//
//----- CGraphicsPolyline -----//
//------------------------------//
constexpr auto _graphicsPolylineDocString =
        "Graphics item to display polyline in the overlay layer of an image. "
        "Such graphics is defined by a list of 2D points (x,y) corresponding to vertices.\n"
        "Derived from :py:class:`~ikomia.core.pycore.CGraphicsItem`.\n\n";

constexpr auto _ctor1GraphicsPolyline =
        "Construct a polyline graphics with the given vertices.\n\n"
        "Args:\n"
        "   arg1: self\n\n"
        "   arg2 (list): list of :py:class:`~ikomia.core.pycore.CPointF` corresponding to vertices\n";

constexpr auto _ctor2GraphicsPolyline =
        "Construct a polyline graphics with the given vertices.\n\n"
        "Args:\n"
        "   arg1: self\n\n"
        "   arg2 (list): list of :py:class:`~ikomia.core.pycore.CPointF` corresponding to vertices\n\n"
        "   arg3 (:py:class:`~ikomia.core.pycore.GraphicsPolylineProperty`): information and visual properties.\n\n";

//------------------------------//
//----- CGraphicsRectangle -----//
//------------------------------//
constexpr auto _graphicsRectangleDocString =
        "Graphics item to display rectangle or square in the overlay layer of an image. "
        "Such graphics is defined by:\n\n"
        "- top-left point (x,y)\n"
        "- width (float)\n"
        "- height (float)\n"
        "Derived from :py:class:`~ikomia.core.pycore.CGraphicsItem`.\n\n";

constexpr auto _ctor1GraphicsRectangle =
        "Construct a rectangle graphics with the given dimensions.\n\n"
        "Args:\n"
        "   arg1: self\n\n"
        "   arg2 (float): x coordinate of the top-left point\n\n"
        "   arg3 (float): y coordinate of the top-left point\n\n"
        "   arg4 (float): width\n\n"
        "   arg5 (float): height\n";

constexpr auto _ctor2GraphicsRectangle =
        "Construct a rectangle graphics with the given dimensions.\n\n"
        "Args:\n"
        "   arg1: self\n\n"
        "   arg2 (float): x coordinate of the top-left point\n\n"
        "   arg3 (float): y coordinate of the top-left point\n\n"
        "   arg4 (float): width\n\n"
        "   arg5 (float): height\n\n"
        "   arg6 (:py:class:`~ikomia.core.pycore.GraphicsRectProperty`): information and visual properties.\n\n";

//-------------------------//
//----- CGraphicsText -----//
//-------------------------//
constexpr auto _graphicsTextDocString =
        "Graphics item to display text in the overlay layer of an image. "
        "Such graphics is defined by:\n\n"
        "- top-left position (x,y)\n"
        "- text (str) \n"
        "Derived from :py:class:`~ikomia.core.pycore.CGraphicsItem`.\n\n";

constexpr auto _ctor1GraphicsText =
        "Construct a graphics item with the given text.\n\n"
        "Args:\n"
        "   arg1: self\n\n"
        "   arg2 (str): text\n";

constexpr auto _ctor2GraphicsText =
        "Construct a graphics item at the given position with the given text.\n\n"
        "Args:\n"
        "   arg1: self\n\n"
        "   arg2 (str): text\n\n"
        "   arg3 (float): left coordinate\n\n"
        "   arg2 (float): top coordinate\n";

constexpr auto _ctor3GraphicsText =
        "Construct a graphics item at the given position with the given text.\n\n"
        "Args:\n"
        "   arg1: self\n\n"
        "   arg2 (str): text\n\n"
        "   arg3 (float): left coordinate\n\n"
        "   arg4 (:py:class:`~ikomia.core.pycore.GraphicsTextProperty`): information and visual properties.\n\n";

//-------------------------------//
//----- CGraphicsConversion -----//
//-------------------------------//
constexpr auto _graphicsConvDocString =
        "Expose conversion operations based on graphics objects and images.\n\n";

constexpr auto _ctorGraphicsConv =
        "Construct a graphics conversion object handling images of given dimensions.\n\n"
        "Args:\n"
        "   arg1: self\n\n"
        "   arg2 (int): image width\n\n"
        "   arg3 (int): image height\n\n";

constexpr auto _graphicsToBinaryMaskDocString =
        "Convert from a list of given graphics to a binary mask.\n\n"
        "Args:\n"
        "   graphics: list of :py:class:`~ikomia.core.pycore.CGraphicsItem`.\n\n";

//-------------------------//
//----- CWorkflowTask -----//
//-------------------------//
constexpr auto _WorkflowTaskDocString =
        "Base class for all tasks that aim to be executed in a workflow. "
        "It provides all basic mechanisms to handle inputs and outputs, task parameters and progress feedbacks. "
        "It also provides an interface through overridable methods to set a formal scope to design custom task. "
        "This interface allows the implementation of various image-based process. "
        "One can also use the derived classes of the API which cover basic needs for "
        "2D image, volume (see :py:class:`~ikomia.dataprocess.pydataprocess.C2dImageTask`), "
        "or video (see :py:class:`~ikomia.dataprocess.pydataprocess.CVideoTask`).\n\n";

constexpr auto _ctorWorkflowTaskDocString =
        "Construct CWorkflowTask object with the given task name.\n\n"
        "Args:\n"
        "   arg1: self\n\n"
        "   arg2 (str): task name, must be unique\n";

constexpr auto _addInputDocString =
        "Add new input to the task.\n\n"
        "Args:\n"
        "   input: :py:class:`~ikomia.core.pycore.CWorkflowTaskIO` based object\n";

constexpr auto _addOutputDocString =
        "Add new output to the task.\n\n"
        "Args:\n"
        "   output: :py:class:`~ikomia.core.pycore.CWorkflowTaskIO` based object\n";

constexpr auto _removeInputDocString =
        "Remove input at the given position.\n\n"
        "Args:\n"
        "   index: zero-based input index\n";

constexpr auto _beginTaskRunDocString =
        "Perform all initialization stuff before running the task. "
        "This method can be overriden to put custom initialization steps. "
        "In this case, don't forget to call the base class method. This method must be the first call of the :py:meth:`~ikomia.core.pycore.CWorkflowTask.run` method.\n";

constexpr auto _emitGraphicsContextChangedDocString =
        "Send event to notify that graphics context has changed inside the task and display has to be updated.\n";

constexpr auto _emitAddSubProgressStepsDocString =
        "Send event to add or remove progress steps to the progress bar.\n\n"
        "Args:\n"
        "   count (int): positive value to add steps, negative value to remove steps.\n";

constexpr auto _emitStepProgressDocString =
        "Send step event to notify progress bar system.\n";

constexpr auto _emitOutputChangedDocString =
        "Send event to notify that some outputs have changed and that display must be updated.\n";

constexpr auto _endTaskRunDocString =
        "Performs all finalization stuff after running the task. "
        "This method can be overriden to put custom finalization steps. "
        "In this case, don't forget to call the base class method. This method must be the last call of the :py:meth:`~ikomia.core.pycore.CWorkflowTask.run` method.\n";

constexpr auto _executeActionsDocString =
        "Execute actions according to the specific defined behavior. In this base class, the method does nothing.\n\n"
        "Args:\n"
        "   action: see :py:class:`~ikomia.core.pycore.ActionFlag` for possible values\n";

constexpr auto _getElapsedTimeDocString =
        "Get the time of the last execution in milliseconds.\n\n"
        "Returns:\n"
        "   float: elapsed time in ms\n";

constexpr auto _getInputDocString =
        "Get input at position index.\n\n"
        "Args:\n"
        "   index (int): input index\n"
        "Returns:\n"
        "   :py:class:`~ikomia.core.pycore.CWorkflowTaskIO` based object\n";

constexpr auto _getInputCountDocString =
        "Get the number of inputs.\n\n"
        "Returns:\n"
        "   int: inputs count\n";

constexpr auto _getInputDataTypeDocString =
        "Get input data type at position index. "
        "This data type can differ from the original type because it can change at runtime according to the data source.\n\n"
        "Args:\n"
        "   index (int): zero-based input index\n"
        "Returns:\n"
        "   :py:class:`~ikomia.core.pycore.IODataType`: input data type\n";

constexpr auto _getInputsDocString =
        "Get the whole list of inputs.\n\n"
        "Returns:\n"
        "   list: list of :py:class:`~ikomia.core.pycore.CWorkflowTaskIO` based objects\n";

constexpr auto _getOutputDocString =
        "Get output at position index.\n\n"
        "Args:\n"
        "   index (int): output index\n"
        "Returns:\n"
        "   :py:class:`~ikomia.core.pycore.CWorkflowTaskIO` based object\n";

constexpr auto _getOutputCountDocString =
        "Get the number of outputs.\n\n"
        "Returns:\n"
        "   int: outputs count\n";

constexpr auto _getOutputDataTypeDocString =
        "Get output data type at position index. "
        "This data type can differ from the original type because it can change at runtime according to the data source.\n\n"
        "Args:\n"
        "   index (int): zero-based input index\n"
        "Returns:\n"
        "   :py:class:`~ikomia.core.pycore.IODataType`: output data type\n";

constexpr auto _getOutputsDocString =
        "Get the whole list of outputs.\n\n"
        "Returns:\n"
        "   list: list of :py:class:`~ikomia.core.pycore.CWorkflowTaskIO` based objects\n";

constexpr auto _getParamDocString =
        "Get task parameters.\n\n"
        "Returns:\n"
        "   :py:class:`~ikomia.core.pycore.CWorkflowTaskParam` based object\n";

constexpr auto _getParamValuesDocString =
        "Get values of task parameters.\n\n"
        "Returns:\n"
        "   :py:class:`ikomia.core.pycore.ParamMap`: dict-like structure of string pairs (parameter name, value)\n";

constexpr auto _getProgressSteps1DocString =
        "Get the number of progress steps when the system runs the task.\n\n"
        "Returns:\n"
        "   int: steps count\n";

constexpr auto _getProgressSteps2DocString =
        "Get the number of progress steps when the systems runs the task. "
        "It takes into account the number of unit element to process.\n\n"
        "Args:\n"
        "   unit_elt_count (int): unit elements count. See :py:class:`~ikomia.core.pycore.CWorkflowTaskIO` for more details.\n\n"
        "Returns:\n"
        "   int: steps count\n";

constexpr auto _globalInputChangedDocString =
        "Notify that the inputs of the workflow have changed. "
        "This method can be overriden to implement custom actions when this event happens. "
        "The method does nothing in this base class.\n\n"
        "Args:\n"
        "   is_new_sequence (bool): indicate if new input is a new sequence (ex: new frame of the same video is not a new sequence)\n";

constexpr auto _graphicsChangedDocString =
        "Notify that graphics layers of input images have changed. "
        "This method can be overriden to implement custom actions when this event happens. "
        "The method does nothing in this base class.\n";

constexpr auto _isGraphicsChangedListeningDocString =
        "Check whether the task is listening to graphics changed event.\n\n"
        "Returns:\n"
        "   bool: True or False\n";

constexpr auto _parametersModifiedDocString =
        "Notify that the task parameters have changed. "
        "This method can be overriden to implement custom actions when this event happens. "
        "The method does nothing in this base class.\n";

constexpr auto _runDocString =
        "Run the task. It's where the main process of the task has to be implemented. "
        "In this base class, the method just forwards the inputs to outputs. It has to be overriden in derived class.\n";

constexpr auto _stopDocString =
        "Notify that the task is requested to stop. "
        "It is higly recommended to manage this stop event and override the method for time-consuming task. "
        "Base class implementation must be called before any other instructions.\n";

constexpr auto _setActionFlagDocString =
        "Enable or disable the given action. If the action does not exist, it is added to the list.\n\n"
        "Args:\n"
        "   action: action to enable or disable, see :py:class:`~ikomia.core.pycore.ActionFlag` for possible values\n"
        "   is_enable (bool): True or False\n";

constexpr auto _setActiveDocString =
        "Set the active state of the task. "
        "The active task is the one selected in the workflow, thus, user has access to parameters and "
        "can visualize results associated with the task.\n\n"
        "Args:\n"
        "   is_active: True or False\n";

constexpr auto _setInputDocString =
        "Set input at position index with the given one. "
        "If the input at position index does not exist, the function creates as many generic inputs to reach the number of index+1 "
        "and sets the input at position index.\n\n"
        "Args:\n"
        "   input: :py:class:`~ikomia.core.pycore.CWorkflowTaskIO` based object\n\n"
        "   index (int): zero-based input index\n\n"
        "   is_new_sequence (bool): indicate if new input is a new sequence (ex: new frame of the same video is not a new sequence)\n";

constexpr auto _setInputDataTypeDocString =
        "Set the data type for the input at position index. "
        "If the input at position index does not exist, the function creates as many generic inputs to reach the number of index+1 "
        "and sets the data type for the input at position index.\n\n"
        "Args:\n"
        "   data_type (:py:class:`~ikomia.core.pycore.IODataType`): input data type\n\n"
        "   index (int): zero-based input index\n";

constexpr auto _setInputsDocString =
        "Set the whole list of inputs with the given one.\n\n"
        "Args:\n"
        "   inputs (list): list of :py:class:`~ikomia.core.pycore.CWorkflowTaskIO` based objects\n"
        "   is_new_sequence (bool): indicate if new input is a new sequence (ex: new frame of the same video is not a new sequence)\n";

constexpr auto _setOutputDocString =
        "Set output at position index with the given one. "
        "If the output at position index does not exist, the function creates as many generic outputs to reach the number of index+1 "
        "and sets the output at position index.\n\n"
        "Args:\n"
        "   output: :py:class:`~ikomia.core.pycore.CWorkflowTaskIO` based object\n\n"
        "   index (int): zero-based output index\n";

constexpr auto _setOutputDataTypeDocString =
        "Set the data type for the output at position index. "
        "If the output at position index does not exist, the function creates as many generic outputs to reach the number of index+1 "
        "and sets the data type for the output at position index.\n\n"
        "Args:\n"
        "   data_type (:py:class:`~ikomia.core.pycore.IODataType`): output data type\n\n"
        "   index (int): zero-based output index\n";

constexpr auto _setOutputsDocString =
        "Set the whole list of outputs with the given one.\n\n"
        "Args:\n"
        "   outputs (list): list of :py:class:`~ikomia.core.pycore.CWorkflowTaskIO` based objects\n";

constexpr auto _setParamDocString =
        "Set the task parameters object. Task can have only one parameters object.\n\n"
        "Args:\n"
        "   param: :py:class:`~ikomia.core.pycore.CWorkflowTaskParam` based object\n";

constexpr auto _setParamValuesDocString =
        "Set values of task parameters.\n\n"
        "Args:\n"
        "   values: :py:class:`~ikomia.core.pycore.ParamMap`, dict-like structure of string pairs (parameter name, value)\n";

constexpr auto _updateStaticOutputsDocString =
        "Updates the static information deduced from inputs. "
        "The static data corresponds to all data that can be deduced without the runtime context. "
        "This method is called each time inputs changed.\n";

//---------------------------//
//----- CWorkflowTaskIO -----//
//---------------------------//
constexpr auto _WorkflowTaskIODocString =
        "Base class for the management of task inputs and outputs. "
        "Each task can have as many inputs and outputs, and each must be derived from this class.\n\n";

constexpr auto _ctor1WorkflowTaskIODocString =
        "Constructor with parameters\n\n"
        "Args:\n"
        "   data type (:py:class:`~ikomia.core.pycore.IODataType`)\n";

constexpr auto _ctor2WorkflowTaskIODocString =
        "Constructor with parameters\n\n"
        "Args:\n"
        "   data type (:py:class:`~ikomia.core.pycore.IODataType`)\n\n"
        "   name (str)\n";

constexpr auto _getUnitElementCountDocString =
        "Get the number of unit element in terms of processing scheme. "
        "This value is used to define the number of progress steps for progress bar component. "
        "For an image, the count is 1. For Z-stack volume, the count is the number of Z levels. "
        "Should be overriden for custom input or output.\n\n"
        "Returns:\n"
        "   int: number of unit element to process.\n";

constexpr auto _isDataAvailableDocString =
        "Check whether input or output objects contain valid data. "
        "For inputs, it is the case when the previous process contains valid output data. "
        "For outputs, it is the case when the associated task had been computed at least once. "
        "Should be overriden for custom input or output.\n\n"
        "Returns:\n"
        "   bool: True if data is available, False otherwise.\n";

constexpr auto _isAutoInputDocString =
        "Check whether input data from external source is mandatory.\n\n"
        "Returns:\n"
        "   bool: True or False.\n\n";

constexpr auto _setDisplayableDocString =
        "Make the output displayable or not in Ikomia Studio. The output still appear in the workflow editor "
        "and can be connected to compatible input.\n\n"
        "Args\n"
        "   displayable (boolean): True or False.\n\n";

constexpr auto _clearDataDocString =
        "Clear the data stored in the object. Should be overriden for custom input or output.\n\n";

constexpr auto _copyStaticDataDocString =
        "Copy the static data from the given input or ouput. "
        "Static data are those which are not generated at runtime. Should be overriden for custom input or output.\n\n"
        "Args:\n"
        "   io (:py:class:`~ikomia.core.pycore.CWorkflowTaskIO`): input or ouput instance from which data is copied.\n";

//----------------------------------//
//----- CWorkflowTaskIOFactory -----//
//----------------------------------//
constexpr auto _ioFactoryDocString =
        "Abstract class defining the core structure of the task I/O factory. "
        "The system extensibility is based on the factory design pattern. "
        "Each task input/output must implement a factory class derived from this class. "
        "Then the system is able to instantiate dynamically a I/O object (even user-defined).\n\n";

constexpr auto _ioFactoryCreateDocString =
        "Pure virtual method to create new task I/O instance with the given data type. "
        "Must be implemented.\n\n"
        "Args:\n"
        "   data type (:py:class:`~ikomia.core.pycore.IODataType`)"
        "Returns:\n"
        "   :py:class:`~ikomia.core.pycore.CWorkflowTaskIO` based object\n";

//------------------------------//
//----- CWorkflowTaskParam -----//
//------------------------------//
constexpr auto _WorkflowTaskParamDocString =
        "Base class for the management of task parameters. "
        "It consists of two virtual methods that should be overriden to fill or get the structure holding parameters value. "
        "This structure is used internally by the system to save and load parameters values of task used in a workflow.\n\n";

constexpr auto _setParamMapDocString =
        "Set task parameter names and values from the given :py:class:`ParamMap` object (same use as Python dict).\n\n"
        "Args:\n"
        "   params (:py:class:`~ikomia.core.pycore.ParamMap`): pairs of string (parameter name, parameter value)\n"
        "       Numeric values must be converted from str to the desired numeric type before use\n\n";

constexpr auto _getParamMapDocString =
        "Get task parameter names and values.\n\n"
        "Returns:\n"
        "   :py:class:`~ikomia.core.pycore.ParamMap`: dict-like structure storing pairs of string (parameter name, parameter value)\n";

constexpr auto _getHashValueDocString =
        "Get hash value from parameters values. "
        "The system uses this method to determine if task configuration has changed. \n\n"
        "Returns:\n"
        "   int: hash value\n";

//-------------------------------//
//----- CWorkflowTaskWidget -----//
//-------------------------------//
constexpr auto _WorkflowTaskWidget =
        "Base class for all widgets associated with a task. "
        "Each process comes with a Qt widget to manage algorithm parameters and user actions. "
        "The widget class implementation is mandatory. "
        "This class offers some basic tools to build rapidly a functionnal user interface. "
        "As it is derived from Qt QWidget, one can easily customize it to fit more complex needs. U"
        "se of bindings like PyQt5 or PySide2 are recommended.\n";

constexpr auto _setLayoutDocString =
        "Set the main layout of the widget. "
        "Use :py:mod:`~ikomia.utils.qtconversion` module to get C++ handle from Python Qt-based framework.\n\n"
        "Args:\n"
        "   layout: compatible C++ layout handle\n";

constexpr auto _setApplyBtnHiddenDocString =
        "Make the Apply button of the widget visible or not.\n\n"
        "Args:\n"
        "   is_hidden: True or False\n";

constexpr auto _initDocString =
        "Initialization of the widget. Must be overriden.\n";

constexpr auto _applyDocString =
        "Called when user presses the Apply button.\n";

constexpr auto _emitApplyDocString =
        "Send apply event to the system. The system then launches the task execution.\n";

constexpr auto _emitSendProcessActionDocString =
        "Send event to request specific action of the process task. "
        "Use this mechanism to manage user interaction with the process task for example.\n\n"
        "Args:\n"
        "   action (:py:class:`~ikomia.core.pycore.ActionFlag`): action code (can be user defined)";

constexpr auto _emitSetGraphicsToolDocString =
        "Send event to change the current graphic tool.\n\n"
        "Args:\n"
        "   tool (:py:class:`~ikomia.core.pycore.GraphicsShape`): new current tool\n";

constexpr auto _emitSetGraphicsCategoryDocString =
        "Send event to change the current graphic category.\n\n"
        "Args:\n"
        "   category (str): category name\n";

//--------------------//
//----- CMeasure -----//
//--------------------//
constexpr auto _measureDocString =
        "Class to handle available measures that can be computed on blob object in image. "
        "Here is the list of possible measures (:py:class:`~ikomia.core.pycore.MeasureId):` \n"
        "   - Surface: `cv::contourArea <https://docs.opencv.org/4.5.2/d3/dc0/group__imgproc__shape.html#ga2c759ed9f497d4a618048a2f56dc97f1>`_\n"
        "   - Perimeter: `cv::arcLength <https://docs.opencv.org/4.5.2/d3/dc0/group__imgproc__shape.html#ga8d26483c636be6b35c3ec6335798a47c>`_\n"
        "   - Centroid: computed from moments or average on points coordinates if moments are not available `cv::Moments <https://docs.opencv.org/4.5.2/d8/d23/classcv_1_1Moments.html>`_\n"
        "   - Bbox: `cv::boundingRect <https://docs.opencv.org/4.5.2/d3/dc0/group__imgproc__shape.html#ga103fcbda2f540f3ef1c042d6a9b35ac7>`_\n"
        "   - Oriented bbox: `cv::minAreaRect <https://docs.opencv.org/4.5.2/d3/dc0/group__imgproc__shape.html#ga3d476a3417130ae5154aea421ca7ead9>`_\n"
        "   - Equivalent diameter: estimation based on blob surface\n"
        "   - Elongation: computed from R. Mukundan and K.R. Ramakrishnan. Moment Functions in Image Analysis –Theory and Applications. World Scientific, 1998\n"
        "   - Circularity: computed from surface and perimeter -> circularity = (4*PI*surface) / (perimeter²)\n"
        "   - Solidity: computed from the convex hull -> solidity = surface / hullSurface\n";

constexpr auto _ctor1MeasureDocString =
        "Construct a measure with the given identifier.\n\n"
        "Args:\n"
        "   id: :py:class:`~ikomia.core.pycore.MeasureId";

constexpr auto _getAvailableMeasuresDocString =
        "Get available measures (static method).\n"
        "Returns:"
        "   list of CMeasure\n";

constexpr auto _getNameDocString =
        "Get measure name from its identifier (static method).\n"
        "Returns:"
        "   measure name (str)\n";

#endif
