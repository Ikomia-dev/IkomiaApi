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

#ifndef COCVANISOTROPICDIFFUSION_HPP
#define COCVANISOTROPICDIFFUSION_HPP

#include "Core/C2dImageTask.h"
#include "IO/CImageIO.h"
#include <opencv2/ximgproc.hpp>

//-----------------------------------------//
//----- COcvAnisotropicDiffusionParam -----//
//-----------------------------------------//
class COcvAnisotropicDiffusionParam: public CWorkflowTaskParam
{
    public:

        COcvAnisotropicDiffusionParam() : CWorkflowTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_alpha = std::stof(paramMap.at("alpha"));
            m_k = std::stof(paramMap.at("K"));
            m_nbIter = std::stoi(paramMap.at("nbIteration"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("alpha", std::to_string(m_alpha)));
            map.insert(std::make_pair("K", std::to_string(m_k)));
            map.insert(std::make_pair("nbIteration", std::to_string(m_nbIter)));
            return map;
        }

    public:

        float   m_alpha = 1.0;
        float   m_k = 1.0;
        int     m_nbIter = 1;
};

//------------------------------------//
//----- COcvAnisotropicDiffusion -----//
//------------------------------------//
class COcvAnisotropicDiffusion : public C2dImageTask
{
    public:

        COcvAnisotropicDiffusion() : C2dImageTask()
        {
        }
        COcvAnisotropicDiffusion(const std::string name, const std::shared_ptr<COcvAnisotropicDiffusionParam>& pParam) : C2dImageTask(name)
        {
            m_pParam = std::make_shared<COcvAnisotropicDiffusionParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvAnisotropicDiffusionParam>(m_pParam);

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
                cv::ximgproc::anisotropicDiffusion(imgSrc, imgDst, pParam->m_alpha, pParam->m_k, pParam->m_nbIter);
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

class COcvAnisotropicDiffusionFactory : public CTaskFactory
{
    public:

        COcvAnisotropicDiffusionFactory()
        {
            m_info.m_name = "ocv_anisotropic_diffusion";
            m_info.m_description = QObject::tr("The process applies Perona-Malik anisotropic diffusion to an image.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Extended Image Processing").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "anisotropic,diffusion,Perona-Malik";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/df/d2d/group__ximgproc.html#gaffedd976e0a8efb5938107acab185ec2";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvAnisotropicDiffusionParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvAnisotropicDiffusion>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvAnisotropicDiffusionParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvAnisotropicDiffusion>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVANISOTROPICDIFFUSION_HPP
