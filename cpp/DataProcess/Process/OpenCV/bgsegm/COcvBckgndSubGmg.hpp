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

#ifndef COCVBCKGNDSUBGMG_HPP
#define COCVBCKGNDSUBGMG_HPP

#include "opencv2/bgsegm.hpp"
#include "Core/CVideoProcess.h"

//---------------------------------//
//----- COcvBckgndSubGmgParam -----//
//---------------------------------//
class COcvBckgndSubGmgParam: public CProtocolTaskParam
{
    public:

        COcvBckgndSubGmgParam()
        {
        }

        void        setParamMap(const UMapString& paramMap) override
        {
            m_initializationFrames = std::stoi(paramMap.at("initializationFrames"));
            m_threshold = std::stod(paramMap.at("threshold"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("initializationFrames", std::to_string(m_initializationFrames)));
            map.insert(std::make_pair("threshold", std::to_string(m_threshold)));
            return map;
        }

    public:

        int     m_initializationFrames = 120;
        double  m_threshold = 0.8;
};

//----------------------------//
//----- COcvBckgndSubGmg -----//
//----------------------------//
class COcvBckgndSubGmg : public CVideoProcess
{
    public:

        COcvBckgndSubGmg() : CVideoProcess()
        {
            setOutputDataType(IODataType::IMAGE_BINARY, 0);
            addOutput(std::make_shared<CImageProcessIO>());
            setOutputColorMap(1, 0, {{255,0,0}});
        }
        COcvBckgndSubGmg(const std::string name, const std::shared_ptr<COcvBckgndSubGmgParam>& pParam) : CVideoProcess(name)
        {
            setOutputDataType(IODataType::IMAGE_BINARY, 0);
            addOutput(std::make_shared<CImageProcessIO>());
            setOutputColorMap(1, 0, {{255,0,0}});
            m_pParam = std::make_shared<COcvBckgndSubGmgParam>(*pParam);
        }

        size_t     getProgressSteps() override
        {
            return 3;
        }

        void    notifyVideoStart(int frameCount) override
        {
            Q_UNUSED(frameCount);

            auto pParam = std::dynamic_pointer_cast<COcvBckgndSubGmgParam>(m_pParam);
            if(pParam == nullptr)
                return;

            m_pBgSubtractor = cv::bgsegm::createBackgroundSubtractorGMG(pParam->m_initializationFrames, pParam->m_threshold);
        }

        void    run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pParam = std::dynamic_pointer_cast<COcvBckgndSubGmgParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgDst;
            CMat imgSrc = pInput->getImage();
            emit m_signalHandler->doProgress();

            try
            {
                if(m_pBgSubtractor.empty())
                    m_pBgSubtractor = cv::bgsegm::createBackgroundSubtractorGMG(pParam->m_initializationFrames, pParam->m_threshold);

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

        cv::Ptr<cv::bgsegm::BackgroundSubtractorGMG>    m_pBgSubtractor;
};

//-----------------------------------//
//----- COcvBckgndSubGmgFactory -----//
//-----------------------------------//
class COcvBckgndSubGmgFactory : public CProcessFactory
{
    public:

        COcvBckgndSubGmgFactory()
        {
            m_info.m_name = QObject::tr("GMG background substractor").toStdString();
            m_info.m_description = QObject::tr("This process removes background on video sequence.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Improved Background-Foreground Segmentation Methods").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "video,background,substractor,segmentation,binary,variable,light";
            m_info.m_authors = "Andrew B Godbehere, Akihiro Matsukawa, and Ken Goldberg";
            m_info.m_article = "Visual tracking of human visitors under variable-lighting conditions for a responsive audio art installation.";
            m_info.m_journal = "In American Control Conference (ACC), 2012, pages 4305–4312. IEEE.";
            m_info.m_year = 2012;
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d1/d5c/classcv_1_1bgsegm_1_1BackgroundSubtractorGMG.html";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvBckgndSubGmgParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvBckgndSubGmg>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvBckgndSubGmgParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvBckgndSubGmg>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVBCKGNDSUBGMG_HPP
