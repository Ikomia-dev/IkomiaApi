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

#ifndef COCVGUIDEDFILTER_HPP
#define COCVGUIDEDFILTER_HPP

#include "opencv2/ximgproc.hpp"
#include "Core/C2dImageTask.h"

//-------------------------------------//
//----- COcvGuidedFilterParam -----//
//-------------------------------------//
class COcvGuidedFilterParam: public CWorkflowTaskParam
{
    public:

        COcvGuidedFilterParam() : CWorkflowTaskParam()
        {
        }

        void        setParamMap(const UMapString& paramMap) override
        {
            Q_UNUSED(paramMap)
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            return map;
        }

    public:

        int         m_radius = 8;
        double      m_eps = 0.1;
        int         m_ddepth = -1;
};

//--------------------------------//
//----- COcvGuidedFilter -----//
//--------------------------------//
class COcvGuidedFilter : public C2dImageTask
{
    public:

        COcvGuidedFilter() : C2dImageTask()
        {
            insertInput(std::make_shared<CImageIO>(), 1);
        }
        COcvGuidedFilter(const std::string name, const std::shared_ptr<COcvGuidedFilterParam>& pParam) : C2dImageTask(name)
        {
            insertInput(std::make_shared<CImageIO>(), 1);
            m_pParam = std::make_shared<COcvGuidedFilterParam>(*pParam);
        }

        size_t     getProgressSteps() override
        {
            return 3;
        }

        void    run() override
        {
            beginTaskRun();

            auto pInput1 = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            auto pInput2 = std::dynamic_pointer_cast<CImageIO>(getInput(1));
            auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsInput>(getInput(2));
            auto pParam = std::dynamic_pointer_cast<COcvGuidedFilterParam>(m_pParam);

            if(pInput1 == nullptr || pInput2 == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput1->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgDst, imgGuide;
            CMat imgSrc = pInput1->getImage();

            if(imgSrc.type() == CV_8U || imgSrc.type() == CV_8UC3)
                imgSrc.convertTo(imgSrc, CV_32F, 1.0f/255.0f);

            if(pInput2->isDataAvailable() == false)
                imgGuide = imgSrc;
            else
                imgGuide = pInput2->getImage();

            if(imgGuide.type() == CV_8U || imgGuide.type() == CV_8UC3)
                imgGuide.convertTo(imgGuide, CV_32F, 1.0f/255.0f);

            createGraphicsMask(imgSrc.getNbCols(), imgSrc.getNbRows(), pGraphicsInput);
            emit m_signalHandler->doProgress();

            try
            {
                /*auto ptr = cv::ximgproc::createGuidedFilter(imgGuide, pParam->m_radius, pParam->m_eps);
                ptr->filter(imgSrc, imgDst, pParam->m_ddepth);*/

                cv::ximgproc::guidedFilter(imgGuide, imgSrc, imgDst, pParam->m_radius, pParam->m_eps, pParam->m_ddepth);

                if(imgSrc.type() == CV_8U || imgSrc.type() == CV_8UC3)
                    imgDst.convertTo(imgDst, CV_8U, 255);
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

class COcvGuidedFilterFactory : public CTaskFactory
{
    public:

        COcvGuidedFilterFactory()
        {
            m_info.m_name = QObject::tr("Guided Filter").toStdString();
            m_info.m_description = QObject::tr("This process applies the guided filter.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Extended Image Processing/Filters").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Smooth, Blur,Anisotropic,Filter,Gaussian,Edge preserving";
            m_info.m_authors = "Kaiming He, Jian Sun, and Xiaoou Tang";
            m_info.m_article = "Guided image filtering.";
            m_info.m_journal = "ECCV 2010, pages 1–14";
            m_info.m_year = 2010;
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/da/d17/group__ximgproc__filters.html#ga86813d59f8452a9600e979c6287805f5";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pManifoldParam = std::dynamic_pointer_cast<COcvGuidedFilterParam>(pParam);
            if(pManifoldParam != nullptr)
                return std::make_shared<COcvGuidedFilter>(m_info.m_name, pManifoldParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pManifoldParam = std::make_shared<COcvGuidedFilterParam>();
            assert(pManifoldParam != nullptr);
            return std::make_shared<COcvGuidedFilter>(m_info.m_name, pManifoldParam);

        }
};

#endif // COCVGUIDEDFILTER_HPP
