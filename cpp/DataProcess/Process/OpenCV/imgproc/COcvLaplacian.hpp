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

#ifndef COCVLAPLACIAN_HPP
#define COCVLAPLACIAN_HPP

#include "Core/C2dImageTask.h"
#include "IO/CImageIO.h"

//------------------------------//
//----- COcvLaplacianParam -----//
//------------------------------//
class COcvLaplacianParam: public CWorkflowTaskParam
{
    public:

        COcvLaplacianParam() : CWorkflowTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_ddepth = std::stoi(paramMap.at("ddepth"));
            m_ksize = std::stoi(paramMap.at("kSize"));
            m_scale = std::stod(paramMap.at("scale"));
            m_delta = std::stod(paramMap.at("delta"));
            m_borderType = std::stoi(paramMap.at("borderType"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("ddepth", std::to_string(m_ddepth)));
            map.insert(std::make_pair("kSize", std::to_string(m_ksize)));
            map.insert(std::make_pair("scale", std::to_string(m_scale)));
            map.insert(std::make_pair("delta", std::to_string(m_delta)));
            map.insert(std::make_pair("borderType", std::to_string(m_borderType)));
            return map;
        }

    public:

        int         m_ddepth = -1;
        int         m_ksize = 1;
        double      m_scale = 1;
        double      m_delta = 0;
        int         m_borderType = cv::BORDER_DEFAULT;
};

//-------------------------//
//----- COcvLaplacian -----//
//-------------------------//
class COcvLaplacian : public C2dImageTask
{
    public:

        COcvLaplacian() : C2dImageTask()
        {
        }
        COcvLaplacian(const std::string name, const std::shared_ptr<COcvLaplacianParam>& pParam) : C2dImageTask(name)
        {
            m_pParam = std::make_shared<COcvLaplacianParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvLaplacianParam>(m_pParam);

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
                cv::Laplacian(imgSrc, imgDst, pParam->m_ddepth, pParam->m_ksize, pParam->m_scale, pParam->m_delta, pParam->m_borderType);
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

class COcvLaplacianFactory : public CTaskFactory
{
    public:

        COcvLaplacianFactory()
        {
            m_info.m_name = QObject::tr("Laplacian").toStdString();
            m_info.m_description = QObject::tr("The function calculates the Laplacian of the source image by adding up the second x and y derivatives calculated using the Sobel operator.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Image processing/Image filtering").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Derivative,Laplace,Second,Order";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d4/d86/group__imgproc__filter.html#gad78703e4c8fe703d479c1860d76429e6";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pLaplacianParam = std::dynamic_pointer_cast<COcvLaplacianParam>(pParam);
            if(pLaplacianParam != nullptr)
                return std::make_shared<COcvLaplacian>(m_info.m_name, pLaplacianParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pLaplacianParam = std::make_shared<COcvLaplacianParam>();
            assert(pLaplacianParam != nullptr);
            return std::make_shared<COcvLaplacian>(m_info.m_name, pLaplacianParam);
        }
};

#endif // COCVLAPLACIAN_HPP
