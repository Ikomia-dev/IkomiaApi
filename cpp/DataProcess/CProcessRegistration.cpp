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

#include "CProcessRegistration.h"
#include "Process/Core/ProcessCore.hpp"
#include "Process/OpenCV/ProcessOcv.hpp"
#include "Process/Gmic/ProcessGmic.hpp"
#include "Widget/Core/WidgetCore.hpp"
#include "Widget/OpenCV/WidgetOcv.hpp"
#include "Widget/Gmic/WidgetGmic.hpp"

CProcessRegistration::CProcessRegistration()
{
    CPyEnsureGIL gil;
    registerCore();
    registerOpenCV();
    registerGmic();
}

CProcessRegistration::~CProcessRegistration()
{
    CPyEnsureGIL gil;
    m_processFactory.clear();
    m_widgetFactory.clear();
}

void CProcessRegistration::registerProcess(const std::shared_ptr<CTaskFactory>& pProcessFactory,
                                           const std::shared_ptr<CWidgetFactory>& pWidgetFactory)
{
    assert(pProcessFactory && pWidgetFactory);

    auto name = pProcessFactory->getInfo().getName();
    if(m_processFactory.isCreatorExists(name) == true)
    {
        m_processFactory.unregisterCreator(name);
        m_processFactory.remove(name);
        m_widgetFactory.unregisterCreator(name);
        m_widgetFactory.remove(name);
    }

    m_processFactory.getList().push_back(pProcessFactory);
    //Passage par lambda -> pFactory par valeur pour assurer la portée du pointeur
    auto pProcessFunc = [pProcessFactory](const WorkflowTaskParamPtr& param)
    {
        return pProcessFactory->create(param);
    };
    m_processFactory.registerCreator(pProcessFactory->getInfo().m_name, pProcessFunc);

    m_widgetFactory.getList().push_back(pWidgetFactory);
    auto pWidgetFunc = [pWidgetFactory](const WorkflowTaskParamPtr& param)
    {
        return pWidgetFactory->create(param);
    };
    m_widgetFactory.registerCreator(pWidgetFactory->getName(), pWidgetFunc);

    //Pour mémoire
    //Passage par std::bind -> cast nécessaire car 2 méthodes create() existent
    //auto pFunc = static_cast<std::shared_ptr<CWorkflowTask> (CTaskFactory::*)(const std::shared_ptr<CWorkflowTaskParam>&)>(&CTaskFactory::create);
    //m_factory.registerCreator(pFactory->name(), std::bind(pFunc, pFactory, std::placeholders::_1));
}

void CProcessRegistration::unregisterProcess(const std::string &name)
{
    m_processFactory.remove(name);
    m_widgetFactory.remove(name);
}

WorkflowTaskPtr CProcessRegistration::createProcessObject(const std::string &name, const WorkflowTaskParamPtr &paramPtr)
{
    WorkflowTaskPtr taskPtr = nullptr;
    try
    {
        taskPtr = m_processFactory.createObject(name, std::move(paramPtr));
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException());
    }
    return taskPtr;
}

WorkflowTaskWidgetPtr CProcessRegistration::createWidgetObject(const std::string &name, const WorkflowTaskParamPtr &paramPtr)
{
    WorkflowTaskWidgetPtr widgetPtr = nullptr;
    try
    {
        widgetPtr = m_widgetFactory.createObject(name, std::move(paramPtr));
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException());
    }
    return widgetPtr;
}

void CProcessRegistration::reset()
{
    m_processFactory.getList().clear();
    m_widgetFactory.getList().clear();
    registerCore();
    registerOpenCV();
    registerGmic();
}

void CProcessRegistration::registerCore()
{
    registerProcess(std::make_shared<CBlobMeasureFactory>(), std::make_shared<CWidgetBlobMeasureFactory>());
    registerProcess(std::make_shared<CCutFactory>(), std::make_shared<CWidgetCutFactory>());
    registerProcess(std::make_shared<CFillHolesFactory>(), std::make_shared<CWidgetFillHolesFactory>());
    registerProcess(std::make_shared<CPlotMergeFactory>(), std::make_shared<CWidgetPlotMergeFactory>());
    registerProcess(std::make_shared<CRgbHlsThresholdFactory>(), std::make_shared<CWidgetRgbHlsThresholdFactory>());
    registerProcess(std::make_shared<CBinaryToGraphicsFactory>(), std::make_shared<CWidgetBinaryToGraphicsFactory>());
    registerProcess(std::make_shared<CGraphicsToBinaryFactory>(), std::make_shared<CWidgetGraphicsToBinaryFactory>());
}

void CProcessRegistration::registerOpenCV()
{
    registerCvCore();
    registerCvDnn();
    registerCvFeatures2d();
    registerCvImgproc();
    registerCvPhoto();
    registerCvTracking();
    registerCvVideo();
    registerCvBgsegm();
    registerCvXimgproc();
    registerCvXphoto();
    registerCvOptflow();
    registerCvBioinspired();
    registerCvSaliency();
    registerCvSuperres();
    registerCvObjdetect();
    registerCvText();

    registerProcess(std::make_shared<COcvInpaintFuzzyFactory>(), std::make_shared<COcvWidgetInpaintFuzzyFactory>());
    registerProcess(std::make_shared<COcvHfsSegmentFactory>(), std::make_shared<COcvWidgetHfsSegmentFactory>());
}

void CProcessRegistration::registerGmic()
{
    //Colors
    registerProcess(std::make_shared<CGmicAutoBalanceFactory>(), std::make_shared<CGmicWidgetAutoBalanceFactory>());
    registerProcess(std::make_shared<CGmicBoostChromaFactory>(), std::make_shared<CGmicWidgetBoostChromaFactory>());
    registerProcess(std::make_shared<CGmicBoostFadeFactory>(), std::make_shared<CGmicWidgetBoostFadeFactory>());
    registerProcess(std::make_shared<CGmicColorPresetsFactory>(), std::make_shared<CGmicWidgetColorPresetsFactory>());

    //Contours
    registerProcess(std::make_shared<CGmicDoGFactory>(), std::make_shared<CGmicWidgetDoGFactory>());
    registerProcess(std::make_shared<CGmicDistanceTransformFactory>(), std::make_shared<CGmicWidgetDistanceTransformFactory>());
    registerProcess(std::make_shared<CGmicSkeletonFactory>(), std::make_shared<CGmicWidgetSkeletonFactory>());
    registerProcess(std::make_shared<CGmicSuperPixelsFactory>(), std::make_shared<CGmicWidgetSuperPixelsFactory>());

    //Details
    registerProcess(std::make_shared<CGmicConstrainedSharpenFactory>(), std::make_shared<CGmicWidgetConstrainedSharpenFactory>());
    registerProcess(std::make_shared<CGmicDynamicRangeIncreaseFactory>(), std::make_shared<CGmicWidgetDynamicRangeIncreaseFactory>());
    registerProcess(std::make_shared<CGmicMagicDetailsFactory>(), std::make_shared<CGmicWidgetMagicDetailsFactory>());
    registerProcess(std::make_shared<CGmicSharpenDeblurFactory>(), std::make_shared<CGmicWidgetSharpenDeblurFactory>());
    registerProcess(std::make_shared<CGmicSharpenGradientFactory>(), std::make_shared<CGmicWidgetSharpenGradientFactory>());
    registerProcess(std::make_shared<CGmicSharpenLucyFactory>(), std::make_shared<CGmicWidgetSharpenLucyFactory>());
    registerProcess(std::make_shared<CGmicSharpenTonesFactory>(), std::make_shared<CGmicWidgetSharpenTonesFactory>());

    //Repair
    registerProcess(std::make_shared<CGmicHotPixelsFactory>(), std::make_shared<CGmicWidgetHotPixelsFactory>());
    registerProcess(std::make_shared<CGmicInpaintFactory>(), std::make_shared<CGmicWidgetInpaintFactory>());
    registerProcess(std::make_shared<CGmicRedEyeFactory>(), std::make_shared<CGmicWidgetRedEyeFactory>());
}

void CProcessRegistration::registerCvCore()
{
    registerProcess(std::make_shared<COcvAbsdiffFactory>(), std::make_shared<COcvWidgetAbsdiffFactory>());
    registerProcess(std::make_shared<COcvAddFactory>(), std::make_shared<COcvWidgetAddFactory>());
    registerProcess(std::make_shared<COcvAddWeightedFactory>(), std::make_shared<COcvWidgetAddWeightedFactory>());
    registerProcess(std::make_shared<COcvCompareFactory>(), std::make_shared<COcvWidgetCompareFactory>());
    registerProcess(std::make_shared<COcvConvertToFactory>(), std::make_shared<COcvWidgetConvertToFactory>());
    registerProcess(std::make_shared<COcvCopyMakeBorderFactory>(), std::make_shared<COcvWidgetCopyMakeBorderFactory>());
    registerProcess(std::make_shared<COcvCountNonZeroFactory>(), std::make_shared<COcvWidgetCountNonZeroFactory>());
    registerProcess(std::make_shared<COcvCropFactory>(), std::make_shared<COcvWidgetCropFactory>());
    registerProcess(std::make_shared<COcvDftFactory>(), std::make_shared<COcvWidgetDftFactory>());
    registerProcess(std::make_shared<COcvDftInvFactory>(), std::make_shared<COcvWidgetDftInvFactory>());
    registerProcess(std::make_shared<COcvDivideFactory>(), std::make_shared<COcvWidgetDivideFactory>());
    registerProcess(std::make_shared<COcvExpFactory>(), std::make_shared<COcvWidgetExpFactory>());
    registerProcess(std::make_shared<COcvExtractChannelFactory>(), std::make_shared<COcvWidgetExtractChannelFactory>());
    registerProcess(std::make_shared<COcvFlipFactory>(), std::make_shared<COcvWidgetFlipFactory>());
    registerProcess(std::make_shared<COcvInRangeFactory>(), std::make_shared<COcvWidgetInRangeFactory>());
    registerProcess(std::make_shared<COcvInsertChannelFactory>(), std::make_shared<COcvWidgetInsertChannelFactory>());
    registerProcess(std::make_shared<COcvKMeansFactory>(), std::make_shared<COcvWidgetKMeansFactory>());
    registerProcess(std::make_shared<COcvLogFactory>(), std::make_shared<COcvWidgetLogFactory>());
    registerProcess(std::make_shared<COcvLogicalOpFactory>(), std::make_shared<COcvWidgetLogicalOpFactory>());
    registerProcess(std::make_shared<COcvMagnitudeFactory>(), std::make_shared<COcvWidgetMagnitudeFactory>());    
    registerProcess(std::make_shared<COcvMaxFactory>(), std::make_shared<COcvWidgetMaxFactory>());
    registerProcess(std::make_shared<COcvMergeFactory>(), std::make_shared<COcvWidgetMergeFactory>());
    registerProcess(std::make_shared<COcvMinFactory>(), std::make_shared<COcvWidgetMinFactory>());
    registerProcess(std::make_shared<COcvMultiplyFactory>(), std::make_shared<COcvWidgetMultiplyFactory>());
    registerProcess(std::make_shared<COcvNegativeFactory>(), std::make_shared<COcvWidgetNegativeFactory>());
    registerProcess(std::make_shared<COcvNormalizeFactory>(), std::make_shared<COcvWidgetNormalizeFactory>());
    registerProcess(std::make_shared<COcvPSNRFactory>(), std::make_shared<COcvWidgetPSNRFactory>());
    registerProcess(std::make_shared<COcvRotateFactory>(), std::make_shared<COcvWidgetRotateFactory>());
    registerProcess(std::make_shared<COcvSplitFactory>(), std::make_shared<COcvWidgetSplitFactory>());
    registerProcess(std::make_shared<COcvSubtractFactory>(), std::make_shared<COcvWidgetSubtractFactory>());
}

