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

#ifndef COCVGAUSSIANBLUR_H
#define COCVGAUSSIANBLUR_H

#include "Core/C2dImageTask.h"
#include "IO/CImageIO.h"

//------------------------------//
//----- COcvGaussianBlurParam -----//
//------------------------------//
class COcvGaussianBlurParam: public CWorkflowTaskParam
{
    public:

        COcvGaussianBlurParam() : CWorkflowTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_size.width = std::stoi(paramMap.at("sizeX"));
            m_size.height = std::stoi(paramMap.at("sizeY"));
            m_sigmaX = std::stod(paramMap.at("sigmaX"));
            m_sigmaY = std::stod(paramMap.at("sigmaY"));
            m_borderType = std::stoi(paramMap.at("borderType"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("sizeX", std::to_string(m_size.width)));
            map.insert(std::make_pair("sizeY", std::to_string(m_size.height)));
            map.insert(std::make_pair("sigmaX", std::to_string(m_sigmaX)));
            map.insert(std::make_pair("sigmaY", std::to_string(m_sigmaY)));
            map.insert(std::make_pair("borderType", std::to_string(m_borderType)));
            return map;
        }

    public:

        cv::Size    m_size = cv::Size(3,3);
        double      m_sigmaX = 1.0;
        double      m_sigmaY = 1.0;
        int         m_borderType = cv::BORDER_DEFAULT;
};

//-------------------------//
//----- COcvGaussianBlur -----//
//-------------------------//
class COcvGaussianBlur : public C2dImageTask
{
    public:

        COcvGaussianBlur() : C2dImageTask()
        {
        }
        COcvGaussianBlur(const std::string name, const std::shared_ptr<COcvGaussianBlurParam>& pParam) : C2dImageTask(name)
        {
            m_pParam = std::make_shared<COcvGaussianBlurParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvGaussianBlurParam>(m_pParam);

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
                cv::GaussianBlur(imgSrc, imgDst, pParam->m_size, pParam->m_sigmaX, pParam->m_sigmaY, pParam->m_borderType);
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

class COcvGaussianBlurFactory : public CTaskFactory
{
    public:

        COcvGaussianBlurFactory()
        {
            m_info.m_name = QObject::tr("Gaussian Blur").toStdString();
            m_info.m_description = QObject::tr("This process smoothes your image with a Gaussian filter.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Image processing/Image filtering").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "smooth,blur,filter,gaussian";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d4/d86/group__imgproc__filter.html#gaabe8c836e97159a9193fb0b11ac52cf1";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvGaussianBlurParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvGaussianBlur>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvGaussianBlurParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvGaussianBlur>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVGAUSSIANBLUR_H
