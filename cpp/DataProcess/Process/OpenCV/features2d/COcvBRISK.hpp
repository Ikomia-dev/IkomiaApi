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

#ifndef COCVBRISK_HPP
#define COCVBRISK_HPP

#include "Core/C2dFeatureImageTask.h"
#include "IO/CImageIO.h"

//--------------------------------//
//----- COcvBRISKParam -----//
//--------------------------------//
class COcvBRISKParam: public CWorkflowTaskParam
{
    public:

        COcvBRISKParam() : CWorkflowTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_thresh = std::stoi(paramMap.at("thresh"));
            m_octaves = std::stoi(paramMap.at("octaves"));
            m_patternScale = std::stof(paramMap.at("patternScale"));
            m_bUseProvidedKeypoints = std::stoi(paramMap.at("bUseProvidedKeypoints"));
            m_bDetect = std::stoi(paramMap.at("bDetect"));
            m_bCompute = std::stoi(paramMap.at("bCompute"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("thresh", std::to_string(m_thresh)));
            map.insert(std::make_pair("octaves", std::to_string(m_octaves)));
            map.insert(std::make_pair("patternScale", std::to_string(m_patternScale)));
            map.insert(std::make_pair("bUseProvidedKeypoints", std::to_string(m_bUseProvidedKeypoints)));
            map.insert(std::make_pair("bDetect", std::to_string(m_bDetect)));
            map.insert(std::make_pair("bCompute", std::to_string(m_bCompute)));
            return map;
        }

    public:

        int                     m_thresh = 30;
        int                     m_octaves = 3;
        float                   m_patternScale = 1.0f;
        bool                    m_bUseProvidedKeypoints = false;
        bool                    m_bDetect = true;
        bool                    m_bCompute = true;
};

//---------------------------//
//----- COcvBRISK -----//
//---------------------------//
class COcvBRISK : public C2dFeatureImageTask
{
    public:

        COcvBRISK() : C2dFeatureImageTask()
        {
            addKeypointInput();
            addDescriptorAndKeypointOuputs();
        }
        COcvBRISK(const std::string name, const std::shared_ptr<COcvBRISKParam>& pParam) : C2dFeatureImageTask(name)
        {
            addKeypointInput();
            addDescriptorAndKeypointOuputs();
            m_pParam = std::make_shared<COcvBRISKParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvBRISKParam>(m_pParam);

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
                m_pFeatures = cv::BRISK::create(pParam->m_thresh, pParam->m_octaves, pParam->m_patternScale);
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

class COcvBRISKFactory : public CTaskFactory
{
    public:

        COcvBRISKFactory()
        {
            m_info.m_name = QObject::tr("BRISK").toStdString();
            m_info.m_description = QObject::tr("This process implements the BRISK keypoint detector and descriptor extractor.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/2D features framework/Feature detection and description").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Features,Keypoints,Descriptors,Interest";
            m_info.m_authors = "Stefan Leutenegger, Margarita Chli, and Roland Yves Siegwart";
            m_info.m_article = "Brisk: Binary robust invariant scalable keypoints";
            m_info.m_journal = "ICCV 2011, pages 2548–2555";
            m_info.m_year = 2011;
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/de/dbf/classcv_1_1BRISK.html";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvBRISKParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvBRISK>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvBRISKParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvBRISK>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVBRISK_HPP
