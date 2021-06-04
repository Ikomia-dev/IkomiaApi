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

#ifndef COCVSTYLIZATION_HPP
#define COCVSTYLIZATION_HPP

#include "Core/C2dImageTask.h"
#include "IO/CImageIO.h"
#include <opencv2/photo.hpp>

//----------------------------------//
//----- COcvStylizationParam -----//
//----------------------------------//
class COcvStylizationParam : public CWorkflowTaskParam
{
    public:

        COcvStylizationParam() : CWorkflowTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_sigmaS = std::stof(paramMap.at("sigmaS"));
            m_sigmaR = std::stof(paramMap.at("sigmaR"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("sigmaS", std::to_string(m_sigmaS)));
            map.insert(std::make_pair("sigmaR", std::to_string(m_sigmaR)));
            return map;
        }

    public:
        float m_sigmaS = 60.0f;
        float m_sigmaR = 0.45f;
};

//-----------------------------//
//----- COcvStylization -----//
//-----------------------------//
class COcvStylization : public C2dImageTask
{
    public:

        COcvStylization() : C2dImageTask()
        {
        }
        COcvStylization(const std::string name, const std::shared_ptr<COcvStylizationParam>& pParam) : C2dImageTask(name)
        {
            m_pParam = std::make_shared<COcvStylizationParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvStylizationParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            auto image = pInput->getImage();

            if(image.channels() != 3)
                throw CException(CoreExCode::INVALID_PARAMETER, "Color image required", __func__, __FILE__, __LINE__);

            CMat imgDst;
            createGraphicsMask(image.getNbCols(), image.getNbRows(), pGraphicsInput);

            emit m_signalHandler->doProgress();

            try
            {
                cv::stylization(image, imgDst, pParam->m_sigmaS, pParam->m_sigmaR);
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

class COcvStylizationFactory : public CTaskFactory
{
    public:

        COcvStylizationFactory()
        {
            m_info.m_name = QObject::tr("Stylization").toStdString();
            m_info.m_description = QObject::tr("Stylization aims to produce digital imagery with a wide variety of effects not focused on photorealism. Edge-aware filters are ideal for stylization, as they can abstract regions of low contrast while preserving, or enhancing, high-contrast features.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Computational photography/Non-photorealistic rendering").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Stylization";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/df/dac/group__photo__render.html#gacb0f7324017df153d7b5d095aed53206";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pStylizationParam = std::dynamic_pointer_cast<COcvStylizationParam>(pParam);
            if(pStylizationParam != nullptr)
                return std::make_shared<COcvStylization>(m_info.m_name, pStylizationParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pStylizationParam = std::make_shared<COcvStylizationParam>();
            assert(pStylizationParam != nullptr);
            return std::make_shared<COcvStylization>(m_info.m_name, pStylizationParam);
        }
};

#endif // COCVSTYLIZATION_HPP
