top_srcdir=$$PWD
top_builddir=$$shadowed($$PWD)

defineReplace(absolute_paths) {
    result =
    for(path, ARGS) {
        result += $$absolute_path($$path, $$_PRO_FILE_PWD_)
    }
    return($$result)
}

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

defineReplace(link_opencv) {
    #Dynamic link to OpenCV
    win32:CONFIG(release, debug|release): libs += -lopencv_core$${OPENCV_VERSION} -lopencv_imgcodecs$${OPENCV_VERSION} -lopencv_imgproc$${OPENCV_VERSION} -lopencv_videoio$${OPENCV_VERSION}
    else:win32:CONFIG(debug, debug|release): libs += -lopencv_core$${OPENCV_VERSION}d -lopencv_imgcodecs$${OPENCV_VERSION}d -lopencv_imgproc$${OPENCV_VERSION}d -lopencv_videoio$${OPENCV_VERSION}d
    unix: libs += -lopencv_core -lopencv_imgcodecs -lopencv_imgproc -lopencv_videoio
    return($$libs)
}

defineReplace(link_boost) {
    #Dynamic link to Boost
    win32:CONFIG(release, debug|release): libs += -lboost_filesystem-vc$${BOOST_VC_VERSION}-mt-x64-$${BOOST_VERSION} -lboost_system-vc$${BOOST_VC_VERSION}-mt-x64-$${BOOST_VERSION} -lboost_python$${PYTHON_VERSION_EXT}-vc$${BOOST_VC_VERSION}-mt-x64-$${BOOST_VERSION}
    else:win32:CONFIG(debug, debug|release): libs += -lboost_filesystem-vc$${BOOST_VC_VERSION}-mt-gd-x64-$${BOOST_VERSION} -lboost_system-vc$${BOOST_VC_VERSION}-mt-gd-x64-$${BOOST_VERSION} -lboost_python$${PYTHON_VERSION_EXT}-vc$${BOOST_VC_VERSION}-mt-gd-x64-$${BOOST_VERSION}
    centos7 {
        unix:!macx: libs += -lboost_filesystem -lboost_system -lboost_python37
    } else {
        unix:!macx: libs += -lboost_filesystem -lboost_system -lboost_python3
    }
    macx:libs += -lboost_filesystem -lboost_system -lboost_python37
    return($$libs)
}

defineReplace(link_vtk) {
    #Dynamic link to VTK
    win32:libs += -lvtkCommonCore-$${VTK_VERSION} -lvtkCommonDataModel-$${VTK_VERSION} -lvtkCommonExecutionModel-$${VTK_VERSION} -lvtkCommonMath-$${VTK_VERSION} -lvtkCommonSystem-$${VTK_VERSION} -lvtkCommonTransforms-$${VTK_VERSION}
    win32:libs += -lvtkFiltersCore-$${VTK_VERSION} -lvtkFiltersGeneral-$${VTK_VERSION} -lvtkFiltersImaging-$${VTK_VERSION} -lvtkImagingColor-$${VTK_VERSION} -lvtkImagingCore-$${VTK_VERSION}
    win32:libs += -lvtkImagingGeneral-$${VTK_VERSION} -lvtkImagingMath-$${VTK_VERSION} -lvtkImagingSources-$${VTK_VERSION} -lvtkInteractionImage-$${VTK_VERSION} -lvtkInteractionStyle-$${VTK_VERSION} -lvtkIOCore-$${VTK_VERSION} -lvtkIOImage-$${VTK_VERSION}
    win32:libs += -lvtkRenderingContext2D-$${VTK_VERSION} -lvtkRenderingCore-$${VTK_VERSION} -lvtkRenderingImage-$${VTK_VERSION} -lvtkRenderingOpenGL2-$${VTK_VERSION} -lvtksys-$${VTK_VERSION} -lvtkViewsCore-$${VTK_VERSION} -lvtkDICOM-$${VTK_VERSION}

    unix:!macx:libs += -lvtkCommonCore-$${VTK_VERSION} -lvtkCommonDataModel-$${VTK_VERSION} -lvtkCommonExecutionModel-$${VTK_VERSION} -lvtkCommonMath-$${VTK_VERSION} -lvtkCommonSystem-$${VTK_VERSION} -lvtkCommonTransforms-$${VTK_VERSION}
    unix:!macx:libs += -lvtkFiltersCore-$${VTK_VERSION} -lvtkFiltersGeneral-$${VTK_VERSION} -lvtkFiltersImaging-$${VTK_VERSION} -lvtkImagingColor-$${VTK_VERSION} -lvtkImagingCore-$${VTK_VERSION}
    unix:!macx:libs += -lvtkImagingGeneral-$${VTK_VERSION} -lvtkImagingMath-$${VTK_VERSION} -lvtkImagingSources-$${VTK_VERSION} -lvtkInteractionImage-$${VTK_VERSION} -lvtkInteractionStyle-$${VTK_VERSION} -lvtkIOCore-$${VTK_VERSION} -lvtkIOImage-$${VTK_VERSION}
    unix:!macx:libs += -lvtkRenderingContext2D-$${VTK_VERSION} -lvtkRenderingCore-$${VTK_VERSION} -lvtkRenderingImage-$${VTK_VERSION} -lvtkRenderingOpenGL2-$${VTK_VERSION} -lvtksys-$${VTK_VERSION} -lvtkViewsCore-$${VTK_VERSION} -lvtkDICOM-$${VTK_VERSION}

    macx:libs += -lvtkCommonCore-$${VTK_VERSION} -lvtkCommonDataModel-$${VTK_VERSION} -lvtkCommonExecutionModel-$${VTK_VERSION} -lvtkCommonMath-$${VTK_VERSION} -lvtkCommonSystem-$${VTK_VERSION} -lvtkCommonTransforms-$${VTK_VERSION}
    macx:libs += -lvtkImagingCore-$${VTK_VERSION} -lvtkIOCore-$${VTK_VERSION} -lvtkIOImage-$${VTK_VERSION} -lvtksys-$${VTK_VERSION} -lvtkRenderingOpenGL2-$${VTK_VERSION} -lvtkInteractionStyle-$${VTK_VERSION} -lvtkRenderingCore-$${VTK_VERSION}
    macx:libs += -lvtkDICOM-$${VTK_VERSION}
    return($$libs)
}

