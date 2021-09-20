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

#ifndef COCVLEARNINGBASEDWB_HPP
#define COCVLEARNINGBASEDWB_HPP

#include "Core/C2dImageTask.h"
#include "IO/CImageIO.h"
#include <opencv2/xphoto.hpp>

//------------------------------//
//----- COcvLearningBasedWBParam -----//
//------------------------------//
class COcvLearningBasedWBParam: public CWorkflowTaskParam
{
    public:

        COcvLearningBasedWBParam() : CWorkflowTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_histBinNum = std::stoi(paramMap.at("histBinNum"));
            m_rangeMaxVal = std::stoi(paramMap.at("rangeMaxVal"));
            m_satThreshold = std::stof(paramMap.at("satThreshold"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("histBinNum", std::to_string(m_histBinNum)));
            map.insert(std::make_pair("rangeMaxVal", std::to_string(m_rangeMaxVal)));
            map.insert(std::make_pair("satThreshold", std::to_string(m_satThreshold)));

            return map;
        }

    public:
        int     m_histBinNum = 64;
        int     m_rangeMaxVal = 255;
        float   m_satThreshold = 0.98f;
};

//-------------------------//
//----- COcvLearningBasedWB -----//
//-------------------------//
class COcvLearningBasedWB : public C2dImageTask
{
    public:

        COcvLearningBasedWB() : C2dImageTask()
        {
        }
        COcvLearningBasedWB(const std::string name, const std::shared_ptr<COcvLearningBasedWBParam>& pParam) : C2dImageTask(name)
        {
            m_pParam = std::make_shared<COcvLearningBasedWBParam>(*pParam);
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        void run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsInput>(getInput(1));
            auto pParam = std::dynamic_pointer_cast<COcvLearningBasedWBParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgDst;
            CMat imgSrc = pInput->getImage();
            createGraphicsMask(imgSrc.getNbCols(), imgSrc.getNbRows(), pGraphicsInput);
            emit m_signalHandler->doProgress();

            try
            {
                auto pLWB = cv::xphoto::createLearningBasedWB();
                pLWB->setHistBinNum(pParam->m_histBinNum);
                pLWB->setRangeMaxVal(pParam->m_rangeMaxVal);
                pLWB->setSaturationThreshold(pParam->m_satThreshold);
                pLWB->balanceWhite(imgSrc, imgDst);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            endTaskRun();
            applyGraphicsMask(imgSrc, imgDst, 0);
            emit m_signalHandler->doProgress();

            auto pOutput = std::dynamic_pointer_cast<CImageIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(imgDst);

            emit m_signalHandler->doProgress();
        }
};

class COcvLearningBasedWBFactory : public CTaskFactory
{
    public:

        COcvLearningBasedWBFactory()
        {
            m_info.m_name = "ocv_learning_based_wb";
            m_info.m_description = QObject::tr("More sophisticated learning-based automatic white balance algorithm.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Additional photo processing algorithms").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "White,Balance,AutoWhiteBalance";
            m_info.m_authors = "Dongliang Cheng, Brian Price, Scott Cohen, and Michael S Brown";
            m_info.m_article = "Effective learning-based illuminant estimation using simple features";
            m_info.m_journal = "IEEE CVPR, pages 1000–1008";
            m_info.m_year = 2015;
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d4/d3b/classcv_1_1xphoto_1_1LearningBasedWB.html";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvLearningBasedWBParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvLearningBasedWB>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvLearningBasedWBParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvLearningBasedWB>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVLEARNINGBASEDWB_HPP
