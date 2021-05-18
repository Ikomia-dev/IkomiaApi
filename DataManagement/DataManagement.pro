#-------------------------------------------------
#
# Project created by QtCreator 2017-07-06T09:53:53
#
#-------------------------------------------------
include(../IkomiaApi.pri)

QT  += core gui widgets sql concurrent

TARGET = ikDataManagement
TEMPLATE = lib
VERSION = $$IKOMIA_LIB_VERSION

DEFINES += DATAMANAGEMENT_LIBRARY BOOST_ALL_NO_LIB

SOURCES += \
        CImageDataManager.cpp \
        CVideoDataManager.cpp

HEADERS += \
        CDataManager.hpp \
        DataManagementGlobal.hpp \
        CImageDataManager.h \
        CVideoDataManager.h

#Use of 'lupdate DataManagement.pro' to update translation files
#Use of 'lrelease DataManagement.pro' to generate .qm files
TRANSLATIONS = Translations/datamanagement_fr.ts

#Make install directive
target.path = ../Build/Lib/Cpp
INSTALLS += target

#Dynamic link to OpenCV
win32:CONFIG(release, debug|release): LIBS += -lopencv_core$${OPENCV_VERSION} -lopencv_highgui$${OPENCV_VERSION}
else:win32:CONFIG(debug, debug|release): LIBS += -lopencv_core$${OPENCV_VERSION}d -lopencv_highgui$${OPENCV_VERSION}d
unix:LIBS += -lopencv_core -lopencv_highgui

LIBS += $$link_boost()

LIBS += $$link_utils()
INCLUDEPATH += $$PWD/../Utils
DEPENDPATH += $$PWD/../Utils

LIBS += $$link_core()
INCLUDEPATH += $$PWD/../Core
DEPENDPATH += $$PWD/../Core

LIBS += $$link_dataio()
INCLUDEPATH += $$PWD/../DataIO
DEPENDPATH += $$PWD/../DataIO
