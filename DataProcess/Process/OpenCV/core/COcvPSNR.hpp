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

#ifndef COCVPSNR_HPP
#define COCVPSNR_HPP

#include "Core/CImageProcess2d.h"
#include "Main/CoreTools.hpp"

//-------------------------//
//----- COcvPSNRParam -----//
//-------------------------//
class COcvPSNRParam: public CProtocolTaskParam
{
    public:

        COcvPSNRParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            Q_UNUSED(paramMap);
        }

        UMapString  getParamMap() const override
        {
            return UMapString();
        }
};

//--------------------//
//----- COcvPSNR -----//
//--------------------//
class COcvPSNR : public CImageProcess2d
{
    public:

        COcvPSNR() : CImageProcess2d()
        {
            removeInput(1);
            addInput(std::make_shared<CImageProcessIO>());
            removeOutput(0);
            addOutput(std::make_shared<CFeatureProcessIO<double>>());
        }
        COcvPSNR(const std::string name, const std::shared_ptr<COcvPSNRParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<COcvPSNRParam>(*pParam);
            removeInput(1);
            addInput(std::make_shared<CImageProcessIO>());
            removeOutput(0);
            addOutput(std::make_shared<CFeatureProcessIO<double>>());
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

            auto pInput1 = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pInput2 = std::dynamic_pointer_cast<CImageProcessIO>(getInput(1));

            if(pInput1 == nullptr || pInput2 == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput1->isDataAvailable() == false || pInput2->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty images", __func__, __FILE__, __LINE__);

            double psnr = 0;
            CMat imgSrc1 = pInput1->getImage();
            CMat imgSrc2 = pInput2->getImage();
            double maxValue = Utils::Image::getMaxValue(imgSrc1);
            emit m_signalHandler->doProgress();

            try
            {
                psnr = cv::PSNR(imgSrc1, imgSrc2, maxValue);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            endTaskRun();
            emit m_signalHandler->doProgress();

            auto pOutput = std::dynamic_pointer_cast<CFeatureProcessIO<double>>(getOutput(0));
            if(pOutput)
            {
                std::vector<double> values;
                values.push_back(psnr);
                pOutput->addValueList(values, "PSNR");
            }
            emit m_signalHandler->doProgress();
        }
};

class COcvPSNRFactory : public CProcessFactory
{
    public:

        COcvPSNRFactory()
        {
            m_info.m_name = QObject::tr("PSNR").toStdString();
            m_info.m_description = QObject::tr("The function calculates the Peak Signal-to-Noise Ratio (PSNR) image quality metric in decibels (dB), between two input images. The arrays must have the same type.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Core functionality/Operations on arrays").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "metric,peak,signal,noise,ratio,quality";
            m_info.m_docLink = "https://docs.opencv.org/4.0.1/d2/de8/group__core__array.html#ga3119e3ea73010a6f810bb05aa36ac8d6";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvPSNRParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvPSNR>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvPSNRParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvPSNR>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVPSNR_HPP
