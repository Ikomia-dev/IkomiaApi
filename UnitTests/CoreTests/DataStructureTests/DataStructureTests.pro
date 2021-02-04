include(../../../IkomiaApi.pri)

QT += core gui widgets testlib sql concurrent

TARGET = DataStructureTests
CONFIG += console
CONFIG -= app_bundle

# For Jenkins
CONFIG += testcase

DEFINES += BOOST_ALL_NO_LIB

TEMPLATE = app

unix {
    target.path = ../../../Build/Tests/
    INSTALLS += target
    QMAKE_RPATHDIR += $$PWD/../../../Build/Lib
    QMAKE_RPATHDIR += /usr/local/lib
}

SOURCES += \
    CDataStructureTests.cpp

HEADERS += \
    CDataStructureTests.h

#Dynamic link OpenCV
win32:CONFIG(release, debug|release):LIBS += -lopencv_core$${OPENCV_VERSION}  -lopencv_highgui$${OPENCV_VERSION} -lopencv_imgcodecs$${OPENCV_VERSION} -lopencv_imgproc$${OPENCV_VERSION}
win32:CONFIG(debug, debug|release):LIBS += -lopencv_core$${OPENCV_VERSION}d  -lopencv_highgui$${OPENCV_VERSION}d -lopencv_imgcodecs$${OPENCV_VERSION}d -lopencv_imgproc$${OPENCV_VERSION}d
unix: LIBS += -lopencv_core -lopencv_imgcodecs -lopencv_imgproc -lopencv_highgui -lopencv_videoio

LIBS += $$link_boost()

#Dynamic link to VTK
win32:LIBS += -lvtkCommonCore-$${VTK_VERSION} -lvtkCommonDataModel-$${VTK_VERSION} -lvtkCommonExecutionModel-$${VTK_VERSION} -lvtkCommonMath-$${VTK_VERSION} -lvtkCommonSystem-$${VTK_VERSION} -lvtkCommonTransforms-$${VTK_VERSION}
win32:LIBS += -lvtkFiltersCore-$${VTK_VERSION} -lvtkFiltersGeneral-$${VTK_VERSION} -lvtkFiltersImaging-$${VTK_VERSION} -lvtkImagingColor-$${VTK_VERSION} -lvtkImagingCore-$${VTK_VERSION}
win32:LIBS += -lvtkImagingGeneral-$${VTK_VERSION} -lvtkImagingMath-$${VTK_VERSION} -lvtkImagingSources-$${VTK_VERSION} -lvtkInteractionImage-$${VTK_VERSION} -lvtkInteractionStyle-$${VTK_VERSION} -lvtkIOCore-$${VTK_VERSION} -lvtkIOImage-$${VTK_VERSION}
win32:LIBS += -lvtkRenderingContext2D-$${VTK_VERSION} -lvtkRenderingCore-$${VTK_VERSION} -lvtkRenderingImage-$${VTK_VERSION} -lvtkRenderingOpenGL2-$${VTK_VERSION} -lvtksys-$${VTK_VERSION} -lvtkViewsCore-$${VTK_VERSION}
win32:LIBS += -lvtkDICOM-$${VTK_VERSION}

unix:!macx:LIBS += -lvtkCommonCore-$${VTK_VERSION} -lvtkCommonDataModel-$${VTK_VERSION} -lvtkCommonExecutionModel-$${VTK_VERSION} -lvtkCommonMath-$${VTK_VERSION} -lvtkCommonSystem-$${VTK_VERSION} -lvtkCommonTransforms-$${VTK_VERSION}
unix:!macx:LIBS += -lvtkFiltersCore-$${VTK_VERSION} -lvtkFiltersGeneral-$${VTK_VERSION} -lvtkFiltersImaging-$${VTK_VERSION} -lvtkImagingColor-$${VTK_VERSION} -lvtkImagingCore-$${VTK_VERSION}
unix:!macx:LIBS += -lvtkImagingGeneral-$${VTK_VERSION} -lvtkImagingMath-$${VTK_VERSION} -lvtkImagingSources-$${VTK_VERSION} -lvtkInteractionImage-$${VTK_VERSION} -lvtkInteractionStyle-$${VTK_VERSION} -lvtkIOCore-$${VTK_VERSION} -lvtkIOImage-$${VTK_VERSION}
unix:!macx:LIBS += -lvtkRenderingContext2D-$${VTK_VERSION} -lvtkRenderingCore-$${VTK_VERSION} -lvtkRenderingImage-$${VTK_VERSION} -lvtkRenderingOpenGL2-$${VTK_VERSION} -lvtksys-$${VTK_VERSION} -lvtkViewsCore-$${VTK_VERSION}
unix:!macx:LIBS += -lvtkDICOM-$${VTK_VERSION}

macx:LIBS += -lvtkCommonCore-8.1 -lvtkCommonDataModel-8.1 -lvtkCommonExecutionModel-8.1 -lvtkCommonMath-8.1 -lvtkCommonSystem-8.1 -lvtkCommonTransforms-8.1
macx:LIBS += -lvtkImagingCore-8.1 -lvtkIOCore-8.1 -lvtkIOImage-8.1 -lvtksys-8.1

#Dynamic link to Utils
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../Utils/release/ -likUtils
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../Utils/debug/ -likUtils
else:unix: LIBS += -L$$OUT_PWD/../../../Utils/ -likUtils
INCLUDEPATH += $$PWD/../../../Utils
DEPENDPATH += $$PWD/../../../Utils

#Dynamic link to Core
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../Core/release/ -likCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../Core/debug/ -likCore
else:unix: LIBS += -L$$OUT_PWD/../../../Core/ -likCore
INCLUDEPATH += $$PWD/../../../Core
DEPENDPATH += $$PWD/../../../Core

#Dynamic link to DataIO
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../DataIO/release/ -likDataIO
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../DataIO/debug/ -likDataIO
else:unix: LIBS += -L$$OUT_PWD/../../../DataIO/ -likDataIO
INCLUDEPATH += $$PWD/../../../DataIO
DEPENDPATH += $$PWD/../../../DataIO

#Dynamic link to DataManagement
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../DataManagement/release/ -likDataManagement
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../DataManagement/debug/ -likDataManagement
else:unix: LIBS += -L$$OUT_PWD/../../../DataManagement/ -likDataManagement
INCLUDEPATH += $$PWD/../../../DataManagement
DEPENDPATH += $$PWD/../../../DataManagement
