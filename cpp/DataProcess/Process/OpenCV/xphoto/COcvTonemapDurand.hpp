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

#ifndef COCVTONEMAPDURAND_HPP
#define COCVTONEMAPDURAND_HPP

#include "Core/C2dImageTask.h"
#include "IO/CImageIO.h"
#include <opencv2/xphoto.hpp>

//----------------------------------//
//----- COcvTonemapDurandParam -----//
//----------------------------------//
class COcvTonemapDurandParam : public CWorkflowTaskParam
{
    public:

        COcvTonemapDurandParam() : CWorkflowTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_gamma = std::stof(paramMap.at("gamma"));
            m_contrast = std::stof(paramMap.at("contrast"));
            m_saturation = std::stof(paramMap.at("sat"));
            m_sigmaSpace = std::stof(paramMap.at("sigmaS"));
            m_sigmaColor = std::stof(paramMap.at("sigmaC"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("gamma", std::to_string(m_gamma)));
            map.insert(std::make_pair("contrast", std::to_string(m_contrast)));
            map.insert(std::make_pair("sat", std::to_string(m_saturation)));
            map.insert(std::make_pair("sigmaS", std::to_string(m_sigmaSpace)));
            map.insert(std::make_pair("sigmaC", std::to_string(m_sigmaColor)));
            return map;
        }

    public:
        float m_gamma = 1.0;
        float m_contrast = 4.0f;
        float m_saturation = 1.0f;
        float m_sigmaSpace = 2.0f;
        float m_sigmaColor = 2.0f;
};

//-----------------------------//
//----- COcvTonemapDurand -----//
//-----------------------------//
class COcvTonemapDurand : public C2dImageTask
{
    public:

        COcvTonemapDurand() : C2dImageTask()
        {
        }
        COcvTonemapDurand(const std::string name, const std::shared_ptr<COcvTonemapDurandParam>& pParam) : C2dImageTask(name)
        {
            m_pParam = std::make_shared<COcvTonemapDurandParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvTonemapDurandParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            auto image = pInput->getImage();

            CMat imgDst;
            createGraphicsMask(image.getNbCols(), image.getNbRows(), pGraphicsInput);
            emit m_signalHandler->doProgress();

            try
            {
                 auto pTonemapDurand = cv::xphoto::createTonemapDurand(pParam->m_gamma, pParam->m_contrast, pParam->m_saturation, pParam->m_sigmaSpace, pParam->m_sigmaColor);
                 pTonemapDurand->process(image, imgDst);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            endTaskRun();
            applyGraphicsMask(image, imgDst, 0);
            emit m_signalHandler->doProgress();

            auto pOutput = std::dynamic_pointer_cast<CImageIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(imgDst);

            emit m_signalHandler->doProgress();
        }
};

class COcvTonemapDurandFactory : public CTaskFactory
{
    public:

        COcvTonemapDurandFactory()
        {
            m_info.m_name = "ocv_tonemap_durand";
            m_info.m_description = QObject::tr("This algorithm decomposes image into two layers: base layer and detail layer using bilateral filter and compresses contrast of the base layer thus preserving all the details.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Additional photo processing algorithms").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Tonemapping, Durand, HDR";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/da/d3d/classcv_1_1TonemapDurand.html";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pTonemapDurandParam = std::dynamic_pointer_cast<COcvTonemapDurandParam>(pParam);
            if(pTonemapDurandParam != nullptr)
                return std::make_shared<COcvTonemapDurand>(m_info.m_name, pTonemapDurandParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pTonemapDurandParam = std::make_shared<COcvTonemapDurandParam>();
            assert(pTonemapDurandParam != nullptr);
            return std::make_shared<COcvTonemapDurand>(m_info.m_name, pTonemapDurandParam);
        }
};

#endif // COCVTONEMAPDURAND_HPP
