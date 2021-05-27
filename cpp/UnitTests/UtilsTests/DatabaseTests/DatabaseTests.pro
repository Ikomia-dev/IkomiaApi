#-------------------------------------------------
#
# Project created by QtCreator 2017-08-09T18:44:28
#
#-------------------------------------------------
include(../../../IkomiaApi.pri)

QT += core gui widgets testlib sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UtilsTests
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

DEFINES += BOOST_ALL_NO_LIB

unix {
    target.path = ../../../Build/Tests/
    INSTALLS += target
    QMAKE_RPATHDIR += $$PWD/../../../Build/Lib
    QMAKE_RPATHDIR += /usr/local/lib
}

SOURCES += \
    CDatabaseTests.cpp

HEADERS += \
    CDatabaseTests.h

DEFINES += SRCDIR=\\\"$$PWD/\\\"

LIBS += $$link_boost()

#Dynamic link to OpenCV
win32:CONFIG(release, debug|release):LIBS += -lopencv_core$${OPENCV_VERSION} -lopencv_imgcodecs$${OPENCV_VERSION} -lopencv_highgui$${OPENCV_VERSION} -lopencv_imgproc$${OPENCV_VERSION}
win32:CONFIG(debug, debug|release):LIBS += -lopencv_core$${OPENCV_VERSION}d -lopencv_imgcodecs$${OPENCV_VERSION}d -lopencv_highgui$${OPENCV_VERSION}d -lopencv_imgproc$${OPENCV_VERSION}d
unix: LIBS += -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc

#Dynamic link to Utils
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../Utils/release/ -likUtils
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../Utils/debug/ -likUtils
else:unix: LIBS += -L$$OUT_PWD/../../../Utils/ -likUtils
INCLUDEPATH += $$PWD/../../../Utils
DEPENDPATH += $$PWD/../../../Utils

#Dynamic link to Core
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../Core/release/ -iklCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../Core/debug/ -iklCore
else:unix: LIBS += -L$$OUT_PWD/../../../Core/ -likCore
INCLUDEPATH += $$PWD/../../../Core
DEPENDPATH += $$PWD/../../../Core
