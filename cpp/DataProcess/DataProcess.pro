#-------------------------------------------------
#
# Project created by QtCreator 2017-06-28T16:37:17
#
#-------------------------------------------------
include(../IkomiaApi.pri)

QT       += core gui widgets sql concurrent

TARGET = ikDataProcess
TEMPLATE = lib
VERSION = $$IKOMIA_LIB_VERSION

DEFINES += DATAPROCESS_LIBRARY BOOST_ALL_NO_LIB

# Enable OpenMP
win32: QMAKE_CXXFLAGS += -openmp -bigobj
unix: QMAKE_CXXFLAGS += -fopenmp
win32: QMAKE_LFLAGS +=  -openmp
unix:!macx: QMAKE_LFLAGS +=  -fopenmp
macx: LIBS += -lomp

# Use Precompiled headers (PCH)
unix:CONFIG += precompile_header
unix:PRECOMPILED_HEADER = pch.hpp

SOURCES += \
        CProcessRegistration.cpp \
        Core/C2dFeatureImageTask.cpp \
        Core/C2dFeatureMatcherTask.cpp \
        Core/C2dImageInteractiveTask.cpp \
        Core/C2dImageTask.cpp \
        Core/CDnnTrainTask.cpp \
        Core/CIkomiaRegistry.cpp \
        Core/CMlflowTrainTask.cpp \
        Core/CRunTaskManager.cpp \
        Core/CTaskInfo.cpp \
        Core/CVideoTask.cpp \
        Core/CWorkflow.cpp \
        IO/CArrayIO.cpp \
        IO/CBlobMeasureIO.cpp \
        IO/CConvertIO.cpp \
        IO/CDatasetIO.cpp \
        IO/CGraphicsInput.cpp \
        IO/CGraphicsOutput.cpp \
        IO/CImageIO.cpp \
        IO/CNumericIO.cpp \
        IO/CPathIO.cpp \
        IO/CTaskIORegistration.cpp \
        IO/CVideoIO.cpp \
        IO/CWidgetOutput.cpp \
        Process/Core/CBinaryToGraphics.cpp \
        Process/Core/CBlobMeasure.cpp \
        Process/Core/CGraphicsToBinary.cpp \
        Process/Core/CRgbHlsThreshold.cpp \
        Process/Gmic/CGmicImageConverter.cpp \
        Widget/Core/CWidgetBlobMeasure.cpp \
        Widget/Core/CWidgetRgbHlsThreshold.cpp \
        Process/OpenCV/dnn/COcvDnnProcess.cpp \
        Widget/OpenCV/dnn/COcvWidgetDnnCore.cpp \
        Widget/OpenCV/dnn/COcvWidgetDnnProcess.cpp \
        Process/OpenCV/superres/COcvSuperResBTVL1.cpp \
        Process/OpenCV/superres/CSuperResFrameSrc.cpp \
        Core/CVideoOFTask.cpp \
        Core/CVideoTrackingTask.cpp \
        Process/OpenCV/dnn/COcvDnnSegmentation.cpp

