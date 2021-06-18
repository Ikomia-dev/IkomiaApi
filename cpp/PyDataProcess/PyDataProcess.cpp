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

#include "PyDataProcess.h"
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include "Data/CvMatNumpyArrayConverter.h"
#include "PyDataProcessDocString.hpp"
#include "CTaskFactoryWrap.h"
#include "CWidgetFactoryWrap.h"
#include "CPluginProcessInterfaceWrap.h"
#include "C2dImageTaskWrap.h"
#include "C2dImageInteractiveTaskWrap.h"
#include "CVideoTaskWrap.h"
#include "CVideoOFTaskWrap.h"
#include "CVideoTrackingTaskWrap.h"
#include "CDnnTrainTaskWrap.h"
#include "CDnnTrainTaskParamWrap.h"
#include "CFeatureIOWrap.hpp"
#include "CGraphicsInputWrap.h"
#include "CImageIOWrap.h"
#include "CVideoIOWrap.h"
#include "CWidgetOutputWrap.h"
#include "CDatasetIOWrap.h"
#include "CPathIOWrap.h"
#include "CArrayIOWrap.h"
#include "CIkomiaRegistry.h"

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

template<typename Type>
void exposeFeatureIO(const std::string& className)
{
    //Overload member functions
    void (CFeatureIO<Type>::*addValueList1)(const std::vector<Type>&) = &CFeatureIO<Type>::addValueList;
    void (CFeatureIO<Type>::*addValueList2)(const std::vector<Type>&, const std::string&) = &CFeatureIO<Type>::addValueList;
    void (CFeatureIO<Type>::*addValueList3)(const std::vector<Type>&, const std::vector<std::string>&) = &CFeatureIO<Type>::addValueList;
    void (CFeatureIO<Type>::*addValueList4)(const std::vector<Type>&, const std::string&, const std::vector<std::string>&) = &CFeatureIO<Type>::addValueList;

    class_<CFeatureIOWrap<Type>, bases<CWorkflowTaskIO>, std::shared_ptr<CFeatureIOWrap<Type>>>(className.c_str(), _featureProcessIODocString)
        .def(init<>("Default constructor"))
        .def(init<const CFeatureIO<Type>&>("Copy constructor"))
        .def("setOutputType", &CFeatureIO<Type>::setOutputType, _setOutputTypeDocString, args("self", "type"))
        .def("setPlotType", &CFeatureIO<Type>::setPlotType, _setPlotTypeDocString, args("self", "type"))
        .def("addValueList", addValueList1, _addValueList1DocString, args("self", "values"))
        .def("addValueList", addValueList2, _addValueList2DocString, args("self", "values", "header_label"))
        .def("addValueList", addValueList3, _addValueList3DocString, args("self", "values", "labels"))
        .def("addValueList", addValueList4, _addValueList4DocString, args("self", "values", "header_label", "labels"))
        .def("getOutputType", &CFeatureIO<Type>::getOutputType, _getOutputTypeDocString, args("self"))
        .def("getPlotType", &CFeatureIO<Type>::getPlotType, _getPlotTypeDocString, args("self"))
        .def("getValueList", &CFeatureIO<Type>::getValueList, _getValueListDocString, args("self", "index"))
        .def("getAllValueList", &CFeatureIO<Type>::getAllValues, _getAllValueListDocString, args("self"))
        .def("getAllLabelList", &CFeatureIO<Type>::getAllValueLabels, _getAllLabelListDocString, args("self"))
        .def("getUnitElementCount", &CFeatureIO<Type>::getUnitElementCount, &CFeatureIOWrap<Type>::default_getUnitElementCount, _getUnitEltCountDerivedDocString, args("self"))
        .def("isDataAvailable", &CFeatureIO<Type>::isDataAvailable, &CFeatureIOWrap<Type>::default_isDataAvailable, _isDataAvailableDerivedDocString, args("self"))
        .def("clearData", &CFeatureIO<Type>::clearData, &CFeatureIOWrap<Type>::default_clearData, _clearDataDerivedDocString, args("self"))
        .def("copyStaticData", &CFeatureIO<Type>::copyStaticData, &CFeatureIOWrap<Type>::default_copyStaticData, _copyStaticDataDerivedDocString, args("self", "io"))
    ;
}


