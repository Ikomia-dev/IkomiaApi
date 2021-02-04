#include "PyCore.h"
#include "PyCoreDocString.hpp"
#include <QString>
#include <QHash>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include "CProtocolTaskParamWrap.h"
#include "CProtocolTaskIOWrap.h"
#include "CProtocolTaskWrap.h"
#include "CProtocolTaskWidgetWrap.h"
#include "Data/CvMatNumpyArrayConverter.h"
#include "CGraphicsItemWrap.h"
#include "Graphics/CGraphicsComplexPolygon.h"
#include "Graphics/CGraphicsEllipse.h"
#include "Graphics/CGraphicsPoint.h"
#include "Graphics/CGraphicsPolygon.h"
#include "Graphics/CGraphicsPolyline.h"
#include "Graphics/CGraphicsRectangle.h"
#include "Graphics/CGraphicsText.h"
#include "Graphics/CGraphicsConversion.h"

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#define PY_ARRAY_UNIQUE_SYMBOL IKOMIA_ARRAY_API
#include <numpy/ndarrayobject.h>

//Numpy initialization
static bool init_numpy()
{
    import_array();

    if(PyArray_API == NULL)
        return false;
    else
        return true;
}

template<typename T>
void exposeCPoint(const std::string& className)
{
    class_<CPoint<T>>(className.c_str(), "Generic 2D point class.", init<>("Default constructor"))
        .def(init<T, T>(_ctorCPointDocString))
        .add_property("x", &CPoint<T>::getX, &CPoint<T>::setX, "x-coordinate")
        .add_property("y", &CPoint<T>::getY, &CPoint<T>::setY, "y-coordinate")
    ;
}


BOOST_PYTHON_MODULE(pycore)
{
    // Enable user-defined docstrings and python signatures, while disabling the C++ signatures
    docstring_options local_docstring_options(true, true, false);

    // Set the docstring of the current module scope
    scope().attr("__doc__") = _moduleDocString;

    //Numpy initialization
    init_numpy();

    //CMat <-> Numpy NdArray converters
    to_python_converter<CMat, BoostCvMatToNumpyArrayConverter>();
    BoostNumpyArrayToCvMatConverter();

    //Register smart pointers
    register_ptr_to_python<std::shared_ptr<CProtocolTaskIO>>();
    register_ptr_to_python<std::shared_ptr<CProtocolTask>>();
    register_ptr_to_python<std::shared_ptr<CProtocolTaskWidget>>();
    register_ptr_to_python<std::shared_ptr<CProxyGraphicsItem>>();

    registerStdVector<int>();
    registerStdVector<double>();
    registerStdVector<std::string>();
    registerStdVector<CPoint<float>>();
    registerStdVector<std::vector<CPoint<float>>>();
    registerStdVector<ProxyGraphicsItemPtr>();

    //--------------------//
    //----- Graphics -----//
    //--------------------//
    enum_<GraphicsShape>("GraphicsShape", "Enum - List of available graphics shapes")
        .value("ELLIPSE", GraphicsShape::ELLIPSE)
        .value("FREEHAND_POLYGON", GraphicsShape::FREEHAND_POLYGON)
        .value("FREEHAND_POLYLINE", GraphicsShape::FREEHAND_POLYLINE)
        .value("LINE", GraphicsShape::LINE)
        .value("POINT", GraphicsShape::POINT)
        .value("POLYGON", GraphicsShape::POLYGON)
        .value("POLYLINE", GraphicsShape::POLYLINE)
        .value("RECTANGLE", GraphicsShape::RECTANGLE)
        .value("SELECTION", GraphicsShape::SELECTION)
        .value("TEXT", GraphicsShape::TEXT)
    ;

    enum_<GraphicsItem>("GraphicsItem", "Enum - List of available graphics item types (ie annotations)")
        .value("LAYER", GraphicsItem::LAYER)
        .value("ELLIPSE", GraphicsItem::ELLIPSE)
        .value("RECTANGLE", GraphicsItem::RECTANGLE)
        .value("POINT", GraphicsItem::POINT)
        .value("POLYGON", GraphicsItem::POLYGON)
        .value("COMPLEX_POLYGON", GraphicsItem::COMPLEX_POLYGON)
        .value("POLYLINE", GraphicsItem::POLYLINE)
        .value("TEXT", GraphicsItem::TEXT)
    ;

    exposeCPoint<float>("CPointF");    

    //Base class of all graphics items
    class_<CGraphicsItemWrap, std::shared_ptr<CGraphicsItemWrap>, boost::noncopyable>("CGraphicsItem", _graphicsItemDocString, init<>("Default constructor"))
        .def(init<GraphicsItem>("Constructor with item type definition"))
        .def("setCategory", &CProxyGraphicsItem::setCategory, _setGraphicsCategoryDocString, args("self", "category"))
        .def("getId", &CProxyGraphicsItem::getId, _getGraphicsIdDocString, args("self"))
        .def("getType", &CProxyGraphicsItem::getType, _getGraphicsTypeDocString, args("self"))
        .def("getCategory", &CProxyGraphicsItem::getCategory, _getGraphicsCategoryDocString, args("self"))
        .def("isTextItem", &CProxyGraphicsItem::isTextItem, &CGraphicsItemWrap::default_isTextItem, _isGraphicsTextItemDocString, args("self"))
        .def("insertToImage", pure_virtual(&CGraphicsItemWrap::insertToImage), "**Internal use only.**")
    ;

    //Complex polygon
    class_<CProxyGraphicsComplexPoly, bases<CProxyGraphicsItem>, std::shared_ptr<CProxyGraphicsComplexPoly>>("CGraphicsComplexPolygon", _graphicsComplexPolyDocString)
        .def(init<>("Default constructor"))
        .def(init<const std::vector<CPoint<float>>, const std::vector<PolygonF>>(_ctor1GraphicsComplexPoly))
        .def(init<const std::vector<CPoint<float>>, const std::vector<PolygonF>, const GraphicsPolygonProperty&>(_ctor2GraphicsComplexPoly))
        .add_property("outer", &CProxyGraphicsComplexPoly::getOuter, &CProxyGraphicsComplexPoly::setOuter, "Outer polygon (list of vertices)")
        .add_property("inners", &CProxyGraphicsComplexPoly::getInners, &CProxyGraphicsComplexPoly::setInners, "Inner polygons (list of inner polygons corresponding to holes)")
    ;

    //Ellipse
    class_<GraphicsEllipseProperty>("GraphicsEllipseProperty", "Visual properties for :py:class:`~PyCore.CGraphicsEllipse` item.")
        .def_readwrite("pen_color", &GraphicsEllipseProperty::m_penColor, "Outline color (list - rgba)")
        .def_readwrite("brush_color", &GraphicsEllipseProperty::m_brushColor, "Fill color (list - rgba)")
        .def_readwrite("line_size", &GraphicsEllipseProperty::m_lineSize, "Outline size")
        .def_readwrite("category", &GraphicsEllipseProperty::m_category, "Graphics category")
    ;

    class_<CProxyGraphicsEllipse, bases<CProxyGraphicsItem>, std::shared_ptr<CProxyGraphicsEllipse>>("CGraphicsEllipse", _graphicsEllipseDocString)
        .def(init<>("Default constructor"))
        .def(init<float, float, float, float>(_ctor1GraphicsEllipse))
        .def(init<float, float, float, float, const GraphicsEllipseProperty&>(_ctor2GraphicsEllipse))
        .add_property("x", &CProxyGraphicsEllipse::getX, &CProxyGraphicsEllipse::setX, "x coordinate of top-left point")
        .add_property("y", &CProxyGraphicsEllipse::getY, &CProxyGraphicsEllipse::setY, "y coordinate of top-left point")
        .add_property("width", &CProxyGraphicsEllipse::getWidth, &CProxyGraphicsEllipse::setWidth, "Ellipse width")
        .add_property("height", &CProxyGraphicsEllipse::getHeight, &CProxyGraphicsEllipse::setHeight, "Ellipse height")
    ;

    //Point
    class_<GraphicsPointProperty>("GraphicsPointProperty", "Visual properties for :py:class:`~PyCore.CGraphicsPoint` item.")
        .def_readwrite("pen_color", &GraphicsPointProperty::m_penColor, "Outline color (list - rgba)")
        .def_readwrite("brush_color", &GraphicsPointProperty::m_brushColor, "Fill color (list - rgba)")
        .def_readwrite("size", &GraphicsPointProperty::m_size, "Size")
        .def_readwrite("category", &GraphicsPointProperty::m_category, "Graphics category")
    ;

    class_<CProxyGraphicsPoint, bases<CProxyGraphicsItem>, std::shared_ptr<CProxyGraphicsPoint>>("CGraphicsPoint", _graphicsPointDocString)
        .def(init<>("Default constructor"))
        .def(init<const CPoint<float>&>(_ctor1GraphicsPoint))
        .def(init<const CPoint<float>&, const GraphicsPointProperty&>(_ctor2GraphicsPoint))
        .add_property("point", &CProxyGraphicsPoint::getPoint, &CProxyGraphicsPoint::setPoint, "2D point coordinates (:py:class:`CPointF`)")
    ;

    //Polygon
    class_<GraphicsPolygonProperty>("GraphicsPolygonProperty", "Visual properties for :py:class:`~PyCore.CGraphicsPolygon` item.")
        .def_readwrite("pen_color", &GraphicsPolygonProperty::m_penColor, "Outline color (list - rgba)")
        .def_readwrite("brush_color", &GraphicsPolygonProperty::m_brushColor, "Fill color (list - rgba)")
        .def_readwrite("line_size", &GraphicsPolygonProperty::m_lineSize, "Outline size")
        .def_readwrite("category", &GraphicsPolygonProperty::m_category, "Graphics category")
    ;

    class_<CProxyGraphicsPolygon, bases<CProxyGraphicsItem>, std::shared_ptr<CProxyGraphicsPolygon>>("CGraphicsPolygon", _graphicsPolygonDocString)
        .def(init<>("Default constructor"))
        .def(init<const std::vector<CPoint<float>>>(_ctor1GraphicsPolygon))
        .def(init<const std::vector<CPoint<float>>, const GraphicsPolygonProperty&>(_ctor2GraphicsPolygon))
        .add_property("points", &CProxyGraphicsPolygon::getPoints, &CProxyGraphicsPolygon::setPoints, "List of polygon vertices (:py:class:`CPointF`)")
    ;

    //Polyline
    class_<GraphicsPolylineProperty>("GraphicsPolylineProperty", "Visual properties for :py:class:`~PyCore.CGraphicsPolyline` item.")
        .def_readwrite("pen_color", &GraphicsPolylineProperty::m_penColor, "Outline color (list - rgba)")
        .def_readwrite("line_size", &GraphicsPolylineProperty::m_lineSize, "Outline size")
        .def_readwrite("category", &GraphicsPolylineProperty::m_category, "Graphics category")
    ;

    class_<CProxyGraphicsPolyline, bases<CProxyGraphicsItem>, std::shared_ptr<CProxyGraphicsPolyline>>("CGraphicsPolyline", _graphicsPolylineDocString)
        .def(init<>("Default constructor"))
        .def(init<const std::vector<CPoint<float>>>(_ctor1GraphicsPolyline))
        .def(init<const std::vector<CPoint<float>>, const GraphicsPolylineProperty&>(_ctor2GraphicsPolyline))
        .add_property("points", &CProxyGraphicsPolyline::getPoints, &CProxyGraphicsPolyline::setPoints, "List of polyline vertices (:py:class:`CPointF`)")
    ;

    //Rectangle
    class_<GraphicsRectProperty>("GraphicsRectProperty", "Visual properties for :py:class:`~PyCore.CGraphicsRectangle` item.")
        .def_readwrite("pen_color", &GraphicsRectProperty::m_penColor, "Outline color (list - rgba)")
        .def_readwrite("brush_color", &GraphicsRectProperty::m_brushColor, "Fill color (list - rgba)")
        .def_readwrite("line_size", &GraphicsRectProperty::m_lineSize, "Outline size")
        .def_readwrite("category", &GraphicsRectProperty::m_category, "Graphics category")
    ;

    class_<CProxyGraphicsRect, bases<CProxyGraphicsItem>, std::shared_ptr<CProxyGraphicsRect>>("CGraphicsRectangle", _graphicsRectangleDocString)
        .def(init<>("Default constructor"))
        .def(init<float, float, float, float>(_ctor1GraphicsRectangle))
        .def(init<float, float, float, float, const GraphicsRectProperty&>(_ctor2GraphicsRectangle))
        .add_property("x", &CProxyGraphicsRect::getX, &CProxyGraphicsRect::setX, "x coordinate of top-left point")
        .add_property("y", &CProxyGraphicsRect::getY, &CProxyGraphicsRect::setY, "y coordinate of top-left point")
        .add_property("width", &CProxyGraphicsRect::getWidth, &CProxyGraphicsRect::setWidth, "Rectangle width")
        .add_property("height", &CProxyGraphicsRect::getHeight, &CProxyGraphicsRect::setHeight, "Rectangle height")
    ;

    //Text
    class_<GraphicsTextProperty>("GraphicsTextProperty", "Visual properties for :py:class:`~PyCore.CGraphicsText` item.")
        .def_readwrite("color", &GraphicsTextProperty::m_color, "Text color (list - rgba)")
        .def_readwrite("font_name", &GraphicsTextProperty::m_fontName, "Font family name")
        .def_readwrite("font_size", &GraphicsTextProperty::m_fontSize, "Font size")
        .def_readwrite("bold", &GraphicsTextProperty::m_bBold, "Bold (boolean)")
        .def_readwrite("italic", &GraphicsTextProperty::m_bItalic, "Italic (boolean)")
        .def_readwrite("underline", &GraphicsTextProperty::m_bUnderline, "Underline (boolean)")
        .def_readwrite("strikeout", &GraphicsTextProperty::m_bStrikeOut, "Strikeout (boolean)")
    ;

    class_<CProxyGraphicsText, bases<CProxyGraphicsItem>, std::shared_ptr<CProxyGraphicsText>>("CGraphicsText", _graphicsTextDocString)
        .def(init<>("Default constructor"))
        .def(init<const std::string&>(_ctor1GraphicsText))
        .def(init<const std::string&, float, float>(_ctor2GraphicsText))
        .def(init<const std::string&, float, float, const GraphicsTextProperty&>(_ctor3GraphicsText))
        .add_property("x", &CProxyGraphicsText::getX, &CProxyGraphicsText::setX, "x coordinate of top-left point")
        .add_property("y", &CProxyGraphicsText::getY, &CProxyGraphicsText::setY, "y coordinate of top-left point")
        .add_property("text", &CProxyGraphicsText::getText, &CProxyGraphicsText::setText, "Text string")
    ;

    //-------------------------------//
    //----- CGraphicsConversion -----//
    //-------------------------------//
    CMat (CGraphicsConversion::*proxyGraphicsToBinaryMask)(const std::vector<std::shared_ptr<CProxyGraphicsItem>>&) = &CGraphicsConversion::graphicsToBinaryMask;

    class_<CGraphicsConversion>("CGraphicsConversion", _graphicsConvDocString)
        .def(init<>("Default constructor"))
        .def(init<int, int>(_ctorGraphicsConv))
        .def("graphicsToBinaryMask", proxyGraphicsToBinaryMask, _graphicsToBinaryMaskDocString, args("self", "graphics"))
    ;

    //------------------------------//
    //----- CProtocolTaskParam -----//
    //------------------------------//
    class_<std::unordered_map<std::string, std::string>>("ParamMap", "Data structure (same as Python dict) to store task parameters names and values")
        .def(map_indexing_suite<std::unordered_map<std::string, std::string>>())
    ;

    class_<CProtocolTaskParamWrap, std::shared_ptr<CProtocolTaskParamWrap>>("CProtocolTaskParam", _protocolTaskParamDocString)
        .enable_pickling()
        .def(init<>("Default constructor")[incref_return_value_policy<>()])
        .def("__copy__", &generic_copy<CProtocolTaskParamWrap>)
        .def("__deepcopy__", &generic_deepcopy<CProtocolTaskParamWrap>)
        .def("setParamMap", &CProtocolTaskParam::setParamMap, &CProtocolTaskParamWrap::default_setParamMap, _setParamMapDocString, args("self", "params"))
        .def("getParamMap", &CProtocolTaskParam::getParamMap, &CProtocolTaskParamWrap::default_getParamMap, _getParamMapDocString, args("self"))
        .def("getHashValue", &CProtocolTaskParam::getHashValue, &CProtocolTaskParamWrap::default_getHashValue, _getHashValueDocString, args("self"))
    ;

    //---------------------------//
    //----- CProtocolTaskIO -----//
    //---------------------------//
    enum_<IODataType>("IODataType", "Enum - List of available input/output data types")
        .value("NONE", IODataType::NONE)
        .value("IMAGE", IODataType::IMAGE)
        .value("IMAGE_BINARY", IODataType::IMAGE_BINARY)
        .value("IMAGE_LABEL", IODataType::IMAGE_LABEL)
        .value("VOLUME", IODataType::VOLUME)
        .value("VOLUME_BINARY", IODataType::VOLUME_BINARY)
        .value("VOLUME_LABEL", IODataType::VOLUME_LABEL)
        .value("INPUT_GRAPHICS", IODataType::INPUT_GRAPHICS)
        .value("OUTPUT_GRAPHICS", IODataType::OUTPUT_GRAPHICS)
        .value("NUMERIC_VALUES", IODataType::NUMERIC_VALUES)
        .value("DESCRIPTORS", IODataType::DESCRIPTORS)
        .value("WIDGET", IODataType::WIDGET)
        .value("FOLDER_PATH", IODataType::FOLDER_PATH)
        .value("FILE_PATH", IODataType::FILE_PATH)
        .value("DNN_DATASET", IODataType::DNN_DATASET)
    ;

    class_<CProtocolTaskIOWrap, std::shared_ptr<CProtocolTaskIOWrap>>("CProtocolTaskIO", _protocolTaskIODocString, init<>("Default constructor"))
        .def(init<IODataType>(_ctorProtocolTaskIODocString))
        .def(init<const CProtocolTaskIO&>("Copy constructor"))
        .add_property("dataType", &CProtocolTaskIO::getDataType, &CProtocolTaskIO::setDataType, "I/O data type")
        .add_property("dimCount", &CProtocolTaskIO::getDimensionCount, &CProtocolTaskIO::setDimensionCount, "Number of dimensions")
        .def("getUnitElementCount", &CProtocolTaskIO::getUnitElementCount, &CProtocolTaskIOWrap::default_getUnitElementCount, _getUnitElementCountDocString, args("self"))
        .def("isDataAvailable", &CProtocolTaskIO::isDataAvailable, &CProtocolTaskIOWrap::default_isDataAvailable, _isDataAvailableDocString, args("self"))
        .def("isAutoInput", &CProtocolTaskIO::isAutoInput, &CProtocolTaskIOWrap::default_isAutoInput, _isAutoInputDocString, args("self"))
        .def("clearData", &CProtocolTaskIO::clearData, &CProtocolTaskIOWrap::default_clearData, _clearDataDocString, args("self"))
        .def("copyStaticData", &CProtocolTaskIO::copyStaticData, &CProtocolTaskIOWrap::default_copyStaticData, _copyStaticDataDocString, args("self", "io"))
    ;

    //-------------------------//
    //----- CProtocolTask -----//
    //-------------------------//
    enum_<CProtocolTask::Type>("TaskType", "Enum - List of available process or task types")
        .value("GENERIC", CProtocolTask::Type::GENERIC)
        .value("IMAGE_PROCESS_2D", CProtocolTask::Type::IMAGE_PROCESS_2D)
        .value("IMAGE_PROCESS_3D", CProtocolTask::Type::IMAGE_PROCESS_3D)
        .value("VIDEO", CProtocolTask::Type::VIDEO)
    ;

    enum_<CProtocolTask::ActionFlag>("ActionFlag", "Enum - List of specific behaviors or actions that can be enable/disable for a task")
        .value("APPLY_VOLUME", CProtocolTask::ActionFlag::APPLY_VOLUME)
        .value("OUTPUT_AUTO_EXPORT", CProtocolTask::ActionFlag::OUTPUT_AUTO_EXPORT)
    ;

    //Overload member functions
    size_t (CProtocolTask::*getProgressSteps1)() = &CProtocolTask::getProgressSteps;
    size_t (CProtocolTask::*getProgressSteps2)(size_t) = &CProtocolTask::getProgressSteps;
    size_t (CProtocolTaskWrap::*default_getProgressSteps1)() = &CProtocolTaskWrap::default_getProgressSteps;
    size_t (CProtocolTaskWrap::*default_getProgressSteps2)(size_t) = &CProtocolTaskWrap::default_getProgressSteps;
    InputOutputVect (CProtocolTask::*getInputs1)() const = &CProtocolTask::getInputs;
    InputOutputVect (CProtocolTaskWrap::*getInputs2)(const std::vector<IODataType>&) const = &CProtocolTaskWrap::getInputs;
    InputOutputVect (CProtocolTask::*getOutputs1)() const = &CProtocolTask::getOutputs;
    InputOutputVect (CProtocolTaskWrap::*getOutputs2)(const std::vector<IODataType>&) const = &CProtocolTaskWrap::getOutputs;
    void (CProtocolTask::*addInputRef)(const ProtocolTaskIOPtr&) = &CProtocolTask::addInput;
    void (CProtocolTask::*addOutputRef)(const ProtocolTaskIOPtr&) = &CProtocolTask::addOutput;

    class_<CProtocolTaskWrap, std::shared_ptr<CProtocolTaskWrap>>("CProtocolTask", _protocolTaskDocString)
        .def(init<>("Default constructor")[incref_return_value_policy<>()])
        .def(init<const std::string&>(_ctorProtocolTaskDocString)[incref_return_value_policy<>()])
        .def(init<const CProtocolTask&>("Copy constructor")[incref_return_value_policy<>()])
        .add_property("type", &CProtocolTask::getType, "Main purpose or data type on which the task is dedicated to.")
        .add_property("name", &CProtocolTask::getName, &CProtocolTask::setName, "Task name (must be unique)")
        .def("setInputDataType", &CProtocolTask::setInputDataType, &CProtocolTaskWrap::default_setInputDataType, _setInputDataTypeDocString, args("self", "data_type", "index"))
        .def("setInput", &CProtocolTask::setInput, &CProtocolTaskWrap::default_setInput, _setInputDocString, args("self", "input", "index", "is_new_sequence"))
        .def("setInputs", &CProtocolTask::setInputs, &CProtocolTaskWrap::default_setInputs, _setInputsDocString, args("self", "inputs", "is_new_sequence"))
        .def("setOutputDataType", &CProtocolTask::setOutputDataType, &CProtocolTaskWrap::default_setOutputDataType, _setOutputDataTypeDocString, args("self", "data_type", "index"))
        .def("setOutput", &CProtocolTask::setOutput, &CProtocolTaskWrap::default_setOutput, _setOutputDocString, args("self", "output", "index"))
        .def("setOutputs", &CProtocolTask::setOutputs, &CProtocolTaskWrap::default_setOutputs, _setOutputsDocString, args("self", "outputs"))
        .def("setParam", &CProtocolTask::setParam, &CProtocolTaskWrap::default_setParam, _setParamDocString, args("self", "param"))
        .def("setActionFlag", &CProtocolTask::setActionFlag, _setActionFlagDocString, args("self", "action", "is_enable"))
        .def("setActive", &CProtocolTask::setActive, &CProtocolTaskWrap::default_setActive, _setActiveDocString, args("self", "is_active"))
        .def("getInputCount", &CProtocolTask::getInputCount, _getInputCountDocString, args("self"))
        .def("getInputs", getInputs1, _getInputsDocString, args("self"))
        .def("getInputs", getInputs2, _getInputsDocString, args("self", "types"))
        .def("getInput", &CProtocolTask::getInput, _getInputDocString, args("self", "index"))
        .def("getInputDataType", &CProtocolTask::getInputDataType, _getInputDataTypeDocString, args("self", "index"))
        .def("getOutputCount", &CProtocolTask::getOutputCount, _getOutputCountDocString, args("self"))
        .def("getOutputs", getOutputs1, _getOutputs1DocString, args("self"))
        .def("getOutputs", getOutputs2, _getOutputs2DocString, args("self", "types"))
        .def("getOutput", &CProtocolTask::getOutput, _getOutputDocString, args("self", "index"))
        .def("getParam", &CProtocolTask::getParam, _getParamDocString, args("self"))
        .def("getOutputDataType", &CProtocolTask::getOutputDataType, _getOutputDataTypeDocString, args("self", "index"))
        .def("getElapsedTime", &CProtocolTask::getElapsedTime, _getElapsedTimeDocString, args("self"))
        .def("getProgressSteps", getProgressSteps1, default_getProgressSteps1, _getProgressSteps1DocString, args("self"))
        .def("getProgressSteps", getProgressSteps2, default_getProgressSteps2, _getProgressSteps2DocString, args("self", "unit_elt_count"))
        .def("isGraphicsChangedListening", &CProtocolTask::isGraphicsChangedListening, &CProtocolTaskWrap::default_isGraphicsChangedListening, _isGraphicsChangedListeningDocString, args("self"))
        .def("addInput", addInputRef, &CProtocolTaskWrap::default_addInput, _addInputDocString, args("self", "input"))
        .def("addOutput", addOutputRef, &CProtocolTaskWrap::default_addOutput, _addOutputDocString, args("self", "output"))
        .def("removeInput", &CProtocolTask::removeInput, _removeInputDocString, args("self", "index"))
        .def("run", &CProtocolTask::run, &CProtocolTaskWrap::default_run, _runDocString, args("self"))
        .def("stop", &CProtocolTask::stop, &CProtocolTaskWrap::default_stop, _stopDocString, args("self"))
        .def("executeActions", &CProtocolTask::executeActions, &CProtocolTaskWrap::default_executeActions, _executeActionsDocString, args("self", "action"))
        .def("updateStaticOutputs", &CProtocolTask::updateStaticOutputs, &CProtocolTaskWrap::default_updateStaticOutputs, _updateStaticOutputsDocString, args("self"))
        .def("beginTaskRun", &CProtocolTask::beginTaskRun, &CProtocolTaskWrap::default_beginTaskRun, _beginTaskRunDocString, args("self"))
        .def("endTaskRun", &CProtocolTask::endTaskRun, &CProtocolTaskWrap::default_endTaskRun, _endTaskRunDocString, args("self"))
        .def("parametersModified", &CProtocolTask::parametersModified, &CProtocolTaskWrap::default_parametersModified, _parametersModifiedDocString, args("self"))
        .def("graphicsChanged", &CProtocolTask::graphicsChanged, &CProtocolTaskWrap::default_graphicsChanged, _graphicsChangedDocString, args("self"))
        .def("globalInputChanged", &CProtocolTask::globalInputChanged, &CProtocolTaskWrap::default_globalInputChanged, _globalInputChangedDocString, args("self", "is_new_sequence"))
        .def("emitStepProgress", &CProtocolTaskWrap::emitStepProgress, _emitStepProgressDocString, args("self"))
        .def("emitGraphicsContextChanged", &CProtocolTaskWrap::emitGraphicsContextChanged, _emitGraphicsContextChangedDocString, args("self"))
        .def("emitOutputChanged", &CProtocolTaskWrap::emitOutputChanged, _emitOutputChangedDocString, args("self"))
    ;

    //-------------------------------//
    //----- CProtocolTaskWidget -----//
    //-------------------------------//
    class_<CProtocolTaskWidgetWrap, std::shared_ptr<CProtocolTaskWidgetWrap>, boost::noncopyable>("CProtocolTaskWidget", _protocolTaskWidget)
        .def(init<>("Default constructor")[incref_return_value_policy<>()])
        .def(init<QWidget*>("Construct CProtocolTaskWidget with parent.")[incref_return_value_policy<>()])
        .def("setLayout", &CProtocolTaskWidgetWrap::setLayout, _setLayoutDocString, args("self", "layout"))
        .def("setApplyBtnHidden", &CProtocolTaskWidget::setApplyBtnHidden, _setApplyBtnHiddenDocString, args("self", "is_hidden"))
        .def("init", pure_virtual(&CProtocolTaskWidgetWrap::init), _initDocString, args("self"))
        .def("onApply", &CProtocolTaskWidget::onApply, &CProtocolTaskWidgetWrap::default_onApply, _applyDocString, args("self"))
        .def("emitApply", &CProtocolTaskWidgetWrap::emitApply, _emitApplyDocString, args("self"))
        .def("emitSendProcessAction", &CProtocolTaskWidgetWrap::emitSendProcessAction, _emitSendProcessActionDocString, args("self", "action"))
        .def("emitSetGraphicsTool", &CProtocolTaskWidgetWrap::emitSetGraphicsTool, _emitSetGraphicsToolDocString, args("self", "tool"))
        .def("emitSetGraphicsCategory", &CProtocolTaskWidgetWrap::emitSetGraphicsCategory, _emitSetGraphicsCategoryDocString, args("self", "category"))
    ;
}
