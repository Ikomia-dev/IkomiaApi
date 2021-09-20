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

#ifndef COCVRIDGEFILTER_HPP
#define COCVRIDGEFILTER_HPP

#include "opencv2/ximgproc.hpp"
#include "Core/C2dImageTask.h"

//-------------------------------------//
//----- COcvRidgeFilterParam -----//
//-------------------------------------//
class COcvRidgeFilterParam: public CWorkflowTaskParam
{
    public:

        COcvRidgeFilterParam() : CWorkflowTaskParam()
        {
        }

        void        setParamMap(const UMapString& paramMap) override
        {
            m_ddepth = std::stoi(paramMap.at("ddepth"));
            m_dx = std::stoi(paramMap.at("dx"));
            m_dy = std::stoi(paramMap.at("dy"));
            m_ksize = std::stoi(paramMap.at("ksize"));
            m_out_dtype = std::stoi(paramMap.at("out_dtype"));
            m_scale = std::stod(paramMap.at("scale"));
            m_delta = std::stod(paramMap.at("delta"));
            m_borderType = std::stoi(paramMap.at("borderType"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("ddepth", std::to_string(m_ddepth)));
            map.insert(std::make_pair("dx", std::to_string(m_dx)));
            map.insert(std::make_pair("dy", std::to_string(m_dy)));
            map.insert(std::make_pair("ksize", std::to_string(m_ksize)));
            map.insert(std::make_pair("out_dtype", std::to_string(m_out_dtype)));
            map.insert(std::make_pair("scale", std::to_string(m_scale)));
            map.insert(std::make_pair("delta", std::to_string(m_delta)));
            map.insert(std::make_pair("borderType", std::to_string(m_borderType)));
            return map;
        }

    public:
        int  	m_ddepth = CV_32FC1;
        int  	m_dx = 1;
        int  	m_dy = 1;
        int  	m_ksize = 3;
        int  	m_out_dtype = CV_8UC1;
        double  m_scale = 1;
        double  m_delta = 0;
        int  	m_borderType = cv::BORDER_DEFAULT;

};

//--------------------------------//
//----- COcvRidgeFilter -----//
//--------------------------------//
class COcvRidgeFilter : public C2dImageTask
{
    public:

        COcvRidgeFilter() : C2dImageTask()
        {
        }
        COcvRidgeFilter(const std::string& name, const std::shared_ptr<COcvRidgeFilterParam>& pParam) : C2dImageTask(name)
        {
            m_pParam = std::make_shared<COcvRidgeFilterParam>(*pParam);
        }

        size_t     getProgressSteps() override
        {
            return 3;
        }

        void    run() override
        {
            beginTaskRun();

            auto pInput = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsInput>(getInput(1));
            auto pParam = std::dynamic_pointer_cast<COcvRidgeFilterParam>(m_pParam);

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
                auto pRidge = cv::ximgproc::RidgeDetectionFilter::create(pParam->m_ddepth,
                                                                         pParam->m_dx,
                                                                         pParam->m_dy,
                                                                         pParam->m_ksize,
                                                                         pParam->m_out_dtype,
                                                                         pParam->m_scale,
                                                                         pParam->m_delta,
                                                                         pParam->m_borderType);
                pRidge->getRidgeFilteredImage(imgSrc, imgDst);
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

class COcvRidgeFilterFactory : public CTaskFactory
{
    public:

        COcvRidgeFilterFactory()
        {
            m_info.m_name = "ocv_ridge_filter";
            m_info.m_description = QObject::tr("Applies Ridge Detection Filter to an input image. Implements Ridge detection similar to the one in Mathematica using the eigen values from the Hessian Matrix of the input image using Sobel Derivatives. Additional refinement can be done using Skeletonization and Binarization.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Extended Image Processing/Filters").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Segmentation,Veins";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d4/d36/classcv_1_1ximgproc_1_1RidgeDetectionFilter.html";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pManifoldParam = std::dynamic_pointer_cast<COcvRidgeFilterParam>(pParam);
            if(pManifoldParam != nullptr)
                return std::make_shared<COcvRidgeFilter>(m_info.m_name, pManifoldParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pManifoldParam = std::make_shared<COcvRidgeFilterParam>();
            assert(pManifoldParam != nullptr);
            return std::make_shared<COcvRidgeFilter>(m_info.m_name, pManifoldParam);

        }
};

#endif // COCVRIDGEFILTER_HPP
