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

#ifndef COCVILLUMINATIONCHANGE_HPP
#define COCVILLUMINATIONCHANGE_HPP

#include "Core/C2dImageTask.h"
#include "IO/CImageIO.h"

//----------------------------//
//----- COcvIlluminationChangeParam -----//
//----------------------------//
class COcvIlluminationChangeParam: public CWorkflowTaskParam
{
    public:

        COcvIlluminationChangeParam() : CWorkflowTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_alpha = std::stof(paramMap.at("alpha"));
            m_beta = std::stof(paramMap.at("beta"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("alpha", std::to_string(m_alpha)));
            map.insert(std::make_pair("beta", std::to_string(m_beta)));
            return map;
        }

    public:
        float m_alpha = 0.2f;
        float m_beta = 0.4f;
};

//-----------------------//
//----- COcvIlluminationChange -----//
//-----------------------//
class COcvIlluminationChange : public C2dImageTask
{
    public:

        COcvIlluminationChange() : C2dImageTask()
        {
        }
        COcvIlluminationChange(const std::string name, const std::shared_ptr<COcvIlluminationChangeParam>& pParam) : C2dImageTask(name)
        {
            m_pParam = std::make_shared<COcvIlluminationChangeParam>(*pParam);
        }

        size_t     getProgressSteps() override
        {
            return 3;
        }

        void    run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            auto pGraphics = std::dynamic_pointer_cast<CGraphicsInput>(getInput(1));

            auto pParam = std::dynamic_pointer_cast<COcvIlluminationChangeParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            if(pGraphics->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Graphics are required", __func__, __FILE__, __LINE__);

            CMat imgDst;
            CMat imgSrc = pInput->getImage();

            createGraphicsMask(imgSrc.getNbCols(), imgSrc.getNbRows(), pGraphics);

            emit m_signalHandler->doProgress();

            if(imgSrc.channels() != 3)
                throw CException(CoreExCode::INVALID_PARAMETER, "Color image required", __func__, __FILE__, __LINE__);

            try
            {
                auto mask = getGraphicsMask(0);
                cv::illuminationChange(imgSrc, mask, imgDst, pParam->m_alpha, pParam->m_beta);
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

class COcvIlluminationChangeFactory : public CTaskFactory
{
    public:

        COcvIlluminationChangeFactory()
        {
            m_info.m_name = QObject::tr("IlluminationChange").toStdString();
            m_info.m_description = QObject::tr("Applying an appropriate non-linear transformation to the gradient field inside the selection and then integrating back with a Poisson solver, modifies locally the apparent illumination of an image.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Computational photography/Seamless cloning").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Brightness,Correction,Specular,Light,Color,Mixing,Seamless";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/df/da0/group__photo__clone.html#gac5025767cf2febd8029d474278e886c7";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pIlluminationChangeParam = std::dynamic_pointer_cast<COcvIlluminationChangeParam>(pParam);
            if(pIlluminationChangeParam != nullptr)
                return std::make_shared<COcvIlluminationChange>(m_info.m_name, pIlluminationChangeParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pIlluminationChangeParam = std::make_shared<COcvIlluminationChangeParam>();
            assert(pIlluminationChangeParam != nullptr);
            return std::make_shared<COcvIlluminationChange>(m_info.m_name, pIlluminationChangeParam);
        }
};

#endif // COCVILLUMINATIONCHANGE_HPP