HEADERS += \
        Core/C2dFeatureImageTask.h \
        Core/C2dFeatureMatcherTask.h \
        Core/C2dImageInteractiveTask.h \
        Core/C2dImageTask.h \
        Core/CDnnTrainTask.h \
        Core/CIkomiaRegistry.h \
        Core/CMlflowTrainTask.h \
        Core/CRunTaskManager.h \
        Core/CTaskFactory.hpp \
        Core/CTaskInfo.h \
        Core/CVideoTask.h \
        Core/CWorkflow.h \
        IO/CArrayIO.h \
        IO/CBlobMeasureIO.h \
        IO/CConvertIO.h \
        IO/CDatasetIO.h \
        IO/CGraphicsInput.h \
        IO/CGraphicsOutput.h \
        IO/CImageIO.h \
        IO/CNumericIO.h \
        IO/CPathIO.h \
        IO/CTaskIORegistration.h \
        IO/CVideoIO.h \
        IO/CWidgetOutput.h \
        Process/Core/CBinaryToGraphics.h \
        Process/Core/CGraphicsToBinary.h \
        Process/Gmic/CGmicImageConverter.h \
        Process/Gmic/CGmicTaskParam.hpp \
        Process/Gmic/Colors/CGmicAutoBalance.hpp \
        Process/Gmic/Colors/CGmicBoostChroma.hpp \
        Process/Gmic/Colors/CGmicBoostFade.hpp \
        Process/Gmic/Colors/CGmicColorPresets.hpp \
        Process/Gmic/Contours/CGmicDOG.hpp \
        Process/Gmic/Contours/CGmicDistanceTransform.hpp \
        Process/Gmic/Contours/CGmicSkeleton.hpp \
        Process/Gmic/Contours/CGmicSuperPixels.hpp \
        Process/Gmic/Details/CGmicConstrainedSharpen.hpp \
        Process/Gmic/Details/CGmicDynamicRangeIncrease.hpp \
        Process/Gmic/Details/CGmicMagicDetails.hpp \
        Process/Gmic/Details/CGmicSharpenDeblur.hpp \
        Process/Gmic/Details/CGmicSharpenGradient.hpp \
        Process/Gmic/Details/CGmicSharpenLucy.hpp \
        Process/Gmic/Details/CGmicSharpenTones.hpp \
        Process/Gmic/ProcessGmic.hpp \
        Process/Gmic/Repair/CGmicHotPixels.hpp \
        Process/Gmic/Repair/CGmicInpaint.hpp \
        Process/Gmic/Repair/CGmicRedEye.hpp \
        Process/OpenCV/core/COcvCompare.hpp \
        Process/OpenCV/core/COcvCopyMakeBorder.hpp \
        Process/OpenCV/core/COcvCountNonZero.hpp \
        Process/OpenCV/core/COcvExtractChannel.hpp \
        Process/OpenCV/core/COcvInsertChannel.hpp \
        Process/OpenCV/core/COcvPSNR.hpp \
        Process/OpenCV/features2d/COcvSIFT.hpp \
        Process/OpenCV/imgproc/COcvHoughCircles.hpp \
        Process/OpenCV/imgproc/COcvHoughLines.hpp \
        Widget/Core/CWidgetBinaryToGraphics.hpp \
        Widget/Core/CWidgetGraphicsToBinary.hpp \
        Widget/Gmic/Colors/CGmicWidgetAutoBalance.hpp \
        Widget/Gmic/Colors/CGmicWidgetBoostChroma.hpp \
        Widget/Gmic/Colors/CGmicWidgetBoostFade.hpp \
        Widget/Gmic/Colors/CGmicWidgetColorPresets.hpp \
        Widget/Gmic/Contours/CGmicWidgetDOG.hpp \
        Widget/Gmic/Contours/CGmicWidgetDistanceTransform.hpp \
        Widget/Gmic/Contours/CGmicWidgetSkeleton.hpp \
        Widget/Gmic/Contours/CGmicWidgetSuperPixels.hpp \
        Widget/Gmic/Details/CGmicWidgetConstrainedSharpen.hpp \
        Widget/Gmic/Details/CGmicWidgetDynamicRangeIncrease.hpp \
        Widget/Gmic/Details/CGmicWidgetMagicDetails.hpp \
        Widget/Gmic/Details/CGmicWidgetSharpenDeblur.hpp \
        Widget/Gmic/Details/CGmicWidgetSharpenGradient.hpp \
        Widget/Gmic/Details/CGmicWidgetSharpenLucy.hpp \
        Widget/Gmic/Details/CGmicWidgetSharpenTones.hpp \
        Widget/Gmic/Repair/CGmicWidgetHotPixels.hpp \
        Widget/Gmic/Repair/CGmicWidgetInpaint.hpp \
        Widget/Gmic/Repair/CGmicWidgetRedEye.hpp \
        Widget/Gmic/WidgetGmic.hpp \
        Widget/OpenCV/core/COcvWidgetCompare.hpp \
        Widget/OpenCV/core/COcvWidgetCopyMakeBorder.hpp \
        Widget/OpenCV/core/COcvWidgetCountNonZero.hpp \
        Widget/OpenCV/core/COcvWidgetExtractChannel.hpp \
        Widget/OpenCV/core/COcvWidgetInsertChannel.hpp \
        Widget/OpenCV/core/COcvWidgetPSNR.hpp \
        Widget/OpenCV/dnn/COcvWidgetDnnCore.h \
        Widget/OpenCV/features2d/COcvWidgetSIFT.hpp \
        Widget/OpenCV/imgproc/COcvWidgetHoughCircles.hpp \
        Widget/OpenCV/imgproc/COcvWidgetHoughLines.hpp \
        pch.hpp \
        DataProcess.hpp \
        CProcessRegistration.h \
        CPluginProcessInterface.hpp \
        DataProcessTools.hpp \
        Process/Core/ProcessCore.hpp \
        Process/Core/CBlobMeasure.h \
        Process/Core/CPlotMerge.hpp \
        Process/Core/CRgbHlsThreshold.h \
        Process/OpenCV/ProcessOcv.hpp \
        Process/OpenCV/COcvInpaintFuzzy.hpp \
        Process/OpenCV/COcvHfsSegment.hpp \
        Widget/Core/WidgetCore.hpp \
        Widget/Core/CWidgetBlobMeasure.h \
        Widget/Core/CWidgetApplyOnly.hpp \
        Widget/Core/CWidgetPlotMerge.hpp \
        Widget/Core/CWidgetRgbHlsThreshold.h \
        Widget/OpenCV/WidgetOcv.hpp \
        Widget/OpenCV/COcvWidgetInpaintFuzzy.hpp \
        Widget/OpenCV/COcvWidgetHfsSegment.hpp \
        DataProcessGlobal.hpp \
        Process/OpenCV/xphoto/COcvGrayworldWB.hpp \
        Widget/OpenCV/xphoto/COcvWidgetGrayworldWB.hpp \
        Process/OpenCV/xphoto/COcvTonemapDurand.hpp \
        Widget/OpenCV/xphoto/COcvWidgetTonemapDurand.hpp \
        Process/OpenCV/photo/COcvTextureFlattening.hpp \
        Widget/OpenCV/photo/COcvWidgetTextureFlattening.hpp \
        Process/OpenCV/photo/COcvPencilSketch.hpp \
        Widget/OpenCV/photo/COcvWidgetPencilSketch.hpp \
        Process/OpenCV/photo/COcvStylization.hpp \
        Widget/OpenCV/photo/COcvWidgetStylization.hpp \
        Process/OpenCV/ximgproc/COcvBilateralTextureFilter.hpp \
        Widget/OpenCV/ximgproc/COcvWidgetBilateralTextureFilter.hpp \
        Process/OpenCV/ximgproc/COcvGradientDeriche.hpp \
        Widget/OpenCV/ximgproc/COcvWidgetGradientDeriche.hpp \
        Process/OpenCV/ximgproc/COcvGradientPaillou.hpp \
        Widget/OpenCV/ximgproc/COcvWidgetGradientPaillou.hpp \
        Process/OpenCV/ximgproc/COcvRollingGuidanceFilter.hpp \
        Widget/OpenCV/ximgproc/COcvWidgetRollingGuidanceFilter.hpp \
        Widget/OpenCV/ximgproc/COcvWidgetL0Smooth.hpp \
        Process/OpenCV/ximgproc/COcvL0Smooth.hpp \
        Widget/OpenCV/imgproc/COcvWidgetSobel.hpp \
        Process/OpenCV/imgproc/COcvSobel.hpp \
        Process/OpenCV/tracking/COcvTrackerGOTURN.hpp \
        Widget/OpenCV/tracking/COcvWidgetTrackerGOTURN.hpp \
        Process/OpenCV/core/COcvMagnitude.hpp \
        Widget/OpenCV/core/COcvWidgetMagnitude.hpp \
        Process/OpenCV/tracking/COcvTrackerKCF.hpp \
        Widget/OpenCV/tracking/COcvWidgetTrackerKCF.hpp \
        Process/OpenCV/features2d/COcvORB.hpp \
        Widget/OpenCV/features2d/COcvWidgetORB.hpp \
        Process/OpenCV/features2d/COcvAKAZE.hpp \
        Widget/OpenCV/features2d/COcvWidgetAKAZE.hpp \
        Process/OpenCV/features2d/COcvBRISK.hpp \
        Widget/OpenCV/features2d/COcvWidgetBRISK.hpp \
        Process/OpenCV/features2d/COcvFAST.hpp \
        Widget/OpenCV/features2d/COcvWidgetFAST.hpp \
        Process/OpenCV/imgproc/COcvBilateral.hpp \
        Process/OpenCV/imgproc/COcvBoxFilter.hpp \
        Widget/OpenCV/imgproc/COcvWidgetBilateral.hpp \
        Widget/OpenCV/imgproc/COcvWidgetBoxFilter.hpp \
        Process/OpenCV/features2d/COcvGFTT.hpp \
        Widget/OpenCV/features2d/COcvWidgetGFTT.hpp \
        Process/OpenCV/features2d/COcvAGAST.hpp \
        Widget/OpenCV/features2d/COcvWidgetAGAST.hpp \
        Process/OpenCV/features2d/COcvKAZE.hpp \
        Widget/OpenCV/features2d/COcvWidgetKAZE.hpp \
        Process/OpenCV/features2d/COcvSimpleBlobDetector.hpp \
        Widget/OpenCV/features2d/COcvWidgetSimpleBlobDetector.hpp \
        Process/OpenCV/ximgproc/COcvAnisotropicDiffusion.hpp \
        Widget/OpenCV/ximgproc/COcvWidgetAnisotropicDiffusion.hpp \
        Process/OpenCV/ximgproc/COcvPeiLinNormalization.hpp \
        Widget/OpenCV/ximgproc/COcvWidgetPeiLinNormalization.hpp \
        Process/OpenCV/ximgproc/COcvAdaptiveManifold.hpp \
        Widget/OpenCV/ximgproc/COcvWidgetAdaptiveManifold.hpp \
        Process/OpenCV/ximgproc/COcvNiblackThreshold.hpp \
        Widget/OpenCV/ximgproc/COcvWidgetNiblackThreshold.hpp \
        Widget/OpenCV/ximgproc/COcvWidgetThinning.hpp \
        Process/OpenCV/ximgproc/COcvThinning.hpp \
        Process/OpenCV/xphoto/COcvInpaintX.hpp \
        Widget/OpenCV/xphoto/COcvWidgetInpaintX.hpp \
        Process/OpenCV/photo/COcvDetailEnhance.hpp \
        Widget/OpenCV/photo/COcvWidgetDetailEnhance.hpp \
        Process/OpenCV/photo/COcvEdgePreservingFilter.hpp \
        Widget/OpenCV/photo/COcvWidgetEdgePreservingFilter.hpp \
        Process/OpenCV/photo/COcvFastNlMeans.hpp \
        Widget/OpenCV/photo/COcvWidgetFastNlMeans.hpp \
        Process/OpenCV/photo/COcvInpaint.hpp \
        Widget/OpenCV/photo/COcvWidgetInpaint.hpp \
        Widget/OpenCV/photo/COcvWidgetDecolor.hpp \
        Process/OpenCV/photo/COcvDecolor.hpp \
        Process/OpenCV/imgproc/COcvMorphologyEx.hpp \
        Widget/OpenCV/imgproc/COcvWidgetMorphologyEx.hpp \
        Widget/OpenCV/imgproc/COcvWidgetGaussianBlur.hpp \
        Process/OpenCV/imgproc/COcvGaussianBlur.hpp \
        Process/OpenCV/imgproc/COcvCvtColor.hpp \
        Widget/OpenCV/imgproc/COcvWidgetCvtColor.hpp \
        Widget/OpenCV/imgproc/COcvWidgetAdaptiveThreshold.hpp \
        Process/OpenCV/imgproc/COcvAdaptiveThreshold.hpp \
        Process/OpenCV/imgproc/COcvGrabCut.hpp \
        Widget/OpenCV/imgproc/COcvWidgetGrabCut.hpp \
        Widget/OpenCV/imgproc/COcvWidgetDistanceTransform.hpp \
        Process/OpenCV/imgproc/COcvDistanceTransform.hpp \
        Process/OpenCV/imgproc/COcvWatershed.hpp \
        Widget/OpenCV/imgproc/COcvWidgetWatershed.hpp \
        Widget/OpenCV/imgproc/COcvWidgetCalcHist.hpp \
        Process/OpenCV/imgproc/COcvCalcHist.hpp \
        Process/OpenCV/imgproc/COcvEqualizeHist.hpp \
        Widget/OpenCV/imgproc/COcvWidgetEqualizeHist.hpp \
        Widget/OpenCV/imgproc/COcvWidgetResize.hpp \
        Process/OpenCV/imgproc/COcvResize.hpp \
        Process/OpenCV/imgproc/COcvRotateEx.hpp \
        Widget/OpenCV/imgproc/COcvWidgetRotateEx.hpp \
        Widget/OpenCV/imgproc/COcvWidgetColorMap.hpp \
        Process/OpenCV/imgproc/COcvColorMap.hpp \
        Process/OpenCV/ximgproc/COcvFastGlobalSmootherFilter.hpp \
        Widget/OpenCV/ximgproc/COcvWidgetFastGlobalSmootherFilter.hpp \
        Process/OpenCV/ximgproc/COcvFastLineDetector.hpp \
        Widget/OpenCV/ximgproc/COcvWidgetFastLineDetector.hpp \
        Process/OpenCV/core/COcvAddWeighted.hpp \
        Widget/OpenCV/core/COcvWidgetAddWeighted.hpp \
        Process/OpenCV/core/COcvSplit.hpp \
        Widget/OpenCV/core/COcvWidgetSplit.hpp \
        Process/OpenCV/core/COcvMerge.hpp \
        Widget/OpenCV/core/COcvWidgetMerge.hpp \
        Widget/OpenCV/core/COcvWidgetDft.hpp \
        Widget/OpenCV/core/COcvWidgetDftInv.hpp \
        Process/OpenCV/core/COcvDft.hpp \
        Process/OpenCV/core/COcvDftInv.hpp \
        Process/OpenCV/core/COcvFlip.hpp \
        Widget/OpenCV/core/COcvWidgetFlip.hpp \
        Widget/OpenCV/core/COcvWidgetLogicalOp.hpp \
        Process/OpenCV/core/COcvLogicalOp.hpp \
        Process/OpenCV/core/COcvRotate.hpp \
        Widget/OpenCV/core/COcvWidgetRotate.hpp \
        Widget/OpenCV/core/COcvWidgetCrop.hpp \
        Process/OpenCV/core/COcvCrop.hpp \
        Process/OpenCV/core/COcvKMeans.hpp \
        Widget/OpenCV/core/COcvWidgetKMeans.hpp \
        Process/OpenCV/video/COcvCamShift.hpp \
        Widget/OpenCV/video/COcvWidgetCamShift.hpp \
        Widget/OpenCV/video/COcvWidgetMeanShift.hpp \
        Process/OpenCV/video/COcvMeanShift.hpp \
        Process/OpenCV/video/COcvFarnebackOpticalFlow.hpp \
        Process/OpenCV/video/COcvBckgndSubMog2.hpp \
        Process/OpenCV/video/COcvBckgndSubKnn.hpp \
        Widget/OpenCV/video/COcvWidgetBckgndSubKnn.hpp \
        Widget/OpenCV/video/COcvWidgetBckgndSubMog2.hpp \
        Widget/OpenCV/video/COcvWidgetFarnebackOpticalFlow.hpp \
        Process/OpenCV/features2d/COcvBFMatcher.hpp \
        Widget/OpenCV/features2d/COcvWidgetBFMatcher.hpp \
        Process/OpenCV/photo/COcvFastNlMeansMulti.hpp \
        Widget/OpenCV/photo/COcvWidgetFastNlMeansMulti.hpp \
        Process/OpenCV/bgsegm/COcvBckgndSubCnt.hpp \
        Widget/OpenCV/bgsegm/COcvWidgetBckgndSubCnt.hpp \
        Process/OpenCV/bgsegm/COcvBckgndSubGmg.hpp \
        Widget/OpenCV/bgsegm/COcvWidgetBckgndSubGmg.hpp \
        Process/OpenCV/bgsegm/COcvBckgndSubGsoc.hpp \
        Widget/OpenCV/bgsegm/COcvWidgetBckgndSubGsoc.hpp \
        Process/OpenCV/bgsegm/COcvBckgndSubLsbp.hpp \
        Widget/OpenCV/bgsegm/COcvWidgetBckgndSubLsbp.hpp \
        Process/OpenCV/bgsegm/COcvBckgndSubMog.hpp \
        Widget/OpenCV/bgsegm/COcvWidgetBckgndSubMog.hpp \
        Process/OpenCV/features2d/COcvFlannMatcher.hpp \
        Widget/OpenCV/features2d/COcvWidgetFlannMatcher.hpp \
        Process/OpenCV/photo/COcvSeamlessCloning.hpp \
        Widget/OpenCV/photo/COcvWidgetSeamlessCloning.hpp \
        Process/OpenCV/photo/COcvColorchange.hpp \
        Widget/OpenCV/photo/COcvWidgetColorchange.hpp \
        Process/OpenCV/photo/COcvIlluminationChange.hpp \
        Widget/OpenCV/photo/COcvWidgetIlluminationChange.hpp \
        Process/OpenCV/xphoto/COcvLearningBasedWB.hpp \
        Widget/OpenCV/xphoto/COcvWidgetLearningBasedWB.hpp \
        Process/OpenCV/xphoto/COcvSimpleWB.hpp \
        Widget/OpenCV/xphoto/COcvWidgetSimpleWB.hpp \
        Process/OpenCV/dnn/COcvDnnProcess.h \
        Process/OpenCV/dnn/COcvDnnClassifier.hpp \
        Widget/OpenCV/dnn/COcvWidgetDnnProcess.h \
        Widget/OpenCV/dnn/COcvWidgetDnnClassifier.hpp \
        Process/OpenCV/dnn/COcvDnnDetector.hpp \
        Widget/OpenCV/dnn/COcvWidgetDnnDetector.hpp \
        Process/OpenCV/dnn/COcvDnnColorization.hpp \
        Widget/OpenCV/dnn/COcvWidgetDnnColorization.hpp \
        Process/OpenCV/optflow/COcvPCAOpticalFlow.hpp \
        Widget/OpenCV/optflow/COcvWidgetPCAOpticalFlow.hpp \
        Process/OpenCV/imgproc/COcvLaplacian.hpp \
        Widget/OpenCV/imgproc/COcvWidgetLaplacian.hpp \
        Process/OpenCV/core/COcvMultiply.hpp \
        Widget/OpenCV/core/COcvWidgetMultiply.hpp \
        Widget/OpenCV/core/COcvWidgetDivide.hpp \
        Process/OpenCV/core/COcvDivide.hpp \
        Process/OpenCV/core/COcvAbsdiff.hpp \
        Widget/OpenCV/core/COcvWidgetAbsdiff.hpp \
        Process/OpenCV/core/COcvAdd.hpp \
        Widget/OpenCV/core/COcvWidgetAdd.hpp \
        Process/OpenCV/core/COcvSubtract.hpp \
        Process/OpenCV/core/COcvNormalize.hpp \
        Widget/OpenCV/core/COcvWidgetNormalize.hpp \
        Process/OpenCV/core/COcvConvertTo.hpp \
        Widget/OpenCV/core/COcvWidgetConvertTo.hpp \
        Process/OpenCV/bioinspired/COcvRetina.hpp \
        Widget/OpenCV/bioinspired/COcvWidgetRetina.hpp \
        Process/OpenCV/bioinspired/COcvRetinaToneMapping.hpp \
        Widget/OpenCV/bioinspired/COcvWidgetRetinaToneMapping.hpp \
        Process/OpenCV/bioinspired/COcvRetinaSegmentation.hpp \
        Widget/OpenCV/bioinspired/COcvWidgetRetinaSegmentation.hpp \
        Process/OpenCV/saliency/COcvSaliencySpectralResidual.hpp \
        Widget/OpenCV/saliency/COcvWidgetSaliencySpectralResidual.hpp \
        Process/OpenCV/saliency/COcvSaliencyFineGrained.hpp \
        Widget/OpenCV/saliency/COcvWidgetSaliencyFineGrained.hpp \
        Process/OpenCV/saliency/COcvMotionSaliencyBinWangApr2014.hpp \
        Widget/OpenCV/saliency/COcvWidgetMotionSaliencyBinWangApr2014.hpp \
        Process/OpenCV/saliency/COcvObjectnessBING.hpp \
        Widget/OpenCV/saliency/COcvWidgetObjectnessBING.hpp \
        Process/OpenCV/core/COcvMin.hpp \
        Widget/OpenCV/core/COcvWidgetMin.hpp \
        Process/OpenCV/core/COcvMax.hpp \
        Widget/OpenCV/core/COcvWidgetMax.hpp \
        Process/OpenCV/ximgproc/COcvGuidedFilter.hpp \
        Widget/OpenCV/ximgproc/COcvWidgetGuidedFilter.hpp \
        Process/OpenCV/core/COcvExp.hpp \
        Widget/OpenCV/core/COcvWidgetExp.hpp \
        Process/OpenCV/core/COcvLog.hpp \
        Widget/OpenCV/core/COcvWidgetLog.hpp \
        Widget/OpenCV/ximgproc/COcvWidgetRidgeFilter.hpp \
        Process/OpenCV/ximgproc/COcvRidgeFilter.hpp \
        Process/OpenCV/optflow/COcvSimpleFlow.hpp \
        Widget/OpenCV/optflow/COcvWidgetSimpleFlow.hpp \
        Widget/OpenCV/optflow/COcvWidgetSparseToDenseOF.hpp \
        Process/OpenCV/optflow/COcvSparseToDenseOF.hpp \
        Process/OpenCV/optflow/COcvDeepFlow.hpp \
        Widget/OpenCV/optflow/COcvWidgetDeepFlow.hpp \
        Process/OpenCV/ximgproc/COcvDTFilterStylize.hpp \
        Widget/OpenCV/ximgproc/COcvWidgetDTFilterStylize.hpp \
        Process/OpenCV/ximgproc/COcvDTFilter.hpp \
        Widget/OpenCV/ximgproc/COcvWidgetDTFilter.hpp \
        Process/OpenCV/ximgproc/COcvDTFilterEnhance.hpp \
        Widget/OpenCV/ximgproc/COcvWidgetDTFilterEnhance.hpp \
        Process/OpenCV/ximgproc/COcvJointBilateralFilter.hpp \
        Widget/OpenCV/ximgproc/COcvWidgetJointBilateralFilter.hpp \
        Process/OpenCV/objdetect/COcvQRCodeDetector.hpp \
        Widget/OpenCV/objdetect/COcvWidgetQRCodeDetector.hpp \
        Process/OpenCV/superres/COcvSuperResBTVL1.h \
        Process/OpenCV/superres/CSuperResFrameSrc.h \
        Widget/OpenCV/superres/COcvWidgetSuperResBTVL1.hpp \
        Process/OpenCV/photo/COcvDenoiseTVL1.hpp \
        Widget/OpenCV/photo/COcvWidgetDenoiseTVL1.hpp \
        Process/OpenCV/imgproc/COcvCanny.hpp \
        Widget/OpenCV/imgproc/COcvWidgetCanny.hpp \
        Process/OpenCV/text/COcvOCRTesseract.hpp \
        Widget/OpenCV/text/COcvWidgetOCRTesseract.hpp \
        Process/OpenCV/ximgproc/COcvSuperpixelLSC.hpp \
        Widget/OpenCV/ximgproc/COcvWidgetSuperpixelLSC.hpp \
        Process/OpenCV/ximgproc/COcvSuperpixelSEEDS.hpp \
        Widget/OpenCV/ximgproc/COcvWidgetSuperpixelSEEDS.hpp \
        Process/OpenCV/ximgproc/COcvSuperpixelSLIC.hpp \
        Widget/OpenCV/ximgproc/COcvWidgetSuperpixelSLIC.hpp \
        Process/OpenCV/ximgproc/COcvStructuredEdgeDetection.hpp \
        Widget/OpenCV/ximgproc/COcvWidgetStructuredEdgeDetection.hpp \
        Core/CVideoOFTask.h \
        Core/CVideoTrackingTask.h \
        Process/Core/CFillHoles.hpp \
        Core/CWidgetFactory.hpp \
        Widget/Core/CWidgetFillHoles.hpp \
        Process/Core/CCut.hpp \
        Widget/Core/CWidgetCut.hpp \
        Widget/OpenCV/core/COcvWidgetSubtract.hpp \
        Widget/OpenCV/dnn/COcvWidgetDnnSegmentation.hpp \
        Process/OpenCV/ximgproc/COcvGraphSegmentation.hpp \
        Widget/OpenCV/ximgproc/COcvWidgetGraphSegmentation.hpp \
        Process/OpenCV/ximgproc/COcvSelectiveSearchSegmentation.hpp \
        Widget/OpenCV/ximgproc/COcvWidgetSelectiveSearchSegmentation.hpp \
        Process/OpenCV/imgproc/COcvThreshold.hpp \
        Widget/OpenCV/imgproc/COcvWidgetThreshold.hpp \
        Process/OpenCV/imgproc/COcvCLAHE.hpp \
        Widget/OpenCV/imgproc/COcvWidgetCLAHE.hpp \
        Process/OpenCV/core/COcvInRange.hpp \
        Widget/OpenCV/core/COcvWidgetInRange.hpp \
        Process/OpenCV/objdetect/COcvCascadeClassifier.hpp \
        Widget/OpenCV/objdetect/COcvWidgetCascadeClassifier.hpp \
        Process/OpenCV/core/COcvNegative.hpp \
        Widget/OpenCV/core/COcvWidgetNegative.hpp \
        Process/OpenCV/imgproc/COcvMedian.hpp \
        Widget/OpenCV/imgproc/COcvWidgetMedian.hpp \
        Process/OpenCV/dnn/COcvDnnSegmentation.h \
        Process/OpenCV/video/COcvDISOpticalFlow.hpp \
        Process/OpenCV/optflow/COcvDualTVL1OpticalFlow.hpp \
        Widget/OpenCV/video/COcvWidgetDISOpticalFlow.hpp \
        Widget/OpenCV/optflow/COcvWidgetDualTVL1OpticalFlow.hpp

#Use of 'lupdate DataProcess.pro' to update translation files
#Use of 'lrelease DataProcess.pro' to generate .qm files
TRANSLATIONS = Translations/dataprocess_fr.ts

#Make install directive
target.path = ../Build/Lib

deployInclude.path = ../Build/Include/DataProcess
deployInclude.files = \
        CPluginProcessInterface.hpp \
        DataProcessGlobal.hpp

deployIncludeCore.path = ../Build/Include/DataProcess/Core
deployIncludeCore.files = \
        Core/CWorkflow.h \
        Core/CTaskFactory.hpp \
        Core/C2dImageInteractiveTask.h \
        Core/C2dImageTask.h \
        Core/CVideoTask.h \
        Core/CVideoOFTask.h \
        Core/CVideoTrackingTask.h \
        Core/CFeatureImageProcess2d.h \
        Core/CFeatureMatcher2d.h \
        Core/CTaskInfo.h \
        Core/CWidgetFactory.hpp \
        Core/CDnnTrainTask.h \
        Core/CMlflowTrainTask.h \
        Core/CRunTaskManager.h

deployIncludeIO.path = ../Build/Include/DataProcess/IO
deployIncludeIO.files = \
        IO/CImageIO.h \
        IO/CBlobMeasureIO.h \
        IO/CGraphicsInput.h \
        IO/CGraphicsOutput.h \
        IO/CGraphicsProcessIO.hpp \
        IO/CTaskIORegistration.h \
        IO/CNumericIO.h \
        IO/CVideoIO.h \
        IO/CViewPropertyIO.h \
        IO/CDatasetIO.h

