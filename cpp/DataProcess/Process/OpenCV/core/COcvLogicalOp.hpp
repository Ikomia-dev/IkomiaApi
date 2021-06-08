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

#ifndef COCVAND_HPP
#define COCVAND_HPP


#include "Core/C2dImageTask.h"
#include "IO/CImageIO.h"

//------------------------------//
//----- COcvLogicalOpParam -----//
//------------------------------//
class COcvLogicalOpParam : public CWorkflowTaskParam
{
    public:

        enum BitwiseOp {AND, OR, NOT, XOR};

        COcvLogicalOpParam() : CWorkflowTaskParam()
        {
        }

        void        setParamMap(const UMapString& paramMap) override
        {
            m_op = std::stoi(paramMap.at("operation"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("operation", std::to_string(m_op)));
            return map;
        }

    public:

        int m_op = BitwiseOp::AND;
};

//-------------------------//
//----- COcvLogicalOp -----//
//-------------------------//
class COcvLogicalOp : public C2dImageTask
{
    public:

        COcvLogicalOp() : C2dImageTask()
        {
            insertInput(std::make_shared<CImageIO>(), 1);
        }
        COcvLogicalOp(const std::string name, const std::shared_ptr<COcvLogicalOpParam>& pParam) : C2dImageTask(name)
        {
            insertInput(std::make_shared<CImageIO>(), 1);
            m_pParam = std::make_shared<COcvLogicalOpParam>(*pParam);
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        void    run() override
        {
            beginTaskRun();

            if(getInputCount() < 2)
                throw CException(CoreExCode::INVALID_PARAMETER, "Not enough inputs", __func__, __FILE__, __LINE__);

            auto pInput1 = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            auto pInput2 = std::dynamic_pointer_cast<CImageIO>(getInput(1));
            auto pParam = std::dynamic_pointer_cast<COcvLogicalOpParam>(m_pParam);

            if(pInput1 == nullptr || pInput2 == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput1->isDataAvailable() == false || pInput2->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty images", __func__, __FILE__, __LINE__);

            CMat imgDst;
            CMat imgSrc1 = pInput1->getImage();
            CMat imgSrc2 = pInput2->getImage();
            emit m_signalHandler->doProgress();

            try
            {
                imgDst = imgSrc1.clone();
                switch(pParam->m_op)
                {
                    case COcvLogicalOpParam::AND: cv::bitwise_and(imgSrc1, imgSrc2, imgDst, getGraphicsMask(0)); break;
                    case COcvLogicalOpParam::OR: cv::bitwise_or(imgSrc1, imgSrc2, imgDst, getGraphicsMask(0)); break;
                    case COcvLogicalOpParam::NOT: cv::bitwise_not(imgSrc1, imgDst, getGraphicsMask(0)); break;
                    case COcvLogicalOpParam::XOR: cv::bitwise_xor(imgSrc1, imgSrc2, imgDst, getGraphicsMask(0)); break;
                }
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }
            emit m_signalHandler->doProgress();

            auto pOutput = std::dynamic_pointer_cast<CImageIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(imgDst);

            applyInputGraphicsMask(2, 0, 0, MaskMode::MASK_ONLY);
            endTaskRun();
            emit m_signalHandler->doProgress();
        }
};

class COcvLogicalOpFactory : public CTaskFactory
{
    public:

        COcvLogicalOpFactory()
        {
            m_info.m_name = QObject::tr("Logical operations").toStdString();
            m_info.m_description = QObject::tr("This process computes bitwise logical operations of two images.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Core functionality/Operations on arrays").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "and, or, xor, not, logical, bitwise";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d2/de8/group__core__array.html#ga60b4d04b251ba5eb1392c34425497e14";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pLogicalOpParam = std::dynamic_pointer_cast<COcvLogicalOpParam>(pParam);
            if(pLogicalOpParam != nullptr)
                return std::make_shared<COcvLogicalOp>(m_info.m_name, pLogicalOpParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pLogicalOpParam = std::make_shared<COcvLogicalOpParam>();
            assert(pLogicalOpParam != nullptr);
            return std::make_shared<COcvLogicalOp>(m_info.m_name, pLogicalOpParam);
        }
};

#endif // COCVAND_HPP
