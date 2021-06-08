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

#ifndef COCVCOUNTNONZERO_HPP
#define COCVCOUNTNONZERO_HPP

#include "Core/C2dImageTask.h"

//---------------------------------//
//----- COcvCountNonZeroParam -----//
//---------------------------------//
class COcvCountNonZeroParam: public CWorkflowTaskParam
{
    public:

        COcvCountNonZeroParam() : CWorkflowTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            Q_UNUSED(paramMap);
        }

        UMapString  getParamMap() const override
        {
            return UMapString();
        }
};

//----------------------------//
//----- COcvCountNonZero -----//
//----------------------------//
class COcvCountNonZero : public C2dImageTask
{
    public:

        COcvCountNonZero() : C2dImageTask(false)
        {
            removeOutput(0);
            addOutput(std::make_shared<CFeatureIO<int>>());
        }
        COcvCountNonZero(const std::string name, const std::shared_ptr<COcvCountNonZeroParam>& pParam) : C2dImageTask(name, false)
        {
            m_pParam = std::make_shared<COcvCountNonZeroParam>(*pParam);
            removeOutput(0);
            addOutput(std::make_shared<CFeatureIO<int>>());
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        void run() override
        {
            beginTaskRun();

            if(getInputCount() < 1)
                throw CException(CoreExCode::INVALID_PARAMETER, "Not enough inputs", __func__, __FILE__, __LINE__);

            auto pInput = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            if(pInput == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            int nonZeroCount = 0;
            emit m_signalHandler->doProgress();

            try
            {
                nonZeroCount = cv::countNonZero(pInput->getImage());
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            endTaskRun();
            emit m_signalHandler->doProgress();

            auto pOutput = std::dynamic_pointer_cast<CFeatureIO<int>>(getOutput(0));
            if(pOutput)
            {
                std::vector<int> values;
                values.push_back(nonZeroCount);
                pOutput->addValueList(values, "Number of non-zero elements");
            }
            emit m_signalHandler->doProgress();
        }
};

class COcvCountNonZeroFactory : public CTaskFactory
{
    public:

        COcvCountNonZeroFactory()
        {
            m_info.m_name = QObject::tr("CountNonZero").toStdString();
            m_info.m_description = QObject::tr("The function returns the number of non-zero elements in source image.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Core functionality/Operations on arrays").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "count,zero";
            m_info.m_docLink = "https://docs.opencv.org/4.0.1/d2/de8/group__core__array.html#gaa4b89393263bb4d604e0fe5986723914";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvCountNonZeroParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvCountNonZero>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvCountNonZeroParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvCountNonZero>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVCOUNTNONZERO_HPP
