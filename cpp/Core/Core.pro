#-------------------------------------------------
#
# Project created by QtCreator 2017-06-29T10:09:20
#
#-------------------------------------------------
include(../IkomiaApi.pri)

QT      += core gui widgets sql concurrent

TARGET = ikCore
TEMPLATE = lib
VERSION = $$IKOMIA_LIB_VERSION
DEFINES += CORE_LIBRARY BOOST_ALL_NO_LIB

SOURCES += \
        Data/CDataArrayInfo.cpp \
        Data/CDataConversion.cpp \
        Data/CDataImageInfo.cpp \
        Data/CDataInfo.cpp \
        Data/CDatasetInfo.cpp \
        Data/CDataDicomInfo.cpp \
        Data/CDataVideoInfo.cpp \
        Data/CMeasure.cpp \
        Graphics/CGraphicsLayer.cpp \
        Graphics/CGraphicsJSON.cpp \
        Graphics/CGraphicsComplexPolygon.cpp \
        Graphics/CGraphicsEllipse.cpp \
        Graphics/CGraphicsPolygon.cpp \
        Graphics/CGraphicsPolyline.cpp \
        Graphics/CGraphicsRectangle.cpp \
        Graphics/CGraphicsText.cpp \
        Graphics/CGraphicsConversion.cpp \
        Graphics/CGraphicsPoint.cpp \
        Graphics/CGraphicsContext.cpp \
        Workflow/CViewPropertyIO.cpp \
        Workflow/CWorkflowTask.cpp \
        Workflow/CWorkflowTaskIO.cpp \
        Workflow/CWorkflowTaskParam.cpp \
        Workflow/CWorkflowTaskWidget.cpp

HEADERS += \
        Data/CDataArrayInfo.h \
        Data/CDataConversion.h \
        Data/CDataImageInfo.h \
        Data/CDataset.hpp \
        Data/CDataInfo.h \
        Data/CDatasetInfo.h \
        Data/CDataSubset.hpp \
        Data/CMat.hpp \
        Data/CMeasure.h \
        Data/CndArray.hpp \
        Data/CDataDicomInfo.h \
        Data/CDataVideoInfo.h \
        Graphics/CGraphicsItem.hpp \
        Graphics/CPoint.hpp \
        Graphics/GraphicsProperty.hpp \
        Graphics/CGraphicsLayer.h \
        Graphics/CGraphicsJSON.h \
        Graphics/CGraphicsComplexPolygon.h \
        Graphics/CGraphicsEllipse.h \
        Graphics/CGraphicsPolygon.h \
        Graphics/CGraphicsPolyline.h \
        Graphics/CGraphicsRectangle.h \
        Graphics/CGraphicsText.h \
        Graphics/CGraphicsPoint.h \
        Graphics/CGraphicsConversion.h \
        Graphics/CGraphicsContext.h \
        Workflow/CViewPropertyIO.h \
        Main/CoreDefine.hpp \
        Main/CoreTools.hpp \
        Main/forwards.hpp \
        Main/CoreGlobal.hpp \
        Workflow/CWorkflowEdge.hpp \
        Workflow/CWorkflowTask.h \
        Workflow/CWorkflowTaskIO.h \
        Workflow/CWorkflowTaskParam.h \
        Workflow/CWorkflowTaskWidget.h

#Use of 'lupdate Core.pro' to update translation files
#Use of 'lrelease Core.pro' to generate .qm files
TRANSLATIONS = Translations/core_fr.ts

#Make install directive
target.path = ../Build/Lib

deployIncludeData.path = ../Build/Include/Core/Data
deployIncludeData.files = \
        Data/CDataConversion.h \
        Data/CDataImageInfo.h \
        Data/CDataset.hpp \
        Data/CDataInfo.h \
        Data/CDatasetInfo.h \
        Data/CDataSubset.hpp \
        Data/CMat.hpp \
        Data/CndArray.hpp \
        Data/CMeasure.h \
        Data/CDataVideoInfo.h

deployIncludeGraphics.path = ../Build/Include/Core/Graphics
deployIncludeGraphics.files = \
        Graphics/CGraphicsLayer.h \
        Graphics/CGraphicsItem.hpp \
        Graphics/CGraphicsJSON.h \
        Graphics/CGraphicsEllipse.h \
        Graphics/CGraphicsPolygon.h \
        Graphics/CGraphicsComplexPolygon.h \
        Graphics/CGraphicsPolyline.h \
        Graphics/CGraphicsRectangle.h \
        Graphics/CGraphicsText.h \
        Graphics/CGraphicsPoint.h \
        Graphics/CGraphicsConversion.h \
        Graphics/CGraphicsContext.h \
        Graphics/CPoint.hpp \
        Graphics/GraphicsProperty.hpp

deployIncludeWorkflow.path = ../Build/Include/Core/Workflow
deployIncludeWorkflow.files = \
        Workflow/CWorkflowEdge.hpp \
        Workflow/CWorkflowTaskParam.h \
        Workflow/CWorkflowTaskWidget.h \
        Workflow/CWorkflowTask.h \
        Workflow/CWorkflowTaskIO.h \
        Workflow/CViewPropertyIO.h

deployIncludeMain.path = ../Build/Include/Core/Main
deployIncludeMain.files = \
        Main/CoreDefine.hpp \
        Main/CoreTools.hpp \
        Main/CoreGlobal.hpp

INSTALLS += target
INSTALLS += deployIncludeData
INSTALLS += deployIncludeGraphics
INSTALLS += deployIncludeWorkflow
INSTALLS += deployIncludeMain

LIBS += $$link_opencv()
unix: LIBS += -lavcodec

LIBS += $$link_boost()

LIBS += $$link_utils()
INCLUDEPATH += $$PWD/../Utils
DEPENDPATH += $$PWD/../Utils
