include(../../../IkomiaApi.pri)

QT += core gui widgets testlib sql

CONFIG += console
CONFIG -= app_bundle

# For Jenkins
CONFIG += testcase

DEFINES += BOOST_ALL_NO_LIB

unix {
    target.path = ../../../Build/Tests/
    INSTALLS += target
    QMAKE_RPATHDIR += $$PWD/../../../Build/Lib
    QMAKE_RPATHDIR += /usr/local/lib
}

SOURCES += \
    CToolsTests.cpp

HEADERS += \
    CToolsTests.h

#Dynamic link OpenCV
win32:CONFIG(release, debug|release):LIBS += -lopencv_core$${OPENCV_VERSION} -lopencv_imgcodecs$${OPENCV_VERSION} -lopencv_imgproc$${OPENCV_VERSION}
win32:CONFIG(debug, debug|release):LIBS += -lopencv_core$${OPENCV_VERSION}d -lopencv_imgcodecs$${OPENCV_VERSION}d -lopencv_imgproc$${OPENCV_VERSION}d
unix: LIBS += -lopencv_core -lopencv_imgcodecs -lopencv_imgproc

LIBS += $$link_boost()

#Dynamic link to Utils
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../Utils/release/ -likUtils
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../Utils/debug/ -likUtils
else:unix: LIBS += -L$$OUT_PWD/../../../Utils/ -likUtils
INCLUDEPATH += $$PWD/../../../Utils
DEPENDPATH += $$PWD/../../../Utils
