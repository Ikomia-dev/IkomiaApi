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

#ifndef COCVMIN_HPP
#define COCVMIN_HPP


#include "Core/C2dImageTask.h"

//------------------------//
//----- COcvMinParam -----//
//------------------------//
class COcvMinParam: public CWorkflowTaskParam
{
    public:

        COcvMinParam() : CWorkflowTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_bScalar = std::stoi(paramMap.at("isScalar"));
            m_scalar[0] = std::stod(paramMap.at("scalar0"));
            m_scalar[1] = std::stod(paramMap.at("scalar1"));
            m_scalar[2] = std::stod(paramMap.at("scalar2"));
            m_scalar[3] = std::stod(paramMap.at("scalar3"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("isScalar", std::to_string(m_bScalar)));
            map.insert(std::make_pair("scalar0", std::to_string(m_scalar[0])));
            map.insert(std::make_pair("scalar1", std::to_string(m_scalar[1])));
            map.insert(std::make_pair("scalar2", std::to_string(m_scalar[2])));
            map.insert(std::make_pair("scalar3", std::to_string(m_scalar[3])));
            return map;
        }

    public:

        bool        m_bScalar = false;
        cv::Scalar  m_scalar = {-1, -1, -1, -1};
};

//-------------------//
//----- COcvMin -----//
//-------------------//
class COcvMin : public C2dImageTask
{
    public:

        COcvMin() : C2dImageTask()
        {
            insertInput(std::make_shared<CImageIO>(), 1);
        }
        COcvMin(const std::string name, const std::shared_ptr<COcvMinParam>& pParam) : C2dImageTask(name)
        {
            m_pParam = std::make_shared<COcvMinParam>(*pParam);
            insertInput(std::make_shared<CImageIO>(), 1);
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        void run() override
        {
            beginTaskRun();

            if(getInputCount() < 3)
                throw CException(CoreExCode::INVALID_PARAMETER, "Not enough inputs", __func__, __FILE__, __LINE__);

            auto pInput1 = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            auto pInput2 = std::dynamic_pointer_cast<CImageIO>(getInput(1));
            auto pParam = std::dynamic_pointer_cast<COcvMinParam>(m_pParam);

            if(pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            CMat imgDst, imgSrc1;
            if(pParam->m_bScalar == true)
            {
                if(pInput1 == nullptr)
                    throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

                if(pInput1->isDataAvailable() == false)
                    throw CException(CoreExCode::INVALID_PARAMETER, "Empty images", __func__, __FILE__, __LINE__);

                imgSrc1 = pInput1->getImage();
                emit m_signalHandler->doProgress();

                try
                {
                    cv::min(imgSrc1, pParam->m_scalar, imgDst);
                }
                catch(cv::Exception& e)
                {
                    throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
                }
            }
            else
            {
                if(pInput1 == nullptr || pInput2 == nullptr)
                    throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

                if(pInput1->isDataAvailable() == false || pInput2->isDataAvailable() == false)
                    throw CException(CoreExCode::INVALID_PARAMETER, "Empty images", __func__, __FILE__, __LINE__);

                imgSrc1 = pInput1->getImage();
                cv::Mat imgSrc2 = pInput2->getImage();
                emit m_signalHandler->doProgress();

                try
                {
                    cv::min(imgSrc1, imgSrc2, imgDst);
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

            applyInputGraphicsMask(2, 0, 0, MaskMode::MASK_ONLY);
            endTaskRun();
            emit m_signalHandler->doProgress();
        }
};

class COcvMinFactory : public CTaskFactory
{
    public:

        COcvMinFactory()
        {
            m_info.m_name = QObject::tr("Min").toStdString();
            m_info.m_description = QObject::tr("The function calculates the per-element minimum of two arrays or array and a scalar.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Core functionality/Operations on arrays").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "arithmetic,min,per-element";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d2/de8/group__core__array.html#ga9af368f182ee76d0463d0d8d5330b764";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvMinParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvMin>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvMinParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvMin>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVMIN_HPP
