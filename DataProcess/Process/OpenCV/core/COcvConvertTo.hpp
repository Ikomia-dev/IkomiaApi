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

#ifndef COCVCONVERTTO_HPP
#define COCVCONVERTTO_HPP
#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"

//----------------------------//
//----- COcvConvertToParam -----//
//----------------------------//
class COcvConvertToParam: public CProtocolTaskParam
{
    public:

        COcvConvertToParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_alpha = std::stod(paramMap.at("alpha"));
            m_beta = std::stod(paramMap.at("beta"));
            m_dtype = std::stoi(paramMap.at("dtype"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("alpha", std::to_string(m_alpha)));
            map.insert(std::make_pair("beta", std::to_string(m_beta)));
            map.insert(std::make_pair("dtype", std::to_string(m_dtype)));

            return map;
        }

    public:
        int     m_dtype = -1;
        double  m_alpha = 1;
        double  m_beta = 0;

};

//-----------------------//
//----- COcvConvertTo -----//
//-----------------------//
class COcvConvertTo : public CImageProcess2d
{
    public:

        COcvConvertTo() : CImageProcess2d(false)
        {
        }
        COcvConvertTo(const std::string name, const std::shared_ptr<COcvConvertToParam>& pParam) : CImageProcess2d(name, false)
        {
            m_pParam = std::make_shared<COcvConvertToParam>(*pParam);
        }

        size_t     getProgressSteps() override
        {
            return 3;
        }

        void    run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pParam = std::dynamic_pointer_cast<COcvConvertToParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgDst;
            CMat imgSrc = pInput->getImage();
            emit m_signalHandler->doProgress();

            try
            {
                imgSrc.convertTo(imgDst, pParam->m_dtype, pParam->m_alpha, pParam->m_beta);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            endTaskRun();
            emit m_signalHandler->doProgress();

            auto pOutput = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(imgDst);

            emit m_signalHandler->doProgress();
        }
};

class COcvConvertToFactory : public CProcessFactory
{
    public:

        COcvConvertToFactory()
        {
            m_info.m_name = QObject::tr("ConvertTo").toStdString();
            m_info.m_description = QObject::tr("This process converts image to another type.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Core functionality/Operations on arrays").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Conversion,Format,Type";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d3/d63/classcv_1_1Mat.html#adf88c60c5b4980e05bb556080916978b";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pConvertToParam = std::dynamic_pointer_cast<COcvConvertToParam>(pParam);
            if(pConvertToParam != nullptr)
                return std::make_shared<COcvConvertTo>(m_info.m_name, pConvertToParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pConvertToParam = std::make_shared<COcvConvertToParam>();
            assert(pConvertToParam != nullptr);
            return std::make_shared<COcvConvertTo>(m_info.m_name, pConvertToParam);
        }
};

#endif // COCVCONVERTTO_HPP