deployIncludeProcessCore.path = ../Build/Include/DataProcess/Process/Core
deployIncludeProcessCore.files = \
        Process/Core/CBlobMeasure.h

deployIncludeProcessCvDnn.path = ../Build/Include/DataProcess/Process/OpenCV/dnn
deployIncludeProcessCvDnn.files = \
        Process/OpenCV/dnn/COcvDnnProcess.h

deployIncludeWidgetCvDnn.path = ../Build/Include/DataProcess/Widget/OpenCV/dnn
deployIncludeWidgetCvDnn.files = \
        Widget/OpenCV/dnn/COcvWidgetDnnCore.h

INSTALLS += target
INSTALLS += deployInclude
INSTALLS += deployIncludeCore
INSTALLS += deployIncludeIO
INSTALLS += deployIncludeProcessCore
INSTALLS += deployIncludeProcessCvDnn
INSTALLS += deployIncludeWidgetCvDnn

#Windows SDK DLLs
win32: LIBS += -lshell32

LIBS += $$link_boost()

#Dynamic link to OpenCV
win32:CONFIG(release, debug|release): LIBS += -lopencv_core$${OPENCV_VERSION} -lopencv_imgproc$${OPENCV_VERSION} -lopencv_objdetect$${OPENCV_VERSION} -lopencv_photo$${OPENCV_VERSION} -lopencv_ximgproc$${OPENCV_VERSION} -lopencv_highgui$${OPENCV_VERSION}
win32:CONFIG(release, debug|release): LIBS += -lopencv_xphoto$${OPENCV_VERSION} -lopencv_fuzzy$${OPENCV_VERSION} -lopencv_hfs$${OPENCV_VERSION} -lopencv_dnn$${OPENCV_VERSION} -lopencv_tracking$${OPENCV_VERSION} -lopencv_video$${OPENCV_VERSION}
win32:CONFIG(release, debug|release): LIBS += -lopencv_bgsegm$${OPENCV_VERSION} -lopencv_optflow$${OPENCV_VERSION} -lopencv_bioinspired$${OPENCV_VERSION} -lopencv_saliency$${OPENCV_VERSION} -lopencv_superres$${OPENCV_VERSION} -lopencv_text$${OPENCV_VERSION}
win32:CONFIG(release, debug|release): LIBS += -lopencv_features2d$${OPENCV_VERSION}
win32:!ik_cpu:CONFIG(release, debug|release): LIBS += -lopencv_cudawarping$${OPENCV_VERSION}
win32:CONFIG(debug, debug|release): LIBS += -lopencv_core$${OPENCV_VERSION}d -lopencv_imgproc$${OPENCV_VERSION}d -lopencv_objdetect$${OPENCV_VERSION}d -lopencv_photo$${OPENCV_VERSION}d -lopencv_ximgproc$${OPENCV_VERSION}d -lopencv_highgui$${OPENCV_VERSION}d
win32:CONFIG(debug, debug|release): LIBS += -lopencv_xphoto$${OPENCV_VERSION}d -lopencv_fuzzy$${OPENCV_VERSION}d -lopencv_hfs$${OPENCV_VERSION}d -lopencv_dnn$${OPENCV_VERSION}d -lopencv_tracking$${OPENCV_VERSION}d -lopencv_video$${OPENCV_VERSION}d
win32:CONFIG(debug, debug|release): LIBS += -lopencv_bgsegm$${OPENCV_VERSION}d -lopencv_optflow$${OPENCV_VERSION}d -lopencv_bioinspired$${OPENCV_VERSION}d -lopencv_saliency$${OPENCV_VERSION}d -lopencv_superres$${OPENCV_VERSION}d -lopencv_text$${OPENCV_VERSION}d
win32:CONFIG(debug, debug|release): LIBS += -lopencv_features2d$${OPENCV_VERSION}d
win32:!ik_cpu:CONFIG(debug, debug|release): LIBS += -lopencv_cudawarping$${OPENCV_VERSION}d

unix: LIBS += -lopencv_core -lopencv_imgproc -lopencv_objdetect -lopencv_photo -lopencv_ximgproc -lopencv_highgui
unix: LIBS += -lopencv_xphoto -lopencv_fuzzy -lopencv_hfs -lopencv_dnn -lopencv_tracking -lopencv_video
unix: LIBS += -lopencv_bgsegm -lopencv_optflow -lopencv_bioinspired -lopencv_saliency -lopencv_superres -lopencv_text
unix: LIBS += -lopencv_face -lopencv_features2d
unix: !ik_cpu: LIBS += -lopencv_cudawarping

#Dynamic link to gmic
LIBS += -lgmic

LIBS += $$link_utils()
INCLUDEPATH += $$PWD/../Utils
DEPENDPATH += $$PWD/../Utils

LIBS += $$link_core()
INCLUDEPATH += $$PWD/../Core
DEPENDPATH += $$PWD/../Core

LIBS += $$link_dataio()
INCLUDEPATH += $$PWD/../DataIO
DEPENDPATH += $$PWD/../DataIO