void CProcessRegistration::registerCvDnn()
{
    registerProcess(std::make_shared<COcvDnnClassifierFactory>(), std::make_shared<COcvWidgetDnnClassifierFactory>());
    registerProcess(std::make_shared<COcvDnnColorizationFactory>(), std::make_shared<COcvWidgetDnnColorizationFactory>());
    registerProcess(std::make_shared<COcvDnnDetectorFactory>(), std::make_shared<COcvWidgetDnnDetectorFactory>());
    registerProcess(std::make_shared<COcvDnnSegmentationFactory>(), std::make_shared<COcvWidgetDnnSegmentationFactory>());
}

void CProcessRegistration::registerCvImgproc()
{
    registerProcess(std::make_shared<COcvAdaptiveThresholdFactory>(), std::make_shared<COcvWidgetAdaptiveThresholdFactory>());
    registerProcess(std::make_shared<COcvBilateralFactory>(), std::make_shared<COcvWidgetBilateralFactory>());
    registerProcess(std::make_shared<COcvBoxFilterFactory>(), std::make_shared<COcvWidgetBoxFilterFactory>());
    registerProcess(std::make_shared<COcvCalcHistFactory>(), std::make_shared<COcvWidgetCalcHistFactory>());
    registerProcess(std::make_shared<COcvCannyFactory>(), std::make_shared<COcvWidgetCannyFactory>());
    registerProcess(std::make_shared<COcvCascadeClassifierFactory>(), std::make_shared<COcvWidgetCascadeClassifierFactory>());
    registerProcess(std::make_shared<COcvCLAHEFactory>(), std::make_shared<COcvWidgetCLAHEFactory>());
    registerProcess(std::make_shared<COcvColorMapFactory>(), std::make_shared<COcvWidgetColorMapFactory>());
    registerProcess(std::make_shared<COcvCvtColorFactory>(), std::make_shared<COcvWidgetCvtColorFactory>());
    registerProcess(std::make_shared<COcvDistanceTransformFactory>(), std::make_shared<COcvWidgetDistanceTransformFactory>());
    registerProcess(std::make_shared<COcvEqualizeHistFactory>(), std::make_shared<COcvWidgetEqualizeHistFactory>());
    registerProcess(std::make_shared<COcvGaussianBlurFactory>(), std::make_shared<COcvWidgetGaussianBlurFactory>());
    registerProcess(std::make_shared<COcvGrabCutFactory>(), std::make_shared<COcvWidgetGrabCutFactory>());
    registerProcess(std::make_shared<COcvHoughCirclesFactory>(), std::make_shared<COcvWidgetHoughCirclesFactory>());
    registerProcess(std::make_shared<COcvHoughLinesFactory>(), std::make_shared<COcvWidgetHoughLinesFactory>());
    registerProcess(std::make_shared<COcvLaplacianFactory>(), std::make_shared<COcvWidgetLaplacianFactory>());
    registerProcess(std::make_shared<COcvMedianFactory>(), std::make_shared<COcvWidgetMedianFactory>());
    registerProcess(std::make_shared<COcvMorphologyExFactory>(), std::make_shared<COcvWidgetMorphologyExFactory>());
    registerProcess(std::make_shared<COcvResizeFactory>(), std::make_shared<COcvWidgetResizeFactory>());
    registerProcess(std::make_shared<COcvRotateExFactory>(), std::make_shared<COcvWidgetRotateExFactory>());
    registerProcess(std::make_shared<COcvSobelFactory>(), std::make_shared<COcvWidgetSobelFactory>());
    registerProcess(std::make_shared<COcvThresholdFactory>(), std::make_shared<COcvWidgetThresholdFactory>());
    registerProcess(std::make_shared<COcvWatershedFactory>(), std::make_shared<COcvWidgetWatershedFactory>());
}

void CProcessRegistration::registerCvFeatures2d()
{
    registerProcess(std::make_shared<COcvAGASTFactory>(), std::make_shared<COcvWidgetAGASTFactory>());
    registerProcess(std::make_shared<COcvAKAZEFactory>(), std::make_shared<COcvWidgetAKAZEFactory>());
    registerProcess(std::make_shared<COcvBFMatcherFactory>(), std::make_shared<COcvWidgetBFMatcherFactory>());
    registerProcess(std::make_shared<COcvBRISKFactory>(), std::make_shared<COcvWidgetBRISKFactory>());
    registerProcess(std::make_shared<COcvFASTFactory>(), std::make_shared<COcvWidgetFASTFactory>());
    registerProcess(std::make_shared<COcvFlannMatcherFactory>(), std::make_shared<COcvWidgetFlannMatcherFactory>());
    registerProcess(std::make_shared<COcvGFTTFactory>(), std::make_shared<COcvWidgetGFTTFactory>());
    registerProcess(std::make_shared<COcvKAZEFactory>(), std::make_shared<COcvWidgetKAZEFactory>());
    registerProcess(std::make_shared<COcvORBFactory>(), std::make_shared<COcvWidgetORBFactory>());
    registerProcess(std::make_shared<COcvSIFTFactory>(), std::make_shared<COcvWidgetSIFTFactory>());
    registerProcess(std::make_shared<COcvSimpleBlobDetectorFactory>(), std::make_shared<COcvWidgetSimpleBlobDetectorFactory>());
}

void CProcessRegistration::registerCvPhoto()
{
    registerProcess(std::make_shared<COcvDecolorFactory>(), std::make_shared<COcvWidgetDecolorFactory>());
    registerProcess(std::make_shared<COcvInpaintFactory>(), std::make_shared<COcvWidgetInpaintFactory>());
    registerProcess(std::make_shared<COcvColorchangeFactory>(), std::make_shared<COcvWidgetColorchangeFactory>());
    registerProcess(std::make_shared<COcvDenoiseTVL1Factory>(), std::make_shared<COcvWidgetDenoiseTVL1Factory>());
    registerProcess(std::make_shared<COcvDetailEnhanceFactory>(), std::make_shared<COcvWidgetDetailEnhanceFactory>());
    registerProcess(std::make_shared<COcvEdgePreservingFilterFactory>(), std::make_shared<COcvWidgetEdgePreservingFilterFactory>());
    registerProcess(std::make_shared<COcvFastNlMeansFactory>(), std::make_shared<COcvWidgetFastNlMeansFactory>());
    registerProcess(std::make_shared<COcvFastNlMeansMultiFactory>(), std::make_shared<COcvWidgetFastNlMeansMultiFactory>());
    registerProcess(std::make_shared<COcvIlluminationChangeFactory>(), std::make_shared<COcvWidgetIlluminationChangeFactory>());
    registerProcess(std::make_shared<COcvPencilSketchFactory>(), std::make_shared<COcvWidgetPencilSketchFactory>());
    registerProcess(std::make_shared<COcvSeamlessCloningFactory>(), std::make_shared<COcvWidgetSeamlessCloningFactory>());
    registerProcess(std::make_shared<COcvStylizationFactory>(), std::make_shared<COcvWidgetStylizationFactory>());
    registerProcess(std::make_shared<COcvTextureFlatteningFactory>(), std::make_shared<COcvWidgetTextureFlatteningFactory>());
}

void CProcessRegistration::registerCvTracking()
{
    registerProcess(std::make_shared<COcvTrackerGOTURNFactory>(), std::make_shared<COcvWidgetTrackerGOTURNFactory>());
    registerProcess(std::make_shared<COcvTrackerKCFFactory>(), std::make_shared<COcvWidgetTrackerKCFFactory>());
}

void CProcessRegistration::registerCvVideo()
{
    registerProcess(std::make_shared<COcvBckgndSubKnnFactory>(), std::make_shared<COcvWidgetBckgndSubKnnFactory>());
    registerProcess(std::make_shared<COcvBckgndSubMog2Factory>(), std::make_shared<COcvWidgetBckgndSubMog2Factory>());
    registerProcess(std::make_shared<COcvCamShiftFactory>(), std::make_shared<COcvWidgetCamShiftFactory>());
    registerProcess(std::make_shared<COcvDISOFFactory>(), std::make_shared<COcvWidgetDISOFFactory>());
    registerProcess(std::make_shared<COcvFarnebackOFFactory>(), std::make_shared<COcvWidgetFarnebackOFFactory>());
    registerProcess(std::make_shared<COcvMeanShiftFactory>(), std::make_shared<COcvWidgetMeanShiftFactory>());
}

void CProcessRegistration::registerCvBgsegm()
{
    registerProcess(std::make_shared<COcvBckgndSubCntFactory>(), std::make_shared<COcvWidgetBckgndSubCntFactory>());
    registerProcess(std::make_shared<COcvBckgndSubGmgFactory>(), std::make_shared<COcvWidgetBckgndSubGmgFactory>());
    registerProcess(std::make_shared<COcvBckgndSubGsocFactory>(), std::make_shared<COcvWidgetBckgndSubGsocFactory>());
    registerProcess(std::make_shared<COcvBckgndSubLsbpFactory>(), std::make_shared<COcvWidgetBckgndSubLsbpFactory>());
    registerProcess(std::make_shared<COcvBckgndSubMogFactory>(), std::make_shared<COcvWidgetBckgndSubMogFactory>());
}

void CProcessRegistration::registerCvXimgproc()
{
    registerProcess(std::make_shared<COcvAnisotropicDiffusionFactory>(), std::make_shared<COcvWidgetAnisotropicDiffusionFactory>());
    registerProcess(std::make_shared<COcvNiblackThresholdFactory>(), std::make_shared<COcvWidgetNiblackThresholdFactory>());
    registerProcess(std::make_shared<COcvPeiLinNormalizationFactory>(), std::make_shared<COcvWidgetPeiLinNormalizationFactory>());
    registerProcess(std::make_shared<COcvThinningFactory>(), std::make_shared<COcvWidgetThinningFactory>());

    registerProcess(std::make_shared<COcvAdaptiveManifoldFactory>(), std::make_shared<COcvWidgetAdaptiveManifoldFactory>());
    registerProcess(std::make_shared<COcvBilateralTextureFilterFactory>(), std::make_shared<COcvWidgetBilateralTextureFilterFactory>());
    registerProcess(std::make_shared<COcvDTFilterFactory>(), std::make_shared<COcvWidgetDTFilterFactory>());
    registerProcess(std::make_shared<COcvDTFilterEnhanceFactory>(), std::make_shared<COcvWidgetDTFilterEnhanceFactory>());
    registerProcess(std::make_shared<COcvDTFilterStylizeFactory>(), std::make_shared<COcvWidgetDTFilterStylizeFactory>());
    registerProcess(std::make_shared<COcvFastGlobalSmootherFilterFactory>(), std::make_shared<COcvWidgetFastGlobalSmootherFilterFactory>());
    registerProcess(std::make_shared<COcvFastLineDetectorFactory>(), std::make_shared<COcvWidgetFastLineDetectorFactory>());
    registerProcess(std::make_shared<COcvGradientDericheFactory>(), std::make_shared<COcvWidgetGradientDericheFactory>());
    registerProcess(std::make_shared<COcvGradientPaillouFactory>(), std::make_shared<COcvWidgetGradientPaillouFactory>());
    registerProcess(std::make_shared<COcvGraphSegmentationFactory>(), std::make_shared<COcvWidgetGraphSegmentationFactory>());
    registerProcess(std::make_shared<COcvGuidedFilterFactory>(), std::make_shared<COcvWidgetGuidedFilterFactory>());
    registerProcess(std::make_shared<COcvJointBilateralFilterFactory>(), std::make_shared<COcvWidgetJointBilateralFilterFactory>());
    registerProcess(std::make_shared<COcvL0SmoothFactory>(), std::make_shared<COcvWidgetL0SmoothFactory>());
    registerProcess(std::make_shared<COcvRidgeFilterFactory>(), std::make_shared<COcvWidgetRidgeFilterFactory>());
    registerProcess(std::make_shared<COcvRollingGuidanceFilterFactory>(), std::make_shared<COcvWidgetRollingGuidanceFilterFactory>());
    registerProcess(std::make_shared<COcvSelectiveSearchSegmentationFactory>(), std::make_shared<COcvWidgetSelectiveSearchSegmentationFactory>());
    registerProcess(std::make_shared<COcvSuperpixelLSCFactory>(), std::make_shared<COcvWidgetSuperpixelLSCFactory>());
    registerProcess(std::make_shared<COcvSuperpixelSEEDSFactory>(), std::make_shared<COcvWidgetSuperpixelSEEDSFactory>());
    registerProcess(std::make_shared<COcvSuperpixelSLICFactory>(), std::make_shared<COcvWidgetSuperpixelSLICFactory>());
    registerProcess(std::make_shared<COcvStructuredEdgeDetectionFactory>(), std::make_shared<COcvWidgetStructuredEdgeDetectionFactory>());
}

