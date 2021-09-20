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

#include "COcvSuperResBTVL1.h"

//----------------------------------//
//----- COcvSuperResBTVL1Param -----//
//----------------------------------//
COcvSuperResBTVL1Param::COcvSuperResBTVL1Param()
{
}

void COcvSuperResBTVL1Param::setParamMap(const UMapString &paramMap)
{
    m_scale = std::stoi(paramMap.at("scale"));
    m_iterations = std::stoi(paramMap.at("iterations"));
    m_tau = std::stod(paramMap.at("tau"));
    m_lambda = std::stod(paramMap.at("lambda"));
    m_alpha = std::stod(paramMap.at("alpha"));
    m_btvKernelSize = std::stoi(paramMap.at("btvKernelSize"));
    m_blurKernelSize = std::stoi(paramMap.at("blurKernelSize"));
    m_blurSigma = std::stod(paramMap.at("blurSigma"));
    m_temporalAreaRadius = std::stoi(paramMap.at("temporalAreaRadius"));
    m_opticalFlowType = std::stoi(paramMap.at("opticalFlowType"));
}

UMapString COcvSuperResBTVL1Param::getParamMap() const
{
    UMapString map;
    map.insert(std::make_pair("scale", std::to_string(m_scale)));
    map.insert(std::make_pair("iterations", std::to_string(m_iterations)));
    map.insert(std::make_pair("tau", std::to_string(m_tau)));
    map.insert(std::make_pair("lambda", std::to_string(m_lambda)));
    map.insert(std::make_pair("alpha", std::to_string(m_alpha)));
    map.insert(std::make_pair("btvKernelSize", std::to_string(m_btvKernelSize)));
    map.insert(std::make_pair("blurKernelSize", std::to_string(m_blurKernelSize)));
    map.insert(std::make_pair("blurSigma", std::to_string(m_blurSigma)));
    map.insert(std::make_pair("temporalAreaRadius", std::to_string(m_temporalAreaRadius)));
    map.insert(std::make_pair("opticalFlowType", std::to_string(m_opticalFlowType)));
    return map;
}

//-----------------------------//
//----- COcvSuperResBTVL1 -----//
//-----------------------------//
COcvSuperResBTVL1::COcvSuperResBTVL1() : CVideoTask()
{
    m_bCuda = Utils::Gpu::isCudaAvailable();
}

COcvSuperResBTVL1::COcvSuperResBTVL1(const std::string name, const std::shared_ptr<COcvSuperResBTVL1Param> &pParam)
    : CVideoTask(name)
{
    m_bCuda = Utils::Gpu::isCudaAvailable();
    m_pParam = std::make_shared<COcvSuperResBTVL1Param>(*pParam);
}

size_t COcvSuperResBTVL1::getProgressSteps()
{
    return 3;
}

void COcvSuperResBTVL1::notifyVideoStart(int frameCount)
{
    Q_UNUSED(frameCount);
    m_bInit = false;
    m_bVideoStart = true;

    auto pParam = std::dynamic_pointer_cast<COcvSuperResBTVL1Param>(m_pParam);
    if(pParam == nullptr)
        return;

    //Create Super Resolution pointer
    if(m_bCuda)
        m_superResPtr = cv::superres::createSuperResolution_BTVL1_CUDA();
    else
        m_superResPtr = cv::superres::createSuperResolution_BTVL1();

    //Create frame source
    m_frameSrcPtr = cv::makePtr<CSuperResFrameSrc>(m_bCuda);

    //Create Optical Flow pointer
    auto of = createOpticalFlow();
    if(of.empty())
        return;

    //Initialization
    m_superResPtr->setOpticalFlow(of);
    m_superResPtr->setInput(m_frameSrcPtr);
    m_superResPtr->setScale(pParam->m_scale);
    m_superResPtr->setTau(pParam->m_tau);
    m_superResPtr->setLambda(pParam->m_lambda);
    m_superResPtr->setAlpha(pParam->m_alpha);
    m_superResPtr->setBlurKernelSize(pParam->m_blurKernelSize);
    m_superResPtr->setBlurSigma(pParam->m_blurSigma);
    m_superResPtr->setIterations(pParam->m_iterations);
    m_superResPtr->setTemporalAreaRadius(pParam->m_temporalAreaRadius);
    m_bInit = true;
}

