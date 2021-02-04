#ifndef OCV_HPP
#define OCV_HPP

#include "COcvInpaintFuzzy.hpp"
#include "COcvHfsSegment.hpp"
/********************
 *CORE
********************/
#include "core/COcvAbsdiff.hpp"
#include "core/COcvAdd.hpp"
#include "core/COcvAddWeighted.hpp"
#include "core/COcvCompare.hpp"
#include "core/COcvConvertTo.hpp"
#include "core/COcvCopyMakeBorder.hpp"
#include "core/COcvCountNonZero.hpp"
#include "core/COcvCrop.hpp"
#include "core/COcvDft.hpp"
#include "core/COcvDftInv.hpp"
#include "core/COcvDivide.hpp"
#include "core/COcvExp.hpp"
#include "core/COcvExtractChannel.hpp"
#include "core/COcvFlip.hpp"
#include "core/COcvInRange.hpp"
#include "core/COcvInsertChannel.hpp"
#include "core/COcvKMeans.hpp"
#include "core/COcvLog.hpp"
#include "core/COcvLogicalOp.hpp"
#include "core/COcvMagnitude.hpp"
#include "core/COcvMax.hpp"
#include "core/COcvMerge.hpp"
#include "core/COcvMin.hpp"
#include "core/COcvMultiply.hpp"
#include "core/COcvNormalize.hpp"
#include "core/COcvPSNR.hpp"
#include "core/COcvRotate.hpp"
#include "core/COcvSplit.hpp"
#include "core/COcvSubtract.hpp"
#include "core/COcvNegative.hpp"
/********************
 * DNN
********************/
#include "dnn/COcvDnnClassifier.hpp"
#include "dnn/COcvDnnColorization.hpp"
#include "dnn/COcvDnnDetector.hpp"
#include "dnn/COcvDnnSegmentation.h"
/********************
 *FEATURES2D
********************/
#include "features2d/COcvORB.hpp"
#include "features2d/COcvAKAZE.hpp"
#include "features2d/COcvBRISK.hpp"
#include "features2d/COcvFAST.hpp"
#include "features2d/COcvGFTT.hpp"
#include "features2d/COcvAGAST.hpp"
#include "features2d/COcvKAZE.hpp"
#include "features2d/COcvSIFT.hpp"
#include "features2d/COcvSimpleBlobDetector.hpp"
#include "features2d/COcvBFMatcher.hpp"
#include "features2d/COcvFlannMatcher.hpp"
/********************
 *IMGPROC
********************/
#include "imgproc/COcvAdaptiveThreshold.hpp"
#include "imgproc/COcvBilateral.hpp"
#include "imgproc/COcvBoxFilter.hpp"
#include "imgproc/COcvCalcHist.hpp"
#include "imgproc/COcvColorMap.hpp"
#include "imgproc/COcvCvtColor.hpp"
#include "imgproc/COcvDistanceTransform.hpp"
#include "imgproc/COcvEqualizeHist.hpp"
#include "imgproc/COcvGaussianBlur.hpp"
#include "imgproc/COcvGrabCut.hpp"
#include "imgproc/COcvHoughLines.hpp"
#include "imgproc/COcvHoughCircles.hpp"
#include "imgproc/COcvMorphologyEx.hpp"
#include "imgproc/COcvResize.hpp"
#include "imgproc/COcvRotateEx.hpp"
#include "imgproc/COcvSobel.hpp"
#include "imgproc/COcvWatershed.hpp"
#include "imgproc/COcvLaplacian.hpp"
#include "imgproc/COcvCanny.hpp"
#include "imgproc/COcvThreshold.hpp"
#include "imgproc/COcvCLAHE.hpp"
#include "imgproc/COcvMedian.hpp"
/********************
 *PHOTO
********************/
#include "photo/COcvDecolor.hpp"
#include "photo/COcvDetailEnhance.hpp"
#include "photo/COcvEdgePreservingFilter.hpp"
#include "photo/COcvFastNlMeans.hpp"
#include "photo/COcvFastNlMeansMulti.hpp"
#include "photo/COcvInpaint.hpp"
#include "photo/COcvPencilSketch.hpp"
#include "photo/COcvStylization.hpp"
#include "photo/COcvTextureFlattening.hpp"
#include "photo/COcvSeamlessCloning.hpp"
#include "photo/COcvColorchange.hpp"
#include "photo/COcvIlluminationChange.hpp"
#include "photo/COcvDenoiseTVL1.hpp"
/********************
 *TRACKING
********************/
#include "tracking/COcvTrackerGOTURN.hpp"
#include "tracking/COcvTrackerKCF.hpp"
/********************
 *VIDEO
********************/
#include "video/COcvBckgndSubKnn.hpp"
#include "video/COcvBckgndSubMog2.hpp"
#include "video/COcvCamShift.hpp"
#include "video/COcvDISOpticalFlow.hpp"
#include "video/COcvFarnebackOpticalFlow.hpp"
#include "video/COcvMeanShift.hpp"
/********************
 *BGSEGM
********************/
#include "bgsegm/COcvBckgndSubCnt.hpp"
#include "bgsegm/COcvBckgndSubGmg.hpp"
#include "bgsegm/COcvBckgndSubGsoc.hpp"
#include "bgsegm/COcvBckgndSubLsbp.hpp"
#include "bgsegm/COcvBckgndSubMog.hpp"
/********************
 *XIMGPROC
********************/
#include "ximgproc/COcvAdaptiveManifold.hpp"
#include "ximgproc/COcvAnisotropicDiffusion.hpp"
#include "ximgproc/COcvBilateralTextureFilter.hpp"
#include "ximgproc/COcvFastGlobalSmootherFilter.hpp"
#include "ximgproc/COcvFastLineDetector.hpp"
#include "ximgproc/COcvGradientDeriche.hpp"
#include "ximgproc/COcvGradientPaillou.hpp"
#include "ximgproc/COcvJointBilateralFilter.hpp"
#include "ximgproc/COcvL0Smooth.hpp"
#include "ximgproc/COcvNiblackThreshold.hpp"
#include "ximgproc/COcvPeiLinNormalization.hpp"
#include "ximgproc/COcvRollingGuidanceFilter.hpp"
#include "ximgproc/COcvThinning.hpp"
#include "ximgproc/COcvGuidedFilter.hpp"
#include "ximgproc/COcvRidgeFilter.hpp"
#include "ximgproc/COcvDTFilterStylize.hpp"
#include "ximgproc/COcvDTFilter.hpp"
#include "ximgproc/COcvDTFilterEnhance.hpp"
#include "ximgproc/COcvSuperpixelLSC.hpp"
#include "ximgproc/COcvSuperpixelSEEDS.hpp"
#include "ximgproc/COcvSuperpixelSLIC.hpp"
#include "ximgproc/COcvStructuredEdgeDetection.hpp"
#include "ximgproc/COcvGraphSegmentation.hpp"
#include "ximgproc/COcvSelectiveSearchSegmentation.hpp"
/********************
 *XPHOTO
********************/
#include "xphoto/COcvGrayworldWB.hpp"
#include "xphoto/COcvInpaintX.hpp"
#include "xphoto/COcvLearningBasedWB.hpp"
#include "xphoto/COcvSimpleWB.hpp"
#include "xphoto/COcvTonemapDurand.hpp"
/********************
 *OPTFLOW
********************/
#include "optflow/COcvDualTVL1OpticalFlow.hpp"
#include "optflow/COcvPCAOpticalFlow.hpp"
#include "optflow/COcvSimpleFlow.hpp"
#include "optflow/COcvSparseToDenseOF.hpp"
#include "optflow/COcvDeepFlow.hpp"
/********************
 *BIOINSPIRED
********************/
#include "bioinspired/COcvRetina.hpp"
#include "bioinspired/COcvRetinaToneMapping.hpp"
#include "bioinspired/COcvRetinaSegmentation.hpp"
/********************
 *SALIENCY
********************/
#include "saliency/COcvSaliencySpectralResidual.hpp"
#include "saliency/COcvSaliencyFineGrained.hpp"
#include "saliency/COcvMotionSaliencyBinWangApr2014.hpp"
#include "saliency/COcvObjectnessBING.hpp"
/********************
 *SUPERRES
********************/
#include "superres/COcvSuperResBTVL1.h"
/********************
 *OBJDETECT
********************/
#include "objdetect/COcvCascadeClassifier.hpp"
#include "objdetect/COcvQRCodeDetector.hpp"
/********************
 *TEXT
********************/
#include "text/COcvOCRTesseract.hpp"

#endif // OCV_HPP
