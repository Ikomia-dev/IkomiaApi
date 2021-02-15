// Copyright (C) 2021 Ikomia SAS
// Contact: https://www.ikomia.com
//
// This file is part of the Ikomia API libraries.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#ifndef OCVWIDGET_HPP
#define OCVWIDGET_HPP

#include "COcvWidgetInpaintFuzzy.hpp"
#include "COcvWidgetHfsSegment.hpp"
/********************
 *CORE
********************/
#include "core/COcvWidgetAbsdiff.hpp"
#include "core/COcvWidgetAdd.hpp"
#include "core/COcvWidgetAddWeighted.hpp"
#include "core/COcvWidgetCompare.hpp"
#include "core/COcvWidgetConvertTo.hpp"
#include "core/COcvWidgetCopyMakeBorder.hpp"
#include "core/COcvWidgetCountNonZero.hpp"
#include "core/COcvWidgetCrop.hpp"
#include "core/COcvWidgetDft.hpp"
#include "core/COcvWidgetDftInv.hpp"
#include "core/COcvWidgetDivide.hpp"
#include "core/COcvWidgetExp.hpp"
#include "core/COcvWidgetExtractChannel.hpp"
#include "core/COcvWidgetFlip.hpp"
#include "core/COcvWidgetInRange.hpp"
#include "core/COcvWidgetInsertChannel.hpp"
#include "core/COcvWidgetKMeans.hpp"
#include "core/COcvWidgetLog.hpp"
#include "core/COcvWidgetLogicalOp.hpp"
#include "core/COcvWidgetMagnitude.hpp"
#include "core/COcvWidgetMax.hpp"
#include "core/COcvWidgetMerge.hpp"
#include "core/COcvWidgetMin.hpp"
#include "core/COcvWidgetMultiply.hpp"
#include "core/COcvWidgetNormalize.hpp"
#include "core/COcvWidgetPSNR.hpp"
#include "core/COcvWidgetRotate.hpp"
#include "core/COcvWidgetSplit.hpp"
#include "core/COcvWidgetSubtract.hpp"
#include "core/COcvWidgetNegative.hpp"
/********************
 *DNN
********************/
#include "dnn/COcvWidgetDnnClassifier.hpp"
#include "dnn/COcvWidgetDnnColorization.hpp"
#include "dnn/COcvWidgetDnnDetector.hpp"
#include "dnn/COcvWidgetDnnSegmentation.hpp"
/********************
 *FEATURES2D
********************/
#include "features2d/COcvWidgetORB.hpp"
#include "features2d/COcvWidgetAKAZE.hpp"
#include "features2d/COcvWidgetBRISK.hpp"
#include "features2d/COcvWidgetFAST.hpp"
#include "features2d/COcvWidgetGFTT.hpp"
#include "features2d/COcvWidgetAGAST.hpp"
#include "features2d/COcvWidgetKAZE.hpp"
#include "features2d/COcvWidgetSIFT.hpp"
#include "features2d/COcvWidgetSimpleBlobDetector.hpp"
#include "features2d/COcvWidgetBFMatcher.hpp"
#include "features2d/COcvWidgetFlannMatcher.hpp"
/********************
 *IMGPROC
********************/
#include "imgproc/COcvWidgetAdaptiveThreshold.hpp"
#include "imgproc/COcvWidgetBilateral.hpp"
#include "imgproc/COcvWidgetBoxFilter.hpp"
#include "imgproc/COcvWidgetCalcHist.hpp"
#include "imgproc/COcvWidgetColorMap.hpp"
#include "imgproc/COcvWidgetCvtColor.hpp"
#include "imgproc/COcvWidgetDistanceTransform.hpp"
#include "imgproc/COcvWidgetEqualizeHist.hpp"
#include "imgproc/COcvWidgetGaussianBlur.hpp"
#include "imgproc/COcvWidgetGrabCut.hpp"
#include "imgproc/COcvWidgetHoughLines.hpp"
#include "imgproc/COcvWidgetHoughCircles.hpp"
#include "imgproc/COcvWidgetMorphologyEx.hpp"
#include "imgproc/COcvWidgetResize.hpp"
#include "imgproc/COcvWidgetRotateEx.hpp"
#include "imgproc/COcvWidgetSobel.hpp"
#include "imgproc/COcvWidgetWatershed.hpp"
#include "imgproc/COcvWidgetLaplacian.hpp"
#include "imgproc/COcvWidgetCanny.hpp"
#include "imgproc/COcvWidgetThreshold.hpp"
#include "imgproc/COcvWidgetCLAHE.hpp"
#include "imgproc/COcvWidgetMedian.hpp"
/********************
 *PHOTO
********************/
#include "photo/COcvWidgetDecolor.hpp"
#include "photo/COcvWidgetDetailEnhance.hpp"
#include "photo/COcvWidgetEdgePreservingFilter.hpp"
#include "photo/COcvWidgetFastNlMeans.hpp"
#include "photo/COcvWidgetFastNlMeansMulti.hpp"
#include "photo/COcvWidgetInpaint.hpp"
#include "photo/COcvWidgetPencilSketch.hpp"
#include "photo/COcvWidgetStylization.hpp"
#include "photo/COcvWidgetTextureFlattening.hpp"
#include "photo/COcvWidgetTonemapDurand.hpp"
#include "photo/COcvWidgetSeamlessCloning.hpp"
#include "photo/COcvWidgetColorchange.hpp"
#include "photo/COcvWidgetIlluminationChange.hpp"
#include "photo/COcvWidgetDenoiseTVL1.hpp"
/********************
 *TRACKING
********************/
#include "tracking/COcvWidgetTrackerGOTURN.hpp"
#include "tracking/COcvWidgetTrackerKCF.hpp"
/********************
 *VIDEO
********************/
#include "video/COcvWidgetBckgndSubKnn.hpp"
#include "video/COcvWidgetBckgndSubMog2.hpp"
#include "video/COcvWidgetCamShift.hpp"
#include "video/COcvWidgetDISOpticalFlow.hpp"
#include "video/COcvWidgetFarnebackOpticalFlow.hpp"
#include "video/COcvWidgetMeanShift.hpp"
/********************
 *BGSEGM
********************/
#include "bgsegm/COcvWidgetBckgndSubCnt.hpp"
#include "bgsegm/COcvWidgetBckgndSubGmg.hpp"
#include "bgsegm/COcvWidgetBckgndSubGsoc.hpp"
#include "bgsegm/COcvWidgetBckgndSubLsbp.hpp"
#include "bgsegm/COcvWidgetBckgndSubMog.hpp"
/********************
 *XPHOTO
********************/
#include "xphoto/COcvWidgetInpaintX.hpp"
#include "xphoto/COcvWidgetGrayworldWB.hpp"
#include "xphoto/COcvWidgetLearningBasedWB.hpp"
#include "xphoto/COcvWidgetSimpleWB.hpp"
/********************
 *XIMGPROC
********************/
#include "ximgproc/COcvWidgetAdaptiveManifold.hpp"
#include "ximgproc/COcvWidgetAnisotropicDiffusion.hpp"
#include "ximgproc/COcvWidgetBilateralTextureFilter.hpp"
#include "ximgproc/COcvWidgetFastGlobalSmootherFilter.hpp"
#include "ximgproc/COcvWidgetFastLineDetector.hpp"
#include "ximgproc/COcvWidgetGradientDeriche.hpp"
#include "ximgproc/COcvWidgetGradientPaillou.hpp"
#include "ximgproc/COcvWidgetJointBilateralFilter.hpp"
#include "ximgproc/COcvWidgetL0Smooth.hpp"
#include "ximgproc/COcvWidgetNiblackThreshold.hpp"
#include "ximgproc/COcvWidgetPeiLinNormalization.hpp"
#include "ximgproc/COcvWidgetRollingGuidanceFilter.hpp"
#include "ximgproc/COcvWidgetThinning.hpp"
#include "ximgproc/COcvWidgetGuidedFilter.hpp"
#include "ximgproc/COcvWidgetRidgeFilter.hpp"
#include "ximgproc/COcvWidgetDTFilterStylize.hpp"
#include "ximgproc/COcvWidgetDTFilter.hpp"
#include "ximgproc/COcvWidgetDTFilterEnhance.hpp"
#include "ximgproc/COcvWidgetSuperpixelLSC.hpp"
#include "ximgproc/COcvWidgetSuperpixelSEEDS.hpp"
#include "ximgproc/COcvWidgetSuperpixelSLIC.hpp"
#include "ximgproc/COcvWidgetStructuredEdgeDetection.hpp"
#include "ximgproc/COcvWidgetGraphSegmentation.hpp"
#include "ximgproc/COcvWidgetSelectiveSearchSegmentation.hpp"
/********************
 *OPTFLOW
********************/
#include "optflow/COcvWidgetDualTVL1OpticalFlow.hpp"
#include "optflow/COcvWidgetPCAOpticalFlow.hpp"
#include "optflow/COcvWidgetSimpleFlow.hpp"
#include "optflow/COcvWidgetSparseToDenseOF.hpp"
#include "optflow/COcvWidgetDeepFlow.hpp"
/********************
 *BIOINSPIRED
********************/
#include "bioinspired/COcvWidgetRetina.hpp"
#include "bioinspired/COcvWidgetRetinaToneMapping.hpp"
#include "bioinspired/COcvWidgetRetinaSegmentation.hpp"
/********************
 *SALIENCY
********************/
#include "saliency/COcvWidgetSaliencySpectralResidual.hpp"
#include "saliency/COcvWidgetSaliencyFineGrained.hpp"
#include "saliency/COcvWidgetMotionSaliencyBinWangApr2014.hpp"
#include "saliency/COcvWidgetObjectnessBING.hpp"
/********************
 *SUPERRES
********************/
#include "superres/COcvWidgetSuperResBTVL1.hpp"
/********************
 *OBJDETECT
********************/
#include "objdetect/COcvWidgetCascadeClassifier.hpp"
#include "objdetect/COcvWidgetQRCodeDetector.hpp"
/********************
 *TEXT
********************/
#include "text/COcvWidgetOCRTesseract.hpp"

#endif // OCVLAYOUT_HPP
