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
#include "CProcessFactoryWrap.h"
#include "CWidgetFactoryWrap.h"
#include "CPluginProcessInterfaceWrap.h"
#include "CImageProcess2dWrap.h"
#include "CInteractiveImageProcess2dWrap.h"
#include "CVideoProcessWrap.h"
#include "CVideoProcessOFWrap.h"
#include "CVideoProcessTrackingWrap.h"
#include "CDnnTrainProcessWrap.h"
#include "CDnnTrainProcessParamWrap.h"
#include "CFeatureProcessIOWrap.hpp"
#include "CGraphicsInputWrap.h"
#include "CImageProcessIOWrap.h"
#include "CVideoProcessIOWrap.h"
#include "CWidgetOutputWrap.h"
#include "CDatasetIOWrap.h"
#include "CPathIOWrap.h"
#include "CArrayIOWrap.h"

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
    void (CFeatureProcessIO<Type>::*addValueList1)(const std::vector<Type>&) = &CFeatureProcessIO<Type>::addValueList;
    void (CFeatureProcessIO<Type>::*addValueList2)(const std::vector<Type>&, const std::string&) = &CFeatureProcessIO<Type>::addValueList;
    void (CFeatureProcessIO<Type>::*addValueList3)(const std::vector<Type>&, const std::vector<std::string>&) = &CFeatureProcessIO<Type>::addValueList;
    void (CFeatureProcessIO<Type>::*addValueList4)(const std::vector<Type>&, const std::string&, const std::vector<std::string>&) = &CFeatureProcessIO<Type>::addValueList;

    class_<CFeatureProcessIOWrap<Type>, bases<CProtocolTaskIO>, std::shared_ptr<CFeatureProcessIOWrap<Type>>>(className.c_str(), _featureProcessIODocString)
        .def(init<>("Default constructor"))
        .def(init<const CFeatureProcessIO<Type>&>("Copy constructor"))
        .def("setOutputType", &CFeatureProcessIO<Type>::setOutputType, _setOutputTypeDocString, args("self", "type"))
        .def("setPlotType", &CFeatureProcessIO<Type>::setPlotType, _setPlotTypeDocString, args("self", "type"))
        .def("addValueList", addValueList1, _addValueList1DocString, args("self", "values"))
        .def("addValueList", addValueList2, _addValueList2DocString, args("self", "values", "header_label"))
        .def("addValueList", addValueList3, _addValueList3DocString, args("self", "values", "labels"))
        .def("addValueList", addValueList4, _addValueList4DocString, args("self", "values", "header_label", "labels"))
        .def("getOutputType", &CFeatureProcessIO<Type>::getOutputType, _getOutputTypeDocString, args("self"))
        .def("getPlotType", &CFeatureProcessIO<Type>::getPlotType, _getPlotTypeDocString, args("self"))
        .def("getValueList", &CFeatureProcessIO<Type>::getValueList, _getValueListDocString, args("self", "index"))
        .def("getAllValueList", &CFeatureProcessIO<Type>::getAllValues, _getAllValueListDocString, args("self"))
        .def("getAllLabelList", &CFeatureProcessIO<Type>::getAllValueLabels, _getAllLabelListDocString, args("self"))
        .def("getUnitElementCount", &CFeatureProcessIO<Type>::getUnitElementCount, &CFeatureProcessIOWrap<Type>::default_getUnitElementCount, _getUnitEltCountDerivedDocString, args("self"))
        .def("isDataAvailable", &CFeatureProcessIO<Type>::isDataAvailable, &CFeatureProcessIOWrap<Type>::default_isDataAvailable, _isDataAvailableDerivedDocString, args("self"))
        .def("clearData", &CFeatureProcessIO<Type>::clearData, &CFeatureProcessIOWrap<Type>::default_clearData, _clearDataDerivedDocString, args("self"))
        .def("copyStaticData", &CFeatureProcessIO<Type>::copyStaticData, &CFeatureProcessIOWrap<Type>::default_copyStaticData, _copyStaticDataDerivedDocString, args("self", "io"))
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
    register_ptr_to_python<std::shared_ptr<CProcessFactory>>();
    register_ptr_to_python<std::shared_ptr<CWidgetFactory>>();
    register_ptr_to_python<std::shared_ptr<CGraphicsProcessInput>>();
    register_ptr_to_python<std::shared_ptr<CImageProcessIO>>();
    register_ptr_to_python<std::shared_ptr<CVideoProcessIO>>();
    register_ptr_to_python<std::shared_ptr<CWidgetOutput>>();
    register_ptr_to_python<std::shared_ptr<CPathIO>>();
    register_ptr_to_python<std::shared_ptr<CDatasetIO>>();
    register_ptr_to_python<std::shared_ptr<CArrayIO>>();
    register_ptr_to_python<std::shared_ptr<CImageProcess2d>>();
    register_ptr_to_python<std::shared_ptr<CInteractiveImageProcess2d>>();
    register_ptr_to_python<std::shared_ptr<CVideoProcess>>();
    register_ptr_to_python<std::shared_ptr<CVideoProcessOF>>();
    register_ptr_to_python<std::shared_ptr<CVideoProcessTracking>>();
    register_ptr_to_python<std::shared_ptr<CDnnTrainProcess>>();
    register_ptr_to_python<std::shared_ptr<CDnnTrainProcessParam>>();

    //Register std::vector<T> <-> python list converters
    registerStdVector<uchar>();
    registerStdVector<std::vector<uchar>>();

    //------------------------//
    //----- CProcessInfo -----//
    //------------------------//
    enum_<CProcessInfo::Language>("Language", "Enum - List of supported programming language")
        .value("CPP", CProcessInfo::CPP)
        .value("PYTHON", CProcessInfo::PYTHON)
    ;

    class_<CProcessInfo>("CProcessInfo", _processInfoDocString, init<>("Default constructor"))
        .add_property("name", &CProcessInfo::getName, &CProcessInfo::setName, "Name of the plugin (mandatory - must be unique)")
        .add_property("path", &CProcessInfo::getPath, &CProcessInfo::setPath, "Path in the library tree view of Ikomia")
        .add_property("shortDescription", &CProcessInfo::getShortDescription, &CProcessInfo::setShortDescription, "Short description of the plugin (mandatory)")
        .add_property("description", &CProcessInfo::getDescription, &CProcessInfo::setDescription, "Full description of the plugin (mandatory)")
        .add_property("documentationLink", &CProcessInfo::getDocumentationLink, &CProcessInfo::setDocumentationLink, "Address (URL) of online documentation")
        .add_property("iconPath", &CProcessInfo::getIconPath, &CProcessInfo::setIconPath, "Relative path to the plugin icon")
        .add_property("keywords", &CProcessInfo::getKeywords, &CProcessInfo::setKeywords, "Keywords associated with the plugin (Used for Ikomia search engine")
        .add_property("authors", &CProcessInfo::getAuthors, &CProcessInfo::setAuthors, "Authors of the plugin and/or corresponding paper (mandatory)")
        .add_property("article", &CProcessInfo::getArticle, &CProcessInfo::setArticle, "Title of the corresponding paper")
        .add_property("journal", &CProcessInfo::getJournal, &CProcessInfo::setJournal, "Paper journal")
        .add_property("version", &CProcessInfo::getVersion, &CProcessInfo::setVersion, "Plugin version (mandatory)")
        .add_property("year", &CProcessInfo::getYear, &CProcessInfo::setYear, "Year of paper publication")
        .add_property("language", &CProcessInfo::getLanguage, &CProcessInfo::setLanguage, "Python")
        .add_property("license", &CProcessInfo::getLicense, &CProcessInfo::setLicense, "License of the plugin")
        .add_property("repository", &CProcessInfo::getRepository, &CProcessInfo::setRepository, "Address of code repository (GitHub, GitLab, BitBucket...)")
    ;

    //---------------------------//
    //----- CProcessFactory -----//
    //---------------------------//
    //Overload member functions
    ProtocolTaskPtr (CProcessFactory::*create_void)() = &CProcessFactory::create;
    ProtocolTaskPtr (CProcessFactory::*create_param)(const ProtocolTaskParamPtr&) = &CProcessFactory::create;
    CProcessInfo& (CProcessFactory::*getInfoByRef)() = &CProcessFactory::getInfo;

    class_<CProcessFactoryWrap, std::shared_ptr<CProcessFactoryWrap>, boost::noncopyable>("CProcessFactory", _processFactoryDocString)
        .add_property("info", make_function(getInfoByRef, return_internal_reference<>()), &CProcessFactory::setInfo, _processFactoryInfoDocString)
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
    class_<CGraphicsInputWrap, bases<CProtocolTaskIO>, std::shared_ptr<CGraphicsInputWrap>>("CGraphicsInput", _graphicsInputDocString)
        .def(init<>("Default constructor"))
        .def(init<const CGraphicsProcessInput&>("Copy constructor"))
        .def("setItems", &CGraphicsProcessInput::setItems, _setItemsDocString, args("self", "items"))
        .def("getItems", &CGraphicsProcessInput::getItems, _getItemsDocString, args("self"))
        .def("isDataAvailable", &CGraphicsProcessInput::isDataAvailable, &CGraphicsInputWrap::default_isDataAvailable, _isGraphicsDataAvailableDocString, args("self"))
        .def("clearData", &CGraphicsProcessInput::clearData, &CGraphicsInputWrap::default_clearData, _clearGraphicsDataDocString, args("self"))
    ;

    //---------------------------//
    //----- CGraphicsOutput -----//
    //---------------------------//
    ProxyGraphicsItemPtr (CGraphicsProcessOutput::*addPoint1)(const CPointF&) = &CGraphicsProcessOutput::addPoint;
    ProxyGraphicsItemPtr (CGraphicsProcessOutput::*addPoint2)(const CPointF&, const GraphicsPointProperty&) = &CGraphicsProcessOutput::addPoint;
    ProxyGraphicsItemPtr (CGraphicsProcessOutput::*addRectangle1)(float, float, float, float) = &CGraphicsProcessOutput::addRectangle;
    ProxyGraphicsItemPtr (CGraphicsProcessOutput::*addRectangle2)(float, float, float, float, const GraphicsRectProperty&) = &CGraphicsProcessOutput::addRectangle;
    ProxyGraphicsItemPtr (CGraphicsProcessOutput::*addEllipse1)(float, float, float, float) = &CGraphicsProcessOutput::addEllipse;
    ProxyGraphicsItemPtr (CGraphicsProcessOutput::*addEllipse2)(float, float, float, float, const GraphicsEllipseProperty&) = &CGraphicsProcessOutput::addEllipse;
    ProxyGraphicsItemPtr (CGraphicsProcessOutput::*addPolygon1)(const std::vector<CPointF>&) = &CGraphicsProcessOutput::addPolygon;
    ProxyGraphicsItemPtr (CGraphicsProcessOutput::*addPolygon2)(const std::vector<CPointF>&, const GraphicsPolygonProperty&) = &CGraphicsProcessOutput::addPolygon;
    ProxyGraphicsItemPtr (CGraphicsProcessOutput::*addPolyline1)(const std::vector<CPointF>&) = &CGraphicsProcessOutput::addPolyline;
    ProxyGraphicsItemPtr (CGraphicsProcessOutput::*addPolyline2)(const std::vector<CPointF>&, const GraphicsPolylineProperty&) = &CGraphicsProcessOutput::addPolyline;
    ProxyGraphicsItemPtr (CGraphicsProcessOutput::*addComplexPolygon1)(const PolygonF&, const std::vector<PolygonF>&) = &CGraphicsProcessOutput::addComplexPolygon;
    ProxyGraphicsItemPtr (CGraphicsProcessOutput::*addComplexPolygon2)(const PolygonF&, const std::vector<PolygonF>&, const GraphicsPolygonProperty&) = &CGraphicsProcessOutput::addComplexPolygon;
    ProxyGraphicsItemPtr (CGraphicsProcessOutput::*addText1)(const std::string&, float x, float y) = &CGraphicsProcessOutput::addText;
    ProxyGraphicsItemPtr (CGraphicsProcessOutput::*addText2)(const std::string&, float x, float y, const GraphicsTextProperty&) = &CGraphicsProcessOutput::addText;

    class_<CGraphicsProcessOutput, bases<CProtocolTaskIO>, std::shared_ptr<CGraphicsProcessOutput>>("CGraphicsOutput", _graphicsOutputDocString)
        .def(init<>("Default constructor"))
        .def("setNewLayer", &CGraphicsProcessOutput::setNewLayer, _setNewLayerDocString, args("self", "name"))
        .def("setImageIndex", &CGraphicsProcessOutput::setImageIndex, _setImageIndexDocString, args("self", "index"))
        .def("addItem", &CGraphicsProcessOutput::addItem, _addItemDocString, args("self", "item"))
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

    //---------------------------//
    //----- CImageProcessIO -----//
    //---------------------------//
    class_<CImageProcessIOWrap, bases<CProtocolTaskIO>, std::shared_ptr<CImageProcessIOWrap>>("CImageProcessIO", _imageProcessIODocString)
        .def(init<>("Default constructor"))
        .def(init<const CMat&>(_ctor1imageProcessIODocString))
        .def(init<IODataType>(_ctor2imageProcessIODocString))
        .def(init<IODataType, const CMat&>(_ctor3imageProcessIODocString))
        .def(init<const CImageProcessIO&>("Copy constructor"))
        .def("setImage", &CImageProcessIO::setImage, _setImageDocString, args("self", "image"))
        .def("setOverlayMask", &CImageProcessIO::setOverlayMask, _setOverlayMaskDocString, args("self", "mask"))
        .def("setChannelCount", &CImageProcessIO::setChannelCount, _setChannelCountDocString, args("self", "nb"))
        .def("setCurrentImage", &CImageProcessIO::setCurrentImage, _setCurrentImageDocString, args("self", "index"))
        .def("getChannelCount", &CImageProcessIO::getChannelCount, _getChannelCountDocString, args("self"))
        .def("getData", &CImageProcessIO::getData, _getDataDocString, args("self"))
        .def("getImage", &CImageProcessIO::getImage, &CImageProcessIOWrap::default_getImage, _getImageDocString, args("self"))
        .def("getOverlayMask", &CImageProcessIO::getOverlayMask, _getOverlayMaskDocString, args("self"))
        .def("getUnitElementCount", &CImageProcessIO::getUnitElementCount, &CImageProcessIOWrap::default_getUnitElementCount, _getImageUnitElementCountDocString, args("self"))
        .def("isDataAvailable", &CImageProcessIO::isDataAvailable, &CImageProcessIOWrap::default_isDataAvailable, _isImageDataAvailableDocString, args("self"))
        .def("isOverlayAvailable", &CImageProcessIO::isOverlayAvailable, _isOverlayAvailableDocString, args("self"))
        .def("clearData", &CImageProcessIO::clearData, &CImageProcessIOWrap::default_clearData, _clearImageDataDocString, args("self"))
        .def("copyStaticData", &CImageProcessIO::copyStaticData, &CImageProcessIOWrap::default_copyStaticData, _copyImageStaticDataDocString, args("self", "io"))
    ;

    //-----------------------------//
    //----- CFeatureProcessIO -----//
    //-----------------------------//
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

    //---------------------------//
    //----- CVideoProcessIO -----//
    //---------------------------//
    class_<CVideoProcessIOWrap, bases<CImageProcessIO>, std::shared_ptr<CVideoProcessIOWrap>>("CVideoProcessIO", _videoProcessIODocString)
        .def(init<>("Default constructor"))
        .def(init<const CMat&>(_ctor1VideoProcessIODocString))
        .def(init<IODataType>(_ctor2VideoProcessIODocString))
        .def(init<IODataType, const CMat&>(_ctor3VideoProcessIODocString))
        .def(init<const CVideoProcessIO&>("Copy constructor"))
        .def("setVideoPath", &CVideoProcessIO::setVideoPath, _setVideoPathDocString, args("self", "path"))
        .def("setVideoPos", &CVideoProcessIO::setVideoPos, _setVideoPosDocString, args("self", "position"))
        .def("getVideoFrameCount", &CVideoProcessIO::getVideoFrameCount, _getVideoFrameCountDocString, args("self"))
        .def("getVideoImages", &CVideoProcessIO::getVideoImages, _getVideoImagesDocString, args("self"))
        .def("getVideoPath", &CVideoProcessIO::getVideoPath, _getVideoPathDocString, args("self"))
        .def("getSnapshot", &CVideoProcessIO::getSnapshot, _getSnapshotDocString, args("self", "position"))
        .def("getCurrentPos", &CVideoProcessIO::getCurrentPos, _getCurrentPosDocString, args("self"))
        .def("startVideo", &CVideoProcessIO::startVideo, _startVideoDocString, args("self"))
        .def("stopVideo", &CVideoProcessIO::stopVideo, _stopVideoDocString, args("self"))
        .def("startVideoWrite", &CVideoProcessIO::startVideoWrite, _startVideoWriteDocString, args("self"))
        .def("stopVideoWrite", &CVideoProcessIO::stopVideoWrite, _stopVideoWriteDocString, args("self"))
        .def("addVideoImage", &CVideoProcessIO::addVideoImage, _addVideoImageDocString, args("self", "image"))
        .def("writeImage", &CVideoProcessIO::writeImage, _writeImageDocString, args("self", "image"))
        .def("hasVideo", &CVideoProcessIO::hasVideo, _hasVideoDocString, args("self"))
        .def("getImage", &CVideoProcessIO::getImage, &CVideoProcessIOWrap::default_getImage, _getVideoImageDocString, args("self"))
        .def("getUnitElementCount", &CVideoProcessIO::getUnitElementCount, &CVideoProcessIOWrap::default_getUnitElementCount, _getVideoUnitElementCountDocString, args("self"))
        .def("isDataAvailable", &CVideoProcessIO::isDataAvailable, &CVideoProcessIOWrap::default_isDataAvailable, _isVideoDataAvailableDocString, args("self"))
        .def("clearData", &CVideoProcessIO::clearData, &CVideoProcessIOWrap::default_clearData, _clearVideoDataDocString, args("self"))
        .def("copyStaticData", &CVideoProcessIO::copyStaticData, &CVideoProcessIOWrap::default_copyStaticData, _copyStaticDataDerivedDocString, args("self"))
    ;

    //-------------------------//
    //----- CWidgetOutput -----//
    //-------------------------//
    class_<CWidgetOutputWrap, bases<CProtocolTaskIO>, std::shared_ptr<CWidgetOutputWrap>>("CWidgetOutput", _widgetOutputDocString)
        .def(init<>("Default constructor"))
        .def(init<IODataType>(_ctorWidgetOutputDocString))
        .def("setWidget", &CWidgetOutputWrap::setWidget, _setWidgetDocString, args("self", "widget"))
        .def("isDataAvailable", &CWidgetOutput::isDataAvailable, &CWidgetOutputWrap::default_isDataAvailable, _isWidgetDataAvailableDocString, args("self"))
        .def("clearData", &CWidgetOutput::clearData, &CWidgetOutputWrap::default_clearData, _clearWidgetDataDocString, args("self"))
    ;

    //-------------------//
    //----- CPathIO -----//
    //-------------------//
    class_<CPathIOWrap, bases<CProtocolTaskIO>, std::shared_ptr<CPathIOWrap>>("CPathIO", _pathIODocString)
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

    class_<CDatasetIOWrap, bases<CProtocolTaskIO>, std::shared_ptr<CDatasetIOWrap>, boost::noncopyable>("CDatasetIO", _datasetIODocString)
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
    class_<CArrayIOWrap, bases<CProtocolTaskIO>, std::shared_ptr<CArrayIOWrap>>("CArrayIO", _arrayIODocString)
        .def(init<>("Default constructor"))
        .def(init<const CMat&>(_ctor1ArrayIODocString))
        .def(init<const CArrayIO&>("Copy constructor"))
        .def("setArray", &CArrayIO::setArray, _setArrayDocString, args("self", "array"))
        .def("getArray", &CArrayIO::getArray, _getArrayDocString, args("self"))
        .def("getUnitElementCount", &CArrayIO::getUnitElementCount, &CArrayIOWrap::default_getUnitElementCount, _getArrayUnitElementCountDocString, args("self"))
        .def("isDataAvailable", &CArrayIO::isDataAvailable, &CArrayIOWrap::default_isDataAvailable, _isArrayDataAvailableDocString, args("self"))
        .def("clearData", &CArrayIO::clearData, &CArrayIOWrap::default_clearData, _clearArrayDataDocString, args("self"))
    ;

    //---------------------------//
    //----- CImageProcess2d -----//
    //---------------------------//
    //Overload member functions
    size_t (CImageProcess2d::*getProgressSteps1)() = &CImageProcess2d::getProgressSteps;
    size_t (CImageProcess2d::*getProgressSteps2)(size_t) = &CImageProcess2d::getProgressSteps;
    size_t (CImageProcess2dWrap::*default_getProgressSteps1)() = &CImageProcess2dWrap::default_getProgressSteps;
    size_t (CImageProcess2dWrap::*default_getProgressSteps2)(size_t) = &CImageProcess2dWrap::default_getProgressSteps;

    class_<CImageProcess2dWrap, bases<CProtocolTask>, std::shared_ptr<CImageProcess2dWrap>>("CImageProcess2d", _imageProcess2dDocString)
        .def(init<>("Default constructor"))
        .def(init<bool>(_ctor1ImageProcess2dDocString))
        .def(init<const std::string&>(_ctor2ImageProcess2dDocString))
        .def(init<const std::string&, bool>(_ctor3ImageProcess2dDocString))
        .def("setActive", &CImageProcess2d::setActive, &CImageProcess2dWrap::default_setActive, _setActiveDocString, args("self", "is_active"))
        .def("setOutputColorMap", &CImageProcess2dWrap::setOutputColorMap, _setOutputColorMapDocString, args("self", "index", "mask_index", "colors"))
        .def("updateStaticOutputs", &CImageProcess2d::updateStaticOutputs, &CImageProcess2dWrap::default_updateStaticOutputs, _updateStaticOutputsDocString, args("self"))
        .def("beginTaskRun", &CImageProcess2d::beginTaskRun, &CImageProcess2dWrap::default_beginTaskRun, _beginTaskRunDocString, args("self"))
        .def("endTaskRun", &CImageProcess2d::endTaskRun, &CImageProcess2dWrap::default_endTaskRun, _endTaskRunDocString, args("self"))
        .def("graphicsChanged", &CImageProcess2d::graphicsChanged, &CImageProcess2dWrap::default_graphicsChanged, _graphicsChangedDocString, args("self"))
        .def("globalInputChanged", &CImageProcess2d::globalInputChanged, &CImageProcess2dWrap::default_globalInputChanged, _globalInputChangedDocString, args("self", "is_new_sequence"))
        .def("createGraphicsMask", &CImageProcess2d::createGraphicsMask, _createGraphicsMaskDocString, args("self", "width", "height", "graphics"))
        .def("applyGraphicsMask", &CImageProcess2d::applyGraphicsMask, _applyGraphicsMaskDocString, args("self", "src", "dst", "index"))
        .def("applyGraphicsMaskToBinary", &CImageProcess2d::applyGraphicsMaskToBinary, _applyGraphicsMaskToBinaryDocString, args("self", "src", "dst", "index"))
        .def("getProgressSteps", getProgressSteps1, default_getProgressSteps1, _getProgressStepsDocString, args("self"))
        .def("getProgressSteps", getProgressSteps2, default_getProgressSteps2, _getProgressStepsDocString, args("self", "unit_elt_count"))
        .def("getGraphicsMask", &CImageProcess2d::getGraphicsMask, _getGraphicsMaskDocString, args("self", "index"))
        .def("isMaskAvailable", &CImageProcess2d::isMaskAvailable, _isMaskAvailableDocString, args("self", "index"))
        .def("run", &CImageProcess2d::run, &CImageProcess2dWrap::default_run, _runDocString, args("self"))
        .def("stop", &CImageProcess2d::stop, &CImageProcess2dWrap::default_stop, _stopDocString, args("self"))
        .def("forwardInputImage", &CImageProcess2d::forwardInputImage, _forwardInputImageDocString, args("self", "input_index", "output_index"))
        .def("emitAddSubProgressSteps", &CImageProcess2dWrap::emitAddSubProgressSteps, _emitAddSubProgressSteps, args("self", "count"))
        .def("emitStepProgress", &CImageProcess2dWrap::emitStepProgress, _emitStepProgressDocString, args("self"))
        .def("emitGraphicsContextChanged", &CImageProcess2dWrap::emitGraphicsContextChanged, _emitGraphicsContextChangedDocString, args("self"))
        .def("emitOutputChanged", &CImageProcess2dWrap::emitOutputChanged, _emitOutputChangedDocString, args("self"))
    ;

    //--------------------------------------//
    //----- CInteractiveImageProcess2d -----//
    //--------------------------------------//
    //Overload member functions
    size_t (CInteractiveImageProcess2d::*getProgressSteps3)() = &CInteractiveImageProcess2d::getProgressSteps;
    size_t (CInteractiveImageProcess2d::*getProgressSteps4)(size_t) = &CInteractiveImageProcess2d::getProgressSteps;
    size_t (CInteractiveImageProcess2dWrap::*default_getProgressSteps3)() = &CInteractiveImageProcess2dWrap::default_getProgressSteps;
    size_t (CInteractiveImageProcess2dWrap::*default_getProgressSteps4)(size_t) = &CInteractiveImageProcess2dWrap::default_getProgressSteps;

    class_<CInteractiveImageProcess2dWrap, bases<CImageProcess2d>, std::shared_ptr<CInteractiveImageProcess2dWrap>>("CInteractiveImageProcess2d", _interactiveImageProcess2d)
        .def(init<>("Default constructor"))
        .def(init<const std::string&>(_ctorInteractiveImageProcessDocString))
        .def("setActive", &CInteractiveImageProcess2d::setActive, &CInteractiveImageProcess2dWrap::default_setActive, _setActiveInteractiveDocString, args("self", "is_active"))
        .def("updateStaticOutputs", &CInteractiveImageProcess2d::updateStaticOutputs, &CInteractiveImageProcess2dWrap::default_updateStaticOutputs, _updateStaticOutputsDocString, args("self"))
        .def("beginTaskRun", &CInteractiveImageProcess2d::beginTaskRun, &CInteractiveImageProcess2dWrap::default_beginTaskRun, _beginTaskRunDocString, args("self"))
        .def("endTaskRun", &CInteractiveImageProcess2d::endTaskRun, &CInteractiveImageProcess2dWrap::default_endTaskRun, _endTaskRunDocString, args("self"))
        .def("graphicsChanged", &CInteractiveImageProcess2d::graphicsChanged, &CInteractiveImageProcess2dWrap::default_graphicsChanged, _graphicsChangedInteractiveDocString, args("self"))
        .def("globalInputChanged", &CInteractiveImageProcess2d::globalInputChanged, &CInteractiveImageProcess2dWrap::default_globalInputChanged, _globalInputChangedInteractiveDocString, args("self", "is_new_sequence"))
        .def("getProgressSteps", getProgressSteps3, default_getProgressSteps3, _getProgressStepsDocString, args("self"))
        .def("getProgressSteps", getProgressSteps4, default_getProgressSteps4, _getProgressStepsDocString, args("self", "unit_elt_count"))
        .def("getInteractionMask", &CInteractiveImageProcess2d::getInteractionMask, _getInteractionMaskDocString, args("self"))
        .def("getBlobs", &CInteractiveImageProcess2d::getBlobs, _getBlobsDocString, args("self"))
        .def("createInteractionMask", &CInteractiveImageProcess2d::createInteractionMask, _createInteractionMaskDocString, args("self", "width", "height"))
        .def("computeBlobs", &CInteractiveImageProcess2d::computeBlobs, _computeBlobsDocString, args("self"))
        .def("clearInteractionLayer", &CInteractiveImageProcess2d::clearInteractionLayer, _clearInteractionLayerDocString, args("self"))
        .def("run", &CInteractiveImageProcess2d::run, &CInteractiveImageProcess2dWrap::default_run, _runDocString, args("self"))
        .def("stop", &CInteractiveImageProcess2d::stop, &CInteractiveImageProcess2dWrap::default_stop, _stopDocString, args("self"))
        .def("emitAddSubProgressSteps", &CInteractiveImageProcess2dWrap::emitAddSubProgressSteps, _emitAddSubProgressSteps, args("self", "count"))
        .def("emitStepProgress", &CInteractiveImageProcess2dWrap::emitStepProgress, _emitStepProgressDocString, args("self"))
        .def("emitGraphicsContextChanged", &CInteractiveImageProcess2dWrap::emitGraphicsContextChanged, _emitGraphicsContextChangedDocString, args("self"))
        .def("emitOutputChanged", &CInteractiveImageProcess2dWrap::emitOutputChanged, _emitOutputChangedDocString, args("self"))
    ;

    //-------------------------//
    //----- CVideoProcess -----//
    //-------------------------//
    //Overload member functions
    size_t (CVideoProcess::*getProgressSteps5)() = &CVideoProcess::getProgressSteps;
    size_t (CVideoProcess::*getProgressSteps6)(size_t) = &CVideoProcess::getProgressSteps;
    size_t (CVideoProcessWrap::*default_getProgressSteps5)() = &CVideoProcessWrap::default_getProgressSteps;
    size_t (CVideoProcessWrap::*default_getProgressSteps6)(size_t) = &CVideoProcessWrap::default_getProgressSteps;

    class_<CVideoProcessWrap, bases<CImageProcess2d>, std::shared_ptr<CVideoProcessWrap>>("CVideoProcess", _videoProcessDocString)
        .def(init<>("Default constructor"))
        .def(init<const std::string&>(_ctorVideoProcessDocString))
        .def("setActive", &CVideoProcess::setActive, &CVideoProcessWrap::default_setActive, _setActiveDocString, args("self", "is_active"))
        .def("updateStaticOutputs", &CVideoProcess::updateStaticOutputs, &CVideoProcessWrap::default_updateStaticOutputs, _updateStaticOutputsDocString, args("self"))
        .def("beginTaskRun", &CVideoProcess::beginTaskRun, &CVideoProcessWrap::default_beginTaskRun, _beginTaskRunVideoDocString, args("self"))
        .def("endTaskRun", &CVideoProcess::endTaskRun, &CVideoProcessWrap::default_endTaskRun, _endTaskRunDocString, args("self"))
        .def("notifyVideoStart", &CVideoProcess::notifyVideoStart, &CVideoProcessWrap::default_notifyVideoStart, _notifyVideoStartDocString, args("self", "frame_count"))
        .def("notifyVideoEnd", &CVideoProcess::notifyVideoEnd, &CVideoProcessWrap::default_notifyVideoEnd, _notifyVideoEndDocString, args("self"))
        .def("graphicsChanged", &CVideoProcess::graphicsChanged, &CVideoProcessWrap::default_graphicsChanged, _graphicsChangedDocString, args("self"))
        .def("globalInputChanged", &CVideoProcess::globalInputChanged, &CVideoProcessWrap::default_globalInputChanged, _globalInputChangedDocString, args("self", "is_new_sequence"))
        .def("getProgressSteps", getProgressSteps5, default_getProgressSteps5, _getProgressStepsDocString, args("self"))
        .def("getProgressSteps", getProgressSteps6, default_getProgressSteps6, _getProcessFactoryDocString, args("self", "unit_elt_count"))
        .def("run", &CVideoProcess::run, &CVideoProcessWrap::default_run, _runDocString, args("self"))
        .def("stop", &CVideoProcess::stop, &CVideoProcessWrap::default_stop, _stopDocString, args("self"))
        .def("emitAddSubProgressSteps", &CVideoProcessWrap::emitAddSubProgressSteps, _emitAddSubProgressSteps, args("self", "count"))
        .def("emitStepProgress", &CVideoProcessWrap::emitStepProgress, _emitStepProgressDocString, args("self"))
        .def("emitGraphicsContextChanged", &CVideoProcessWrap::emitGraphicsContextChanged, _emitGraphicsContextChangedDocString, args("self"))
        .def("emitOutputChanged", &CVideoProcessWrap::emitOutputChanged, _emitOutputChangedDocString, args("self"))
    ;

    //---------------------------//
    //----- CVideoProcessOF -----//
    //---------------------------//
    //Overload member functions
    size_t (CVideoProcessOF::*getProgressSteps7)() = &CVideoProcessOF::getProgressSteps;
    size_t (CVideoProcessOF::*getProgressSteps8)(size_t) = &CVideoProcessOF::getProgressSteps;
    size_t (CVideoProcessOFWrap::*default_getProgressSteps7)() = &CVideoProcessOFWrap::default_getProgressSteps;
    size_t (CVideoProcessOFWrap::*default_getProgressSteps8)(size_t) = &CVideoProcessOFWrap::default_getProgressSteps;

    class_<CVideoProcessOFWrap, bases<CVideoProcess>, std::shared_ptr<CVideoProcessOFWrap>>("CVideoProcessOF", _videoProcessOFDocString)
        .def(init<>("Default constructor"))
        .def(init<const std::string&>(_ctorVideoProcessOFDocString))
        .def("setActive", &CVideoProcessOF::setActive, &CVideoProcessOFWrap::default_setActive, _setActiveDocString, args("self", "is_active"))
        .def("updateStaticOutputs", &CVideoProcessOF::updateStaticOutputs, &CVideoProcessOFWrap::default_updateStaticOutputs, _updateStaticOutputsDocString, args("self"))
        .def("beginTaskRun", &CVideoProcessOF::beginTaskRun, &CVideoProcessOFWrap::default_beginTaskRun, _beginTaskRunVideoOFDocString, args("self"))
        .def("endTaskRun", &CVideoProcessOF::endTaskRun, &CVideoProcessOFWrap::default_endTaskRun, _endTaskRunDocString, args("self"))
        .def("graphicsChanged", &CVideoProcessOF::graphicsChanged, &CVideoProcessOFWrap::default_graphicsChanged, _graphicsChangedDocString, args("self"))
        .def("globalInputChanged", &CVideoProcessOF::globalInputChanged, &CVideoProcessOFWrap::default_globalInputChanged, _globalInputChangedDocString, args("self" "is_new_sequence"))
        .def("getProgressSteps", getProgressSteps7, default_getProgressSteps7, _getProgressStepsDocString, args("self"))
        .def("getProgressSteps", getProgressSteps8, default_getProgressSteps8, _getProgressStepsDocString, args("self", "unit_elt_count"))
        .def("run", &CVideoProcessOF::run, &CVideoProcessOFWrap::default_run, _runDocString, args("self"))
        .def("stop", &CVideoProcessOF::stop, &CVideoProcessOFWrap::default_stop, _stopDocString, args("self"))
        .def("emitAddSubProgressSteps", &CVideoProcessOFWrap::emitAddSubProgressSteps, _emitAddSubProgressSteps, args("self", "count"))
        .def("emitStepProgress", &CVideoProcessOFWrap::emitStepProgress, _emitStepProgressDocString, args("self"))
        .def("emitGraphicsContextChanged", &CVideoProcessOFWrap::emitGraphicsContextChanged, _emitGraphicsContextChangedDocString, args("self"))
        .def("emitOutputChanged", &CVideoProcessOFWrap::emitOutputChanged, _emitOutputChangedDocString, args("self"))
        .def("drawOptFlowMap", &CVideoProcessOF::drawOptFlowMap, _drawOptFlowMapDocString, args("self", "flow", "vectors", "step"))
        .def("flowToDisplay", &CVideoProcessOF::flowToDisplay, _flowToDisplayDocString, args("self", "flow"))
    ;

    //---------------------------------//
    //----- CVideoProcessTracking -----//
    //---------------------------------//
    //Overload member functions
    size_t (CVideoProcessTracking::*getProgressSteps9)() = &CVideoProcessTracking::getProgressSteps;
    size_t (CVideoProcessTracking::*getProgressSteps10)(size_t) = &CVideoProcessTracking::getProgressSteps;
    size_t (CVideoProcessTrackingWrap::*default_getProgressSteps9)() = &CVideoProcessTrackingWrap::default_getProgressSteps;
    size_t (CVideoProcessTrackingWrap::*default_getProgressSteps10)(size_t) = &CVideoProcessTrackingWrap::default_getProgressSteps;

    class_<CVideoProcessTrackingWrap, bases<CVideoProcess>, std::shared_ptr<CVideoProcessTrackingWrap>>("CVideoProcessTracking", _videoProcessTrackingDocString)
        .def(init<>("Default constructor"))
        .def(init<const std::string&>(_ctorVideoTrackingDocString))
        .def("setActive", &CVideoProcessTracking::setActive, &CVideoProcessTrackingWrap::default_setActive, _setActiveDocString, args("self", "is_active"))
        .def("updateStaticOutputs", &CVideoProcessTracking::updateStaticOutputs, &CVideoProcessTrackingWrap::default_updateStaticOutputs, _updateStaticOutputsDocString, args("self"))
        .def("beginTaskRun", &CVideoProcessTracking::beginTaskRun, &CVideoProcessTrackingWrap::default_beginTaskRun, _beginTaskRunVideoOFDocString, args("self"))
        .def("endTaskRun", &CVideoProcessTracking::endTaskRun, &CVideoProcessTrackingWrap::default_endTaskRun, _endTaskRunDocString, args("self"))
        .def("graphicsChanged", &CVideoProcessTracking::graphicsChanged, &CVideoProcessTrackingWrap::default_graphicsChanged, _graphicsChangedDocString, args("self"))
        .def("globalInputChanged", &CVideoProcessTracking::globalInputChanged, &CVideoProcessTrackingWrap::default_globalInputChanged, _globalInputChangedDocString, args("self", "is_new_sequence"))
        .def("getProgressSteps", getProgressSteps9, default_getProgressSteps9, _getProgressStepsDocString, args("self"))
        .def("getProgressSteps", getProgressSteps10, default_getProgressSteps10, _getProgressStepsDocString, args("self" "unit_elt_count"))
        .def("run", &CVideoProcessTracking::run, &CVideoProcessTrackingWrap::default_run, _runDocString, args("self"))
        .def("stop", &CVideoProcessTracking::stop, &CVideoProcessTrackingWrap::default_stop, _stopDocString, args("self"))
        .def("emitAddSubProgressSteps", &CVideoProcessTrackingWrap::emitAddSubProgressSteps, _emitAddSubProgressSteps, args("self", "count"))
        .def("emitStepProgress", &CVideoProcessTrackingWrap::emitStepProgress, _emitStepProgressDocString, args("self"))
        .def("emitGraphicsContextChanged", &CVideoProcessTrackingWrap::emitGraphicsContextChanged, _emitGraphicsContextChangedDocString, args("self"))
        .def("emitOutputChanged", &CVideoProcessTrackingWrap::emitOutputChanged, _emitOutputChangedDocString, args("self"))
        .def("setRoiToTrack", &CVideoProcessTracking::setRoiToTrack, _setRoiToTrackDocString, args("self"))
        .def("manageOutputs", &CVideoProcessTracking::manageOutputs, _manageOutputsDocString, args("self"))
    ;

    //----------------------------//
    //----- CDnnTrainProcess -----//
    //----------------------------//
    //Overload member functions
    size_t (CDnnTrainProcess::*getProgressSteps11)() = &CDnnTrainProcess::getProgressSteps;
    size_t (CDnnTrainProcess::*getProgressSteps12)(size_t) = &CDnnTrainProcess::getProgressSteps;
    size_t (CDnnTrainProcessWrap::*default_getProgressSteps11)() = &CDnnTrainProcessWrap::default_getProgressSteps;
    size_t (CDnnTrainProcessWrap::*default_getProgressSteps12)(size_t) = &CDnnTrainProcessWrap::default_getProgressSteps;

    class_<CDnnTrainProcessWrap, bases<CProtocolTask>, std::shared_ptr<CDnnTrainProcessWrap>>("CDnnTrainProcess", _dnnTrainProcessDocString)
        .def(init<>("Default constructor"))
        .def(init<const std::string&>(_ctor1DnnTrainProcessDocString))
        .def(init<const std::string&, const std::shared_ptr<CDnnTrainProcessParam>&>(_ctor2DnnTrainProcessDocString))
        .def("setActive", &CDnnTrainProcess::setActive, &CDnnTrainProcessWrap::default_setActive, _setActiveDocString, args("self", "is_active"))
        .def("beginTaskRun", &CDnnTrainProcess::beginTaskRun, &CDnnTrainProcessWrap::default_beginTaskRun, _beginTaskRunDocString, args("self"))
        .def("endTaskRun", &CDnnTrainProcess::endTaskRun, &CDnnTrainProcessWrap::default_endTaskRun, _endTaskRunDocString, args("self"))
        .def("getTensorboardLogDir", &CDnnTrainProcess::getTensorboardLogDir, _getTensorboardLogDirDocString, args("self"))
        .def("getProgressSteps", getProgressSteps11, default_getProgressSteps11, _getProgressStepsDocString, args("self"))
        .def("getProgressSteps", getProgressSteps12, default_getProgressSteps12, _getProgressStepsDocString, args("self", "unit_elt_count"))
        .def("run", &CDnnTrainProcess::run, &CDnnTrainProcessWrap::default_run, _runDocString, args("self"))
        .def("stop", &CDnnTrainProcess::stop, &CDnnTrainProcessWrap::default_stop, _stopDocString, args("self"))
        .def("emitAddSubProgressSteps", &CDnnTrainProcessWrap::emitAddSubProgressSteps, _emitAddSubProgressSteps, args("self", "count"))
        .def("emitStepProgress", &CDnnTrainProcessWrap::emitStepProgress, _emitStepProgressDocString, args("self"))
        .def("emitOutputChanged", &CDnnTrainProcessWrap::emitOutputChanged, _emitOutputChangedDocString, args("self"))
        .def("enableMlflow", &CDnnTrainProcess::enableMlflow, _enableMlflowDocString, args("self", "enable"))
        .def("enableTensorboard", &CDnnTrainProcess::enableTensorboard, _enableTensorboardDocString, args("self", "enable"))
    ;

    //---------------------------------//
    //----- CDnnTrainProcessParam -----//
    //---------------------------------//
    class_<CDnnTrainProcessParamWrap, bases<CProtocolTaskParam>, std::shared_ptr<CDnnTrainProcessParamWrap>>("CDnnTrainProcessParam", _dnnTrainProcessParamDocString)
        .enable_pickling()
        .def(init<>("Default constructor"))
        .def("__copy__", &generic_copy<CDnnTrainProcessParamWrap>)
        .def("__deepcopy__", &generic_deepcopy<CDnnTrainProcessParamWrap>)
        .def("setParamMap", &CDnnTrainProcessParam::setParamMap, &CDnnTrainProcessParamWrap::default_setParamMap, _setParamMapDocString, args("self", "params"))
        .def("getParamMap", &CDnnTrainProcessParam::getParamMap, &CDnnTrainProcessParamWrap::default_getParamMap, _getParamMapDocString, args("self"))
        .def("getHashValue", &CDnnTrainProcessParam::getHashValue, &CDnnTrainProcessParamWrap::default_getHashValue, _getHashValueDocString, args("self"))
    ;
}
