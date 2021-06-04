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

#ifndef COCVDTFILTERSTYLIZE_HPP
#define COCVDTFILTERSTYLIZE_HPP
#include "Core/C2dImageTask.h"
#include "IO/CImageIO.h"
#include <opencv2/ximgproc.hpp>

//------------------------------------//
//----- COcvDTFilterStylizeParam -----//
//------------------------------------//
class COcvDTFilterStylizeParam: public CWorkflowTaskParam
{
    public:

        COcvDTFilterStylizeParam() : CWorkflowTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
             m_sigmaColor = std::stoi(paramMap.at("sigmaColor"));
             m_sigmaSpatial = std::stoi(paramMap.at("sigmaSpatial"));
             m_edgesGamma = std::stoi(paramMap.at("edgesGamma"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("sigmaColor", std::to_string(m_sigmaColor)));
            map.insert(std::make_pair("sigmaSpatial", std::to_string(m_sigmaSpatial)));
            map.insert(std::make_pair("edgesGamma", std::to_string(m_edgesGamma)));
            return map;
        }

    public:
        double m_sigmaColor = 0.8;
        double m_sigmaSpatial = 17;
        int    m_edgesGamma = 100;
};

//-------------------------------//
//----- COcvDTFilterStylize -----//
//-------------------------------//
class COcvDTFilterStylize : public C2dImageTask
{
    public:

        COcvDTFilterStylize() : C2dImageTask()
        {
        }
        COcvDTFilterStylize(const std::string name, const std::shared_ptr<COcvDTFilterStylizeParam>& pParam) : C2dImageTask(name)
        {
            m_pParam = std::make_shared<COcvDTFilterStylizeParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvDTFilterStylizeParam>(m_pParam);

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
                if(imgSrc.channels() != 3)
                    throw CException(CoreExCode::INVALID_PARAMETER, "Input image must have 3 channels (color image)", __func__, __FILE__, __LINE__);

                if(imgSrc.type() == CV_8U || imgSrc.type() == CV_8UC3)
                    imgSrc.convertTo(imgSrc, CV_32F, 1.0f/255.0f);

                //blur frame
                cv::Mat filtered;
                cv::ximgproc::dtFilter(imgSrc, imgSrc, filtered, pParam->m_sigmaSpatial, pParam->m_sigmaColor, cv::ximgproc::DTF_NC);

                //compute grayscale blurred frame
                cv::Mat filteredGray;
                cv::cvtColor(filtered, filteredGray, cv::COLOR_BGR2GRAY);

                //find gradients of blurred image
                cv::Mat gradX, gradY;
                Sobel(filteredGray, gradX, CV_32F, 1, 0, 3);
                Sobel(filteredGray, gradY, CV_32F, 0, 1, 3);

                //compute magnitude of gradient and fit it accordingly the gamma parameter
                cv::Mat gradMagnitude;
                cv::magnitude(gradX, gradY, gradMagnitude);
                cv::pow(gradMagnitude, pParam->m_edgesGamma/100.0, gradMagnitude);

                //multiply a blurred frame to the value inversely proportional to the magnitude
                cv::Mat multiplier = 1.0/(1.0 + gradMagnitude*255);
                cv::cvtColor(multiplier, multiplier, cv::COLOR_GRAY2BGR);
                cv::multiply(filtered, multiplier, imgDst, 255, imgSrc.type());
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }            
            emit m_signalHandler->doProgress();

            if(pGraphicsInput->isDataAvailable())
            {
                imgSrc *= 255.0f;
                applyGraphicsMask(imgSrc, imgDst, 0);
            }

            auto pOutput = std::dynamic_pointer_cast<CImageIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(imgDst);

            endTaskRun();
            emit m_signalHandler->doProgress();
        }
};

class COcvDTFilterStylizeFactory : public CTaskFactory
{
    public:

        COcvDTFilterStylizeFactory()
        {
            m_info.m_name = QObject::tr("DTFilterStylize").toStdString();
            m_info.m_description = QObject::tr("This process produces stylized image.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Extended Image Processing/Filters").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Cartoon,Stylization,DTFilterStylize";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDTFilterStylizeParam = std::dynamic_pointer_cast<COcvDTFilterStylizeParam>(pParam);
            if(pDTFilterStylizeParam != nullptr)
                return std::make_shared<COcvDTFilterStylize>(m_info.m_name, pDTFilterStylizeParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDTFilterStylizeParam = std::make_shared<COcvDTFilterStylizeParam>();
            assert(pDTFilterStylizeParam != nullptr);
            return std::make_shared<COcvDTFilterStylize>(m_info.m_name, pDTFilterStylizeParam);
        }
};

#endif // COCVDTFILTERSTYLIZE_HPP
