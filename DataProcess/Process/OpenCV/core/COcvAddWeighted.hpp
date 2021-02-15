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

#ifndef COCVADDWEIGHTED_H
#define COCVADDWEIGHTED_H

#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"

//--------------------------------//
//----- COcvAddWeightedParam -----//
//--------------------------------//
class COcvAddWeightedParam: public CProtocolTaskParam
{
    public:

        COcvAddWeightedParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_alpha = std::stod(paramMap.at("alpha"));
            m_beta = std::stod(paramMap.at("beta"));
            m_gamma = std::stod(paramMap.at("gamma"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("alpha", std::to_string(m_alpha)));
            map.insert(std::make_pair("beta", std::to_string(m_beta)));
            map.insert(std::make_pair("gamma", std::to_string(m_gamma)));
            return map;
        }

    public:

        double  m_alpha = 0.5;
        double  m_beta = 0.5;
        double  m_gamma = 0.0;
};

//---------------------------//
//----- COcvAddWeighted -----//
//---------------------------//
class COcvAddWeighted : public CImageProcess2d
{
    public:

        COcvAddWeighted() : CImageProcess2d()
        {
            insertInput(std::make_shared<CImageProcessIO>(), 0);
        }
        COcvAddWeighted(const std::string name, const std::shared_ptr<COcvAddWeightedParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<COcvAddWeightedParam>(*pParam);
            insertInput(std::make_shared<CImageProcessIO>(), 0);
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
            auto pParam = std::dynamic_pointer_cast<COcvAddWeightedParam>(m_pParam);

            if(pInput1 == nullptr || pInput2 == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput1->isDataAvailable() == false || pInput2->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty images", __func__, __FILE__, __LINE__);

            emit m_signalHandler->doProgress();

            CMat imgDst;
            try
            {
                cv::addWeighted(pInput1->getImage(), pParam->m_alpha, pInput2->getImage(), pParam->m_beta, pParam->m_gamma, imgDst);
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

class COcvAddWeightedFactory : public CProcessFactory
{
    public:

        COcvAddWeightedFactory()
        {
            m_info.m_name = QObject::tr("Add Weighted").toStdString();
            m_info.m_description = QObject::tr("The function addWeighted calculates the weighted sum of two images with the given formula:<br> <center><i>dst = saturate( src1 * alpha + src2 * beta + gamma )</i></center>").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Core functionality/Operations on arrays").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Arithmetic,Add,Weighted,Blending";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d2/de8/group__core__array.html#gafafb2513349db3bcff51f54ee5592a19";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvAddWeightedParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvAddWeighted>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvAddWeightedParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvAddWeighted>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVADDWEIGHTED_H
