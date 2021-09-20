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

#ifndef COCVORB_HPP
#define COCVORB_HPP

#include "Core/C2dFeatureImageTask.h"
#include "IO/CImageIO.h"

//--------------------------------//
//----- COcvORBParam -----//
//--------------------------------//
class COcvORBParam: public CWorkflowTaskParam
{
    public:

        COcvORBParam() : CWorkflowTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_nfeatures = std::stoi(paramMap.at("nfeatures"));
            m_scaleFactor = std::stof(paramMap.at("scaleFactor"));
            m_nlevels = std::stoi(paramMap.at("nlevels"));
            m_edgeThreshold = std::stoi(paramMap.at("edgeThreshold"));
            m_firstLevel = std::stoi(paramMap.at("firstLevel"));
            m_WTA_K = std::stoi(paramMap.at("WTA_K"));
            m_scoreType = std::stoi(paramMap.at("scoreType"));
            m_patchSize = std::stoi(paramMap.at("patchSize"));
            m_fastThreshold = std::stoi(paramMap.at("fastThreshold"));
            m_bUseProvidedKeypoints = std::stoi(paramMap.at("bUseProvidedKeypoints"));
            m_bDetect = std::stoi(paramMap.at("bDetect"));
            m_bCompute = std::stoi(paramMap.at("bCompute"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("nfeatures", std::to_string(m_nfeatures)));
            map.insert(std::make_pair("scaleFactor", std::to_string(m_scaleFactor)));
            map.insert(std::make_pair("nlevels", std::to_string(m_nlevels)));
            map.insert(std::make_pair("edgeThreshold", std::to_string(m_edgeThreshold)));
            map.insert(std::make_pair("firstLevel", std::to_string(m_firstLevel)));
            map.insert(std::make_pair("WTA_K", std::to_string(m_WTA_K)));
            map.insert(std::make_pair("scoreType", std::to_string(m_scoreType)));
            map.insert(std::make_pair("patchSize", std::to_string(m_patchSize)));
            map.insert(std::make_pair("fastThreshold", std::to_string(m_fastThreshold)));
            map.insert(std::make_pair("bUseProvidedKeypoints", std::to_string(m_bUseProvidedKeypoints)));
            map.insert(std::make_pair("bDetect", std::to_string(m_bDetect)));
            map.insert(std::make_pair("bCompute", std::to_string(m_bCompute)));
            return map;
        }

    public:

        int     m_nfeatures = 500;
        float   m_scaleFactor = 1.2f;
        int     m_nlevels = 8;
        int     m_edgeThreshold = 31;
        int     m_firstLevel = 1;
        int     m_WTA_K = 2;
        int     m_scoreType = cv::ORB::HARRIS_SCORE;
        int     m_patchSize = 31;
        int     m_fastThreshold = 20;
        bool    m_bUseProvidedKeypoints = false;
        bool    m_bDetect = true;
        bool    m_bCompute = true;
};

//---------------------------//
//----- COcvORB -----//
//---------------------------//
class COcvORB : public C2dFeatureImageTask
{
    public:

        COcvORB() : C2dFeatureImageTask()
        {
            addKeypointInput();
            addDescriptorAndKeypointOuputs();
        }
        COcvORB(const std::string name, const std::shared_ptr<COcvORBParam>& pParam) : C2dFeatureImageTask(name)
        {
            addKeypointInput();
            addDescriptorAndKeypointOuputs();
            m_pParam = std::make_shared<COcvORBParam>(*pParam);
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

            auto pInput = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsInput>(getInput(1));
            auto pParam = std::dynamic_pointer_cast<COcvORBParam>(m_pParam);

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
                m_pFeatures = cv::ORB::create(pParam->m_nfeatures, pParam->m_scaleFactor, pParam->m_nlevels, pParam->m_edgeThreshold,
                                              pParam->m_firstLevel, pParam->m_WTA_K, static_cast<cv::ORB::ScoreType>(pParam->m_scoreType),
                                              pParam->m_patchSize, pParam->m_fastThreshold);
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

class COcvORBFactory : public CTaskFactory
{
    public:

        COcvORBFactory()
        {
            m_info.m_name = "ocv_orb";
            m_info.m_description = QObject::tr("This process implements the ORB (oriented BRIEF) keypoint detector and descriptor extractor.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/2D features framework/Feature detection and description").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Features,Keypoints,Descriptors,Interest";
            m_info.m_authors = "Ethan Rublee, Vincent Rabaud, Kurt Konolige, and Gary Bradski";
            m_info.m_article = "Orb: an efficient alternative to sift or surf";
            m_info.m_journal = "ICCV 2011, pages 2564–2571";
            m_info.m_year = 2011;
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/db/d95/classcv_1_1ORB.html";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvORBParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvORB>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvORBParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvORB>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVORB_HPP
