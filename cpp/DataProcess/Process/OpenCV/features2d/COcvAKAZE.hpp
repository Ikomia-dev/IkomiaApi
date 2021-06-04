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

#ifndef COCVAKAZE_HPP
#define COCVAKAZE_HPP

#include "Core/C2dFeatureImageTask.h"
#include "IO/CImageIO.h"

//--------------------------------//
//----- COcvAKAZEParam -----//
//--------------------------------//
class COcvAKAZEParam: public CWorkflowTaskParam
{
    public:

        COcvAKAZEParam() : CWorkflowTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_descriptor_type = std::stoi(paramMap.at("descriptorType"));
            m_descriptor_size = std::stoi(paramMap.at("descriptorSize"));
            m_descriptor_channels = std::stoi(paramMap.at("descriptorChannels"));
            m_threshold = std::stof(paramMap.at("threshold"));
            m_nOctaves = std::stoi(paramMap.at("nOctaves"));
            m_nOctaveLayers = std::stoi(paramMap.at("nOctaveLayers"));
            m_diffusivity = std::stoi(paramMap.at("diffusivity"));
            m_bUseProvidedKeypoints = std::stoi(paramMap.at("bUseProvidedKeypoints"));
            m_bDetect = std::stoi(paramMap.at("bDetect"));
            m_bCompute = std::stoi(paramMap.at("bCompute"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("descriptorType", std::to_string(m_descriptor_type)));
            map.insert(std::make_pair("descriptorSize", std::to_string(m_descriptor_size)));
            map.insert(std::make_pair("descriptorChannels", std::to_string(m_descriptor_channels)));
            map.insert(std::make_pair("threshold", std::to_string(m_threshold)));
            map.insert(std::make_pair("nOctaves", std::to_string(m_nOctaves)));
            map.insert(std::make_pair("nOctaveLayers", std::to_string(m_nOctaveLayers)));
            map.insert(std::make_pair("diffusivity", std::to_string(m_diffusivity)));
            map.insert(std::make_pair("bUseProvidedKeypoints", std::to_string(m_bUseProvidedKeypoints)));
            map.insert(std::make_pair("bDetect", std::to_string(m_bDetect)));
            map.insert(std::make_pair("bCompute", std::to_string(m_bCompute)));
            return map;
        }

    public:

        int     m_descriptor_type = cv::AKAZE::DESCRIPTOR_MLDB;
        int     m_descriptor_size = 0;
        int     m_descriptor_channels = 3;
        float   m_threshold = 0.001f;
        int     m_nOctaves = 4;
        int     m_nOctaveLayers = 4;
        int     m_diffusivity = cv::KAZE::DIFF_PM_G2;
        bool    m_bUseProvidedKeypoints = false;
        bool    m_bDetect = true;
        bool    m_bCompute = true;
};

//---------------------------//
//----- COcvAKAZE -----//
//---------------------------//
class COcvAKAZE : public C2dFeatureImageTask
{
    public:

        COcvAKAZE() : C2dFeatureImageTask()
        {
            addKeypointInput();
            addDescriptorAndKeypointOuputs();
        }
        COcvAKAZE(const std::string name, const std::shared_ptr<COcvAKAZEParam>& pParam) : C2dFeatureImageTask(name)
        {
            addKeypointInput();
            addDescriptorAndKeypointOuputs();
            m_pParam = std::make_shared<COcvAKAZEParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvAKAZEParam>(m_pParam);

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
                m_pFeatures = cv::AKAZE::create(static_cast<cv::AKAZE::DescriptorType>(pParam->m_descriptor_type),
                                                pParam->m_descriptor_size, pParam->m_descriptor_channels, pParam->m_threshold,
                                                pParam->m_nOctaves, pParam->m_nOctaveLayers, static_cast<cv::KAZE::DiffusivityType>(pParam->m_diffusivity));
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

class COcvAKAZEFactory : public CTaskFactory
{
    public:

        COcvAKAZEFactory()
        {
            m_info.m_name = QObject::tr("AKAZE").toStdString();
            m_info.m_description = QObject::tr("This process implements the AKAZE keypoint detector and descriptor extractor.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/2D features framework/Feature detection and description").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Features,Keypoints,Descriptors,Interest";
            m_info.m_authors = "Pablo F Alcantarilla, Jesús Nuevo, and Adrien Bartoli";
            m_info.m_article = "Fast explicit diffusion for accelerated features in nonlinear scale spaces";
            m_info.m_journal = "IEEE TPAMI 34(7):1281–1298";
            m_info.m_year = 2011;
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d8/d30/classcv_1_1AKAZE.html";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvAKAZEParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvAKAZE>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvAKAZEParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvAKAZE>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVAKAZE_HPP
