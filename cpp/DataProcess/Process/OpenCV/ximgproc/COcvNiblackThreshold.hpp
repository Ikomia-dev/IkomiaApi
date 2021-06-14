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

#ifndef COCVNIBLACKTHRESHOLD_HPP
#define COCVNIBLACKTHRESHOLD_HPP

#include "opencv2/ximgproc.hpp"
#include "Core/C2dImageTask.h"
#include "IO/CImageIO.h"

//-------------------------------------//
//----- COcvNiblackThresholdParam -----//
//-------------------------------------//
class COcvNiblackThresholdParam: public CWorkflowTaskParam
{
    public:

        COcvNiblackThresholdParam() : CWorkflowTaskParam()
        {
        }

        void        setParamMap(const UMapString& paramMap) override
        {
            m_binaryMethod = std::stoi(paramMap.at("binaryMethod"));
            m_thresholdType = std::stoi(paramMap.at("thresholdType"));
            m_blockSize = std::stoi(paramMap.at("blockSize"));
            m_k = std::stod(paramMap.at("k"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("binaryMethod", std::to_string(m_binaryMethod)));
            map.insert(std::make_pair("thresholdType", std::to_string(m_thresholdType)));
            map.insert(std::make_pair("blockSize", std::to_string(m_blockSize)));
            map.insert(std::make_pair("k", std::to_string(m_k)));
            return map;
        }

    public:

        int     m_thresholdType = cv::THRESH_BINARY;
        int     m_blockSize = 33;
        int     m_binaryMethod = cv::ximgproc::BINARIZATION_NIBLACK;
        double  m_k = 1.0;
};

//--------------------------------------//
//----- COcvAdaptiveThresholdParam -----//
//--------------------------------------//
class COcvNiblackThreshold : public C2dImageTask
{
    public:

        COcvNiblackThreshold() : C2dImageTask()
        {
            setOutputDataType(IODataType::IMAGE_BINARY, 0);
            addOutput(std::make_shared<CImageIO>());
            setOutputColorMap(1, 0, {{255,0,0}});
        }
        COcvNiblackThreshold(const std::string name, const std::shared_ptr<COcvNiblackThresholdParam>& pParam) : C2dImageTask(name)
        {
            m_pParam = std::make_shared<COcvNiblackThresholdParam>(*pParam);
            setOutputDataType(IODataType::IMAGE_BINARY, 0);
            addOutput(std::make_shared<CImageIO>());
            setOutputColorMap(1, 0, {{255,0,0}});
        }

        size_t     getProgressSteps() override
        {
            return 3;
        }

        void    updateStaticOutputs() override
        {
            C2dImageTask::updateStaticOutputs();
            auto pImgOutput =  std::dynamic_pointer_cast<CImageIO>(getOutput(0));
            assert(pImgOutput);
            pImgOutput->setChannelCount(1);
        }

        void    run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsInput>(getInput(1));
            auto pParam = std::dynamic_pointer_cast<COcvNiblackThresholdParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgSrcTmp, imgDst;
            CMat imgSrc = pInput->getImage();
            createGraphicsMask(imgSrc.getNbCols(), imgSrc.getNbRows(), pGraphicsInput);
            emit m_signalHandler->doProgress();

            try
            {
                //Require 8 bits monochrome source image
                imgSrcTmp = conformInput(imgSrc);
                cv::ximgproc::niBlackThreshold(imgSrcTmp, imgDst, 255, pParam->m_thresholdType, pParam->m_blockSize, pParam->m_k, pParam->m_binaryMethod);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            forwardInputImage(0, 1);
            applyGraphicsMaskToBinary(imgDst, imgDst, 0);
            emit m_signalHandler->doProgress();

            auto pOutput = std::dynamic_pointer_cast<CImageIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(imgDst);

            endTaskRun();
            emit m_signalHandler->doProgress();
        }

        CMat    conformInput(CMat source)
        {
            CMat conformImg;

            if(source.depth() != CV_8S || source.depth() != CV_8U)
            {
                source.convertTo(conformImg, CV_8U);
                if(source.channels() != 1)
                    cv::cvtColor(source, conformImg, cv::COLOR_RGB2GRAY, 0);
            }
            else if(source.channels() != 1)
                cv::cvtColor(source, conformImg, cv::COLOR_RGB2GRAY, 0);
            else
                conformImg = source;

            return conformImg;
        }
};

class COcvNiblackThresholdFactory : public CTaskFactory
{
    public:

        COcvNiblackThresholdFactory()
        {
            m_info.m_name = QObject::tr("Niblack Threshold").toStdString();
            m_info.m_description = QObject::tr("Performs thresholding on input images using Niblack's technique or some of the popular variations it inspired.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Extended Image Processing").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "segmentation,adaptive,threshold,niblack,sauvola,nick,wolf";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/df/d2d/group__ximgproc.html#ga7b5b6f57106f4507d7195aef0fbe88d7";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvNiblackThresholdParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvNiblackThreshold>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvNiblackThresholdParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvNiblackThreshold>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVNIBLACKTHRESHOLD_HPP
