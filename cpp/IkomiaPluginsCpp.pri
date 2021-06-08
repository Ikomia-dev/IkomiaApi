# Fichier de configuration Qt du projet GLPlugins
# Initialisation de toutes las variables communes aux projets de plugins
CONFIG += c++14

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


defineReplace(changePath) {
    var1 = $$1
    var2 = $$2
    var3 = $$3
    var4 = $$quote(install_name_tool -change $$var1$$var2 @executable_path/../Frameworks/$$var2 $$var3 $$escape_expand(\n\t))

    return($$var4)
}

defineReplace(changeOpencvPath) {
    var1 = $$1
    var2 = $$2
    var3 = $$3
    var4 = $$quote(install_name_tool -change @rpath/$$var1 @executable_path/../Frameworks/$$var2 $$var3 $$escape_expand(\n\t))

    return($$var4)
}

macx{
    QMAKE_CC = /usr/local/opt/llvm/bin/clang
    QMAKE_CXX = /usr/local/opt/llvm/bin/clang++
}

# Enable OpenMP
msvc {
  QMAKE_CXXFLAGS += -openmp -arch:AVX2 -D "_CRT_SECURE_NO_WARNINGS"
  QMAKE_CXXFLAGS_RELEASE *= -O2
}

##################################################################
# Load local machine settings (dependencies version, options...) #
##################################################################

include(LocalSettings.pri)

VERSION = 0.3.0.0


#####################################################################################
#                       INCLUDE                                                     #
#####################################################################################

# Global include directory for Mac OS X
macx: INCLUDEPATH += /usr/local/include

# Python3
centos7 {
    # Global include
    unix:!macx: INCLUDEPATH += /work/shared/local/include
    # Python include
    unix:!macx: INCLUDEPATH += /work/shared/local/include/python3.7m
    unix:!macx: INCLUDEPATH += /work/shared/local/lib/python3.7/site-packages/numpy/core/include/               # NUMPY path for CENTOS 7
}
else {
    win32: INCLUDEPATH += $$(ProgramW6432)/Python$${PYTHON_VERSION_EXT}/include
    win32: INCLUDEPATH += $$PWD/../numpy/numpy/core/include                                             # NUMPY path for Windows
    macx: pythonPath = /usr/local/python$${PYTHON_VERSION}                                              #/usr/local/opt/python/Frameworks/Python.framework/Versions/3.7
    macx: INCLUDEPATH += $$pythonPath/include/python$${PYTHON_VERSION}m
    macx: INCLUDEPATH += $$pythonPath/lib/python$${PYTHON_VERSION}/site-packages/numpy/core/include
    unix:!macx: INCLUDEPATH += /usr/include/python$${PYTHON_VERSION_EXT}
    unix:!macx: INCLUDEPATH += /usr/local/include/python$${PYTHON_VERSION}
    unix:!macx: INCLUDEPATH += /usr/lib/python$${PYTHON_VERSION}/site-packages/numpy/core/include/      # NUMPY path for Arch
}

# Boost
win32: INCLUDEPATH += $$(PROGRAMFILES)/Boost/include/boost-$${BOOST_VERSION}

# OpenCV
centos7 {
    unix:!macx: INCLUDEPATH += /work/shared/local/include/opencv4
}
win32: INCLUDEPATH += $$(PROGRAMFILES)/OpenCV/include
unix:!macx: INCLUDEPATH += /usr/local/include/opencv4
macx: INCLUDEPATH += /usr/local/include/opencv4

# OpenCL
unix: INCLUDEPATH += $$PWD/../../..
win32: INCLUDEPATH += '$$(PROGRAMFILES)/NVIDIA GPU Computing Toolkit/CUDA/v$${CUDA_VERSION}/include'

# Ikomia
IKOMIA_INCLUDE = $$PWD/Build/Include
INCLUDEPATH += $$IKOMIA_INCLUDE/Utils
INCLUDEPATH += $$IKOMIA_INCLUDE/Core
INCLUDEPATH += $$IKOMIA_INCLUDE/DataProcess


#####################################################################################
#                       LIB                                                         #
#####################################################################################

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

# Python3
win32: LIBS += -L$$(ProgramW6432)/Python$${PYTHON_VERSION_EXT}/libs
win32: LIBS += -L$$(ProgramW6432)/Python$${PYTHON_VERSION_EXT}/
win32: LIBS += -lpython$${PYTHON_VERSION_EXT}
macx: LIBS += -L$$pythonPath/lib
macx: LIBS += -lpython$${PYTHON_VERSION}
unix:!macx: LIBS += -lpython$${PYTHON_VERSION_EXT}

# Boost
win32: LIBS += -L$$(PROGRAMFILES)/Boost/lib
win32: LIBS += -lboost_filesystem-vc$${BOOST_VC_VERSION}-mt-x64-$${BOOST_VERSION} -lboost_system-vc$${BOOST_VC_VERSION}-mt-x64-$${BOOST_VERSION} -lboost_python$${PYTHON_VERSION_EXT}-vc$${BOOST_VC_VERSION}-mt-x64-$${BOOST_VERSION}
centos7 {
    unix:!macx: libs += -lboost_filesystem -lboost_system -lboost_python37
} else {
    unix:!macx: libs += -lboost_filesystem -lboost_system -lboost_python3
}
macx: LIBS += -lboost_filesystem -lboost_system -lboost_python37

