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

#ifndef COCVGFTT_HPP
#define COCVGFTT_HPP

#include "Core/CFeatureImageProcess2d.h"
#include "IO/CImageProcessIO.h"

//--------------------------------//
//----- COcvGFTTParam -----//
//--------------------------------//
class COcvGFTTParam: public CProtocolTaskParam
{
    public:

        COcvGFTTParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_maxCorners = std::stoi(paramMap.at("maxCorners"));
            m_qualityLevel = std::stod(paramMap.at("qualityLevel"));
            m_minDistance = std::stod(paramMap.at("minDistance"));
            m_blockSize = std::stoi(paramMap.at("blockSize"));
            m_bUseHarrisDetector = std::stoi(paramMap.at("bUseHarrisDetector"));
            m_k = std::stod(paramMap.at("k"));
            m_bUseProvidedKeypoints = std::stoi(paramMap.at("bUseProvidedKeypoints"));
            m_bDetect = std::stoi(paramMap.at("bDetect"));
            m_bCompute = std::stoi(paramMap.at("bCompute"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("maxCorners", std::to_string(m_maxCorners)));
            map.insert(std::make_pair("qualityLevel", std::to_string(m_qualityLevel)));
            map.insert(std::make_pair("minDistance", std::to_string(m_minDistance)));
            map.insert(std::make_pair("blockSize", std::to_string(m_blockSize)));
            map.insert(std::make_pair("bUseHarrisDetector", std::to_string(m_bUseHarrisDetector)));
            map.insert(std::make_pair("k", std::to_string(m_k)));
            map.insert(std::make_pair("bUseProvidedKeypoints", std::to_string(m_bUseProvidedKeypoints)));
            map.insert(std::make_pair("bDetect", std::to_string(m_bDetect)));
            map.insert(std::make_pair("bCompute", std::to_string(m_bCompute)));
            return map;
        }

    public:

        int         m_maxCorners = 1000;
        double      m_qualityLevel = 0.01;
        double      m_minDistance = 1;
        int         m_blockSize = 3;
        bool        m_bUseHarrisDetector = false;
        double      m_k = 0.04;
        bool        m_bUseProvidedKeypoints = false;
        bool        m_bDetect = true;
        bool        m_bCompute = false;
};
//---------------------------//
//----- COcvGFTT -----//
//---------------------------//
class COcvGFTT : public CFeatureImageProcess2d
{
    public:

        COcvGFTT() : CFeatureImageProcess2d()
        {
            addKeypointOutput();
        }
        COcvGFTT(const std::string name, const std::shared_ptr<COcvGFTTParam>& pParam) : CFeatureImageProcess2d(name)
        {
            addKeypointOutput();
            m_pParam = std::make_shared<COcvGFTTParam>(*pParam);
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        void run() override
        {
            beginTaskRun();

            if(getInputCount() < 2)
                throw CException(CoreExCode::INVALID_PARAMETER, "Not enough inputs", __func__, __FILE__, __LINE__);

            auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsProcessInput>(getInput(1));
            auto pParam = std::dynamic_pointer_cast<COcvGFTTParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty images", __func__, __FILE__, __LINE__);

            emit m_signalHandler->doProgress();

            CMat imgSrc = pInput->getImage();
            createGraphicsMask(imgSrc.getNbCols(), imgSrc.getNbRows(), pGraphicsInput);
            try
            {
                manageInputs();
                m_pFeatures = cv::GFTTDetector::create(pParam->m_maxCorners, pParam->m_qualityLevel, pParam->m_minDistance, pParam->m_blockSize, pParam->m_bUseHarrisDetector, pParam->m_k);
                makeFeatures(imgSrc, pParam->m_bDetect, pParam->m_bCompute);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            endTaskRun();

            emit m_signalHandler->doProgress();
            manageOutputs();
            emit m_signalHandler->doProgress();
        }
};

class COcvGFTTFactory : public CProcessFactory
{
    public:

        COcvGFTTFactory()
        {
            m_info.m_name = QObject::tr("GFTTDetector").toStdString();
            m_info.m_description = QObject::tr("Feature detection using the goodFeaturesToTrack function.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/2D features framework/Feature detection and description").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Features,Keypoints,Descriptors,Interest";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/df/d21/classcv_1_1GFTTDetector.html";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvGFTTParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvGFTT>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvGFTTParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvGFTT>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVGFTT_HPP
