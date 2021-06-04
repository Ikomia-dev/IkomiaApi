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

#ifndef COCVFAST_HPP
#define COCVFAST_HPP

#include "Core/C2dFeatureImageTask.h"
#include "IO/CImageIO.h"

//--------------------------------//
//----- COcvFASTParam -----//
//--------------------------------//
class COcvFASTParam: public CWorkflowTaskParam
{
    public:

        COcvFASTParam() : CWorkflowTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_threshold = std::stoi(paramMap.at("thresh"));
            m_bNonmaxSuppression = std::stoi(paramMap.at("nonmaxsupp"));
            m_type = std::stoi(paramMap.at("type"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("thresh", std::to_string(m_threshold)));
            map.insert(std::make_pair("nonmaxsupp", std::to_string(m_bNonmaxSuppression)));
            map.insert(std::make_pair("type", std::to_string(m_type)));
            return map;
        }

    public:

        int     m_threshold = 10;
        bool    m_bNonmaxSuppression = true;
        int     m_type = cv::FastFeatureDetector::TYPE_9_16;
};

//---------------------------//
//----- COcvFAST -----//
//---------------------------//
class COcvFAST : public C2dFeatureImageTask
{
    public:

        COcvFAST() : C2dFeatureImageTask()
        {
            addKeypointOutput();
        }
        COcvFAST(const std::string name, const std::shared_ptr<COcvFASTParam>& pParam) : C2dFeatureImageTask(name)
        {
            addKeypointOutput();
            m_pParam = std::make_shared<COcvFASTParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvFASTParam>(m_pParam);

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
                m_pFeatures = cv::FastFeatureDetector::create(pParam->m_threshold, pParam->m_bNonmaxSuppression,
                                                              static_cast<cv::FastFeatureDetector::DetectorType>(pParam->m_type));
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

class COcvFASTFactory : public CTaskFactory
{
    public:

        COcvFASTFactory()
        {
            m_info.m_name = QObject::tr("FAST").toStdString();
            m_info.m_description = QObject::tr("This process implements the FAST keypoint detector and descriptor extractor.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/2D features framework/Feature detection and description").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Features,Keypoints,Descriptors,Interest";
            m_info.m_authors = "Edward Rosten and Tom Drummond";
            m_info.m_article = "Machine learning for high-speed corner detection";
            m_info.m_journal = "ECCV 2006, pages 430-443";
            m_info.m_year = 2006;
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d5/d51/group__features2d__main.html#gaf3185c9bd7496ba7a629add75fb371ad";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvFASTParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvFAST>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvFASTParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvFAST>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVFAST_HPP