void CProcessRegistration::registerCvXphoto()
{
    registerProcess(std::make_shared<COcvGrayworldWBFactory>(), std::make_shared<COcvWidgetGrayworldWBFactory>());
    registerProcess(std::make_shared<COcvInpaintXFactory>(), std::make_shared<COcvWidgetInpaintXFactory>());
    registerProcess(std::make_shared<COcvLearningBasedWBFactory>(), std::make_shared<COcvWidgetLearningBasedWBFactory>());
    registerProcess(std::make_shared<COcvSimpleWBFactory>(), std::make_shared<COcvWidgetSimpleWBFactory>());
    //registerProcess(std::make_shared<COcvTonemapDurandFactory>(), std::make_shared<COcvWidgetTonemapDurandFactory>());
}

void CProcessRegistration::registerCvOptflow()
{
    registerProcess(std::make_shared<COcvDeepFlowFactory>(), std::make_shared<COcvWidgetDeepFlowFactory>());
    registerProcess(std::make_shared<COcvDualTVL1OFFactory>(), std::make_shared<COcvWidgetDualTVL1OFFactory>());
    registerProcess(std::make_shared<COcvPCAOFFactory>(), std::make_shared<COcvWidgetPCAOFFactory>());
    registerProcess(std::make_shared<COcvSimpleFlowFactory>(), std::make_shared<COcvWidgetSimpleFlowFactory>());
    registerProcess(std::make_shared<COcvSparseToDenseOFFactory>(), std::make_shared<COcvWidgetSparseToDenseOFFactory>());
}

void CProcessRegistration::registerCvBioinspired()
{
    registerProcess(std::make_shared<COcvRetinaFactory>(), std::make_shared<COcvWidgetRetinaFactory>());
    //registerProcess(std::make_shared<COcvRetinaToneMappingFactory>(), std::make_shared<COcvWidgetRetinaToneMappingFactory>());
    registerProcess(std::make_shared<COcvRetinaSegmentationFactory>(), std::make_shared<COcvWidgetRetinaSegmentationFactory>());
}

void CProcessRegistration::registerCvSaliency()
{
    registerProcess(std::make_shared<COcvMotionSaliencyBinWangApr2014Factory>(), std::make_shared<COcvWidgetMotionSaliencyBinWangApr2014Factory>());
    registerProcess(std::make_shared<COcvObjectnessBINGFactory>(), std::make_shared<COcvWidgetObjectnessBINGFactory>());
    registerProcess(std::make_shared<COcvSaliencyFineGrainedFactory>(), std::make_shared<COcvWidgetSaliencyFineGrainedFactory>());
    registerProcess(std::make_shared<COcvSaliencySpectralResidualFactory>(), std::make_shared<COcvWidgetSaliencySpectralResidualFactory>());
}

void CProcessRegistration::registerCvSuperres()
{
    registerProcess(std::make_shared<COcvSuperResBTVL1Factory>(), std::make_shared<COcvWidgetSuperResBTVL1Factory>());
}

void CProcessRegistration::registerCvObjdetect()
{
    registerProcess(std::make_shared<COcvQRCodeDetectorFactory>(), std::make_shared<COcvWidgetQRCodeDetectorFactory>());
}

void CProcessRegistration::registerCvText()
{
    registerProcess(std::make_shared<COcvOCRTesseractFactory>(), std::make_shared<COcvWidgetOCRTesseractFactory>());
}

const CTaskAbstractFactory& CProcessRegistration::getProcessFactory() const
{
    CPyEnsureGIL gil;
    return m_processFactory;
}

const CWidgetAbstractFactory& CProcessRegistration::getWidgetFactory() const
{
    CPyEnsureGIL gil;
    return m_widgetFactory;
}
