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

#ifndef COCVAGAST_HPP
#define COCVAGAST_HPP

#include "Core/C2dFeatureImageTask.h"
#include "IO/CImageIO.h"

//--------------------------------//
//----- COcvAGASTParam -----//
//--------------------------------//
class COcvAGASTParam: public CWorkflowTaskParam
{
    public:

        COcvAGASTParam() : CWorkflowTaskParam(){}

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
        int     m_type = cv::AgastFeatureDetector::OAST_9_16;
};

//---------------------------//
//----- COcvAGAST -----//
//---------------------------//
class COcvAGAST : public C2dFeatureImageTask
{
    public:

        COcvAGAST() : C2dFeatureImageTask()
        {
            addKeypointOutput();
        }
        COcvAGAST(const std::string name, const std::shared_ptr<COcvAGASTParam>& pParam) : C2dFeatureImageTask(name)
        {
            addKeypointOutput();
            m_pParam = std::make_shared<COcvAGASTParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvAGASTParam>(m_pParam);

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
                m_pFeatures = cv::AgastFeatureDetector::create(pParam->m_threshold, pParam->m_bNonmaxSuppression,
                                                               static_cast<cv::AgastFeatureDetector::DetectorType>(pParam->m_type));
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

class COcvAGASTFactory : public CTaskFactory
{
    public:

        COcvAGASTFactory()
        {
            m_info.m_name = "ocv_agast";
            m_info.m_description = QObject::tr("This process implements the AGAST keypoint detector.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/2D features framework/Feature detection and description").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Features,Keypoints,Descriptors,Interest";
            m_info.m_authors = "Elmar Mair, Gregory D. Hager, Darius Burschka, Michael Suppa, and Gerhard Hirzinger";
            m_info.m_article = "Adaptive and generic corner detection based on the accelerated segment test";
            m_info.m_journal = "ECCV 2010";
            m_info.m_year = 2010;
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d5/d51/group__features2d__main.html#gaae379d8d4fffa0aa3c37ebf17fe6e07c";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvAGASTParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvAGAST>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvAGASTParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvAGAST>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVAGAST_HPP
