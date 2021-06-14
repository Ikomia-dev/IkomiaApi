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

#ifndef COCVINPAINTX_HPP
#define COCVINPAINTX_HPP

#include "opencv2/xphoto.hpp"
#include "Core/C2dImageTask.h"

class COcvInpaintX : public C2dImageTask
{
    public:

        COcvInpaintX() : C2dImageTask()
        {
        }
        COcvInpaintX(const std::string name, const std::shared_ptr<CWorkflowTaskParam>& pParam) : C2dImageTask(name)
        {
            m_pParam = std::make_shared<CWorkflowTaskParam>(*pParam);
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

            if(pInput == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid input", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgDst;
            CMat imgSrc = pInput->getImage();
            createGraphicsMask(imgSrc.getNbCols(), imgSrc.getNbRows(), pGraphicsInput);
            emit m_signalHandler->doProgress();

            auto mask = getGraphicsMask(0);
            if(mask.data == nullptr)
                throw CException(CoreExCode::NULL_POINTER, "Empty region of interest", __func__, __FILE__, __LINE__);

            try
            {
                cv::bitwise_not(mask, mask);
                if(imgSrc.channels() >= 3)
                {
                    CMat srcTmp;
                    cv::cvtColor(imgSrc, srcTmp, cv::COLOR_RGB2Lab);
                    cv::xphoto::inpaint(srcTmp, mask, imgDst, cv::xphoto::INPAINT_SHIFTMAP);
                    cv::cvtColor(imgDst, imgDst, cv::COLOR_Lab2RGB);
                }
                else
                    cv::xphoto::inpaint(imgSrc, mask, imgDst, cv::xphoto::INPAINT_SHIFTMAP);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            endTaskRun();
            emit m_signalHandler->doProgress();

            auto pOutput = std::dynamic_pointer_cast<CImageIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(imgDst);

            emit m_signalHandler->doProgress();
        }
};

class COcvInpaintXFactory : public CTaskFactory
{
    public:

        COcvInpaintXFactory()
        {
            m_info.m_name = QObject::tr("XPhoto Inpaint").toStdString();
            m_info.m_description = QObject::tr("This process performs inpainting from region of interest drawn in original image.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Additional photo processing algorithms").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "inpainting,photo,deletion";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/de/daa/group__xphoto.html#gab4febba6be53e5fddc480b8cedf51eee";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            return std::make_shared<COcvInpaintX>(m_info.m_name, pParam);
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pParam = std::make_shared<CWorkflowTaskParam>();
            assert(pParam != nullptr);
            return std::make_shared<COcvInpaintX>(m_info.m_name, pParam);
        }
};

#endif // COCVINPAINTX_H
