#-------------------------------------------------
#
# Project created by QtCreator 2018-07-24T12:08:55
#
#-------------------------------------------------
include(../../../IkomiaApi.pri)

QT      += core gui widgets testlib sql concurrent

TARGET  = DataInfoTests
CONFIG  += console
CONFIG  -= app_bundle

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

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    CDataInfoTests.cpp

HEADERS += \
    CDataInfoTests.h


LIBS += $$link_boost()

#Dynamic link OpenCV
win32:CONFIG(release, debug|release):LIBS += -lopencv_core$${OPENCV_VERSION}
win32:CONFIG(debug, debug|release):LIBS += -lopencv_core$${OPENCV_VERSION}d
unix: LIBS += -lopencv_core

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
