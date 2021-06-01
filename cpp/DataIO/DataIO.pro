#-------------------------------------------------
#
# Project created by QtCreator 2017-06-23T11:30:43
#
#-------------------------------------------------
include(../IkomiaApi.pri)

QT      += core gui widgets sql concurrent

TARGET = ikDataIO
TEMPLATE = lib
VERSION = $$IKOMIA_LIB_VERSION
DEFINES += DATAIO_LIBRARY BOOST_ALL_NO_LIB

# Enable OpenMP
win32: QMAKE_CXXFLAGS += -openmp
unix: QMAKE_CXXFLAGS += -fopenmp
win32: QMAKE_LFLAGS +=  -openmp
unix:!macx: QMAKE_LFLAGS +=  -fopenmp
macx: LIBS += -lomp

SOURCES += \
        ../Core/Data/CvMatNumpyArrayConverter.cpp \
        CDataImageIO.cpp \
        CDataVideoBuffer.cpp \
        CNumpyImageIO.cpp \
        COpencvImageIO.cpp \
        CVirtualImageIO.cpp \
        CDicomImageIO.cpp \
        CVirtualVideoIO.cpp \
        COpencvVideoIO.cpp \
        CDataVideoIO.cpp

HEADERS += \
        ../Core/Data/CvMatNumpyArrayConverter.h \
        CDataImageIO.h \
        CDataIO.hpp \
        CDataVideoBuffer.h \
        CNumpyImageIO.h \
        COpencvImageIO.h \
        CVirtualImageIO.h \
        CDicomImageIO.h \
        DataIOTools.hpp \
        DataIOGlobal.hpp \
        CVirtualVideoIO.h \
        COpencvVideoIO.h \
        CDataVideoIO.h

#Use of 'lupdate DataIO.pro' to update translation files
#Use of 'lrelease DataIO.pro' to generate .qm files
TRANSLATIONS = Translations/dataio_fr.ts

#Make install directive
target.path = ../Build/Lib
INSTALLS += target

LIBS += $$link_opencv()

LIBS += $$link_boost()

LIBS += $$link_vtk()

LIBS += $$link_utils()
INCLUDEPATH += $$PWD/../Utils
DEPENDPATH += $$PWD/../Utils

LIBS += $$link_core()
INCLUDEPATH += $$PWD/../Core
DEPENDPATH += $$PWD/../Core
