#-------------------------------------------------
#
# Project created by QtCreator 2018-02-22T17:23:01
#
#-------------------------------------------------
include(../IkomiaApi.pri)

QT += core gui widgets sql concurrent
CONFIG += plugin no_plugin_name_prefix
TARGET = pycore
TEMPLATE = lib
VERSION = $$IKOMIA_LIB_VERSION
win32:QMAKE_EXTENSION_SHLIB = pyd
macx:QMAKE_EXTENSION_SHLIB = so

win32:QMAKE_CXXFLAGS += -bigobj

DEFINES += PYCORE_LIBRARY BOOST_ALL_NO_LIB

SOURCES += \
        ../Core/Data/CvMatNumpyArrayConverter.cpp \
        CGraphicsItemWrap.cpp \
        PyCore.cpp \
        CProtocolTaskIOWrap.cpp \
        CProtocolTaskParamWrap.cpp \
        CProtocolTaskWrap.cpp \
        CProtocolTaskWidgetWrap.cpp

HEADERS += \
        ../Core/Data/CvMatNumpyArrayConverter.h \
        CGraphicsItemWrap.h \
        PyCore.h \
        PyCoreDocString.hpp \
        PyCoreGlobal.h \
        CProtocolTaskIOWrap.h \
        CProtocolTaskParamWrap.h \
        CProtocolTaskWrap.h \
        CProtocolTaskWidgetWrap.h \
        PyCoreTools.hpp

#macx:QMAKE_RPATHDIR += @executable_path/../Frameworks

#Make install directive
target.path = ../../ikomia/core
INSTALLS += target

#Python
win32: INCLUDEPATH += ../../../numpy/numpy/core/include

LIBS += $$link_python()

LIBS += $$link_boost()

#Dynamic link to OpenCV
win32:CONFIG(release, debug|release):LIBS += -lopencv_core$${OPENCV_VERSION} -lopencv_imgcodecs$${OPENCV_VERSION} -lopencv_highgui$${OPENCV_VERSION} -lopencv_imgproc$${OPENCV_VERSION}
win32:CONFIG(debug, debug|release):LIBS += -lopencv_core$${OPENCV_VERSION}d -lopencv_imgcodecs$${OPENCV_VERSION}d -lopencv_highgui$${OPENCV_VERSION}d -lopencv_imgproc$${OPENCV_VERSION}d
unix:LIBS += -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc

LIBS += $$link_utils()
INCLUDEPATH += $$PWD/../Utils
DEPENDPATH += $$PWD/../Utils

LIBS += $$link_core()
INCLUDEPATH += $$PWD/../Core
DEPENDPATH += $$PWD/../Core

