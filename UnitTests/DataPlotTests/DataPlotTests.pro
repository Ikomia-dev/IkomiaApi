#-------------------------------------------------
#
# Project created by QtCreator 2018-02-27T09:39:42
#
#-------------------------------------------------
include(../../IkomiaApi.pri)

QT       += testlib core gui widgets

TARGET = CDataPlotTests
CONFIG   -= app_bundle
# For Jenkins
CONFIG += testcase

DEFINES += BOOST_ALL_NO_LIB

unix {
    target.path = ../../Build/Tests/
    INSTALLS += target
    QMAKE_RPATHDIR += $$PWD/../../Build/Lib
    QMAKE_RPATHDIR += /usr/local/lib
}

TEMPLATE = app

DEFINES += SRCDIR=\\\"$$PWD/\\\"

SOURCES += \
        CDataPlotTests.cpp 

HEADERS += \
    CDataPlotTests.h

LIBS += $$link_boost()

#Dynamic link to OpenCV
win32:CONFIG(release, debug|release):LIBS += -lopencv_core$${OPENCV_VERSION}
win32:CONFIG(debug, debug|release):LIBS += -lopencv_core$${OPENCV_VERSION}d
unix:LIBS += -lopencv_core

#Dynamic link to Utils
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../Utils/release/ -likUtils
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../Utils/debug/ -likUtils
else:unix: LIBS += -L$$OUT_PWD/../../Utils/ -likUtils
INCLUDEPATH += $$PWD/../../Utils
DEPENDPATH += $$PWD/../../Utils

#Dynamic link to Qwt
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../qwt/build/lib/ -lqwt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../qwt/build/lib/ -lqwtd
else:unix:!macx: LIBS += -L$$PWD/../../../qwt/build/lib/ -lqwt
else:macx: LIBS += -F$$PWD/../../../qwt/build/lib/ -framework qwt
INCLUDEPATH += $$PWD/../../../qwt/src

#Dynamic link to DataPlot
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../DataPlot/release/ -likDataPlot
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../DataPlot/debug/ -likDataPlot
else:unix: LIBS += -L$$OUT_PWD/../../DataPlot/ -likDataPlot
INCLUDEPATH += $$PWD/../../DataPlot
DEPENDPATH += $$PWD/../../DataPlot
