#-------------------------------------------------
#
# Project created by QtCreator 2018-02-26T10:27:55
#
#-------------------------------------------------
include(../IkomiaApi.pri)

QT       += core widgets sql

TARGET = ikDataPlot
TEMPLATE = lib
VERSION = $$(IKOMIA_LIB_VERSION)

DEFINES += DATAPLOT_LIBRARY QWT_DLL BOOST_ALL_NO_LIB

SOURCES += \
    CDataPlot.cpp \
    CPlot.cpp \
    CPieMarker.cpp \
    CPiePlot.cpp \
    CMultiBarPlot.cpp \
    CSimpleBarPlot.cpp \
    CHistogramPlot.cpp \
    CCurvePlot.cpp

HEADERS += \
    CDataPlot.h \
    CPlot.h \
    CPieMarker.h \
    CPiePlot.h \
    CMultiBarPlot.h \
    CSimpleBarPlot.h \
    CHistogramPlot.h \
    CCurvePlot.h \
    DataPlotGlobal.h

#Use of 'lupdate DataPlot.pro' to update translation files
#Use of 'lrelease DataPlot.pro' to generate .qm files
TRANSLATIONS = Translations/dataplot_fr.ts

#Make install directive
target.path = ../Build/Lib/Cpp
INSTALLS += target

#Dynamic link with OpenCV
win32:CONFIG(release, debug|release):LIBS += -lopencv_core$${OPENCV_VERSION}
win32:CONFIG(debug, debug|release):LIBS += -lopencv_core$${OPENCV_VERSION}d
unix:LIBS += -lopencv_core

LIBS += $$link_boost()

LIBS += $$link_qwt()
INCLUDEPATH += $$IKOMIA_EXT_LIB_PATH/qwt/src
DEPENDPATH += $$IKOMIA_EXT_LIB_PATH/qwt/build/lib

LIBS += $$link_utils()
INCLUDEPATH += $$PWD/../Utils
DEPENDPATH += $$PWD/../Utils
