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

#ifndef COCVSOBEL_HPP
#define COCVSOBEL_HPP

#include "Core/C2dImageTask.h"
#include "IO/CImageIO.h"

//--------------------------------//
//----- COcvSobelParam -----//
//--------------------------------//
class COcvSobelParam: public CWorkflowTaskParam
{
    public:

        COcvSobelParam() : CWorkflowTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_ddepth = std::stoi(paramMap.at("depth"));
            m_dx = std::stoi(paramMap.at("dx"));
            m_dy = std::stoi(paramMap.at("dy"));
            m_ksize = std::stoi(paramMap.at("ksize"));
            m_scale = std::stoi(paramMap.at("scale"));
            m_delta = std::stoi(paramMap.at("delta"));
            m_border = std::stoi(paramMap.at("border"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("depth", std::to_string(m_ddepth)));
            map.insert(std::make_pair("dx", std::to_string(m_dx)));
            map.insert(std::make_pair("dy", std::to_string(m_dy)));
            map.insert(std::make_pair("ksize", std::to_string(m_ksize)));
            map.insert(std::make_pair("scale", std::to_string(m_scale)));
            map.insert(std::make_pair("delta", std::to_string(m_delta)));
            map.insert(std::make_pair("border", std::to_string(m_border)));
            return map;
        }

    public:

        int     m_ddepth    = CV_32F;
        int     m_dx        = 1; // x-derivative order
        int     m_dy        = 0; // y-derivative order
        int     m_ksize     = 3; // >= 1
        double  m_scale     = 1;
        double  m_delta     = 0;
        int     m_border    = cv::BORDER_DEFAULT;


};

//---------------------------//
//----- COcvSobel -----//
//---------------------------//
class COcvSobel : public C2dImageTask
{
    public:

        COcvSobel() : C2dImageTask()
        {
        }
        COcvSobel(const std::string name, const std::shared_ptr<COcvSobelParam>& pParam) : C2dImageTask(name)
        {
            m_pParam = std::make_shared<COcvSobelParam>(*pParam);
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        void run() override
        {
            beginTaskRun();

            if(getInputCount() < 2)
                throw CException(CoreExCode::INVALID_PARAMETER, "Not enough inputs", __func__, __FILE__, __LINE__);

            auto pInput = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsInput>(getInput(1));
            auto pParam = std::dynamic_pointer_cast<COcvSobelParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty images", __func__, __FILE__, __LINE__);

            emit m_signalHandler->doProgress();

            CMat imgDst;
            CMat imgSrc = pInput->getImage();
            createGraphicsMask(imgSrc.getNbCols(), imgSrc.getNbRows(), pGraphicsInput);
            try
            {
                cv::Sobel(imgSrc, imgDst, pParam->m_ddepth, pParam->m_dx, pParam->m_dy, pParam->m_ksize, pParam->m_scale, pParam->m_delta, pParam->m_border);
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

class COcvSobelFactory : public CTaskFactory
{
    public:

        COcvSobelFactory()
        {
            m_info.m_name = QObject::tr("Sobel").toStdString();
            m_info.m_description = QObject::tr("Calculates the first, second, third, or mixed image derivatives using an extended Sobel operator.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Image processing/Image filtering").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Derivatives,Gradient,First,Second,Third,Order";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d4/d86/group__imgproc__filter.html#gacea54f142e81b6758cb6f375ce782c8d";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvSobelParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvSobel>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvSobelParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvSobel>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVSOBEL_HPP