# OpenCV
win32: LIBS += -L$$(ProgramW6432)/OpenCV/x64/vc$${MSVC_VERSION}/lib

# Cuda + OpenCL
win32: LIBS += -L$$(PROGRAMFILES)/OpenCL/lib
win32: LIBS += -L'$$(ProgramW6432)/NVIDIA GPU Computing Toolkit/CUDA/v$${CUDA_VERSION}/lib/x64'
unix:!macx: LIBS += -L/usr/lib64/nvidia/
unix:!macx: LIBS += -L/usr/local/cuda/lib64/ # centos 7

# OpenGL
win32: LIBS += -L'C:\Program Files(x86)\Windows Kits\10\Lib\10.0.17134.0\um\x64'

# OMP
unix: QMAKE_CXXFLAGS += -fopenmp
unix:!macx: QMAKE_LFLAGS += -fopenmp
macx: LIBS += -lomp

# Ikomia
macx {
    IKOMIA_LIBS = $$PWD/../Ikomia/Build/bin/Ikomia.app/Contents/Frameworks
} else {
    IKOMIA_LIBS = $$PWD/Build/Lib
}

######################
# Useful link macros #
######################
defineReplace(link_opencv) {
    #Dynamic link to OpenCV
    win32:CONFIG(release, debug|release): libs += -lopencv_core$${OPENCV_VERSION} -lopencv_imgproc$${OPENCV_VERSION}
    else:win32:CONFIG(debug, debug|release): libs += -lopencv_core$${OPENCV_VERSION}d -lopencv_imgproc$${OPENCV_VERSION}d
    unix: libs += -lopencv_core -lopencv_imgproc
    return($$libs)
}

defineReplace(link_utils) {
    #Dynamic link to Utils
    win32:CONFIG(release, debug|release): libs += -L$$IKOMIA_LIBS -likUtils
    else:win32:CONFIG(debug, debug|release): libs += -L$$IKOMIA_LIBS -likUtils
    else:unix: libs += -L$$IKOMIA_LIBS -likUtils
    return($$libs)
}

defineReplace(link_core) {
    #Dynamic link to Core
    win32:CONFIG(release, debug|release): libs += -L$$IKOMIA_LIBS -likCore
    else:win32:CONFIG(debug, debug|release): libs += -L$$IKOMIA_LIBS -likCore
    else:unix: libs += -L$$IKOMIA_LIBS -likCore
    return($$libs)
}

defineReplace(link_dataprocess) {
    #Dynamic link to DataProcess
    win32:CONFIG(release, debug|release): libs += -L$$IKOMIA_LIBS -likDataProcess
    else:win32:CONFIG(debug, debug|release): libs += -L$$IKOMIA_LIBS -likDataProcess
    else:unix: libs += -likDataProcess
    return($$libs)
}

# Automatic change for each lib path (for building bundle)
#macx: QMAKE_LFLAGS_SONAME  = -Wl,-install_name,@executable_path/../Frameworks/
macx: QMAKE_LFLAGS_SONAME  += -Wl,-rpath,@executable_path/../Frameworks/

# Modify library ELF header
# Set RPATH to target the current folder (where the plugin lives)
# $ORIGIN is not an environment variable but a linker variable
unix:!macx: QMAKE_LFLAGS_RPATH  =
unix:!macx: QMAKE_LFLAGS  = -Wl,-rpath,\'\$$ORIGIN\'

# DEPLOYMENT
makeDeploy.path = $$DESTDIR

macx {
TARGET_LIB = $$DESTDIR/$${QMAKE_PREFIX_SHLIB}$${TARGET}.$${QMAKE_EXTENSION_SHLIB}
QTDIR = /usr/local/opt/qt/lib/
# QtWidgets
QTLIB = QtWidgets.framework/Versions/5/QtWidgets
makeDeploy.commands += $$changePath($$QTDIR,$$QTLIB,$$TARGET_LIB)
# QtGui
QTLIB = QtGui.framework/Versions/5/QtGui
makeDeploy.commands += $$changePath($$QTDIR,$$QTLIB,$$TARGET_LIB)
# QtCore
QTLIB = QtCore.framework/Versions/5/QtCore
makeDeploy.commands += $$changePath($$QTDIR,$$QTLIB,$$TARGET_LIB)
# QtSql
QTLIB = QtSql.framework/Versions/5/QtSql
makeDeploy.commands += $$changePath($$QTDIR,$$QTLIB,$$TARGET_LIB)
}

#Plugin icons
iconInstall.path = $$DESTDIR/Icon
iconInstall.files = Images/icon.png
INSTALLS += iconInstall

# We prefix the report file name with the project file base name to prevent name collisions.
#VERA_TARGET = $$top_srcdir/Build/code_analysis/$$basename(_PRO_FILE_).vera++.xml
#vera++.commands = vera++ --checkstyle-report $$VERA_TARGET --show-rule $$absolute_paths($$HEADERS) $$absolute_paths($$SOURCES)
QMAKE_CLEAN += $$VERA_TARGET
QMAKE_EXTRA_TARGETS += vera++
