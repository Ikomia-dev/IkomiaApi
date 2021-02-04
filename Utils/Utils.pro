#-------------------------------------------------
#
# Project created by QtCreator 2017-06-28T15:46:07
#
#-------------------------------------------------
include(../IkomiaApi.pri)
include($$PWD/Widgets/QtPropertyBrowser/qtpropertybrowser.pri)
include($$PWD/Widgets/FancyTabBar/FancyTabBar.pri)

# Qt modules
QT  += core sql widgets

# Target name
TARGET = ikUtils
# Output type
TEMPLATE = lib
VERSION = $$(IKOMIA_LIB_VERSION)
DEFINES += UTILS_LIBRARY
DEFINES += QT_QTPROPERTYBROWSER_EXPORT

# Source/header files
SOURCES += \
        CException.cpp \
        CLogManager.cpp \
        CMemoryInfo.cpp \
        Markup.cpp \
        CProgressSignalHandler.cpp \
        CWindowsDeviceEnumerator.cpp \
        Widgets/CBrowseFileWidget.cpp \
        Widgets/CColorPushButton.cpp \
        qaesencryption.cpp

HEADERS += \
        CException.h \
        CLogManager.h \
        Containers/CQueue.hpp \
        DesignPattern/CObjectLocker.hpp \
        ExceptionCode.hpp \
        UtilsTools.hpp \
        CMemoryInfo.h \
        CTreeDbManager.hpp \
        UnitTestUtils.hpp \
        DesignPattern/CFactoryRegister.hpp \
        DesignPattern/CAbstractFactory.hpp \
        DesignPattern/CSingleton.hpp \
        CTimer.hpp \
        UtilsGlobal.hpp \
        PythonThread.hpp \
        Markup.h \
        CProgressSignalHandler.h \
        CWindowsDeviceEnumerator.h \
        Widgets/CBrowseFileWidget.h \
        Widgets/CColorPushButton.h \
        qaesencryption.h

#Use of 'lupdate Utils.pro' to update translation files
#Use of 'lrelease Utils.pro' to generate .qm files
TRANSLATIONS = Translations/utils_fr.ts

#Make install directive
target.path = ../Build/Lib/Cpp

deployInclude.path = ../Build/Include/Utils
deployInclude.files = \
        CException.h \
        CLogManager.h \
        UtilsTools.hpp \
        ExceptionCode.hpp \
        CMemoryInfo.h \
        CTimer.hpp \
        UtilsGlobal.hpp \
        CProgressSignalHandler.h \
        CWindowsDeviceEnumerator.h \
        PythonThread.hpp

deployIncludeDesignPattern.path = ../Build/Include/Utils/DesignPattern
deployIncludeDesignPattern.files = \
        DesignPattern/CFactoryRegister.hpp \
        DesignPattern/CAbstractFactory.hpp \
        DesignPattern/CSingleton.hpp \
        DesignPattern/CObjectLocker.hpp

deployIncludeContainer.path = ../Build/Include/Utils/Containers
deployIncludeContainer.files = \
        Containers/CQueue.hpp

deployIncludeWidget.path = ../Build/Include/Utils/Widgets
deployIncludeWidget.files = \
        Widgets/CColorPushButton.h \
        Widgets/CBrowseFileWidget.h

INSTALLS += target
INSTALLS += deployInclude
INSTALLS += deployIncludeDesignPattern
INSTALLS += deployIncludeContainer
INSTALLS += deployIncludeWidget

#Dynamic link to OpenCV
win32:CONFIG(release, debug|release): LIBS += -lopencv_core$${OPENCV_VERSION}
else:win32:CONFIG(debug, debug|release): LIBS += -lopencv_core$${OPENCV_VERSION}d
unix:LIBS += -lopencv_core

#libiconv for Markup
macx: LIBS += -liconv
