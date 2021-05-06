# Fichier de configuration Qt du projet Ikomia
# Initialisation de toutes las variables communes au projet

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Global path for quazip, qwt, gmic and breakpad
IKOMIA_EXT_LIB_PATH = $$PWD/.. # default path on arch, win32, mac and centos6

# Include some useful macros
include(macros.pri)

macx{
    QMAKE_CC = /usr/local/opt/llvm/bin/clang
    QMAKE_CXX = /usr/local/opt/llvm/bin/clang++
}

CONFIG += c++14

# OSX build option
# CONFIG += osx_deploy

#QMAKE_CXXFLAGS += -std=c++17

##################################################################
# Load local machine settings (dependencies version, options...) #
##################################################################
include(LocalSettings.pri)

#####################################################################################
#                       INCLUDE                                                     #
#####################################################################################

# Global include directory for Mac OS X
macx: INCLUDEPATH += /usr/local/include

# Boost
win32: INCLUDEPATH += $$(ProgramW6432)/Boost/include/boost-$${BOOST_VERSION}

# Python3
centos7 {
    # Global include
    unix:!macx: INCLUDEPATH += /work/shared/local/include
    # Python include
    unix:!macx: INCLUDEPATH += /work/shared/local/include/python3.7m
    unix:!macx: INCLUDEPATH += /work/shared/local/lib/python3.7/site-packages/numpy/core/include/   # NUMPY path for CENTOS 7
}
else {
    win32: INCLUDEPATH += $$(ProgramW6432)/Python$${PYTHON_VERSION_EXT}/include
    win32: INCLUDEPATH += $$PWD/../numpy/numpy/core/include                                         # NUMPY path for Windows
    macx: pythonPath = /usr/local/python$${PYTHON_VERSION}                                          # /usr/local/opt/python/Frameworks/Python.framework/Versions/3.7
    macx: INCLUDEPATH += $$pythonPath/include/python$${PYTHON_VERSION}m
    macx: INCLUDEPATH += $$pythonPath/lib/python$${PYTHON_VERSION}/site-packages/numpy/core/include
    unix:!macx: INCLUDEPATH += /usr/include/python$${PYTHON_VERSION_EXT}
    unix:!macx: INCLUDEPATH += /usr/local/include/python$${PYTHON_VERSION}
    unix:!macx: INCLUDEPATH += /usr/lib/python$${PYTHON_VERSION}/site-packages/numpy/core/include/  # NUMPY path for Arch
}

# VTK
centos7 {
    unix:!macx: INCLUDEPATH += /work/shared/local/include/vtk-8.1/
}
win32: INCLUDEPATH += $$(ProgramW6432)/VTK/include/vtk-$${VTK_VERSION}
unix:!macx: INCLUDEPATH += /usr/include/vtk/8.1.1/
unix:!macx: INCLUDEPATH += /usr/local/include/vtk-$${VTK_VERSION}/                                  # Add path for VTK on CENTOS 6
unix:!macx: INCLUDEPATH += /usr/include/vtk
macx: INCLUDEPATH += /usr/local/include/vtk-$${VTK_VERSION}

#OpenCV
centos7 {
    unix:!macx: INCLUDEPATH += /work/shared/local/include/opencv4
}
win32: INCLUDEPATH += $$(ProgramW6432)/OpenCV/include
unix:!macx: INCLUDEPATH += /usr/local/include/opencv4
macx: INCLUDEPATH += /usr/local/include/opencv4

#OpenCL
unix: INCLUDEPATH += $$PWD/../../..
win32: INCLUDEPATH += '$$(ProgramW6432)/NVIDIA GPU Computing Toolkit/CUDA/v$${CUDA_VERSION}/include'

#gmic
win32: INCLUDEPATH += $$(ProgramW6432)/gmic/include
unix: INCLUDEPATH += $$PWD/../gmic/src

#####################################################################################
#                       LIB                                                         #
#####################################################################################
IKOMIA_LIB_VERSION = 0.5.0.0

# Global lib directory for:
# Linux
unix:!macx: LIBS += -L/usr/local/lib64/ # XFCE
unix:!macx: LIBS += -L/usr/local/lib/   # KDE PLASMA
centos7 {
    unix:!macx: LIBS += -L/work/shared/local/lib/
    unix:!macx: LIBS += -L/work/shared/local/lib64/
}
# Mac OS X
macx: LIBS += -L/usr/local/lib/

# Boost
win32: LIBS += -L$$(ProgramW6432)/Boost/lib

# Python3
win32: LIBS += -L$$(ProgramW6432)/Python$${PYTHON_VERSION_EXT}/libs
win32: LIBS += -L$$(ProgramW6432)/Python$${PYTHON_VERSION_EXT}/
macx: LIBS += -L$$pythonPath/lib

LIBS += $$link_python()

# VTK
win32: LIBS += -L$$(ProgramW6432)/VTK/lib
win32: LIBS += -L$$(ProgramW6432)/VTK/bin