BOOST_PYTHON_MODULE(pydataprocess)
{
    // Enable user-defined docstrings and python signatures, while disabling the C++ signatures
    docstring_options local_docstring_options(true, true, false);

    // Set the docstring of the current module scope
    scope().attr("__doc__") = _moduleDocString;

    //Numpy initialization
    init_numpy();

    //Register smart pointers
    register_ptr_to_python<std::shared_ptr<CTaskFactory>>();
    register_ptr_to_python<std::shared_ptr<CWidgetFactory>>();
    register_ptr_to_python<std::shared_ptr<CGraphicsInput>>();
    register_ptr_to_python<std::shared_ptr<CImageIO>>();
    register_ptr_to_python<std::shared_ptr<CVideoIO>>();
    register_ptr_to_python<std::shared_ptr<CWidgetOutput>>();
    register_ptr_to_python<std::shared_ptr<CPathIO>>();
    register_ptr_to_python<std::shared_ptr<CDatasetIO>>();
    register_ptr_to_python<std::shared_ptr<CArrayIO>>();
    register_ptr_to_python<std::shared_ptr<C2dImageTask>>();
    register_ptr_to_python<std::shared_ptr<C2dImageInteractiveTask>>();
    register_ptr_to_python<std::shared_ptr<CVideoTask>>();
    register_ptr_to_python<std::shared_ptr<CVideoOFTask>>();
    register_ptr_to_python<std::shared_ptr<CVideoTrackingTask>>();
    register_ptr_to_python<std::shared_ptr<CDnnTrainTask>>();
    register_ptr_to_python<std::shared_ptr<CDnnTrainTaskParam>>();

    //Register std::vector<T> <-> python list converters
    registerStdVector<uchar>();
    registerStdVector<std::vector<uchar>>();

    //---------------------//
    //----- CTaskInfo -----//
    //---------------------//
    class_<CTaskInfo>("CTaskInfo", _processInfoDocString, init<>("Default constructor"))
        .add_property("name", &CTaskInfo::getName, &CTaskInfo::setName, "Name of the plugin (mandatory - must be unique)")
        .add_property("path", &CTaskInfo::getPath, &CTaskInfo::setPath, "Path in the library tree view of Ikomia")
        .add_property("shortDescription", &CTaskInfo::getShortDescription, &CTaskInfo::setShortDescription, "Short description of the plugin (mandatory)")
        .add_property("description", &CTaskInfo::getDescription, &CTaskInfo::setDescription, "Full description of the plugin (mandatory)")
        .add_property("documentationLink", &CTaskInfo::getDocumentationLink, &CTaskInfo::setDocumentationLink, "Address (URL) of online documentation")
        .add_property("iconPath", &CTaskInfo::getIconPath, &CTaskInfo::setIconPath, "Relative path to the plugin icon")
        .add_property("keywords", &CTaskInfo::getKeywords, &CTaskInfo::setKeywords, "Keywords associated with the plugin (Used for Ikomia search engine")
        .add_property("authors", &CTaskInfo::getAuthors, &CTaskInfo::setAuthors, "Authors of the plugin and/or corresponding paper (mandatory)")
        .add_property("article", &CTaskInfo::getArticle, &CTaskInfo::setArticle, "Title of the corresponding paper")
        .add_property("journal", &CTaskInfo::getJournal, &CTaskInfo::setJournal, "Paper journal")
        .add_property("version", &CTaskInfo::getVersion, &CTaskInfo::setVersion, "Plugin version (mandatory)")
        .add_property("ikomia_version", &CTaskInfo::getIkomiaVersion, "Ikomia API version")
        .add_property("year", &CTaskInfo::getYear, &CTaskInfo::setYear, "Year of paper publication")
        .add_property("language", &CTaskInfo::getLanguage, &CTaskInfo::setLanguage, "Python")
        .add_property("license", &CTaskInfo::getLicense, &CTaskInfo::setLicense, "License of the plugin")
        .add_property("repository", &CTaskInfo::getRepository, &CTaskInfo::setRepository, "Address of code repository (GitHub, GitLab, BitBucket...)")
        .def_readonly("internal", &CTaskInfo::isInternal, "Indicate a built-in algorithm.")
    ;

    //------------------------//
    //----- CTaskFactory -----//
    //------------------------//
    //Overload member functions
    WorkflowTaskPtr (CTaskFactory::*create_void)() = &CTaskFactory::create;
    WorkflowTaskPtr (CTaskFactory::*create_param)(const WorkflowTaskParamPtr&) = &CTaskFactory::create;
    CTaskInfo& (CTaskFactory::*getInfoByRef)() = &CTaskFactory::getInfo;

    class_<CTaskFactoryWrap, std::shared_ptr<CTaskFactoryWrap>, boost::noncopyable>("CTaskFactory", _processFactoryDocString)
        .add_property("info", make_function(getInfoByRef, return_internal_reference<>()), &CTaskFactory::setInfo, _processFactoryInfoDocString)
        .def("create", pure_virtual(create_void), _create1DocString, args("self"))
        .def("create", pure_virtual(create_param), _create2DocString, args("self", "param"))
    ;

    //--------------------------//
    //----- CWidgetFactory -----//
    //--------------------------//
    class_<CWidgetFactoryWrap, std::shared_ptr<CWidgetFactoryWrap>, boost::noncopyable>("CWidgetFactory", _widgetFactoryDocString)
        .add_property("name", &CWidgetFactory::getName, &CWidgetFactory::setName, _widgetFactoryNameDocString)
        .def("create", pure_virtual(&CWidgetFactory::create), _createWidgetDocString, args("self", "param"))
    ;

    //-----------------------------------//
    //----- CPluginProcessInterface -----//
    //-----------------------------------//
    class_<CPluginProcessInterfaceWrap, boost::noncopyable>("CPluginProcessInterface", _pluginInterfaceDocString)
        .def("getProcessFactory", pure_virtual(&CPluginProcessInterface::getProcessFactory), _getProcessFactoryDocString, args("self"))
        .def("getWidgetFactory", pure_virtual(&CPluginProcessInterface::getWidgetFactory), _getWidgetFactoryDocString, args("self"))
    ;

    //--------------------------//
    //----- CGraphicsInput -----//
    //--------------------------//
    class_<CGraphicsInputWrap, bases<CWorkflowTaskIO>, std::shared_ptr<CGraphicsInputWrap>>("CGraphicsInput", _graphicsInputDocString)
        .def(init<>("Default constructor"))
        .def(init<const CGraphicsInput&>("Copy constructor"))
        .def("setItems", &CGraphicsInput::setItems, _setItemsDocString, args("self", "items"))
        .def("getItems", &CGraphicsInput::getItems, _getItemsDocString, args("self"))
        .def("isDataAvailable", &CGraphicsInput::isDataAvailable, &CGraphicsInputWrap::default_isDataAvailable, _isGraphicsDataAvailableDocString, args("self"))
        .def("clearData", &CGraphicsInput::clearData, &CGraphicsInputWrap::default_clearData, _clearGraphicsDataDocString, args("self"))
    ;

    //---------------------------//
    //----- CGraphicsOutput -----//
    //---------------------------//
    ProxyGraphicsItemPtr (CGraphicsOutput::*addPoint1)(const CPointF&) = &CGraphicsOutput::addPoint;
    ProxyGraphicsItemPtr (CGraphicsOutput::*addPoint2)(const CPointF&, const GraphicsPointProperty&) = &CGraphicsOutput::addPoint;
    ProxyGraphicsItemPtr (CGraphicsOutput::*addRectangle1)(float, float, float, float) = &CGraphicsOutput::addRectangle;
    ProxyGraphicsItemPtr (CGraphicsOutput::*addRectangle2)(float, float, float, float, const GraphicsRectProperty&) = &CGraphicsOutput::addRectangle;
    ProxyGraphicsItemPtr (CGraphicsOutput::*addEllipse1)(float, float, float, float) = &CGraphicsOutput::addEllipse;
    ProxyGraphicsItemPtr (CGraphicsOutput::*addEllipse2)(float, float, float, float, const GraphicsEllipseProperty&) = &CGraphicsOutput::addEllipse;
    ProxyGraphicsItemPtr (CGraphicsOutput::*addPolygon1)(const std::vector<CPointF>&) = &CGraphicsOutput::addPolygon;
    ProxyGraphicsItemPtr (CGraphicsOutput::*addPolygon2)(const std::vector<CPointF>&, const GraphicsPolygonProperty&) = &CGraphicsOutput::addPolygon;
    ProxyGraphicsItemPtr (CGraphicsOutput::*addPolyline1)(const std::vector<CPointF>&) = &CGraphicsOutput::addPolyline;
    ProxyGraphicsItemPtr (CGraphicsOutput::*addPolyline2)(const std::vector<CPointF>&, const GraphicsPolylineProperty&) = &CGraphicsOutput::addPolyline;
    ProxyGraphicsItemPtr (CGraphicsOutput::*addComplexPolygon1)(const PolygonF&, const std::vector<PolygonF>&) = &CGraphicsOutput::addComplexPolygon;
    ProxyGraphicsItemPtr (CGraphicsOutput::*addComplexPolygon2)(const PolygonF&, const std::vector<PolygonF>&, const GraphicsPolygonProperty&) = &CGraphicsOutput::addComplexPolygon;
    ProxyGraphicsItemPtr (CGraphicsOutput::*addText1)(const std::string&, float x, float y) = &CGraphicsOutput::addText;
    ProxyGraphicsItemPtr (CGraphicsOutput::*addText2)(const std::string&, float x, float y, const GraphicsTextProperty&) = &CGraphicsOutput::addText;

    class_<CGraphicsOutput, bases<CWorkflowTaskIO>, std::shared_ptr<CGraphicsOutput>>("CGraphicsOutput", _graphicsOutputDocString)
        .def(init<>("Default constructor"))
        .def("setNewLayer", &CGraphicsOutput::setNewLayer, _setNewLayerDocString, args("self", "name"))
        .def("setImageIndex", &CGraphicsOutput::setImageIndex, _setImageIndexDocString, args("self", "index"))
        .def("addItem", &CGraphicsOutput::addItem, _addItemDocString, args("self", "item"))
        .def("addPoint", addPoint1, _addPoint1DocString, args("self", "point"))
        .def("addPoint", addPoint2, _addPoint2DocString, args("self", "point", "properties"))
        .def("addRectangle", addRectangle1, _addRectangle1DocString, args("self", "x", "y", "width", "height"))
        .def("addRectangle", addRectangle2, _addRectangle2DocString, args("self", "x", "y", "width", "height", "properties"))
        .def("addEllipse", addEllipse1, _addEllipse1DocString, args("self", "x", "y", "width", "height"))
        .def("addEllipse", addEllipse2, _addEllipse2DocString, args("self", "x", "y", "width", "height", "properties"))
        .def("addPolygon", addPolygon1, _addPolygon1DocString, args("self", "points"))
        .def("addPolygon", addPolygon2, _addPolygon2DocString, args("self", "points", "properties"))
        .def("addPolyline", addPolyline1, _addPolyline1DocString, args("self", "points"))
        .def("addPolyline", addPolyline2, _addPolyline2DocString, args("self", "points", "properties"))
        .def("addComplexPolygon", addComplexPolygon1, _addComplexPolygon1DocString, args("self", "outer", "inners"))
        .def("addComplexPolygon", addComplexPolygon2, _addComplexPolygon2DocString, args("self", "outer", "inners", "properties"))
        .def("addText", addText1, _addText1DocString, args("self", "text", "x", "y"))
        .def("addText", addText2, _addText2DocString, args("self", "text", "x", "y", "properties"))
    ;

    //--------------------//
    //----- CImageIO -----//
    //--------------------//
    void (CImageIO::*drawGraphicsIn)(const GraphicsInputPtr&) = &CImageIO::drawGraphics;
    void (CImageIO::*drawGraphicsOut)(const GraphicsOutputPtr&) = &CImageIO::drawGraphics;

    class_<CImageIOWrap, bases<CWorkflowTaskIO>, std::shared_ptr<CImageIOWrap>>("CImageIO", _imageProcessIODocString)
        .def(init<>("Default constructor"))
        .def(init<const CMat&>(_ctor1imageProcessIODocString))
        .def(init<IODataType>(_ctor2imageProcessIODocString))
        .def(init<IODataType, const CMat&>(_ctor3imageProcessIODocString))
        .def(init<const CImageIO&>("Copy constructor"))
        .def("setImage", &CImageIO::setImage, _setImageDocString, args("self", "image"))
        .def("setOverlayMask", &CImageIO::setOverlayMask, _setOverlayMaskDocString, args("self", "mask"))
        .def("setChannelCount", &CImageIO::setChannelCount, _setChannelCountDocString, args("self", "nb"))
        .def("setCurrentImage", &CImageIO::setCurrentImage, _setCurrentImageDocString, args("self", "index"))
        .def("getChannelCount", &CImageIO::getChannelCount, _getChannelCountDocString, args("self"))
        .def("getData", &CImageIO::getData, _getDataDocString, args("self"))
        .def("getImage", &CImageIO::getImage, &CImageIOWrap::default_getImage, _getImageDocString, args("self"))
        .def("getOverlayMask", &CImageIO::getOverlayMask, _getOverlayMaskDocString, args("self"))
        .def("getUnitElementCount", &CImageIO::getUnitElementCount, &CImageIOWrap::default_getUnitElementCount, _getImageUnitElementCountDocString, args("self"))
        .def("isDataAvailable", &CImageIO::isDataAvailable, &CImageIOWrap::default_isDataAvailable, _isImageDataAvailableDocString, args("self"))
        .def("isOverlayAvailable", &CImageIO::isOverlayAvailable, _isOverlayAvailableDocString, args("self"))
        .def("clearData", &CImageIO::clearData, &CImageIOWrap::default_clearData, _clearImageDataDocString, args("self"))
        .def("copyStaticData", &CImageIO::copyStaticData, &CImageIOWrap::default_copyStaticData, _copyImageStaticDataDocString, args("self", "io"))
        .def("drawGraphics", drawGraphicsIn, _drawGraphicsDocString, args("self", "graphics"))
        .def("drawGraphics", drawGraphicsOut, _drawGraphicsDocString, args("self", "graphics"))
    ;

    //----------------------//
    //----- CFeatureIO -----//
    //----------------------//
    enum_<NumericOutputType>("NumericOutputType", "Enum - List of a display types for numeric values")
        .value("NONE", NumericOutputType::NONE)
        .value("TABLE", NumericOutputType::TABLE)
        .value("PLOT", NumericOutputType::PLOT)
        .value("NUMERIC_FILE", NumericOutputType::NUMERIC_FILE)
    ;

    enum_<PlotType>("PlotType", "Enum - List of plot types")
        .value("CURVE", PlotType::CURVE)
        .value("BAR", PlotType::BAR)
        .value("MULTIBAR", PlotType::MULTIBAR)
        .value("HISTOGRAM", PlotType::HISTOGRAM)
        .value("PIE", PlotType::PIE)
    ;

    exposeFeatureIO<double>("CDblFeatureIO");

    //--------------------//
    //----- CVideoIO -----//
    //--------------------//
    class_<CVideoIOWrap, bases<CImageIO>, std::shared_ptr<CVideoIOWrap>>("CVideoIO", _videoProcessIODocString)
        .def(init<>("Default constructor"))
        .def(init<const CMat&>(_ctor1VideoProcessIODocString))
        .def(init<IODataType>(_ctor2VideoProcessIODocString))
        .def(init<IODataType, const CMat&>(_ctor3VideoProcessIODocString))
        .def(init<const CVideoIO&>("Copy constructor"))
        .def("setVideoPath", &CVideoIO::setVideoPath, _setVideoPathDocString, args("self", "path"))
        .def("setVideoPos", &CVideoIO::setVideoPos, _setVideoPosDocString, args("self", "position"))
        .def("getVideoFrameCount", &CVideoIO::getVideoFrameCount, _getVideoFrameCountDocString, args("self"))
        .def("getVideoImages", &CVideoIO::getVideoImages, _getVideoImagesDocString, args("self"))
        .def("getVideoPath", &CVideoIO::getVideoPath, _getVideoPathDocString, args("self"))
        .def("getSnapshot", &CVideoIO::getSnapshot, _getSnapshotDocString, args("self", "position"))
        .def("getCurrentPos", &CVideoIO::getCurrentPos, _getCurrentPosDocString, args("self"))
        .def("startVideo", &CVideoIO::startVideo, _startVideoDocString, args("self"))
        .def("stopVideo", &CVideoIO::stopVideo, _stopVideoDocString, args("self"))
        .def("startVideoWrite", &CVideoIO::startVideoWrite, _startVideoWriteDocString, args("self"))
        .def("stopVideoWrite", &CVideoIO::stopVideoWrite, _stopVideoWriteDocString, args("self"))
        .def("addVideoImage", &CVideoIO::addVideoImage, _addVideoImageDocString, args("self", "image"))
        .def("writeImage", &CVideoIO::writeImage, _writeImageDocString, args("self", "image"))
        .def("hasVideo", &CVideoIO::hasVideo, _hasVideoDocString, args("self"))
        .def("getImage", &CVideoIO::getImage, &CVideoIOWrap::default_getImage, _getVideoImageDocString, args("self"))
        .def("getUnitElementCount", &CVideoIO::getUnitElementCount, &CVideoIOWrap::default_getUnitElementCount, _getVideoUnitElementCountDocString, args("self"))
        .def("isDataAvailable", &CVideoIO::isDataAvailable, &CVideoIOWrap::default_isDataAvailable, _isVideoDataAvailableDocString, args("self"))
        .def("clearData", &CVideoIO::clearData, &CVideoIOWrap::default_clearData, _clearVideoDataDocString, args("self"))
        .def("copyStaticData", &CVideoIO::copyStaticData, &CVideoIOWrap::default_copyStaticData, _copyStaticDataDerivedDocString, args("self"))
    ;

    //-------------------------//
    //----- CWidgetOutput -----//
    //-------------------------//
    class_<CWidgetOutputWrap, bases<CWorkflowTaskIO>, std::shared_ptr<CWidgetOutputWrap>>("CWidgetOutput", _widgetOutputDocString)
        .def(init<>("Default constructor"))
        .def(init<IODataType>(_ctorWidgetOutputDocString))
        .def("setWidget", &CWidgetOutputWrap::setWidget, _setWidgetDocString, args("self", "widget"))
        .def("isDataAvailable", &CWidgetOutput::isDataAvailable, &CWidgetOutputWrap::default_isDataAvailable, _isWidgetDataAvailableDocString, args("self"))
        .def("clearData", &CWidgetOutput::clearData, &CWidgetOutputWrap::default_clearData, _clearWidgetDataDocString, args("self"))
    ;

    //-------------------//
    //----- CPathIO -----//
    //-------------------//
    class_<CPathIOWrap, bases<CWorkflowTaskIO>, std::shared_ptr<CPathIOWrap>>("CPathIO", _pathIODocString)
        .def(init<>("Default constructor"))
        .def(init<IODataType>(_ctor1PathIODocString))
        .def(init<IODataType, const std::string&>(_ctor2PathIODocString))
        .def("setPath", &CPathIO::setPath, _setPathDocString, args("self", "path"))
        .def("getPath", &CPathIO::getPath, _getPathDocString, args("self"))
        .def("isDataAvailable", &CPathIO::isDataAvailable, &CPathIOWrap::default_isDataAvailable, _isVideoDataAvailableDocString, args("self"))
        .def("clearData", &CPathIO::clearData, &CPathIOWrap::default_clearData, _clearDataDocString, args("self"))
    ;

    //----------------------//
    //----- CDatasetIO -----//
    //----------------------//
    class_<std::map<int, std::string>>("MapIntStr", "Data structure (same as Python dict) to store int key and string value")
        .def(map_indexing_suite<std::map<int, std::string>>())
    ;

    class_<CDatasetIOWrap, bases<CWorkflowTaskIO>, std::shared_ptr<CDatasetIOWrap>, boost::noncopyable>("CDatasetIO", _datasetIODocString)
        .def(init<>("Default constructor"))
        .def(init<const std::string&>(_ctor1DatasetIODocString))
        .def("getImagePaths", &CDatasetIO::getImagePaths, &CDatasetIOWrap::default_getImagePaths, _getImagePathsDocStr)
        .def("getCategories", &CDatasetIO::getCategories, &CDatasetIOWrap::default_getCategories, _getCategoriesDocStr)
        .def("getCategoryCount", &CDatasetIO::getCategoryCount, &CDatasetIOWrap::default_getCategoryCount, _getCategoryCountDocStr)
        .def("getMaskPath", &CDatasetIO::getMaskPath, &CDatasetIOWrap::default_getMaskPath, _getMaskPathDocStr)
        .def("getGraphicsAnnotations", &CDatasetIO::getAnnotationGraphics, &CDatasetIOWrap::default_getAnnotationGraphics, _getGraphicsAnnotationsDocStr)
        .def("getSourceFormat", &CDatasetIO::getSourceFormat, _getSourceFormatDocStr)
        .def("isDataAvailable", &CDatasetIO::isDataAvailable, &CDatasetIOWrap::default_isDataAvailable, _isDataAvailableDerivedDocString, args("self"))
        .def("clearData", &CDatasetIO::clearData, &CDatasetIOWrap::default_clearData, _clearDataDerivedDocString, args("self"))
        .def("save", &CDatasetIO::save, &CDatasetIOWrap::default_save, _saveDocStr)
    ;

    //--------------------//
    //----- CArrayIO -----//
    //--------------------//
    class_<CArrayIOWrap, bases<CWorkflowTaskIO>, std::shared_ptr<CArrayIOWrap>>("CArrayIO", _arrayIODocString)
        .def(init<>("Default constructor"))
        .def(init<const CMat&>(_ctor1ArrayIODocString))
        .def(init<const CArrayIO&>("Copy constructor"))
        .def("setArray", &CArrayIO::setArray, _setArrayDocString, args("self", "array"))
        .def("getArray", &CArrayIO::getArray, _getArrayDocString, args("self"))
        .def("getUnitElementCount", &CArrayIO::getUnitElementCount, &CArrayIOWrap::default_getUnitElementCount, _getArrayUnitElementCountDocString, args("self"))
        .def("isDataAvailable", &CArrayIO::isDataAvailable, &CArrayIOWrap::default_isDataAvailable, _isArrayDataAvailableDocString, args("self"))
        .def("clearData", &CArrayIO::clearData, &CArrayIOWrap::default_clearData, _clearArrayDataDocString, args("self"))
    ;

    //------------------------//
    //----- C2dImageTask -----//
    //------------------------//
    //Overload member functions
    size_t (C2dImageTask::*getProgressSteps1)() = &C2dImageTask::getProgressSteps;
    size_t (C2dImageTask::*getProgressSteps2)(size_t) = &C2dImageTask::getProgressSteps;
    size_t (C2dImageTaskWrap::*default_getProgressSteps1)() = &C2dImageTaskWrap::default_getProgressSteps;
    size_t (C2dImageTaskWrap::*default_getProgressSteps2)(size_t) = &C2dImageTaskWrap::default_getProgressSteps;

    class_<C2dImageTaskWrap, bases<CWorkflowTask>, std::shared_ptr<C2dImageTaskWrap>>("C2dImageTask", _imageProcess2dDocString)
        .def(init<>("Default constructor"))
        .def(init<bool>(_ctor1ImageProcess2dDocString))
        .def(init<const std::string&>(_ctor2ImageProcess2dDocString))
        .def(init<const std::string&, bool>(_ctor3ImageProcess2dDocString))
        .def("setActive", &C2dImageTask::setActive, &C2dImageTaskWrap::default_setActive, _setActiveDocString, args("self", "is_active"))
        .def("setOutputColorMap", &C2dImageTaskWrap::setOutputColorMap, _setOutputColorMapDocString, args("self", "index", "mask_index", "colors"))
        .def("updateStaticOutputs", &C2dImageTask::updateStaticOutputs, &C2dImageTaskWrap::default_updateStaticOutputs, _updateStaticOutputsDocString, args("self"))
        .def("beginTaskRun", &C2dImageTask::beginTaskRun, &C2dImageTaskWrap::default_beginTaskRun, _beginTaskRunDocString, args("self"))
        .def("endTaskRun", &C2dImageTask::endTaskRun, &C2dImageTaskWrap::default_endTaskRun, _endTaskRunDocString, args("self"))
        .def("graphicsChanged", &C2dImageTask::graphicsChanged, &C2dImageTaskWrap::default_graphicsChanged, _graphicsChangedDocString, args("self"))
        .def("globalInputChanged", &C2dImageTask::globalInputChanged, &C2dImageTaskWrap::default_globalInputChanged, _globalInputChangedDocString, args("self", "is_new_sequence"))
        .def("createGraphicsMask", &C2dImageTask::createGraphicsMask, _createGraphicsMaskDocString, args("self", "width", "height", "graphics"))
        .def("applyGraphicsMask", &C2dImageTask::applyGraphicsMask, _applyGraphicsMaskDocString, args("self", "src", "dst", "index"))
        .def("applyGraphicsMaskToBinary", &C2dImageTask::applyGraphicsMaskToBinary, _applyGraphicsMaskToBinaryDocString, args("self", "src", "dst", "index"))
        .def("getProgressSteps", getProgressSteps1, default_getProgressSteps1, _getProgressStepsDocString, args("self"))
        .def("getProgressSteps", getProgressSteps2, default_getProgressSteps2, _getProgressStepsDocString, args("self", "unit_elt_count"))
        .def("getGraphicsMask", &C2dImageTask::getGraphicsMask, _getGraphicsMaskDocString, args("self", "index"))
        .def("isMaskAvailable", &C2dImageTask::isMaskAvailable, _isMaskAvailableDocString, args("self", "index"))
        .def("run", &C2dImageTask::run, &C2dImageTaskWrap::default_run, _runDocString, args("self"))
        .def("stop", &C2dImageTask::stop, &C2dImageTaskWrap::default_stop, _stopDocString, args("self"))
        .def("forwardInputImage", &C2dImageTask::forwardInputImage, _forwardInputImageDocString, args("self", "input_index", "output_index"))
        .def("emitAddSubProgressSteps", &C2dImageTaskWrap::emitAddSubProgressSteps, _emitAddSubProgressSteps, args("self", "count"))
        .def("emitStepProgress", &C2dImageTaskWrap::emitStepProgress, _emitStepProgressDocString, args("self"))
        .def("emitGraphicsContextChanged", &C2dImageTaskWrap::emitGraphicsContextChanged, _emitGraphicsContextChangedDocString, args("self"))
        .def("emitOutputChanged", &C2dImageTaskWrap::emitOutputChanged, _emitOutputChangedDocString, args("self"))
    ;

    //-----------------------------------//
    //----- C2dImageInteractiveTask -----//
    //-----------------------------------//
    //Overload member functions
    size_t (C2dImageInteractiveTask::*getProgressSteps3)() = &C2dImageInteractiveTask::getProgressSteps;
    size_t (C2dImageInteractiveTask::*getProgressSteps4)(size_t) = &C2dImageInteractiveTask::getProgressSteps;
    size_t (C2dImageInteractiveTaskWrap::*default_getProgressSteps3)() = &C2dImageInteractiveTaskWrap::default_getProgressSteps;
    size_t (C2dImageInteractiveTaskWrap::*default_getProgressSteps4)(size_t) = &C2dImageInteractiveTaskWrap::default_getProgressSteps;

    class_<C2dImageInteractiveTaskWrap, bases<C2dImageTask>, std::shared_ptr<C2dImageInteractiveTaskWrap>>("C2dImageInteractiveTask", _interactiveImageProcess2d)
        .def(init<>("Default constructor"))
        .def(init<const std::string&>(_ctorInteractiveImageProcessDocString))
        .def("setActive", &C2dImageInteractiveTask::setActive, &C2dImageInteractiveTaskWrap::default_setActive, _setActiveInteractiveDocString, args("self", "is_active"))
        .def("updateStaticOutputs", &C2dImageInteractiveTask::updateStaticOutputs, &C2dImageInteractiveTaskWrap::default_updateStaticOutputs, _updateStaticOutputsDocString, args("self"))
        .def("beginTaskRun", &C2dImageInteractiveTask::beginTaskRun, &C2dImageInteractiveTaskWrap::default_beginTaskRun, _beginTaskRunDocString, args("self"))
        .def("endTaskRun", &C2dImageInteractiveTask::endTaskRun, &C2dImageInteractiveTaskWrap::default_endTaskRun, _endTaskRunDocString, args("self"))
        .def("graphicsChanged", &C2dImageInteractiveTask::graphicsChanged, &C2dImageInteractiveTaskWrap::default_graphicsChanged, _graphicsChangedInteractiveDocString, args("self"))
        .def("globalInputChanged", &C2dImageInteractiveTask::globalInputChanged, &C2dImageInteractiveTaskWrap::default_globalInputChanged, _globalInputChangedInteractiveDocString, args("self", "is_new_sequence"))
        .def("getProgressSteps", getProgressSteps3, default_getProgressSteps3, _getProgressStepsDocString, args("self"))
        .def("getProgressSteps", getProgressSteps4, default_getProgressSteps4, _getProgressStepsDocString, args("self", "unit_elt_count"))
        .def("getInteractionMask", &C2dImageInteractiveTask::getInteractionMask, _getInteractionMaskDocString, args("self"))
        .def("getBlobs", &C2dImageInteractiveTask::getBlobs, _getBlobsDocString, args("self"))
        .def("createInteractionMask", &C2dImageInteractiveTask::createInteractionMask, _createInteractionMaskDocString, args("self", "width", "height"))
        .def("computeBlobs", &C2dImageInteractiveTask::computeBlobs, _computeBlobsDocString, args("self"))
        .def("clearInteractionLayer", &C2dImageInteractiveTask::clearInteractionLayer, _clearInteractionLayerDocString, args("self"))
        .def("run", &C2dImageInteractiveTask::run, &C2dImageInteractiveTaskWrap::default_run, _runDocString, args("self"))
        .def("stop", &C2dImageInteractiveTask::stop, &C2dImageInteractiveTaskWrap::default_stop, _stopDocString, args("self"))
        .def("emitAddSubProgressSteps", &C2dImageInteractiveTaskWrap::emitAddSubProgressSteps, _emitAddSubProgressSteps, args("self", "count"))
        .def("emitStepProgress", &C2dImageInteractiveTaskWrap::emitStepProgress, _emitStepProgressDocString, args("self"))
        .def("emitGraphicsContextChanged", &C2dImageInteractiveTaskWrap::emitGraphicsContextChanged, _emitGraphicsContextChangedDocString, args("self"))
        .def("emitOutputChanged", &C2dImageInteractiveTaskWrap::emitOutputChanged, _emitOutputChangedDocString, args("self"))
    ;

    //----------------------//
    //----- CVideoTask -----//
    //----------------------//
    //Overload member functions
    size_t (CVideoTask::*getProgressSteps5)() = &CVideoTask::getProgressSteps;
    size_t (CVideoTask::*getProgressSteps6)(size_t) = &CVideoTask::getProgressSteps;
    size_t (CVideoTaskWrap::*default_getProgressSteps5)() = &CVideoTaskWrap::default_getProgressSteps;
    size_t (CVideoTaskWrap::*default_getProgressSteps6)(size_t) = &CVideoTaskWrap::default_getProgressSteps;

    class_<CVideoTaskWrap, bases<C2dImageTask>, std::shared_ptr<CVideoTaskWrap>>("CVideoTask", _videoProcessDocString)
        .def(init<>("Default constructor"))
        .def(init<const std::string&>(_ctorVideoProcessDocString))
        .def("setActive", &CVideoTask::setActive, &CVideoTaskWrap::default_setActive, _setActiveDocString, args("self", "is_active"))
        .def("updateStaticOutputs", &CVideoTask::updateStaticOutputs, &CVideoTaskWrap::default_updateStaticOutputs, _updateStaticOutputsDocString, args("self"))
        .def("beginTaskRun", &CVideoTask::beginTaskRun, &CVideoTaskWrap::default_beginTaskRun, _beginTaskRunVideoDocString, args("self"))
        .def("endTaskRun", &CVideoTask::endTaskRun, &CVideoTaskWrap::default_endTaskRun, _endTaskRunDocString, args("self"))
        .def("notifyVideoStart", &CVideoTask::notifyVideoStart, &CVideoTaskWrap::default_notifyVideoStart, _notifyVideoStartDocString, args("self", "frame_count"))
        .def("notifyVideoEnd", &CVideoTask::notifyVideoEnd, &CVideoTaskWrap::default_notifyVideoEnd, _notifyVideoEndDocString, args("self"))
        .def("graphicsChanged", &CVideoTask::graphicsChanged, &CVideoTaskWrap::default_graphicsChanged, _graphicsChangedDocString, args("self"))
        .def("globalInputChanged", &CVideoTask::globalInputChanged, &CVideoTaskWrap::default_globalInputChanged, _globalInputChangedDocString, args("self", "is_new_sequence"))
        .def("getProgressSteps", getProgressSteps5, default_getProgressSteps5, _getProgressStepsDocString, args("self"))
        .def("getProgressSteps", getProgressSteps6, default_getProgressSteps6, _getProcessFactoryDocString, args("self", "unit_elt_count"))
        .def("run", &CVideoTask::run, &CVideoTaskWrap::default_run, _runDocString, args("self"))
        .def("stop", &CVideoTask::stop, &CVideoTaskWrap::default_stop, _stopDocString, args("self"))
        .def("emitAddSubProgressSteps", &CVideoTaskWrap::emitAddSubProgressSteps, _emitAddSubProgressSteps, args("self", "count"))
        .def("emitStepProgress", &CVideoTaskWrap::emitStepProgress, _emitStepProgressDocString, args("self"))
        .def("emitGraphicsContextChanged", &CVideoTaskWrap::emitGraphicsContextChanged, _emitGraphicsContextChangedDocString, args("self"))
        .def("emitOutputChanged", &CVideoTaskWrap::emitOutputChanged, _emitOutputChangedDocString, args("self"))
    ;

    //------------------------//
    //----- CVideoOFTask -----//
    //------------------------//
    //Overload member functions
    size_t (CVideoOFTask::*getProgressSteps7)() = &CVideoOFTask::getProgressSteps;
    size_t (CVideoOFTask::*getProgressSteps8)(size_t) = &CVideoOFTask::getProgressSteps;
    size_t (CVideoOFTaskWrap::*default_getProgressSteps7)() = &CVideoOFTaskWrap::default_getProgressSteps;
    size_t (CVideoOFTaskWrap::*default_getProgressSteps8)(size_t) = &CVideoOFTaskWrap::default_getProgressSteps;

    class_<CVideoOFTaskWrap, bases<CVideoTask>, std::shared_ptr<CVideoOFTaskWrap>>("CVideoOFTask", _videoProcessOFDocString)
        .def(init<>("Default constructor"))
        .def(init<const std::string&>(_ctorVideoProcessOFDocString))
        .def("setActive", &CVideoOFTask::setActive, &CVideoOFTaskWrap::default_setActive, _setActiveDocString, args("self", "is_active"))
        .def("updateStaticOutputs", &CVideoOFTask::updateStaticOutputs, &CVideoOFTaskWrap::default_updateStaticOutputs, _updateStaticOutputsDocString, args("self"))
        .def("beginTaskRun", &CVideoOFTask::beginTaskRun, &CVideoOFTaskWrap::default_beginTaskRun, _beginTaskRunVideoOFDocString, args("self"))
        .def("endTaskRun", &CVideoOFTask::endTaskRun, &CVideoOFTaskWrap::default_endTaskRun, _endTaskRunDocString, args("self"))
        .def("graphicsChanged", &CVideoOFTask::graphicsChanged, &CVideoOFTaskWrap::default_graphicsChanged, _graphicsChangedDocString, args("self"))
        .def("globalInputChanged", &CVideoOFTask::globalInputChanged, &CVideoOFTaskWrap::default_globalInputChanged, _globalInputChangedDocString, args("self" "is_new_sequence"))
        .def("getProgressSteps", getProgressSteps7, default_getProgressSteps7, _getProgressStepsDocString, args("self"))
        .def("getProgressSteps", getProgressSteps8, default_getProgressSteps8, _getProgressStepsDocString, args("self", "unit_elt_count"))
        .def("run", &CVideoOFTask::run, &CVideoOFTaskWrap::default_run, _runDocString, args("self"))
        .def("stop", &CVideoOFTask::stop, &CVideoOFTaskWrap::default_stop, _stopDocString, args("self"))
        .def("emitAddSubProgressSteps", &CVideoOFTaskWrap::emitAddSubProgressSteps, _emitAddSubProgressSteps, args("self", "count"))
        .def("emitStepProgress", &CVideoOFTaskWrap::emitStepProgress, _emitStepProgressDocString, args("self"))
        .def("emitGraphicsContextChanged", &CVideoOFTaskWrap::emitGraphicsContextChanged, _emitGraphicsContextChangedDocString, args("self"))
        .def("emitOutputChanged", &CVideoOFTaskWrap::emitOutputChanged, _emitOutputChangedDocString, args("self"))
        .def("drawOptFlowMap", &CVideoOFTask::drawOptFlowMap, _drawOptFlowMapDocString, args("self", "flow", "vectors", "step"))
        .def("flowToDisplay", &CVideoOFTask::flowToDisplay, _flowToDisplayDocString, args("self", "flow"))
    ;

    //------------------------------//
    //----- CVideoTrackingTask -----//
    //------------------------------//
    //Overload member functions
    size_t (CVideoTrackingTask::*getProgressSteps9)() = &CVideoTrackingTask::getProgressSteps;
    size_t (CVideoTrackingTask::*getProgressSteps10)(size_t) = &CVideoTrackingTask::getProgressSteps;
    size_t (CVideoTrackingTaskWrap::*default_getProgressSteps9)() = &CVideoTrackingTaskWrap::default_getProgressSteps;
    size_t (CVideoTrackingTaskWrap::*default_getProgressSteps10)(size_t) = &CVideoTrackingTaskWrap::default_getProgressSteps;

    class_<CVideoTrackingTaskWrap, bases<CVideoTask>, std::shared_ptr<CVideoTrackingTaskWrap>>("CVideoTrackingTask", _videoProcessTrackingDocString)
        .def(init<>("Default constructor"))
        .def(init<const std::string&>(_ctorVideoTrackingDocString))
        .def("setActive", &CVideoTrackingTask::setActive, &CVideoTrackingTaskWrap::default_setActive, _setActiveDocString, args("self", "is_active"))
        .def("updateStaticOutputs", &CVideoTrackingTask::updateStaticOutputs, &CVideoTrackingTaskWrap::default_updateStaticOutputs, _updateStaticOutputsDocString, args("self"))
        .def("beginTaskRun", &CVideoTrackingTask::beginTaskRun, &CVideoTrackingTaskWrap::default_beginTaskRun, _beginTaskRunVideoOFDocString, args("self"))
        .def("endTaskRun", &CVideoTrackingTask::endTaskRun, &CVideoTrackingTaskWrap::default_endTaskRun, _endTaskRunDocString, args("self"))
        .def("graphicsChanged", &CVideoTrackingTask::graphicsChanged, &CVideoTrackingTaskWrap::default_graphicsChanged, _graphicsChangedDocString, args("self"))
        .def("globalInputChanged", &CVideoTrackingTask::globalInputChanged, &CVideoTrackingTaskWrap::default_globalInputChanged, _globalInputChangedDocString, args("self", "is_new_sequence"))
        .def("getProgressSteps", getProgressSteps9, default_getProgressSteps9, _getProgressStepsDocString, args("self"))
        .def("getProgressSteps", getProgressSteps10, default_getProgressSteps10, _getProgressStepsDocString, args("self" "unit_elt_count"))
        .def("run", &CVideoTrackingTask::run, &CVideoTrackingTaskWrap::default_run, _runDocString, args("self"))
        .def("stop", &CVideoTrackingTask::stop, &CVideoTrackingTaskWrap::default_stop, _stopDocString, args("self"))
        .def("emitAddSubProgressSteps", &CVideoTrackingTaskWrap::emitAddSubProgressSteps, _emitAddSubProgressSteps, args("self", "count"))
        .def("emitStepProgress", &CVideoTrackingTaskWrap::emitStepProgress, _emitStepProgressDocString, args("self"))
        .def("emitGraphicsContextChanged", &CVideoTrackingTaskWrap::emitGraphicsContextChanged, _emitGraphicsContextChangedDocString, args("self"))
        .def("emitOutputChanged", &CVideoTrackingTaskWrap::emitOutputChanged, _emitOutputChangedDocString, args("self"))
        .def("setRoiToTrack", &CVideoTrackingTask::setRoiToTrack, _setRoiToTrackDocString, args("self"))
        .def("manageOutputs", &CVideoTrackingTask::manageOutputs, _manageOutputsDocString, args("self"))
    ;

    //-------------------------//
    //----- CDnnTrainTask -----//
    //-------------------------//
    //Overload member functions
    size_t (CDnnTrainTask::*getProgressSteps11)() = &CDnnTrainTask::getProgressSteps;
    size_t (CDnnTrainTask::*getProgressSteps12)(size_t) = &CDnnTrainTask::getProgressSteps;
    size_t (CDnnTrainTaskWrap::*default_getProgressSteps11)() = &CDnnTrainTaskWrap::default_getProgressSteps;
    size_t (CDnnTrainTaskWrap::*default_getProgressSteps12)(size_t) = &CDnnTrainTaskWrap::default_getProgressSteps;

    class_<CDnnTrainTaskWrap, bases<CWorkflowTask>, std::shared_ptr<CDnnTrainTaskWrap>>("CDnnTrainTask", _dnnTrainProcessDocString)
        .def(init<>("Default constructor"))
        .def(init<const std::string&>(_ctor1DnnTrainProcessDocString))
        .def(init<const std::string&, const std::shared_ptr<CDnnTrainTaskParam>&>(_ctor2DnnTrainProcessDocString))
        .def("setActive", &CDnnTrainTask::setActive, &CDnnTrainTaskWrap::default_setActive, _setActiveDocString, args("self", "is_active"))
        .def("beginTaskRun", &CDnnTrainTask::beginTaskRun, &CDnnTrainTaskWrap::default_beginTaskRun, _beginTaskRunDocString, args("self"))
        .def("endTaskRun", &CDnnTrainTask::endTaskRun, &CDnnTrainTaskWrap::default_endTaskRun, _endTaskRunDocString, args("self"))
        .def("getTensorboardLogDir", &CDnnTrainTask::getTensorboardLogDir, _getTensorboardLogDirDocString, args("self"))
        .def("getProgressSteps", getProgressSteps11, default_getProgressSteps11, _getProgressStepsDocString, args("self"))
        .def("getProgressSteps", getProgressSteps12, default_getProgressSteps12, _getProgressStepsDocString, args("self", "unit_elt_count"))
        .def("run", &CDnnTrainTask::run, &CDnnTrainTaskWrap::default_run, _runDocString, args("self"))
        .def("stop", &CDnnTrainTask::stop, &CDnnTrainTaskWrap::default_stop, _stopDocString, args("self"))
        .def("emitAddSubProgressSteps", &CDnnTrainTaskWrap::emitAddSubProgressSteps, _emitAddSubProgressSteps, args("self", "count"))
        .def("emitStepProgress", &CDnnTrainTaskWrap::emitStepProgress, _emitStepProgressDocString, args("self"))
        .def("emitOutputChanged", &CDnnTrainTaskWrap::emitOutputChanged, _emitOutputChangedDocString, args("self"))
        .def("enableMlflow", &CDnnTrainTask::enableMlflow, _enableMlflowDocString, args("self", "enable"))
        .def("enableTensorboard", &CDnnTrainTask::enableTensorboard, _enableTensorboardDocString, args("self", "enable"))
    ;

    //------------------------------//
    //----- CDnnTrainTaskParam -----//
    //------------------------------//
    class_<CDnnTrainTaskParamWrap, bases<CWorkflowTaskParam>, std::shared_ptr<CDnnTrainTaskParamWrap>>("CDnnTrainTaskParam", _dnnTrainProcessParamDocString)
        .enable_pickling()
        .def(init<>("Default constructor"))
        .def("__copy__", &generic_copy<CDnnTrainTaskParamWrap>)
        .def("__deepcopy__", &generic_deepcopy<CDnnTrainTaskParamWrap>)
        .def("setParamMap", &CDnnTrainTaskParam::setParamMap, &CDnnTrainTaskParamWrap::default_setParamMap, _setParamMapDocString, args("self", "params"))
        .def("getParamMap", &CDnnTrainTaskParam::getParamMap, &CDnnTrainTaskParamWrap::default_getParamMap, _getParamMapDocString, args("self"))
        .def("getHashValue", &CDnnTrainTaskParam::getHashValue, &CDnnTrainTaskParamWrap::default_getHashValue, _getHashValueDocString, args("self"))
    ;

    //---------------------------//
    //----- CIkomiaRegistry -----//
    //---------------------------//
    WorkflowTaskPtr (CIkomiaRegistry::*createInstance1)(const std::string&) = &CIkomiaRegistry::createInstance;
    WorkflowTaskPtr (CIkomiaRegistry::*createInstance2)(const std::string&, const WorkflowTaskParamPtr&) = &CIkomiaRegistry::createInstance;

    class_<CIkomiaRegistry, std::shared_ptr<CIkomiaRegistry>>("CIkomiaRegistry", _ikomiaRegistryDocString)
        .def(init<>("Default constructor"))
        .def("setPluginsDirectory", &CIkomiaRegistry::setPluginsDirectory, _setPluginsDirDocString, args("self", "directory"))
        .def("getPluginsDirectory", &CIkomiaRegistry::getPluginsDirectory, _getPluginsDirDocString, args("self"))
        .def("getAlgorithms", &CIkomiaRegistry::getAlgorithms, _getAlgorithmsDocString, args("self)"))
        .def("getAlgorithmInfo", &CIkomiaRegistry::getAlgorithmInfo, _getAlgorithmInfoDocString, args("self", "name"))
        .def("createInstance", createInstance1, _createInstance1DocString, args("self", "name"))
        .def("createInstance", createInstance2, _createInstance2DocString, args("self", "name", "parameters"))
        .def("registerTask", &CIkomiaRegistry::registerTask, _registerTaskDocString, args("self", "factory"))
        .def("loadCppPlugin", &CIkomiaRegistry::loadCppPlugin, _loadCppPluginDocString, args("self", "path"))
    ;
}
