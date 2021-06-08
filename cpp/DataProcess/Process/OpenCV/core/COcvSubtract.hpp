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

#ifndef COCVSUBTRACT_HPP
#define COCVSUBTRACT_HPP

#include "Core/C2dImageTask.h"
#include "IO/CImageIO.h"

//--------------------------------//
//----- COcvSubtractParam -----//
//--------------------------------//
class COcvSubtractParam: public CWorkflowTaskParam
{
    public:

        COcvSubtractParam() : CWorkflowTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_dtype = std::stoi(paramMap.at("dtype"));
            m_value1 = std::stod(paramMap.at("value1"));
            m_value2 = std::stod(paramMap.at("value2"));
            m_bUseValue1 = std::stod(paramMap.at("bUseValue1"));
            m_bUseValue2 = std::stod(paramMap.at("bUseValue2"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("dtype", std::to_string(m_dtype)));
            map.insert(std::make_pair("value1", std::to_string(m_value1)));
            map.insert(std::make_pair("value2", std::to_string(m_value2)));
            map.insert(std::make_pair("bUseValue1", std::to_string(m_bUseValue1)));
            map.insert(std::make_pair("bUseValue2", std::to_string(m_bUseValue2)));
            return map;
        }

    public:

        int     m_dtype = -1;
        double  m_value1 = 0;
        double  m_value2 = 0;
        bool    m_bUseValue1 = false;
        bool    m_bUseValue2 = false;
};

//---------------------------//
//----- COcvSubtract -----//
//---------------------------//
class COcvSubtract : public C2dImageTask
{
    public:

        COcvSubtract() : C2dImageTask()
        {
            insertInput(std::make_shared<CImageIO>(), 1);
        }
        COcvSubtract(const std::string name, const std::shared_ptr<COcvSubtractParam>& pParam) : C2dImageTask(name)
        {
            m_pParam = std::make_shared<COcvSubtractParam>(*pParam);
            insertInput(std::make_shared<CImageIO>(), 1);
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

            auto pInput1 = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            auto pInput2 = std::dynamic_pointer_cast<CImageIO>(getInput(1));
            auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsInput>(getInput(2));
            auto pParam = std::dynamic_pointer_cast<COcvSubtractParam>(m_pParam);

            if(pInput1 == nullptr || pInput2 == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput1->isDataAvailable() == false || pInput2->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty images", __func__, __FILE__, __LINE__);

            emit m_signalHandler->doProgress();

            CMat imgSrc1, imgSrc2, imgDst;
            CMat im1 = pInput1->getImage();
            CMat im2 = pInput2->getImage();

            if(pParam->m_bUseValue1)
            {
                cv::Scalar color;
                if(im2.channels() == 1)
                    color = cv::Scalar(pParam->m_value1);
                else if(im2.channels() == 3)
                    color = cv::Scalar(pParam->m_value1, pParam->m_value1, pParam->m_value1);
                else
                    throw CException(CoreExCode::INVALID_PARAMETER, "Invalid channel number", __func__, __FILE__, __LINE__);

                imgSrc1 = CMat(im1.size(), im1.type(), color);
            }
            else
                imgSrc1 = im1;

            if(pParam->m_bUseValue2)
            {
                cv::Scalar color;
                if(im1.channels() == 1)
                    color = cv::Scalar(pParam->m_value2);
                else if(im1.channels() == 3)
                    color = cv::Scalar(pParam->m_value2, pParam->m_value2, pParam->m_value2);
                else
                    throw CException(CoreExCode::INVALID_PARAMETER, "Invalid channel number", __func__, __FILE__, __LINE__);

                imgSrc2 = CMat(im2.size(), im2.type(), color);
            }
            else
                imgSrc2 = im2;

            createGraphicsMask(imgSrc1.getNbCols(), imgSrc1.getNbRows(), pGraphicsInput);
            try
            {
                cv::subtract(imgSrc1, imgSrc2, imgDst, getGraphicsMask(0), pParam->m_dtype);
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
            emit m_signalHandler->doProgress();
        }
};

class COcvSubtractFactory : public CTaskFactory
{
    public:

        COcvSubtractFactory()
        {
            m_info.m_name = QObject::tr("Subtract").toStdString();
            m_info.m_description = QObject::tr("The function calculates the per-element difference of two arrays with the given formula:<br> <center><i>dst = saturate( src1 - src2 )</i></center>").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Core functionality/Operations on arrays").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Arithmetic,Subtract,Per-element";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d2/de8/group__core__array.html#gaa0f00d98b4b5edeaeb7b8333b2de353b";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvSubtractParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvSubtract>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvSubtractParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvSubtract>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVSUBTRACT_HPP
