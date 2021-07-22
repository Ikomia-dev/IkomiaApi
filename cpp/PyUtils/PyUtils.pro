#-------------------------------------------------
#
# Project created by QtCreator 2018-02-23T12:03:25
#
#-------------------------------------------------
include(../IkomiaApi.pri)

QT += core sql widgets
CONFIG += plugin no_plugin_name_prefix
TARGET = pyutils
TEMPLATE = lib
VERSION = $$(IKOMIA_LIB_VERSION)
win32:QMAKE_EXTENSION_SHLIB = pyd
macx:QMAKE_EXTENSION_SHLIB = so

DEFINES += PYUTILS_LIBRARY BOOST_ALL_NO_LIB

SOURCES += \
        PyUtils.cpp

HEADERS += \
        PyUtils.h \
        PyUtilsDocString.hpp \
        PyUtilsGlobal.h

#Make install directive
target.path = ../../ikomia/utils
INSTALLS += target

LIBS += $$link_python()

#Dynamic link to OpenCV
win32:CONFIG(release, debug|release):LIBS += -lopencv_core$${OPENCV_VERSION}
win32:CONFIG(debug, debug|release):LIBS += -lopencv_core$${OPENCV_VERSION}d
unix:LIBS += -lopencv_core

LIBS += $$link_boost()

LIBS += $$link_utils()

INCLUDEPATH += $$PWD/../Utils
DEPENDPATH += $$PWD/../Utils
