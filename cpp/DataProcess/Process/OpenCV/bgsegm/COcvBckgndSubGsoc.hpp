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

#ifndef COCVBCKGNDSUBGSOC_HPP
#define COCVBCKGNDSUBGSOC_HPP

#include "opencv2/bgsegm.hpp"
#include "Core/CVideoProcess.h"

//---------------------------------//
//----- COcvBckgndSubGsocParam -----//
//---------------------------------//
class COcvBckgndSubGsocParam: public CProtocolTaskParam
{
    public:

        COcvBckgndSubGsocParam()
        {
        }

        void        setParamMap(const UMapString& paramMap) override
        {
            m_motionComp = std::stoi(paramMap.at("motionComp"));
            m_samples = std::stoi(paramMap.at("samples"));
            m_replaceRate = std::stod(paramMap.at("replaceRate"));
            m_propagationRate = std::stod(paramMap.at("propagationRate"));
            m_hitsThreshold = std::stoi(paramMap.at("hitsThreshold"));
            m_alpha = std::stod(paramMap.at("alpha"));
            m_beta = std::stod(paramMap.at("beta"));
            m_blinkingSupressionDecay = std::stod(paramMap.at("blinkingSupressionDecay"));
            m_blinkingSupressionMultiplier = std::stod(paramMap.at("blinkingSupressionMultiplier"));
            m_noiseRemovalThresholdFacBG = std::stod(paramMap.at("noiseRemovalThresholdFacBG"));
            m_noiseRemovalThresholdFacFG = std::stod(paramMap.at("noiseRemovalThresholdFacFG"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("motionComp", std::to_string(m_motionComp)));
            map.insert(std::make_pair("samples", std::to_string(m_samples)));
            map.insert(std::make_pair("replaceRate", std::to_string(m_replaceRate)));
            map.insert(std::make_pair("propagationRate", std::to_string(m_propagationRate)));
            map.insert(std::make_pair("hitsThreshold", std::to_string(m_hitsThreshold)));
            map.insert(std::make_pair("alpha", std::to_string(m_alpha)));
            map.insert(std::make_pair("beta", std::to_string(m_beta)));
            map.insert(std::make_pair("blinkingSupressionDecay", std::to_string(m_blinkingSupressionDecay)));
            map.insert(std::make_pair("blinkingSupressionMultiplier", std::to_string(m_blinkingSupressionMultiplier)));
            map.insert(std::make_pair("noiseRemovalThresholdFacBG", std::to_string(m_noiseRemovalThresholdFacBG)));
            map.insert(std::make_pair("noiseRemovalThresholdFacFG", std::to_string(m_noiseRemovalThresholdFacFG)));
            return map;
        }

    public:

        int     m_motionComp = cv::bgsegm::LSBP_CAMERA_MOTION_COMPENSATION_NONE;
        int     m_samples = 20;
        float   m_replaceRate = 0.003f;
        float   m_propagationRate = 0.01f;
        int     m_hitsThreshold = 32;
        float   m_alpha = 0.01f;
        float   m_beta = 0.0022f;
        float   m_blinkingSupressionDecay = 0.1f;
        float   m_blinkingSupressionMultiplier = 0.1f;
        float   m_noiseRemovalThresholdFacBG = 0.0004f;
        float   m_noiseRemovalThresholdFacFG = 0.0008f;
};

//----------------------------//
//----- COcvBckgndSubGsoc -----//
//----------------------------//
class COcvBckgndSubGsoc : public CVideoProcess
{
    public:

        COcvBckgndSubGsoc() : CVideoProcess()
        {
            setOutputDataType(IODataType::IMAGE_BINARY, 0);
            addOutput(std::make_shared<CImageProcessIO>());
            setOutputColorMap(1, 0, {{255,0,0}});
        }
        COcvBckgndSubGsoc(const std::string name, const std::shared_ptr<COcvBckgndSubGsocParam>& pParam) : CVideoProcess(name)
        {
            setOutputDataType(IODataType::IMAGE_BINARY, 0);
            addOutput(std::make_shared<CImageProcessIO>());
            setOutputColorMap(1, 0, {{255,0,0}});
            m_pParam = std::make_shared<COcvBckgndSubGsocParam>(*pParam);
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        void    notifyVideoStart(int frameCount) override
        {
            Q_UNUSED(frameCount);

            auto pParam = std::dynamic_pointer_cast<COcvBckgndSubGsocParam>(m_pParam);
            if(pParam == nullptr)
                return;

            m_pBgSubtractor = cv::bgsegm::createBackgroundSubtractorGSOC(pParam->m_motionComp, pParam->m_samples, pParam->m_replaceRate, pParam->m_propagationRate,
                                                                         pParam->m_hitsThreshold, pParam->m_alpha, pParam->m_beta,
                                                                         pParam->m_blinkingSupressionDecay, pParam->m_blinkingSupressionMultiplier,
                                                                         pParam->m_noiseRemovalThresholdFacBG, pParam->m_noiseRemovalThresholdFacFG);
        }

        void    run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pParam = std::dynamic_pointer_cast<COcvBckgndSubGsocParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgDst;
            CMat imgSrc = pInput->getImage();
            emit m_signalHandler->doProgress();

            try
            {
                if(m_pBgSubtractor == nullptr)
                {
                    m_pBgSubtractor = cv::bgsegm::createBackgroundSubtractorGSOC(pParam->m_motionComp, pParam->m_samples, pParam->m_replaceRate,
                                                                                 pParam->m_propagationRate, pParam->m_hitsThreshold, pParam->m_alpha, pParam->m_beta,
                                                                                 pParam->m_blinkingSupressionDecay, pParam->m_blinkingSupressionMultiplier,
                                                                                 pParam->m_noiseRemovalThresholdFacBG, pParam->m_noiseRemovalThresholdFacFG);
                }

                m_pBgSubtractor->apply(imgSrc, imgDst);
            }
            catch(std::exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e.what(), __func__, __FILE__, __LINE__);
            }
            emit m_signalHandler->doProgress();

            auto pOutput = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(imgDst);

            applyInputGraphicsMask(1, 0, 0, MaskMode::MASK_ONLY);
            forwardInputImage(0, 1);
            endTaskRun();
            emit m_signalHandler->doProgress();
        }

    private:

        cv::Ptr<cv::bgsegm::BackgroundSubtractorGSOC>    m_pBgSubtractor = nullptr;
};

//-----------------------------------//
//----- COcvBckgndSubGsocFactory -----//
//-----------------------------------//
class COcvBckgndSubGsocFactory : public CProcessFactory
{
    public:

        COcvBckgndSubGsocFactory()
        {
            m_info.m_name = QObject::tr("GSOC background substractor").toStdString();
            m_info.m_description = QObject::tr("This process removes background on video sequence based on counting algorithm.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Improved Background-Foreground Segmentation Methods").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "video,background,substractor,segmentation,binary";
            m_info.m_journal = "GSOC Conference";
            m_info.m_year = 2014;
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d4/dd5/classcv_1_1bgsegm_1_1BackgroundSubtractorGSOC.html";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvBckgndSubGsocParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvBckgndSubGsoc>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvBckgndSubGsocParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvBckgndSubGsoc>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVBCKGNDSUBGSOC_HPP
