#-------------------------------------------------
#
# Project created by QtCreator 2017-09-20T15:59:15
#
#-------------------------------------------------
include(../IkomiaApi.pri)

QT       += core widgets sql concurrent opengl

TARGET = ikVolumeRender
TEMPLATE = lib
VERSION = $$IKOMIA_LIB_VERSION

DEFINES += VOLUMERENDER_LIBRARY BOOST_ALL_NO_LIB

SOURCES += \
        VolumeRender.cpp \
        CSphereRender.cpp \
        CCylinderRender.cpp \
        CAxisRender.cpp \
        CTextureRender.cpp

HEADERS += \
        VolumeRender.h \
        ocl.hpp \
        Kernels/raycastGray.h \
        Kernels/raycastGrayF2B.h \
        Kernels/raycastGrayIso.h \
        Kernels/raycastGrayMinIP.h \
        Kernels/raycastGrayMIP.h \
        Kernels/raycastGrayTools.h \
        VolumeRenderGlobal.h \
        CSphereRender.h \
        CCylinderRender.h \
        CAxisRender.h \
        VolumeRenderDefine.h \
        CTextureRender.h \
        renderUtils.hpp \
        Kernels/raycastColor.h \
        Kernels/raycastColorF2B.h \
        Kernels/raycastColorIso.h \
        Kernels/raycastColorMinIP.h \
        Kernels/raycastColorMIP.h \
        Kernels/raycastColorTools.h

#Use of 'lupdate VolumeRender.pro' to update translation files
#Use of 'lrelease VolumeRender.pro' to generate .qm files
TRANSLATIONS = Translations/volumerender_fr.ts

RESOURCES += \
    VolumeRender.qrc

#Make install directive
target.path = ../Build/Lib/Cpp
INSTALLS += target

#Dynamic link to OpenCV
win32:CONFIG(release, debug|release):LIBS += -lopencv_core$${OPENCV_VERSION} -lopencv_highgui$${OPENCV_VERSION} -lopencv_imgproc$${OPENCV_VERSION}
win32:CONFIG(debug, debug|release)LIBS += -lopencv_core$${OPENCV_VERSION}d -lopencv_highgui$${OPENCV_VERSION}d -lopencv_imgproc$${OPENCV_VERSION}d
unix: LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc

LIBS += $$link_boost()

#OpenGL
win32: LIBS += -lOpenGL32

#Dynamic link to OpenCL
win32: LIBS += -lOpenCL
unix:!macx: LIBS += -lOpenCL
macx: LIBS += -framework OpenCL
macx: INCLUDEPATH += $$PWD/../../..

win32:INCLUDEPATH += $$PWD/'../../../../Program Files (x86)/AMD APP SDK/3.0/lib/x86_64'
win32:DEPENDPATH += $$PWD/'../../../../Program Files (x86)/AMD APP SDK/3.0/lib/x86_64'

LIBS += $$link_utils()
INCLUDEPATH += $$PWD/../Utils
DEPENDPATH += $$PWD/../Utils

LIBS += $$link_core()
INCLUDEPATH += $$PWD/../Core
DEPENDPATH += $$PWD/../Core

DISTFILES += \
    Kernels/raycastColorF2B.cl \
    Kernels/raycastColorIso.cl \
    Kernels/raycastColorMinIP.cl \
    Kernels/raycastColorMIP.cl \
    Kernels/raycastColorTools.cl \
    Kernels/raycastColorF2B.cl \
    Kernels/raycastColorIso.cl \
    Kernels/raycastColorMinIP.cl \
    Kernels/raycastColorMIP.cl \
    Kernels/raycastColorTools.cl \
    Kernels/raycastColorF2B.cl \
    Kernels/raycastColorF2B.cl \
    Kernels/raycastColorF2B.cl \
    Kernels/raycastColorIso.cl \
    Kernels/raycastColorMinIP.cl \
    Kernels/raycastColorMIP.cl \
    Kernels/raycastColorTools.cl
