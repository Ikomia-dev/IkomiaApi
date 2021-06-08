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

#ifndef COCVROTATE_HPP
#define COCVROTATE_HPP

#include "Core/C2dImageTask.h"
#include "IO/CImageIO.h"

//---------------------------//
//----- COcvRotateParam -----//
//---------------------------//
class COcvRotateParam: public CWorkflowTaskParam
{
    public:

        COcvRotateParam() : CWorkflowTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_rotateCode = std::stoi(paramMap.at("rotateCode"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("rotateCode", std::to_string(m_rotateCode)));
            return map;
        }

    public:

        int m_rotateCode = cv::ROTATE_90_CLOCKWISE;
};

//----------------------//
//----- COcvRotate -----//
//----------------------//
class COcvRotate : public C2dImageTask
{
    public:

        COcvRotate() : C2dImageTask(false)
        {
        }
        COcvRotate(const std::string name, const std::shared_ptr<COcvRotateParam>& pParam) : C2dImageTask(name, false)
        {
            m_pParam = std::make_shared<COcvRotateParam>(*pParam);
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        void run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            auto pParam = std::dynamic_pointer_cast<COcvRotateParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgDst;
            emit m_signalHandler->doProgress();

            try
            {
                cv::rotate(pInput->getImage(), imgDst, pParam->m_rotateCode);
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

class COcvRotateFactory : public CTaskFactory
{
    public:

        COcvRotateFactory()
        {
            m_info.m_name = QObject::tr("Rotate").toStdString();
            m_info.m_description = QObject::tr("This process applies rotation of 90°, 180° or 270°.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Core functionality/Operations on arrays").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "rotation";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d2/de8/group__core__array.html#ga4ad01c0978b0ce64baa246811deeac24";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pRotateParam = std::dynamic_pointer_cast<COcvRotateParam>(pParam);
            if(pRotateParam != nullptr)
                return std::make_shared<COcvRotate>(m_info.m_name, pRotateParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pRotateParam = std::make_shared<COcvRotateParam>();
            assert(pRotateParam != nullptr);
            return std::make_shared<COcvRotate>(m_info.m_name, pRotateParam);
        }
};

#endif // COCVROTATE_HPP
