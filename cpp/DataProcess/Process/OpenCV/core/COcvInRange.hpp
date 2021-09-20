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

#ifndef COCVINRANGE_HPP
#define COCVINRANGE_HPP

#include "Core/C2dImageTask.h"
#include "IO/CImageIO.h"

//--------------------------------//
//----- COcvInRangeParam -----//
//--------------------------------//
class COcvInRangeParam: public CWorkflowTaskParam
{
    public:

        COcvInRangeParam() : CWorkflowTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_bLowerScalar = std::stoi(paramMap.at("isLowerScalar"));
            m_lower[0] = std::stod(paramMap.at("lower0"));
            m_lower[1] = std::stod(paramMap.at("lower1"));
            m_lower[2] = std::stod(paramMap.at("lower2"));
            m_lower[3] = std::stod(paramMap.at("lower3"));

            m_bUpperScalar = std::stoi(paramMap.at("isUpperScalar"));
            m_upper[0] = std::stod(paramMap.at("upper0"));
            m_upper[1] = std::stod(paramMap.at("upper1"));
            m_upper[2] = std::stod(paramMap.at("upper2"));
            m_upper[3] = std::stod(paramMap.at("upper3"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("isLowerScalar", std::to_string(m_bLowerScalar)));
            map.insert(std::make_pair("lower0", std::to_string(m_lower[0])));
            map.insert(std::make_pair("lower1", std::to_string(m_lower[1])));
            map.insert(std::make_pair("lower2", std::to_string(m_lower[2])));
            map.insert(std::make_pair("lower3", std::to_string(m_lower[3])));

            map.insert(std::make_pair("isUpperScalar", std::to_string(m_bUpperScalar)));
            map.insert(std::make_pair("upper0", std::to_string(m_upper[0])));
            map.insert(std::make_pair("upper1", std::to_string(m_upper[1])));
            map.insert(std::make_pair("upper2", std::to_string(m_upper[2])));
            map.insert(std::make_pair("upper3", std::to_string(m_upper[3])));
            return map;
        }

    public:

        bool        m_bLowerScalar = true;
        cv::Scalar  m_lower = {0, 0, 0, 0};
        bool        m_bUpperScalar = true;
        cv::Scalar  m_upper = {1, 1, 1, 1};
};

//---------------------------//
//----- COcvInRange -----//
//---------------------------//
class COcvInRange : public C2dImageTask
{
    public:

        COcvInRange() : C2dImageTask()
        {
            insertInput(std::make_shared<CImageIO>(), 1);
            insertInput(std::make_shared<CImageIO>(), 2);
        }
        COcvInRange(const std::string name, const std::shared_ptr<COcvInRangeParam>& pParam) : C2dImageTask(name)
        {
            m_pParam = std::make_shared<COcvInRangeParam>(*pParam);
            insertInput(std::make_shared<CImageIO>(), 1);
            insertInput(std::make_shared<CImageIO>(), 2);
        }

        size_t getProgressSteps() override
        {
            return 3;
        }

        void run() override
        {
            assert(getInputCount() == 4);
            beginTaskRun();

            auto pInput = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            auto pParam = std::dynamic_pointer_cast<COcvInRangeParam>(m_pParam);

            if(pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            CMat imgDst;
            CMat imgSrc = pInput->getImage();
            emit m_signalHandler->doProgress();

            if(pParam->m_bLowerScalar == true)
            {
                if(pParam->m_bUpperScalar == true)
                {
                    //lower:scalar - upper:scalar
                    try
                    {
                        cv::inRange(imgSrc, pParam->m_lower, pParam->m_upper, imgDst);
                    }
                    catch(cv::Exception& e)
                    {
                        throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
                    }
                }
                else
                {
                    //lower:scalar - upper:image
                    auto pInputUpper = std::dynamic_pointer_cast<CImageIO>(getInput(2));
                    if(pInputUpper == nullptr)
                        throw CException(CoreExCode::NULL_POINTER, "Empty upper image", __func__, __FILE__, __LINE__);

                    if(pInputUpper->isDataAvailable() == false)
                        throw CException(CoreExCode::INVALID_PARAMETER, "Empty upper image", __func__, __FILE__, __LINE__);

                    try
                    {
                        cv::inRange(imgSrc, pParam->m_lower, pInputUpper->getImage(), imgDst);
                    }
                    catch(cv::Exception& e)
                    {
                        throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
                    }
                }
            }
            else if(pParam->m_bUpperScalar == true)
            {
                //lower:image - upper:scalar
                auto pInputLower = std::dynamic_pointer_cast<CImageIO>(getInput(1));
                if(pInputLower == nullptr)
                    throw CException(CoreExCode::NULL_POINTER, "Empty lower image", __func__, __FILE__, __LINE__);

                if(pInputLower->isDataAvailable() == false)
                    throw CException(CoreExCode::INVALID_PARAMETER, "Empty lower image", __func__, __FILE__, __LINE__);

                try
                {
                    cv::inRange(imgSrc, pInputLower->getImage(), pParam->m_upper, imgDst);
                }
                catch(cv::Exception& e)
                {
                    throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
                }
            }
            else
            {
                //Lower:image - upper:image
                auto pInputLower = std::dynamic_pointer_cast<CImageIO>(getInput(1));
                auto pInputUpper = std::dynamic_pointer_cast<CImageIO>(getInput(2));

                if(pInputLower == nullptr || pInputUpper == nullptr)
                    throw CException(CoreExCode::NULL_POINTER, "Empty bounds images", __func__, __FILE__, __LINE__);

                if(pInputLower->isDataAvailable() == false || pInputUpper->isDataAvailable() == false)
                    throw CException(CoreExCode::INVALID_PARAMETER, "Empty bounds image", __func__, __FILE__, __LINE__);

                try
                {
                    cv::inRange(imgSrc, pInputLower->getImage(), pInputUpper->getImage(), imgDst);
                }
                catch(cv::Exception& e)
                {
                    throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
                }
            }
            emit m_signalHandler->doProgress();

            auto pOutput = std::dynamic_pointer_cast<CImageIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(imgDst);

            endTaskRun();
            applyInputGraphicsMask(3, 0, 0, MaskMode::MASK_ONLY);
            emit m_signalHandler->doProgress();
        }
};

class COcvInRangeFactory : public CTaskFactory
{
    public:

        COcvInRangeFactory()
        {
            m_info.m_name = "ocv_in_range";
            m_info.m_description = QObject::tr("The function checks if array elements lie between the elements of two other arrays or scalar").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Core functionality/Operations on arrays").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "arithmetic,inrange,range,per-element";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d2/de8/group__core__array.html#ga48af0ab51e36436c5d04340e036ce981";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvInRangeParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvInRange>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvInRangeParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvInRange>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVINRANGE_HPP
