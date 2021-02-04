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
target.path = ../Build/Lib/Python/ikomia/utils
INSTALLS += target

pyMainFiles.path = ../Build/Lib/Python/ikomia
pyMainFiles.files += ../Python/__init__.py
INSTALLS += pyMainFiles

pyUtilsFiles.path = ../Build/Lib/Python/ikomia/utils
pyUtilsFiles.files += ../Python/utils/__init__.py
pyUtilsFiles.files += ../Python/utils/plugindeps.py
pyUtilsFiles.files += ../Python/utils/qtconversion.py
pyUtilsFiles.files += ../Python/utils/pyqtutils.py
pyUtilsFiles.files += ../Python/utils/data.py
INSTALLS += pyUtilsFiles

pyDnnFiles.path = ../Build/Lib/Python/ikomia/dnn
pyDnnFiles.files += ../Python/dnn/__init__.py
pyDnnFiles.files += ../Python/dnn/dnntrain.py
pyDnnFiles.files += ../Python/dnn/dataset.py
pyDnnFiles.files += ../Python/dnn/datasetio.py
INSTALLS += pyDnnFiles

pyDnnTorchFiles.path = ../Build/Lib/Python/ikomia/dnn/torch
pyDnnTorchFiles.files += ../Python/dnn/torch/utils.py
pyDnnTorchFiles.files += ../Python/dnn/torch/models.py
pyDnnTorchFiles.files += ../Python/dnn/torch/datasetmapper.py
INSTALLS += pyDnnTorchFiles

LIBS += $$link_python()

#Dynamic link to OpenCV
win32:CONFIG(release, debug|release):LIBS += -lopencv_core$${OPENCV_VERSION}
win32:CONFIG(debug, debug|release):LIBS += -lopencv_core$${OPENCV_VERSION}d
unix:LIBS += -lopencv_core

LIBS += $$link_boost()

LIBS += $$link_utils()

INCLUDEPATH += $$PWD/../Utils
DEPENDPATH += $$PWD/../Utils
