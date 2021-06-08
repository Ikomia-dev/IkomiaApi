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

#ifndef COCVCVTCOLOR_H
#define COCVCVTCOLOR_H

#include "Core/C2dImageTask.h"
#include "IO/CImageIO.h"

//-----------------------------//
//----- COcvCvtColorParam -----//
//-----------------------------//
class COcvCvtColorParam: public CWorkflowTaskParam
{
    public:

        COcvCvtColorParam() : CWorkflowTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_code = std::stoi(paramMap.at("code"));
            m_dstCn = std::stoi(paramMap.at("dstCn"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("code", std::to_string(m_code)));
            map.insert(std::make_pair("dstCn", std::to_string(m_dstCn)));
            return map;
        }

    public:

        int m_code = cv::COLOR_RGB2GRAY;
        int m_dstCn = 0;
};

//------------------------//
//----- COcvCvtColor -----//
//------------------------//
class COcvCvtColor : public C2dImageTask
{
    public:

        COcvCvtColor() : C2dImageTask()
        {
        }
        COcvCvtColor(const std::string name, const std::shared_ptr<COcvCvtColorParam>& pParam) : C2dImageTask(name)
        {
            m_pParam = std::make_shared<COcvCvtColorParam>(*pParam);
        }

        size_t     getProgressSteps() override
        {
            return 3;
        }

        void    run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            auto pParam = std::dynamic_pointer_cast<COcvCvtColorParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            emit m_signalHandler->doProgress();
            CMat imgDst;

            try
            {
                cv::cvtColor(pInput->getImage(), imgDst, pParam->m_code, pParam->m_dstCn);
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

class COcvCvtColorFactory : public CTaskFactory
{
    public:

        COcvCvtColorFactory()
        {
            m_info.m_name = QObject::tr("Color Conversion").toStdString();
            m_info.m_description = QObject::tr("This process converts an image from one color space to another.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Image processing/Miscellaneous image transformations").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Conversion,Convert,Color,rgb2gray,gray2rgb";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d7/d1b/group__imgproc__misc.html#ga397ae87e1288a81d2363b61574eb8cab";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pCvtColorParam = std::dynamic_pointer_cast<COcvCvtColorParam>(pParam);
            if(pCvtColorParam != nullptr)
                return std::make_shared<COcvCvtColor>(m_info.m_name, pCvtColorParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pCvtColorParam = std::make_shared<COcvCvtColorParam>();
            assert(pCvtColorParam != nullptr);
            return std::make_shared<COcvCvtColor>(m_info.m_name, pCvtColorParam);
        }
};

#endif // COCVCVTCOLOR_H
