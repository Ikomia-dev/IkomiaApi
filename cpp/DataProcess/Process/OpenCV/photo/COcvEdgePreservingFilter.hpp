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

#ifndef COCVEDGEPRESERVINGFILTER_HPP
#define COCVEDGEPRESERVINGFILTER_HPP

#include "Core/C2dImageTask.h"
#include "IO/CImageIO.h"

//-----------------------------------------//
//----- COcvEdgePreservingFilterParam -----//
//-----------------------------------------//
class COcvEdgePreservingFilterParam: public CWorkflowTaskParam
{
    public:

        COcvEdgePreservingFilterParam() : CWorkflowTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_flag = std::stoi(paramMap.at("flag"));
            m_sigmaS = std::stof(paramMap.at("sigmaS"));
            m_sigmaR = std::stof(paramMap.at("sigmaR"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("flag", std::to_string(m_flag)));
            map.insert(std::make_pair("sigmaS", std::to_string(m_sigmaS)));
            map.insert(std::make_pair("sigmaR", std::to_string(m_sigmaR)));
            return map;
        }

    public:

        int         m_flag = cv::RECURS_FILTER;
        float       m_sigmaS = 60.0f;
        float       m_sigmaR = 0.4f;
};

//------------------------------------//
//----- COcvEdgePreservingFilter -----//
//------------------------------------//
class COcvEdgePreservingFilter : public C2dImageTask
{
    public:

        COcvEdgePreservingFilter() : C2dImageTask()
        {
        }
        COcvEdgePreservingFilter(const std::string name, const std::shared_ptr<COcvEdgePreservingFilterParam>& pParam) : C2dImageTask(name)
        {
            m_pParam = std::make_shared<COcvEdgePreservingFilterParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvEdgePreservingFilterParam>(m_pParam);

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
                if(imgSrc.channels() == 1)
                    cv::cvtColor(imgSrc, imgSrc, cv::COLOR_GRAY2RGB);

                cv::edgePreservingFilter(imgSrc, imgDst, pParam->m_flag, pParam->m_sigmaS, pParam->m_sigmaR);
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

class COcvEdgePreservingFilterFactory : public CTaskFactory
{
    public:

        COcvEdgePreservingFilterFactory()
        {
            m_info.m_name = QObject::tr("Edge Preserving Filter").toStdString();
            m_info.m_description = QObject::tr("This process smoothes your image with an edge preserving filter.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Computational photography/Non-photorealistic rendering").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "smooth,blur,edge,preserving,filter";
            m_info.m_authors = "Eduardo SL Gastal and Manuel M Oliveira";
            m_info.m_article = "Domain transform for edge-aware image and video processing";
            m_info.m_journal = "In ACM Transactions on Graphics (TOG), volume 30, page 69. ACM.";
            m_info.m_year = 2011;
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/df/dac/group__photo__render.html#gafaee2977597029bc8e35da6e67bd31f7";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pEdgePreservingFilterParam = std::dynamic_pointer_cast<COcvEdgePreservingFilterParam>(pParam);
            if(pEdgePreservingFilterParam != nullptr)
                return std::make_shared<COcvEdgePreservingFilter>(m_info.m_name, pEdgePreservingFilterParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pEdgePreservingFilterParam = std::make_shared<COcvEdgePreservingFilterParam>();
            assert(pEdgePreservingFilterParam != nullptr);
            return std::make_shared<COcvEdgePreservingFilter>(m_info.m_name, pEdgePreservingFilterParam);
        }
};

#endif // COCVEDGEPRESERVINGFILTER_HPP
