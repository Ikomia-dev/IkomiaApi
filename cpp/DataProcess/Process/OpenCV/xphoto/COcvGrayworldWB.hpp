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

#ifndef COCVGRAYWORLDWB_HPP
#define COCVGRAYWORLDWB_HPP

#include "Core/C2dImageTask.h"
#include "IO/CImageIO.h"
#include <opencv2/xphoto.hpp>

//------------------------------//
//----- COcvGrayworldWBParam -----//
//------------------------------//
class COcvGrayworldWBParam: public CWorkflowTaskParam
{
    public:

        COcvGrayworldWBParam() : CWorkflowTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_satThreshold = std::stof(paramMap.at("satThreshold"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("satThreshold", std::to_string(m_satThreshold)));

            return map;
        }

    public:
        float   m_satThreshold = 1.0f;
};

//-------------------------//
//----- COcvGrayworldWB -----//
//-------------------------//
class COcvGrayworldWB : public C2dImageTask
{
    public:

        COcvGrayworldWB() : C2dImageTask()
        {
        }
        COcvGrayworldWB(const std::string name, const std::shared_ptr<COcvGrayworldWBParam>& pParam) : C2dImageTask(name)
        {
            m_pParam = std::make_shared<COcvGrayworldWBParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvGrayworldWBParam>(m_pParam);

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
                auto pGWB = cv::xphoto::createGrayworldWB();
                pGWB->setSaturationThreshold(pParam->m_satThreshold);
                pGWB->balanceWhite(imgSrc, imgDst);
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

class COcvGrayworldWBFactory : public CTaskFactory
{
    public:

        COcvGrayworldWBFactory()
        {
            m_info.m_name = QObject::tr("GrayworldWB").toStdString();
            m_info.m_description = QObject::tr("This algorithm scales the values of pixels based on a gray-world assumption which states that the average of all channels should result in a gray image.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Additional photo processing algorithms").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "White,Balance,AutoWhiteBalance";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d7/d71/classcv_1_1xphoto_1_1GrayworldWB.html";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvGrayworldWBParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvGrayworldWB>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvGrayworldWBParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvGrayworldWB>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVGRAYWORLDWB_HPP