# OpenCV
win32: LIBS += -L$$(ProgramW6432)/OpenCV/x64/vc$${MSVC_VERSION}/lib
win32: LIBS += -L$$(ProgramW6432)/OpenCV/x64/vc$${MSVC_VERSION}/bin

# CUDA + OpenCL
win32: LIBS += -L'$$(ProgramW6432)/NVIDIA GPU Computing Toolkit/CUDA/v$${CUDA_VERSION}/lib/x64'
win32: LIBS += -L'$$(ProgramW6432)/NVIDIA GPU Computing Toolkit/CUDA/v$${CUDA_VERSION}/bin'
unix:!macx: LIBS += -L/usr/lib64/nvidia/
unix:!macx: LIBS += -L/usr/local/cuda/lib64/ # centos 7

# OpenGL
win32: LIBS += -L'C:\Program Files(x86)\Windows Kits\10\Lib\10.0.17134.0\um\x64'

# GDCM
win32: LIBS += -L$$(ProgramW6432)/GDCM/bin

# Leptonica
win32: LIBS += -L$$(ProgramW6432)/leptonica/bin

# Tesseract
win32: LIBS += -L$$(ProgramW6432)/tesseract/bin

# TBB
win32: LIBS += -L$$(ProgramW6432)/intel_tbb/bin/intel64/vc14

# ZLib
win32: LIBS += -L$$(ProgramW6432)/zlib/bin

# Quazip
win32: LIBS += -L$$PWD/../quazip/build/lib
macx: LIBS += -L$$PWD/../quazip/build/lib

#Qwt
win32: LIBS += -L$$PWD/../qwt/build/lib
macx: LIBS += -L$$PWD/../qwt/build/lib

#gmic
win32:LIBS += -L$$(ProgramW6432)/gmic/lib/
win32:LIBS += -L$$(ProgramW6432)/gmic/bin/
unix:LIBS += -L$$PWD/../gmic/build/
unix:DEPENDPATH += $$PWD/../gmic/build

#OpenSSL
win32:LIBS += -L$$(ProgramW6432)/OpenSSL-Win64/bin

# On linux, to force search path for linking libs
unix:!macx: QMAKE_LFLAGS += -Wl,-rpath-link,/usr/local/lib # KDE
unix:!macx: QMAKE_LFLAGS += -Wl,-rpath-link,/usr/local/lib64 # XFCE

# Automatic change for each lib path (for building bundle)
# Only if deployment and needs clean/rebuild
osx_deploy{
macx: QMAKE_LFLAGS_SONAME  = -Wl,-install_name,@executable_path/../Frameworks/
macx: QMAKE_LFLAGS_SONAME  += -Wl,-rpath,@executable_path/../Frameworks/
}

#####################################################################################
#                       CODE COVERAGE                                               #
#####################################################################################
IKOMIA_COV{
    #Disable compiler optimization
    QMAKE_CXXFLAGS -= -O1
    QMAKE_CXXFLAGS -= -O2
    QMAKE_CXXFLAGS_DEBUG -= -O1
    QMAKE_CXXFLAGS_DEBUG -= -O2
    QMAKE_CXXFLAGS_RELEASE -= -O1
    QMAKE_CXXFLAGS_RELEASE -= -O2
    QMAKE_CXXFLAGS += -O0
    QMAKE_CXXFLAGS_DEBUG += -O0
    QMAKE_CXXFLAGS_RELEASE += -O0

    QMAKE_LFLAGS -= -O1
    QMAKE_LFLAGS_DEBUG -= -O1
    QMAKE_LFLAGS_RELEASE -= -O1
    QMAKE_LFLAGS += -O0
    QMAKE_LFLAGS_DEBUG += -O0
    QMAKE_LFLAGS_RELEASE += -O0

    #Enable code coverage analysis
    QMAKE_CXXFLAGS += --coverage
    QMAKE_LFLAGS += --coverage
    LIBS += -lgcov
}

# Enable OpenMP
msvc {
  QMAKE_CXXFLAGS += -openmp -arch:AVX2 -D "_CRT_SECURE_NO_WARNINGS"
  QMAKE_CXXFLAGS_RELEASE *= -O2
}

unix: QMAKE_CXXFLAGS += -fopenmp
unix:!macx: QMAKE_LFLAGS +=  -fopenmp
macx: LIBS += -lomp

# We prefix the report file name with the project file base name to prevent name collisions.
VERA_TARGET = $$top_srcdir/Build/code_analysis/$$basename(_PRO_FILE_).vera++.xml
vera++.commands = vera++ --checkstyle-report $$VERA_TARGET --show-rule $$absolute_paths($$HEADERS) $$absolute_paths($$SOURCES)
QMAKE_CLEAN += $$VERA_TARGET
QMAKE_EXTRA_TARGETS += vera++

DISTFILES += \
    $$PWD/MakeMacBundle.pri \
    $$PWD/macros.pri
