#-------------------------------------------------
#
# Project created by QtCreator 2017-08-29T10:07:01
#
#-------------------------------------------------
include(../../../IkomiaApi.pri)

QT       += core gui testlib widgets sql concurrent

TARGET = WorkflowTests

CONFIG   -= app_bundle

# For Jenkins
CONFIG += testcase

DEFINES += BOOST_ALL_NO_LIB

unix {
    target.path = ../../../Build/Tests/
    INSTALLS += target
    QMAKE_RPATHDIR += $$PWD/../../../Build/Lib
    QMAKE_RPATHDIR += /usr/local/lib
}

TEMPLATE = app

SOURCES += \
    CWorkflowTests.cpp

HEADERS += \
    CWorkflowTests.h

#Dynamic link to OpenCV
win32:CONFIG(release, debug|release):LIBS += -lopencv_core$${OPENCV_VERSION} -lopencv_imgproc$${OPENCV_VERSION} -lopencv_objdetect$${OPENCV_VERSION} -lopencv_photo$${OPENCV_VERSION} -lopencv_ximgproc$${OPENCV_VERSION} -lopencv_highgui$${OPENCV_VERSION}
win32:CONFIG(release, debug|release):LIBS += -lopencv_xphoto$${OPENCV_VERSION} -lopencv_fuzzy$${OPENCV_VERSION} -lopencv_hfs$${OPENCV_VERSION} -lopencv_dnn$${OPENCV_VERSION} -lopencv_tracking$${OPENCV_VERSION} -lopencv_video$${OPENCV_VERSION}
win32:CONFIG(release, debug|release):LIBS += -lopencv_bgsegm$${OPENCV_VERSION} -lopencv_optflow$${OPENCV_VERSION} -lopencv_bioinspired$${OPENCV_VERSION} -lopencv_saliency$${OPENCV_VERSION} -lopencv_superres$${OPENCV_VERSION} -lopencv_text$${OPENCV_VERSION}
win32:CONFIG(release, debug|release):LIBS += -lopencv_features2d$${OPENCV_VERSION} -lopencv_cudawarping$${OPENCV_VERSION} -lopencv_imgcodecs$${OPENCV_VERSION}
win32:CONFIG(debug, debug|release):LIBS += -lopencv_core$${OPENCV_VERSION}d -lopencv_imgproc$${OPENCV_VERSION}d -lopencv_objdetect$${OPENCV_VERSION}d -lopencv_photo$${OPENCV_VERSION}d -lopencv_ximgproc$${OPENCV_VERSION}d -lopencv_highgui$${OPENCV_VERSION}d
win32:CONFIG(debug, debug|release):LIBS += -lopencv_xphoto$${OPENCV_VERSION}d -lopencv_fuzzy$${OPENCV_VERSION}d -lopencv_hfs$${OPENCV_VERSION}d -lopencv_dnn$${OPENCV_VERSION}d -lopencv_tracking$${OPENCV_VERSION}d -lopencv_video$${OPENCV_VERSION}d
win32:CONFIG(debug, debug|release):LIBS += -lopencv_bgsegm$${OPENCV_VERSION}d -lopencv_optflow$${OPENCV_VERSION}d -lopencv_bioinspired$${OPENCV_VERSION}d -lopencv_saliency$${OPENCV_VERSION}d -lopencv_superres$${OPENCV_VERSION}d -lopencv_text$${OPENCV_VERSION}d
win32:CONFIG(debug, debug|release):LIBS += -lopencv_features2d$${OPENCV_VERSION}d -lopencv_cudawarping$${OPENCV_VERSION}d -lopencv_imgcodecs$${OPENCV_VERSION}d
unix: LIBS += -lopencv_core -lopencv_imgproc -lopencv_objdetect -lopencv_photo -lopencv_ximgproc -lopencv_highgui
unix: LIBS += -lopencv_xphoto -lopencv_fuzzy -lopencv_hfs -lopencv_dnn -lopencv_cudawarping -lopencv_tracking -lopencv_video
unix: LIBS += -lopencv_bgsegm -lopencv_optflow -lopencv_bioinspired -lopencv_saliency -lopencv_superres -lopencv_text
unix: LIBS += -lopencv_face -lopencv_features2d -lopencv_imgcodecs

LIBS += $$link_boost()

#Dynamic link to gmic
LIBS += -lgmic

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

#Dynamic link to DataProcess
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../DataProcess/release/ -likDataProcess
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../DataProcess/debug/ -likDataProcess
else:unix: LIBS += -L$$OUT_PWD/../../../DataProcess/ -likDataProcess
INCLUDEPATH += $$PWD/../../../DataProcess
DEPENDPATH += $$PWD/../../../DataProcess
