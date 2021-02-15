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

#ifndef COCVCOMPARE_HPP
#define COCVCOMPARE_HPP
#include "Core/CImageProcess2d.h"

//----------------------------//
//----- COcvCompareParam -----//
//----------------------------//
class COcvCompareParam: public CProtocolTaskParam
{
    public:

        COcvCompareParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_operation = std::stoi(paramMap.at("operation"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("operation", std::to_string(m_operation)));
            return map;
        }

    public:

        int m_operation = cv::CMP_NE; //Not equal
};

//-----------------------//
//----- COcvCompare -----//
//-----------------------//
class COcvCompare : public CImageProcess2d
{
    public:

        COcvCompare() : CImageProcess2d()
        {
            insertInput(std::make_shared<CImageProcessIO>(), 0);
            getOutput(0)->setDataType(IODataType::IMAGE_BINARY);
        }
        COcvCompare(const std::string name, const std::shared_ptr<COcvCompareParam>& pParam) : CImageProcess2d(name)
        {
            insertInput(std::make_shared<CImageProcessIO>(), 0);
            getOutput(0)->setDataType(IODataType::IMAGE_BINARY);
            m_pParam = std::make_shared<COcvCompareParam>(*pParam);
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

            auto pInput1 = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pInput2 = std::dynamic_pointer_cast<CImageProcessIO>(getInput(1));
            auto pParam = std::dynamic_pointer_cast<COcvCompareParam>(m_pParam);

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
                cv::compare(imgSrc1, imgSrc2, imgDst, pParam->m_operation);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }
            emit m_signalHandler->doProgress();

            auto pOutput = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(imgDst);

            endTaskRun();
            applyInputGraphicsMask(2, 0, 0, MaskMode::MASK_ONLY);
            emit m_signalHandler->doProgress();
        }
};

class COcvCompareFactory : public CProcessFactory
{
    public:

        COcvCompareFactory()
        {
            m_info.m_name = QObject::tr("Compare").toStdString();
            m_info.m_description = QObject::tr("The function Performs the per-element comparison of two arrays.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Core functionality/Operations on arrays").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "compare,binary";
            m_info.m_docLink = "https://docs.opencv.org/4.0.1/d2/de8/group__core__array.html#ga303cfb72acf8cbb36d884650c09a3a97";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvCompareParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvCompare>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvCompareParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvCompare>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVCOMPARE_HPP
