#-------------------------------------------------
#
# Project created by QtCreator 2018-02-27T11:45:29
#
#-------------------------------------------------
include(../IkomiaApi.pri)

QT += core gui widgets sql concurrent
CONFIG += plugin no_plugin_name_prefix
TARGET = pydataprocess
TEMPLATE = lib
VERSION = $$IKOMIA_LIB_VERSION
win32:QMAKE_EXTENSION_SHLIB = pyd
macx:QMAKE_EXTENSION_SHLIB = so

win32:QMAKE_CXXFLAGS += -bigobj

DEFINES += PYDATAPROCESS_LIBRARY BOOST_ALL_NO_LIB

SOURCES += \
        ../Core/Data/CvMatNumpyArrayConverter.cpp \
        CArrayIOWrap.cpp \
        CDatasetIOWrap.cpp \
        CDnnTrainTaskWrap.cpp \
        CGraphicsInputWrap.cpp \
        CImageIOWrap.cpp \
        CPathIOWrap.cpp \
        CVideoIOWrap.cpp \
        CWidgetOutputWrap.cpp \
        CWorkflowWrap.cpp \
        PyDataProcess.cpp \
        CTaskFactoryWrap.cpp \
        CWidgetFactoryWrap.cpp \
        CPluginProcessInterfaceWrap.cpp \
        C2dImageTaskWrap.cpp \
        C2dImageInteractiveTaskWrap.cpp \
        CVideoTaskWrap.cpp \
        CVideoOFTaskWrap.cpp \
        CVideoTrackingTaskWrap.cpp

HEADERS += \
        ../Core/Data/CvMatNumpyArrayConverter.h \
        CArrayIOWrap.h \
        CDatasetIOWrap.h \
        CDnnTrainTaskWrap.h \
        CFeatureIOWrap.hpp \
        CGraphicsInputWrap.h \
        CImageIOWrap.h \
        CPathIOWrap.h \
        CVideoIOWrap.h \
        CWidgetOutputWrap.h \
        CWorkflowWrap.h \
        PyDataProcess.h \
        PyDataProcessDocString.hpp \
        PyDataProcessGlobal.h \
        PyDataProcessTools.hpp \
        CTaskFactoryWrap.h \
        CWidgetFactoryWrap.h \
        CPluginProcessInterfaceWrap.h \
        C2dImageTaskWrap.h \
        C2dImageInteractiveTaskWrap.h \
        CVideoTaskWrap.h \
        CVideoOFTaskWrap.h \
        CVideoTrackingTaskWrap.h

#Make install directive
target.path = ../Build/Lib
INSTALLS += target

unix {
symlink.path = ../Build/Lib
symlink.extra = ln -sf $$PWD/../Build/Lib/pydataprocess.so $$PWD/../../ikomia/dataprocess
INSTALLS += symlink
}

LIBS += $$link_python()

LIBS += $$link_boost()

# Dynamic link to OpenCV
win32:CONFIG(release, debug|release):LIBS += -lopencv_core$${OPENCV_VERSION} -lopencv_imgproc$${OPENCV_VERSION} -lopencv_objdetect$${OPENCV_VERSION} -lopencv_photo$${OPENCV_VERSION} -lopencv_ximgproc$${OPENCV_VERSION} -lopencv_highgui$${OPENCV_VERSION}
win32:CONFIG(release, debug|release):LIBS += -lopencv_xphoto$${OPENCV_VERSION} -lopencv_fuzzy$${OPENCV_VERSION} -lopencv_hfs$${OPENCV_VERSION} -lopencv_dnn$${OPENCV_VERSION}
win32:CONFIG(debug, debug|release):LIBS += -lopencv_core$${OPENCV_VERSION}d -lopencv_imgproc$${OPENCV_VERSION}d -lopencv_objdetect$${OPENCV_VERSION}d -lopencv_photo$${OPENCV_VERSION}d -lopencv_ximgproc$${OPENCV_VERSION}d -lopencv_highgui$${OPENCV_VERSION}d
win32:CONFIG(debug, debug|release):LIBS += -lopencv_xphoto$${OPENCV_VERSION}d -lopencv_fuzzy$${OPENCV_VERSION}d -lopencv_hfs$${OPENCV_VERSION}d -lopencv_dnn$${OPENCV_VERSION}d
unix: LIBS += -lopencv_core -lopencv_imgproc -lopencv_objdetect -lopencv_photo -lopencv_ximgproc -lopencv_highgui
unix: LIBS += -lopencv_xphoto -lopencv_fuzzy -lopencv_hfs -lopencv_dnn

LIBS += $$link_utils()
INCLUDEPATH += $$PWD/../Utils
DEPENDPATH += $$PWD/../Utils

LIBS += $$link_core()
INCLUDEPATH += $$PWD/../Core
DEPENDPATH += $$PWD/../Core

LIBS += $$link_dataio()
INCLUDEPATH += $$PWD/../DataIO
DEPENDPATH += $$PWD/../DataIO

LIBS += $$link_dataprocess()
INCLUDEPATH += $$PWD/../DataProcess
DEPENDPATH += $$PWD/../DataProcess
