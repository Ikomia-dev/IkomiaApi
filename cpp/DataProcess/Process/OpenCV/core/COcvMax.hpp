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

#ifndef COCVMAX_HPP
#define COCVMAX_HPP


#include "Core/C2dImageTask.h"

//------------------------//
//----- COcvMaxParam -----//
//------------------------//
class COcvMaxParam: public CWorkflowTaskParam
{
    public:

        COcvMaxParam() : CWorkflowTaskParam(){}

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
//----- COcvMax -----//
//-------------------//
class COcvMax : public C2dImageTask
{
    public:

        COcvMax() : C2dImageTask()
        {
            insertInput(std::make_shared<CImageIO>(), 1);
        }
        COcvMax(const std::string name, const std::shared_ptr<COcvMaxParam>& pParam) : C2dImageTask(name)
        {
            m_pParam = std::make_shared<COcvMaxParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvMaxParam>(m_pParam);

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
                    cv::max(imgSrc1, pParam->m_scalar, imgDst);
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
                    cv::max(imgSrc1, imgSrc2, imgDst);
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

class COcvMaxFactory : public CTaskFactory
{
    public:

        COcvMaxFactory()
        {
            m_info.m_name = QObject::tr("Max").toStdString();
            m_info.m_description = QObject::tr("The function calculates the per-element maximum of two arrays or array and a scalar.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Core functionality/Operations on arrays").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "arithmetic,max,per-element";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d2/de8/group__core__array.html#gacc40fa15eac0fb83f8ca70b7cc0b588d";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvMaxParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvMax>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvMaxParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvMax>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVMAX_HPP
