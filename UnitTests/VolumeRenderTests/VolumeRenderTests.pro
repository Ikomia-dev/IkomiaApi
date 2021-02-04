#-------------------------------------------------
#
# Project created by QtCreator 2017-09-21T11:51:12
#
#-------------------------------------------------
include(../../IkomiaApi.pri)

QT       += testlib core widgets sql concurrent

QT       -= gui

TARGET = volumeRenderTests
CONFIG   -= app_bundle

TEMPLATE = app

DEFINES += SRCDIR=\\\"$$PWD/\\\" BOOST_ALL_NO_LIB

unix {
    target.path = ../../Build/Tests/
    INSTALLS += target
    QMAKE_RPATHDIR += $$PWD/../../Build/Lib
    QMAKE_RPATHDIR += /usr/local/lib
}

SOURCES += \
    COpenCLTests.cpp

HEADERS += \
    COpenCLTests.h \
    kernels.hpp

#Dynamic link to OpenCV
win32:CONFIG(release, debug|release):LIBS += -lopencv_core$${OPENCV_VERSION} -lopencv_imgcodecs$${OPENCV_VERSION} -lopencv_highgui$${OPENCV_VERSION} -lopencv_imgproc$${OPENCV_VERSION}
win32:CONFIG(debug, debug|release):LIBS += -lopencv_core$${OPENCV_VERSION}d -lopencv_imgcodecs$${OPENCV_VERSION}d -lopencv_highgui$${OPENCV_VERSION}d -lopencv_imgproc$${OPENCV_VERSION}d
unix: LIBS += -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc

LIBS += $$link_boost()

#OpenGL
win32: LIBS += -lOpenGL32

#Dynamic link to OpenCL
win32: LIBS += -lOpenCL
unix:!macx: LIBS += -lOpenCL
macx: LIBS += -framework OpenCL

#Dynamic link to Utils
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../Utils/release/ -likUtils
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../Utils/debug/ -likUtils
else:unix: LIBS += -L$$OUT_PWD/../../Utils/ -likUtils
INCLUDEPATH += $$PWD/../../Utils
DEPENDPATH += $$PWD/../../Utils

#Dynamic link to Core
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../Core/release/ -likCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../Core/debug/ -likCore
else:unix: LIBS += -L$$OUT_PWD/../../Core/ -likCore
INCLUDEPATH += $$PWD/../../Core
DEPENDPATH += $$PWD/../../Core

#Dynamic link to VolumeRender
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../VolumeRender/release/ -likVolumeRender
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../VolumeRender/debug/ -likVolumeRender
else:unix: LIBS += -L$$OUT_PWD/../../VolumeRender/ -likVolumeRender
INCLUDEPATH += $$PWD/../../VolumeRender
DEPENDPATH += $$PWD/../../VolumeRender
