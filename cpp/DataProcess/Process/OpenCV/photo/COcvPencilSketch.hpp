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

#ifndef COCVPENCILSKETCH_HPP
#define COCVPENCILSKETCH_HPP

#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"
#include <opencv2/photo.hpp>

//----------------------------------//
//----- COcvPencilSketchParam -----//
//----------------------------------//
class COcvPencilSketchParam : public CProtocolTaskParam
{
    public:

        COcvPencilSketchParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_sigmaS = std::stof(paramMap.at("sigmaS"));
            m_sigmaR = std::stof(paramMap.at("sigmaR"));
            m_shadeFactor = std::stof(paramMap.at("shadeFactor"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("sigmaS", std::to_string(m_sigmaS)));
            map.insert(std::make_pair("sigmaR", std::to_string(m_sigmaR)));
            map.insert(std::make_pair("shadeFactor", std::to_string(m_shadeFactor)));
            return map;
        }

    public:
        float m_sigmaS = 60.0f;
        float m_sigmaR = 0.07f;
        float m_shadeFactor = 0.02f;
};

//-----------------------------//
//----- COcvPencilSketch -----//
//-----------------------------//
class COcvPencilSketch : public CImageProcess2d
{
    public:

        COcvPencilSketch() : CImageProcess2d()
        {
            addOutput(std::make_shared<CImageProcessIO>());
        }
        COcvPencilSketch(const std::string name, const std::shared_ptr<COcvPencilSketchParam>& pParam) : CImageProcess2d(name)
        {
            addOutput(std::make_shared<CImageProcessIO>());
            m_pParam = std::make_shared<COcvPencilSketchParam>(*pParam);
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        void run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsProcessInput>(getInput(1));
            auto pParam = std::dynamic_pointer_cast<COcvPencilSketchParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            auto image = pInput->getImage();

            if(image.channels() != 3)
                throw CException(CoreExCode::INVALID_PARAMETER, "Color image required", __func__, __FILE__, __LINE__);

            CMat imgDst, imgDstGray;
            createGraphicsMask(image.getNbCols(), image.getNbRows(), pGraphicsInput);

            emit m_signalHandler->doProgress();

            try
            {
                cv::pencilSketch(image, imgDstGray, imgDst, pParam->m_sigmaS, pParam->m_sigmaR, pParam->m_shadeFactor);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            endTaskRun();
            applyGraphicsMask(image, imgDst, 0);
            emit m_signalHandler->doProgress();

            auto pOutput1 = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(0));
            if(pOutput1)
                pOutput1->setImage(imgDstGray);

            auto pOutput2 = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(1));
            if(pOutput2)
                pOutput2->setImage(imgDst);

            emit m_signalHandler->doProgress();
        }
};

class COcvPencilSketchFactory : public CProcessFactory
{
    public:

        COcvPencilSketchFactory()
        {
            m_info.m_name = QObject::tr("Pencil sketch").toStdString();
            m_info.m_description = QObject::tr("Pencil-like non-photorealistic line drawing.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Computational photography/Non-photorealistic rendering").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Pencil,Sketch";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/df/dac/group__photo__render.html#gae5930dd822c713b36f8529b21ddebd0c";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pPencilSketchParam = std::dynamic_pointer_cast<COcvPencilSketchParam>(pParam);
            if(pPencilSketchParam != nullptr)
                return std::make_shared<COcvPencilSketch>(m_info.m_name, pPencilSketchParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pPencilSketchParam = std::make_shared<COcvPencilSketchParam>();
            assert(pPencilSketchParam != nullptr);
            return std::make_shared<COcvPencilSketch>(m_info.m_name, pPencilSketchParam);
        }
};

#endif // COCVPENCILSKETCH_HPP
