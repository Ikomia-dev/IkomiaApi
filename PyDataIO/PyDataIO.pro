include(../IkomiaApi.pri)

QT += core gui widgets sql concurrent
CONFIG += plugin no_plugin_name_prefix
TARGET = pydataio
TEMPLATE = lib
VERSION = $$IKOMIA_LIB_VERSION
win32:QMAKE_EXTENSION_SHLIB = pyd
macx:QMAKE_EXTENSION_SHLIB = so

DEFINES += PYDATAIO_LIBRARY BOOST_ALL_NO_LIB

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        ../Core/Data/CvMatNumpyArrayConverter.cpp \
        PyDataIO.cpp

HEADERS += \
        ../Core/Data/CvMatNumpyArrayConverter.h \
        PyDataIO.h \
        PyDataIOGlobal.hpp

#Make install directive
target.path = ../Build/Lib/Python/ikomia/dataio
INSTALLS += target

LIBS += $$link_python()

pyDataIOFiles.path = ../Build/Lib/Python/ikomia/dataio
pyDataIOFiles.files += ../Python/dataio/__init__.py
INSTALLS += pyDataIOFiles

LIBS += $$link_boost()

# Opencv
win32:CONFIG(release, debug|release): LIBS += -lopencv_core$${OPENCV_VERSION} -lopencv_imgcodecs$${OPENCV_VERSION}
else:win32:CONFIG(debug, debug|release): LIBS += -lopencv_core$${OPENCV_VERSION}d -lopencv_imgcodecs$${OPENCV_VERSION}d
unix:LIBS += -lopencv_core -lopencv_imgcodecs

LIBS += $$link_utils()
INCLUDEPATH += $$PWD/../Utils
DEPENDPATH += $$PWD/../Utils

LIBS += $$link_core()
INCLUDEPATH += $$PWD/../Core
DEPENDPATH += $$PWD/../Core

LIBS += $$link_dataio()
INCLUDEPATH += $$PWD/../DataIO
DEPENDPATH += $$PWD/../DataIO
