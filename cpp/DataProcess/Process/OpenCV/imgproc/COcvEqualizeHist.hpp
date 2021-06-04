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

#ifndef COCVEQUALIZEHIST_HPP
#define COCVEQUALIZEHIST_HPP

#include "Core/C2dImageTask.h"
#include "IO/CImageIO.h"

//------------------------------//
//----- COcvEqualizeHistParam -----//
//------------------------------//
class COcvEqualizeHistParam: public CWorkflowTaskParam
{
    public:

        COcvEqualizeHistParam() : CWorkflowTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            Q_UNUSED(paramMap);
        }

        UMapString  getParamMap() const override
        {
            return UMapString();
        }
};

//-------------------------//
//----- COcvEqualizeHist -----//
//-------------------------//
class COcvEqualizeHist : public C2dImageTask
{
    public:

        COcvEqualizeHist() : C2dImageTask()
        {
        }
        COcvEqualizeHist(const std::string name, const std::shared_ptr<COcvEqualizeHistParam>& pParam) : C2dImageTask(name)
        {
            m_pParam = std::make_shared<COcvEqualizeHistParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvEqualizeHistParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgDst;
            CMat imgSrc = pInput->getImage();

            if(imgSrc.depth() != CV_8S || imgSrc.depth() != CV_8U)
                imgSrc.convertTo(imgSrc, CV_8U);

            createGraphicsMask(imgSrc.getNbCols(), imgSrc.getNbRows(), pGraphicsInput);
            emit m_signalHandler->doProgress();

            try
            {
                if(imgSrc.channels() == 1)
                    cv::equalizeHist(imgSrc, imgDst);
                else
                {
                    cv::cvtColor(imgSrc, imgDst, cv::COLOR_RGB2Lab);
                    CMat channels[3];
                    cv::split(imgDst, channels);
                    cv::equalizeHist(channels[0], channels[0]);
                    cv::merge(channels, 3, imgDst);
                    cv::cvtColor(imgDst, imgDst, cv::COLOR_Lab2RGB);
                }
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

class COcvEqualizeHistFactory : public CTaskFactory
{
    public:

        COcvEqualizeHistFactory()
        {
            m_info.m_name = QObject::tr("Equalize histogram").toStdString();
            m_info.m_description = QObject::tr("This process normalizes the brightness and increases the contrast of the image.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Image processing/Histograms").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "brightness,contrast,histogram,normalization,equalization";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d6/dc7/group__imgproc__hist.html#ga7e54091f0c937d49bf84152a16f76d6e";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pEqualizeHistParam = std::dynamic_pointer_cast<COcvEqualizeHistParam>(pParam);
            if(pEqualizeHistParam != nullptr)
                return std::make_shared<COcvEqualizeHist>(m_info.m_name, pEqualizeHistParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pEqualizeHistParam = std::make_shared<COcvEqualizeHistParam>();
            assert(pEqualizeHistParam != nullptr);
            return std::make_shared<COcvEqualizeHist>(m_info.m_name, pEqualizeHistParam);
        }
};

#endif // COCVEQUALIZEHIST_HPP
