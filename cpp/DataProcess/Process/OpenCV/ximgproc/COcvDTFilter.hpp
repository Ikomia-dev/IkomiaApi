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

#ifndef COCVDTFILTER_HPP
#define COCVDTFILTER_HPP
#include "Core/C2dImageTask.h"
#include "IO/CImageIO.h"
#include <opencv2/ximgproc.hpp>

//----------------------------//
//----- COcvDTFilterParam -----//
//----------------------------//
class COcvDTFilterParam: public CWorkflowTaskParam
{
    public:

        COcvDTFilterParam() : CWorkflowTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
             m_sigmaColor = std::stod(paramMap.at("sigmaColor"));
             m_sigmaSpatial = std::stod(paramMap.at("sigmaSpatial"));
             m_mode = std::stoi(paramMap.at("mode"));
             m_numIters = std::stoi(paramMap.at("numIters"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("sigmaColor", std::to_string(m_sigmaColor)));
            map.insert(std::make_pair("sigmaSpatial", std::to_string(m_sigmaSpatial)));
            map.insert(std::make_pair("mode", std::to_string(m_mode)));
            map.insert(std::make_pair("numIters", std::to_string(m_numIters)));
            return map;
        }

    public:
        double m_sigmaColor = 0.8;
        double m_sigmaSpatial = 17;
        int    m_mode = cv::ximgproc::DTF_NC;
        int    m_numIters = 3;
};

//-----------------------//
//----- COcvDTFilter -----//
//-----------------------//
class COcvDTFilter : public C2dImageTask
{
    public:

        COcvDTFilter() : C2dImageTask()
        {
            insertInput(std::make_shared<CImageIO>(), 1);
        }
        COcvDTFilter(const std::string name, const std::shared_ptr<COcvDTFilterParam>& pParam) : C2dImageTask(name)
        {
            insertInput(std::make_shared<CImageIO>(), 1);
            m_pParam = std::make_shared<COcvDTFilterParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvDTFilterParam>(m_pParam);

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
                cv::ximgproc::dtFilter(imgGuide, imgSrc, imgDst, pParam->m_sigmaSpatial, pParam->m_sigmaColor, pParam->m_mode, pParam->m_numIters);

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

class COcvDTFilterFactory : public CTaskFactory
{
    public:

        COcvDTFilterFactory()
        {
            m_info.m_name = "ocv_dt_filter";
            m_info.m_description = QObject::tr("This process produces domain transform filtering operation on source image.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Extended Image Processing/Filters").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Smooth,Blur,Edge,Preserving,Anisotropic,DTFilter";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d3/d9d/classcv_1_1ximgproc_1_1DTFilter.html";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDTFilterParam = std::dynamic_pointer_cast<COcvDTFilterParam>(pParam);
            if(pDTFilterParam != nullptr)
                return std::make_shared<COcvDTFilter>(m_info.m_name, pDTFilterParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDTFilterParam = std::make_shared<COcvDTFilterParam>();
            assert(pDTFilterParam != nullptr);
            return std::make_shared<COcvDTFilter>(m_info.m_name, pDTFilterParam);
        }
};

#endif // COCVDTFILTER_HPP