void COcvSuperResBTVL1::run()
{
    beginTaskRun();
    auto pInput = std::dynamic_pointer_cast<CImageIO>(getInput(0));
    auto pParam = std::dynamic_pointer_cast<COcvSuperResBTVL1Param>(m_pParam);

    if(pInput == nullptr || pParam == nullptr)
        throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

    if(pInput->isDataAvailable() == false)
        throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

    if(m_bVideoStart == true && m_bInit == false)
        throw CException(CoreExCode::INVALID_USAGE, "Error while initializing", __func__, __FILE__, __LINE__);

    CMat imgDst;
    CMat imgSrc = pInput->getImage();

    if(m_frameSrcPtr.empty() == false)
        m_frameSrcPtr->setFrame(imgSrc);

    emit m_signalHandler->doProgress();

    try
    {
        if(m_bVideoStart == true)
        {
            if(m_bCuda == true)
            {
                cv::cuda::GpuMat cuImgDst;
                m_superResPtr->nextFrame(cuImgDst);
                cuImgDst.download(imgDst);
            }
            else
                m_superResPtr->nextFrame(imgDst);
        }
        else
            imgDst = imgSrc.clone();
    }
    catch(std::exception& e)
    {
        throw CException(CoreExCode::INVALID_PARAMETER, e.what(), __func__, __FILE__, __LINE__);
    }

    endTaskRun();
    emit m_signalHandler->doProgress();

    auto pOutput = std::dynamic_pointer_cast<CImageIO>(getOutput(0));
    if(pOutput)
        pOutput->setImage(imgDst);

    emit m_signalHandler->doProgress();
}

cv::Ptr<cv::superres::DenseOpticalFlowExt> COcvSuperResBTVL1::createOpticalFlow()
{
    auto pParam = std::dynamic_pointer_cast<COcvSuperResBTVL1Param>(m_pParam);
    assert(pParam);

    switch(pParam->m_opticalFlowType)
    {
        case COcvSuperResBTVL1Param::BROX:
            if(m_bCuda == true)
                return cv::superres::createOptFlow_Brox_CUDA();
            else
                break;

        case COcvSuperResBTVL1Param::DUAL_TVL1:
            if(m_bCuda == true)
                return cv::superres::createOptFlow_DualTVL1_CUDA();
            else
                return cv::superres::createOptFlow_DualTVL1();

        case COcvSuperResBTVL1Param::FARNEBACK:
            if(m_bCuda == true)
                return cv::superres::createOptFlow_Farneback_CUDA();
            else
                return cv::superres::createOptFlow_Farneback();

        case COcvSuperResBTVL1Param::PYR_LK:
            if(m_bCuda == true)
                return cv::superres::createOptFlow_PyrLK_CUDA();
            else
                break;
    }
    return cv::Ptr<cv::superres::DenseOpticalFlowExt>();
}

//------------------------------------//
//----- COcvSuperResBTVL1Factory -----//
//------------------------------------//
COcvSuperResBTVL1Factory::COcvSuperResBTVL1Factory()
{
    m_info.m_name = "ocv_super_resolution_btvl1";
    m_info.m_description = QObject::tr("This process generate super resolution video using duality based TV-L1 optical flow.").toStdString();
    m_info.m_path = QObject::tr("OpenCV/Main modules/Super resolution").toStdString();
    m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
    m_info.m_keywords = "video,super,resolution,optical,flow,dual";
    m_info.m_authors = "Dennis Mitzel, Thomas Pock, Thomas Schoenemann, and Daniel Cremers";
    m_info.m_article = "Video super resolution using duality based tv-l 1 optical flow";
    m_info.m_journal = "In Pattern Recognition, pages 432–441, Springer";
    m_info.m_year = 2009;
    m_info.m_docLink = "https://docs.opencv.org/3.4.3/d7/d0a/group__superres.html#ga73c184b0040c1afa7d6cdf0a9f32a8f8";
}

WorkflowTaskPtr COcvSuperResBTVL1Factory::create(const WorkflowTaskParamPtr &pParam)
{
    auto pDerivedParam = std::dynamic_pointer_cast<COcvSuperResBTVL1Param>(pParam);
    if(pDerivedParam != nullptr)
        return std::make_shared<COcvSuperResBTVL1>(m_info.m_name, pDerivedParam);
    else
        return create();
}

WorkflowTaskPtr COcvSuperResBTVL1Factory::create()
{
    auto pDerivedParam = std::make_shared<COcvSuperResBTVL1Param>();
    assert(pDerivedParam != nullptr);
    return std::make_shared<COcvSuperResBTVL1>(m_info.m_name, pDerivedParam);
}
