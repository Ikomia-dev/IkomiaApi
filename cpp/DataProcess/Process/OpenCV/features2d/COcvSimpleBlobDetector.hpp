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

#ifndef COCVSIMPLEBLOBDETECTOR_HPP
#define COCVSIMPLEBLOBDETECTOR_HPP

#include "Core/C2dFeatureImageTask.h"
#include "IO/CImageIO.h"

//--------------------------------//
//----- COcvSimpleBlobDetectorParam -----//
//--------------------------------//
class COcvSimpleBlobDetectorParam: public CWorkflowTaskParam
{
    public:

        COcvSimpleBlobDetectorParam() : CWorkflowTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_blobParam.blobColor = std::stoi(paramMap.at("blobColor"));
            m_blobParam.minThreshold = std::stoi(paramMap.at("minThreshold"));
            m_blobParam.maxThreshold = std::stoi(paramMap.at("maxThreshold"));
            m_blobParam.minDistBetweenBlobs = std::stoi(paramMap.at("minDistBetweenBlobs"));
            m_blobParam.filterByArea = std::stoi(paramMap.at("filterByArea"));
            m_blobParam.minArea = std::stoi(paramMap.at("minArea"));
            m_blobParam.maxArea = std::stoi(paramMap.at("maxArea"));
            m_blobParam.filterByCircularity = std::stoi(paramMap.at("filterByCircularity"));
            m_blobParam.minCircularity = std::stoi(paramMap.at("minCircularity"));
            m_blobParam.maxCircularity = std::stoi(paramMap.at("maxCircularity"));
            m_blobParam.filterByConvexity = std::stoi(paramMap.at("filterByConvexity"));
            m_blobParam.minConvexity = std::stoi(paramMap.at("minConvexity"));
            m_blobParam.maxConvexity = std::stoi(paramMap.at("maxConvexity"));
            m_blobParam.filterByInertia = std::stoi(paramMap.at("filterByInertia"));
            m_blobParam.minInertiaRatio = std::stoi(paramMap.at("minInertiaRatio"));
            m_blobParam.maxInertiaRatio = std::stoi(paramMap.at("maxInertiaRatio"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("minThreshold", std::to_string(m_blobParam.minThreshold)));
            map.insert(std::make_pair("maxThreshold", std::to_string(m_blobParam.maxThreshold)));
            map.insert(std::make_pair("minDistBetweenBlobs", std::to_string(m_blobParam.minDistBetweenBlobs)));
            map.insert(std::make_pair("blobColor", std::to_string(m_blobParam.blobColor)));
            map.insert(std::make_pair("filterByArea", std::to_string(m_blobParam.filterByArea)));
            map.insert(std::make_pair("minArea", std::to_string(m_blobParam.minArea)));
            map.insert(std::make_pair("maxArea", std::to_string(m_blobParam.maxArea)));
            map.insert(std::make_pair("filterByConvexity", std::to_string(m_blobParam.filterByConvexity)));
            map.insert(std::make_pair("minConvexity", std::to_string(m_blobParam.minConvexity)));
            map.insert(std::make_pair("maxConvexity", std::to_string(m_blobParam.maxConvexity)));
            map.insert(std::make_pair("filterByCircularity", std::to_string(m_blobParam.filterByCircularity)));
            map.insert(std::make_pair("minCircularity", std::to_string(m_blobParam.minCircularity)));
            map.insert(std::make_pair("maxCircularity", std::to_string(m_blobParam.maxCircularity)));
            map.insert(std::make_pair("filterByInertia", std::to_string(m_blobParam.filterByInertia)));
            map.insert(std::make_pair("minInertiaRatio", std::to_string(m_blobParam.minInertiaRatio)));
            map.insert(std::make_pair("maxInertiaRatio", std::to_string(m_blobParam.maxInertiaRatio)));
            return map;
        }

    public:

        cv::SimpleBlobDetector::Params m_blobParam;
};

//---------------------------//
//----- COcvSimpleBlobDetector -----//
//---------------------------//
class COcvSimpleBlobDetector : public C2dFeatureImageTask
{
    public:

        COcvSimpleBlobDetector() : C2dFeatureImageTask()
        {
            addKeypointOutput();
        }
        COcvSimpleBlobDetector(const std::string name, const std::shared_ptr<COcvSimpleBlobDetectorParam>& pParam) : C2dFeatureImageTask(name)
        {
            addKeypointOutput();
            m_pParam = std::make_shared<COcvSimpleBlobDetectorParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvSimpleBlobDetectorParam>(m_pParam);

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
                m_pFeatures = cv::SimpleBlobDetector::create(pParam->m_blobParam);
                makeFeatures(imgSrc, true, false);
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

class COcvSimpleBlobDetectorFactory : public CTaskFactory
{
    public:

        COcvSimpleBlobDetectorFactory()
        {
            m_info.m_name = QObject::tr("SimpleBlobDetector").toStdString();
            m_info.m_description = QObject::tr("This process implements a simple algorithm for extracting blobs from an image.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/2D features framework/Feature detection and description").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Features,Keypoints,Descriptors,Interest";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d0/d7a/classcv_1_1SimpleBlobDetector.html";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvSimpleBlobDetectorParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvSimpleBlobDetector>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvSimpleBlobDetectorParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvSimpleBlobDetector>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVSIMPLEBLOBDETECTOR_HPP
