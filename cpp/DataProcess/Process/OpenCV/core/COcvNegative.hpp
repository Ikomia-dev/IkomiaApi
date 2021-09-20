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

#ifndef COCVNEGATIVE_HPP
#define COCVNEGATIVE_HPP
#include "Core/C2dImageTask.h"
#include "IO/CImageIO.h"

//----------------------------//
//----- COcvNegativeParam -----//
//----------------------------//
class COcvNegativeParam: public CWorkflowTaskParam
{
    public:

        COcvNegativeParam() : CWorkflowTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            Q_UNUSED(paramMap);
        }

        UMapString  getParamMap() const override
        {
            return UMapString();
        }
};

//-----------------------//
//----- COcvNegative -----//
//-----------------------//
class COcvNegative : public C2dImageTask
{
    public:

        COcvNegative() : C2dImageTask()
        {
        }
        COcvNegative(const std::string name, const std::shared_ptr<COcvNegativeParam>& pParam) : C2dImageTask(name)
        {
            m_pParam = std::make_shared<COcvNegativeParam>(*pParam);
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        void    run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            auto pParam = std::dynamic_pointer_cast<COcvNegativeParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgDst;
            CMat imgSrc = pInput->getImage();
            emit m_signalHandler->doProgress();

            try
            {
                cv::bitwise_not(imgSrc, imgDst);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }
            emit m_signalHandler->doProgress();

            auto pOutput = std::dynamic_pointer_cast<CImageIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(imgDst);

            endTaskRun();
            applyInputGraphicsMask(1, 0, 0, MaskMode::MERGE_SOURCE);
            emit m_signalHandler->doProgress();
        }
};

class COcvNegativeFactory : public CTaskFactory
{
    public:

        COcvNegativeFactory()
        {
            m_info.m_name = "ocv_negative";
            m_info.m_description = QObject::tr("This process produces negative image.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Core functionality/Operations on arrays").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Inverse,Negative";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d2/de8/group__core__array.html#ga0002cf8b418479f4cb49a75442baee2f";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pNegativeParam = std::dynamic_pointer_cast<COcvNegativeParam>(pParam);
            if(pNegativeParam != nullptr)
                return std::make_shared<COcvNegative>(m_info.m_name, pNegativeParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pNegativeParam = std::make_shared<COcvNegativeParam>();
            assert(pNegativeParam != nullptr);
            return std::make_shared<COcvNegative>(m_info.m_name, pNegativeParam);
        }
};

#endif // COCVNEGATIVE_HPP
