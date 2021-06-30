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

#include "PyCore.h"
#include "PyCoreDocString.hpp"
#include <QString>
#include <QHash>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include "CWorkflowTaskParamWrap.h"
#include "CWorkflowTaskIOWrap.h"
#include "CWorkflowTaskWrap.h"
#include "CWorkflowTaskWidgetWrap.h"
#include "CTaskIOFactoryWrap.h"
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

    // Numpy initialization
    init_numpy();

    // CMat <-> Numpy NdArray converters
    to_python_converter<CMat, BoostCvMatToNumpyArrayConverter>();
    BoostNumpyArrayToCvMatConverter();

    // Register smart pointers
    register_ptr_to_python<std::shared_ptr<CProxyGraphicsItem>>();
    register_ptr_to_python<std::shared_ptr<CWorkflowTaskParam>>();
    register_ptr_to_python<std::shared_ptr<CWorkflowTaskIO>>();
    register_ptr_to_python<std::shared_ptr<CWorkflowTask>>();
    register_ptr_to_python<std::shared_ptr<CWorkflowTaskWidget>>();

    // Register std::vector<T>
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
        .def(init<const std::vector<CPoint<float>>, const std::vector<PolygonF>, const CGraphicsPolygonProperty&>(_ctor2GraphicsComplexPoly))
        .add_property("outer", &CProxyGraphicsComplexPoly::getOuter, &CProxyGraphicsComplexPoly::setOuter, "Outer polygon (list of vertices)")
        .add_property("inners", &CProxyGraphicsComplexPoly::getInners, &CProxyGraphicsComplexPoly::setInners, "Inner polygons (list of inner polygons corresponding to holes)")
        .add_property("property", &CProxyGraphicsComplexPoly::getProperty, &CProxyGraphicsComplexPoly::setProperty, "Visual polygon properties. See :py:class:`~ikomia.core.PyCore.GraphicsPolygonProperty`.")
    ;

    //Ellipse
    class_<CGraphicsEllipseProperty>("GraphicsEllipseProperty", "Visual properties for :py:class:`~ikomia.core.PyCore.CGraphicsEllipse` item.")
        .add_property("pen_color", &CGraphicsEllipseProperty::getPenColor, &CGraphicsEllipseProperty::setPenColor, "Outline color (list - rgba)")
        .add_property("brush_color", &CGraphicsEllipseProperty::getBrushColor, &CGraphicsEllipseProperty::setBrushColor, "Fill color (list - rgba)")
        .def_readwrite("line_size", &CGraphicsEllipseProperty::m_lineSize, "Outline size")
        .def_readwrite("category", &CGraphicsEllipseProperty::m_category, "Graphics category")
    ;

    class_<CProxyGraphicsEllipse, bases<CProxyGraphicsItem>, std::shared_ptr<CProxyGraphicsEllipse>>("CGraphicsEllipse", _graphicsEllipseDocString)
        .def(init<>("Default constructor"))
        .def(init<float, float, float, float>(_ctor1GraphicsEllipse))
        .def(init<float, float, float, float, const CGraphicsEllipseProperty&>(_ctor2GraphicsEllipse))
        .add_property("x", &CProxyGraphicsEllipse::getX, &CProxyGraphicsEllipse::setX, "x coordinate of top-left point")
        .add_property("y", &CProxyGraphicsEllipse::getY, &CProxyGraphicsEllipse::setY, "y coordinate of top-left point")
        .add_property("width", &CProxyGraphicsEllipse::getWidth, &CProxyGraphicsEllipse::setWidth, "Ellipse width")
        .add_property("height", &CProxyGraphicsEllipse::getHeight, &CProxyGraphicsEllipse::setHeight, "Ellipse height")
        .add_property("property", &CProxyGraphicsEllipse::getProperty, &CProxyGraphicsEllipse::setProperty, "Visual ellipse properties. See :py:class:`~ikomia.core.PyCore.GraphicsEllipseProperty`.")
    ;

    //Point
    class_<CGraphicsPointProperty>("GraphicsPointProperty", "Visual properties for :py:class:`~ikomia.core.PyCore.CGraphicsPoint` item.")
        .add_property("pen_color", &CGraphicsPointProperty::getPenColor, &CGraphicsPointProperty::setPenColor, "Outline color (list - rgba)")
        .add_property("brush_color", &CGraphicsPointProperty::getBrushColor, &CGraphicsPointProperty::setBrushColor, "Fill color (list - rgba)")
        .def_readwrite("size", &CGraphicsPointProperty::m_size, "Size")
        .def_readwrite("category", &CGraphicsPointProperty::m_category, "Graphics category")
    ;

    class_<CProxyGraphicsPoint, bases<CProxyGraphicsItem>, std::shared_ptr<CProxyGraphicsPoint>>("CGraphicsPoint", _graphicsPointDocString)
        .def(init<>("Default constructor"))
        .def(init<const CPoint<float>&>(_ctor1GraphicsPoint))
        .def(init<const CPoint<float>&, const CGraphicsPointProperty&>(_ctor2GraphicsPoint))
        .add_property("point", &CProxyGraphicsPoint::getPoint, &CProxyGraphicsPoint::setPoint, "2D point coordinates (:py:class:`CPointF`)")
        .add_property("property", &CProxyGraphicsPoint::getProperty, &CProxyGraphicsPoint::setProperty, "Visual point properties. See :py:class:`~ikomia.core.PyCore.GraphicsPointProperty`.")
    ;

    //Polygon
    class_<CGraphicsPolygonProperty>("GraphicsPolygonProperty", "Visual properties for :py:class:`~ikomia.core.PyCore.CGraphicsPolygon` item.")
        .add_property("pen_color", &CGraphicsPolygonProperty::getPenColor, &CGraphicsPolygonProperty::setPenColor, "Outline color (list - rgba)")
        .add_property("brush_color", &CGraphicsPolygonProperty::getBrushColor, &CGraphicsPolygonProperty::setBrushColor, "Fill color (list - rgba)")
        .def_readwrite("line_size", &CGraphicsPolygonProperty::m_lineSize, "Outline size")
        .def_readwrite("category", &CGraphicsPolygonProperty::m_category, "Graphics category")
    ;

    class_<CProxyGraphicsPolygon, bases<CProxyGraphicsItem>, std::shared_ptr<CProxyGraphicsPolygon>>("CGraphicsPolygon", _graphicsPolygonDocString)
        .def(init<>("Default constructor"))
        .def(init<const std::vector<CPoint<float>>>(_ctor1GraphicsPolygon))
        .def(init<const std::vector<CPoint<float>>, const CGraphicsPolygonProperty&>(_ctor2GraphicsPolygon))
        .add_property("points", &CProxyGraphicsPolygon::getPoints, &CProxyGraphicsPolygon::setPoints, "List of polygon vertices (:py:class:`CPointF`)")
        .add_property("property", &CProxyGraphicsPolygon::getProperty, &CProxyGraphicsPolygon::setProperty, "Visual point properties. See :py:class:`~ikomia.core.PyCore.GraphicsPolygonProperty`.")
    ;

    //Polyline
    class_<CGraphicsPolylineProperty>("GraphicsPolylineProperty", "Visual properties for :py:class:`~ikomia.core.PyCore.CGraphicsPolyline` item.")
        .add_property("pen_color", &CGraphicsPolylineProperty::getColor, &CGraphicsPolylineProperty::setColor, "Outline color (list - rgba)")
        .def_readwrite("line_size", &CGraphicsPolylineProperty::m_lineSize, "Outline size")
        .def_readwrite("category", &CGraphicsPolylineProperty::m_category, "Graphics category")
    ;

    class_<CProxyGraphicsPolyline, bases<CProxyGraphicsItem>, std::shared_ptr<CProxyGraphicsPolyline>>("CGraphicsPolyline", _graphicsPolylineDocString)
        .def(init<>("Default constructor"))
        .def(init<const std::vector<CPoint<float>>>(_ctor1GraphicsPolyline))
        .def(init<const std::vector<CPoint<float>>, const CGraphicsPolylineProperty&>(_ctor2GraphicsPolyline))
        .add_property("points", &CProxyGraphicsPolyline::getPoints, &CProxyGraphicsPolyline::setPoints, "List of polyline vertices (:py:class:`CPointF`)")
        .add_property("property", &CProxyGraphicsPolyline::getProperty, &CProxyGraphicsPolyline::setProperty, "Visual point properties. See :py:class:`~ikomia.core.PyCore.GraphicsPolylineProperty`.")
    ;

    //Rectangle
    class_<CGraphicsRectProperty>("GraphicsRectProperty", "Visual properties for :py:class:`~ikomia.core.PyCore.CGraphicsRectangle` item.")
        .add_property("pen_color", &CGraphicsRectProperty::getPenColor, &CGraphicsRectProperty::setPenColor, "Outline color (list - rgba)")
        .add_property("brush_color", &CGraphicsRectProperty::getBrushColor, &CGraphicsRectProperty::setBrushColor, "Fill color (list - rgba)")
        .def_readwrite("line_size", &CGraphicsRectProperty::m_lineSize, "Outline size")
        .def_readwrite("category", &CGraphicsRectProperty::m_category, "Graphics category")
    ;

    class_<CProxyGraphicsRect, bases<CProxyGraphicsItem>, std::shared_ptr<CProxyGraphicsRect>>("CGraphicsRectangle", _graphicsRectangleDocString)
        .def(init<>("Default constructor"))
        .def(init<float, float, float, float>(_ctor1GraphicsRectangle))
        .def(init<float, float, float, float, const CGraphicsRectProperty&>(_ctor2GraphicsRectangle))
        .add_property("x", &CProxyGraphicsRect::getX, &CProxyGraphicsRect::setX, "x coordinate of top-left point")
        .add_property("y", &CProxyGraphicsRect::getY, &CProxyGraphicsRect::setY, "y coordinate of top-left point")
        .add_property("width", &CProxyGraphicsRect::getWidth, &CProxyGraphicsRect::setWidth, "Rectangle width")
        .add_property("height", &CProxyGraphicsRect::getHeight, &CProxyGraphicsRect::setHeight, "Rectangle height")
        .add_property("property", &CProxyGraphicsRect::getProperty, &CProxyGraphicsRect::setProperty, "Visual point properties. See :py:class:`~ikomia.core.PyCore.GraphicsRectProperty`.")
    ;

    //Text
    class_<CGraphicsTextProperty>("GraphicsTextProperty", "Visual properties for :py:class:`~ikomia.core.PyCore.CGraphicsText` item.")
        .add_property("color", &CGraphicsTextProperty::getColor, &CGraphicsTextProperty::setColor, "Text color (list - rgba)")
        .def_readwrite("font_name", &CGraphicsTextProperty::m_fontName, "Font family name")
        .def_readwrite("font_size", &CGraphicsTextProperty::m_fontSize, "Font size")
        .def_readwrite("bold", &CGraphicsTextProperty::m_bBold, "Bold (boolean)")
        .def_readwrite("italic", &CGraphicsTextProperty::m_bItalic, "Italic (boolean)")
        .def_readwrite("underline", &CGraphicsTextProperty::m_bUnderline, "Underline (boolean)")
        .def_readwrite("strikeout", &CGraphicsTextProperty::m_bStrikeOut, "Strikeout (boolean)")
    ;

    class_<CProxyGraphicsText, bases<CProxyGraphicsItem>, std::shared_ptr<CProxyGraphicsText>>("CGraphicsText", _graphicsTextDocString)
        .def(init<>("Default constructor"))
        .def(init<const std::string&>(_ctor1GraphicsText))
        .def(init<const std::string&, float, float>(_ctor2GraphicsText))
        .def(init<const std::string&, float, float, const CGraphicsTextProperty&>(_ctor3GraphicsText))
        .add_property("x", &CProxyGraphicsText::getX, &CProxyGraphicsText::setX, "x coordinate of top-left point")
        .add_property("y", &CProxyGraphicsText::getY, &CProxyGraphicsText::setY, "y coordinate of top-left point")
        .add_property("text", &CProxyGraphicsText::getText, &CProxyGraphicsText::setText, "Text string")
        .add_property("property", &CProxyGraphicsText::getProperty, &CProxyGraphicsText::setProperty, "Visual point properties. See :py:class:`~ikomia.core.PyCore.GraphicsTextProperty`.")
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
    //----- CWorkflowTaskParam -----//
    //------------------------------//
    class_<std::unordered_map<std::string, std::string>>("ParamMap", "Data structure (same as Python dict) to store task parameters names and values")
        .def(map_indexing_suite<std::unordered_map<std::string, std::string>>())
    ;

    class_<CWorkflowTaskParamWrap, std::shared_ptr<CWorkflowTaskParamWrap>>("CWorkflowTaskParam", _WorkflowTaskParamDocString)
        .enable_pickling()
        .def(init<>("Default constructor"))
        .def("__copy__", &generic_copy<CWorkflowTaskParamWrap>)
        .def("__deepcopy__", &generic_deepcopy<CWorkflowTaskParamWrap>)
        .def("setParamMap", &CWorkflowTaskParam::setParamMap, &CWorkflowTaskParamWrap::default_setParamMap, _setParamMapDocString, args("self", "params"))
        .def("getParamMap", &CWorkflowTaskParam::getParamMap, &CWorkflowTaskParamWrap::default_getParamMap, _getParamMapDocString, args("self"))
        .def("getHashValue", &CWorkflowTaskParam::getHashValue, &CWorkflowTaskParamWrap::default_getHashValue, _getHashValueDocString, args("self"))
    ;

    //---------------------------//
    //----- CWorkflowTaskIO -----//
    //---------------------------//
    enum_<IODataType>("IODataType", "Enum - List of available input/output data types")
        .value("NONE", IODataType::NONE)
        .value("IMAGE", IODataType::IMAGE)
        .value("IMAGE_BINARY", IODataType::IMAGE_BINARY)
        .value("IMAGE_LABEL", IODataType::IMAGE_LABEL)
        .value("VOLUME", IODataType::VOLUME)
        .value("VOLUME_BINARY", IODataType::VOLUME_BINARY)
        .value("VOLUME_LABEL", IODataType::VOLUME_LABEL)
        .value("VIDEO", IODataType::VIDEO)
        .value("VIDEO_BINARY", IODataType::VIDEO_BINARY)
        .value("VIDEO_LABEL", IODataType::VIDEO_LABEL)
        .value("LIVE_STREAM", IODataType::LIVE_STREAM)
        .value("LIVE_STREAM_BINARY", IODataType::LIVE_STREAM_BINARY)
        .value("LIVE_STREAM_LABEL", IODataType::LIVE_STREAM_LABEL)
        .value("INPUT_GRAPHICS", IODataType::INPUT_GRAPHICS)
        .value("OUTPUT_GRAPHICS", IODataType::OUTPUT_GRAPHICS)
        .value("NUMERIC_VALUES", IODataType::NUMERIC_VALUES)
        .value("DESCRIPTORS", IODataType::DESCRIPTORS)
        .value("WIDGET", IODataType::WIDGET)
        .value("FOLDER_PATH", IODataType::FOLDER_PATH)
        .value("FILE_PATH", IODataType::FILE_PATH)
        .value("DNN_DATASET", IODataType::DNN_DATASET)
        .value("ARRAY", IODataType::ARRAY)
    ;

    class_<CWorkflowTaskIOWrap, std::shared_ptr<CWorkflowTaskIOWrap>>("CWorkflowTaskIO", _WorkflowTaskIODocString)
        .def(init<>("Default constructor"))
        .def(init<IODataType>(_ctorWorkflowTaskIODocString))
        .def(init<const CWorkflowTaskIO&>("Copy constructor"))
        .add_property("dataType", &CWorkflowTaskIO::getDataType, &CWorkflowTaskIO::setDataType, "I/O data type")
        .add_property("dimCount", &CWorkflowTaskIO::getDimensionCount, &CWorkflowTaskIO::setDimensionCount, "Number of dimensions")
        .add_property("description", &CWorkflowTaskIO::getDescription, &CWorkflowTaskIO::setDescription, "Custom description to explain input/output type and use")
        .def("getUnitElementCount", &CWorkflowTaskIO::getUnitElementCount, &CWorkflowTaskIOWrap::default_getUnitElementCount, _getUnitElementCountDocString, args("self"))
        .def("isDataAvailable", &CWorkflowTaskIO::isDataAvailable, &CWorkflowTaskIOWrap::default_isDataAvailable, _isDataAvailableDocString, args("self"))
        .def("isAutoInput", &CWorkflowTaskIO::isAutoInput, &CWorkflowTaskIOWrap::default_isAutoInput, _isAutoInputDocString, args("self"))
        .def("setDisplayable", &CWorkflowTaskIO::setDisplayable, _setDisplayableDocString, args("self", "displayable"))
        .def("clearData", &CWorkflowTaskIO::clearData, &CWorkflowTaskIOWrap::default_clearData, _clearDataDocString, args("self"))
        .def("copyStaticData", &CWorkflowTaskIO::copyStaticData, &CWorkflowTaskIOWrap::default_copyStaticData, _copyStaticDataDocString, args("self", "io"))
    ;

    //----------------------------------//
    //----- CWorkflowTaskIOFactory -----//
    //----------------------------------//
    class_<CTaskIOFactoryWrap, std::shared_ptr<CTaskIOFactoryWrap>, boost::noncopyable>("CWorkflowTaskIOFactory", _ioFactoryDocString)
        .def("create", pure_virtual(&CTaskIOFactoryWrap::create), _ioFactoryCreateDocString, args("self", "datatype"))
    ;

    //-------------------------//
    //----- CWorkflowTask -----//
    //-------------------------//
    enum_<CWorkflowTask::Type>("TaskType", "Enum - List of available process or task types")
        .value("GENERIC", CWorkflowTask::Type::GENERIC)
        .value("IMAGE_PROCESS_2D", CWorkflowTask::Type::IMAGE_PROCESS_2D)
        .value("IMAGE_PROCESS_3D", CWorkflowTask::Type::IMAGE_PROCESS_3D)
        .value("VIDEO", CWorkflowTask::Type::VIDEO)
    ;

    enum_<CWorkflowTask::ActionFlag>("ActionFlag", "Enum - List of specific behaviors or actions that can be enable/disable for a task")
        .value("APPLY_VOLUME", CWorkflowTask::ActionFlag::APPLY_VOLUME)
        .value("OUTPUT_AUTO_EXPORT", CWorkflowTask::ActionFlag::OUTPUT_AUTO_EXPORT)
    ;

    //Overload member functions
    size_t (CWorkflowTask::*getProgressSteps1)() = &CWorkflowTask::getProgressSteps;
    size_t (CWorkflowTask::*getProgressSteps2)(size_t) = &CWorkflowTask::getProgressSteps;
    size_t (CWorkflowTaskWrap::*default_getProgressSteps1)() = &CWorkflowTaskWrap::default_getProgressSteps;
    size_t (CWorkflowTaskWrap::*default_getProgressSteps2)(size_t) = &CWorkflowTaskWrap::default_getProgressSteps;
    InputOutputVect (CWorkflowTask::*getInputs1)() const = &CWorkflowTask::getInputs;
    InputOutputVect (CWorkflowTaskWrap::*getInputs2)(const std::vector<IODataType>&) const = &CWorkflowTaskWrap::getInputs;
    InputOutputVect (CWorkflowTask::*getOutputs1)() const = &CWorkflowTask::getOutputs;
    InputOutputVect (CWorkflowTaskWrap::*getOutputs2)(const std::vector<IODataType>&) const = &CWorkflowTaskWrap::getOutputs;
    void (CWorkflowTask::*addInputRef)(const WorkflowTaskIOPtr&) = &CWorkflowTask::addInput;
    void (CWorkflowTask::*addOutputRef)(const WorkflowTaskIOPtr&) = &CWorkflowTask::addOutput;

    class_<CWorkflowTaskWrap, std::shared_ptr<CWorkflowTaskWrap>>("CWorkflowTask", _WorkflowTaskDocString)
        .def(init<>("Default constructor"))
        .def(init<const std::string&>(_ctorWorkflowTaskDocString))
        .def(init<const CWorkflowTask&>("Copy constructor"))
        .add_property("type", &CWorkflowTask::getType, "Main purpose or data type on which the task is dedicated to.")
        .add_property("name", &CWorkflowTask::getName, &CWorkflowTask::setName, "Task name (must be unique)")
        .def("setInputDataType", &CWorkflowTask::setInputDataType, &CWorkflowTaskWrap::default_setInputDataType, _setInputDataTypeDocString, args("self", "data_type", "index"))
        .def("setInput", &CWorkflowTask::setInput, &CWorkflowTaskWrap::default_setInput, _setInputDocString, args("self", "input", "index", "is_new_sequence"))
        .def("setInputs", &CWorkflowTask::setInputs, &CWorkflowTaskWrap::default_setInputs, _setInputsDocString, args("self", "inputs", "is_new_sequence"))
        .def("setOutputDataType", &CWorkflowTask::setOutputDataType, &CWorkflowTaskWrap::default_setOutputDataType, _setOutputDataTypeDocString, args("self", "data_type", "index"))
        .def("setOutput", &CWorkflowTask::setOutput, &CWorkflowTaskWrap::default_setOutput, _setOutputDocString, args("self", "output", "index"))
        .def("setOutputs", &CWorkflowTask::setOutputs, &CWorkflowTaskWrap::default_setOutputs, _setOutputsDocString, args("self", "outputs"))
        .def("setParam", &CWorkflowTask::setParam, &CWorkflowTaskWrap::default_setParam, _setParamDocString, args("self", "param"))
        .def("setActionFlag", &CWorkflowTask::setActionFlag, _setActionFlagDocString, args("self", "action", "is_enable"))
        .def("setActive", &CWorkflowTask::setActive, &CWorkflowTaskWrap::default_setActive, _setActiveDocString, args("self", "is_active"))
        .def("getInputCount", &CWorkflowTask::getInputCount, _getInputCountDocString, args("self"))
        .def("getInputs", getInputs1, _getInputsDocString, args("self"))
        .def("getInputs", getInputs2, _getInputsDocString, args("self", "types"))
        .def("getInput", &CWorkflowTask::getInput, _getInputDocString, args("self", "index"))
        .def("getInputDataType", &CWorkflowTask::getInputDataType, _getInputDataTypeDocString, args("self", "index"))
        .def("getOutputCount", &CWorkflowTask::getOutputCount, _getOutputCountDocString, args("self"))
        .def("getOutputs", getOutputs1, _getOutputs1DocString, args("self"))
        .def("getOutputs", getOutputs2, _getOutputs2DocString, args("self", "types"))
        .def("getOutput", &CWorkflowTask::getOutput, _getOutputDocString, args("self", "index"))
        .def("getParam", &CWorkflowTask::getParam, _getParamDocString, args("self"))
        .def("getOutputDataType", &CWorkflowTask::getOutputDataType, _getOutputDataTypeDocString, args("self", "index"))
        .def("getElapsedTime", &CWorkflowTask::getElapsedTime, _getElapsedTimeDocString, args("self"))
        .def("getProgressSteps", getProgressSteps1, default_getProgressSteps1, _getProgressSteps1DocString, args("self"))
        .def("getProgressSteps", getProgressSteps2, default_getProgressSteps2, _getProgressSteps2DocString, args("self", "unit_elt_count"))
        .def("isGraphicsChangedListening", &CWorkflowTask::isGraphicsChangedListening, &CWorkflowTaskWrap::default_isGraphicsChangedListening, _isGraphicsChangedListeningDocString, args("self"))
        .def("addInput", addInputRef, &CWorkflowTaskWrap::default_addInput, _addInputDocString, args("self", "input"))
        .def("addOutput", addOutputRef, &CWorkflowTaskWrap::default_addOutput, _addOutputDocString, args("self", "output"))
        .def("removeInput", &CWorkflowTask::removeInput, _removeInputDocString, args("self", "index"))
        .def("run", &CWorkflowTask::run, &CWorkflowTaskWrap::default_run, _runDocString, args("self"))
        .def("stop", &CWorkflowTask::stop, &CWorkflowTaskWrap::default_stop, _stopDocString, args("self"))
        .def("executeActions", &CWorkflowTask::executeActions, &CWorkflowTaskWrap::default_executeActions, _executeActionsDocString, args("self", "action"))
        .def("updateStaticOutputs", &CWorkflowTask::updateStaticOutputs, &CWorkflowTaskWrap::default_updateStaticOutputs, _updateStaticOutputsDocString, args("self"))
        .def("beginTaskRun", &CWorkflowTask::beginTaskRun, &CWorkflowTaskWrap::default_beginTaskRun, _beginTaskRunDocString, args("self"))
        .def("endTaskRun", &CWorkflowTask::endTaskRun, &CWorkflowTaskWrap::default_endTaskRun, _endTaskRunDocString, args("self"))
        .def("parametersModified", &CWorkflowTask::parametersModified, &CWorkflowTaskWrap::default_parametersModified, _parametersModifiedDocString, args("self"))
        .def("graphicsChanged", &CWorkflowTask::graphicsChanged, &CWorkflowTaskWrap::default_graphicsChanged, _graphicsChangedDocString, args("self"))
        .def("globalInputChanged", &CWorkflowTask::globalInputChanged, &CWorkflowTaskWrap::default_globalInputChanged, _globalInputChangedDocString, args("self", "is_new_sequence"))
        .def("emitAddSubProgressSteps", &CWorkflowTaskWrap::emitAddSubProgressSteps, _emitAddSubProgressStepsDocString, args("self", "count"))
        .def("emitStepProgress", &CWorkflowTaskWrap::emitStepProgress, _emitStepProgressDocString, args("self"))
        .def("emitGraphicsContextChanged", &CWorkflowTaskWrap::emitGraphicsContextChanged, _emitGraphicsContextChangedDocString, args("self"))
        .def("emitOutputChanged", &CWorkflowTaskWrap::emitOutputChanged, _emitOutputChangedDocString, args("self"))
    ;

    //-------------------------------//
    //----- CWorkflowTaskWidget -----//
    //-------------------------------//
    class_<CWorkflowTaskWidgetWrap, std::shared_ptr<CWorkflowTaskWidgetWrap>, boost::noncopyable>("CWorkflowTaskWidget", _WorkflowTaskWidget)
        .def(init<>("Default constructor"))
        .def(init<QWidget*>("Construct CWorkflowTaskWidget with parent."))
        .def("setLayout", &CWorkflowTaskWidgetWrap::setLayout, _setLayoutDocString, args("self", "layout"))
        .def("setApplyBtnHidden", &CWorkflowTaskWidget::setApplyBtnHidden, _setApplyBtnHiddenDocString, args("self", "is_hidden"))
        .def("init", pure_virtual(&CWorkflowTaskWidgetWrap::init), _initDocString, args("self"))
        .def("onApply", &CWorkflowTaskWidget::onApply, &CWorkflowTaskWidgetWrap::default_onApply, _applyDocString, args("self"))
        .def("emitApply", &CWorkflowTaskWidgetWrap::emitApply, _emitApplyDocString, args("self"))
        .def("emitSendProcessAction", &CWorkflowTaskWidgetWrap::emitSendProcessAction, _emitSendProcessActionDocString, args("self", "action"))
        .def("emitSetGraphicsTool", &CWorkflowTaskWidgetWrap::emitSetGraphicsTool, _emitSetGraphicsToolDocString, args("self", "tool"))
        .def("emitSetGraphicsCategory", &CWorkflowTaskWidgetWrap::emitSetGraphicsCategory, _emitSetGraphicsCategoryDocString, args("self", "category"))
    ;
}