defineReplace(link_utils) {
    #Dynamic link to Utils
    win32:CONFIG(release, debug|release): libs += -L$$OUT_PWD/../Utils/release/ -likUtils
    else:win32:CONFIG(debug, debug|release): libs += -L$$OUT_PWD/../Utils/debug/ -likUtils
    else:unix: libs += -L$$OUT_PWD/../Utils/ -likUtils
    return($$libs)
}

defineReplace(link_core) {
    #Dynamic link to Core
    win32:CONFIG(release, debug|release): libs += -L$$OUT_PWD/../Core/release/ -likCore
    else:win32:CONFIG(debug, debug|release): libs += -L$$OUT_PWD/../Core/debug/ -likCore
    else:unix: libs += -L$$OUT_PWD/../Core/ -likCore
    return($$libs)
}

defineReplace(link_dataprocess) {
    #Dynamic link to DataProcess
    win32:CONFIG(release, debug|release): libs += -L$$OUT_PWD/../DataProcess/release/ -likDataProcess
    else:win32:CONFIG(debug, debug|release): libs += -L$$OUT_PWD/../DataProcess/debug/ -likDataProcess
    else:unix: libs += -L$$OUT_PWD/../DataProcess/ -likDataProcess
    return($$libs)
}

defineReplace(link_dataio) {
    #Dynamic link to DataIO
    win32:CONFIG(release, debug|release): libs += -L$$OUT_PWD/../DataIO/release/ -likDataIO
    else:win32:CONFIG(debug, debug|release): libs += -L$$OUT_PWD/../DataIO/debug/ -likDataIO
    else:unix: libs += -L$$OUT_PWD/../DataIO/ -likDataIO
    return($$libs)
}

defineReplace(link_qwt) {
    # Dynamic link to QWT
    win32:CONFIG(release, debug|release): libs += -L$$IKOMIA_EXT_LIB_PATH/qwt/build/lib/ -lqwt
    else:win32:CONFIG(debug, debug|release): libs += -L$$IKOMIA_EXT_LIB_PATH/qwt/build/lib/ -lqwtd
    else:unix:!macx: libs += -L$$IKOMIA_EXT_LIB_PATH/qwt/build/lib/ -lqwt
    else:macx: libs += -F$$IKOMIA_EXT_LIB_PATH/qwt/build/lib/ -framework qwt
    return($$libs)
}

defineReplace(link_python) {
    win32:libs += -lpython$${PYTHON_VERSION_EXT}
    macx:libs += -lpython$${PYTHON_VERSION}
    unix:!macx: libs += -lpython$${PYTHON_VERSION_EXT}
    return($$libs)
}
