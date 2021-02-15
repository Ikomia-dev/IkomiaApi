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

#ifndef COCVCOLORCHANGE_HPP
#define COCVCOLORCHANGE_HPP

#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"

//----------------------------//
//----- COcvColorchangeParam -----//
//----------------------------//
class COcvColorchangeParam: public CProtocolTaskParam
{
    public:

        COcvColorchangeParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_red_mul = std::stof(paramMap.at("red_mul"));
            m_green_mul = std::stof(paramMap.at("green_mul"));
            m_blue_mul = std::stof(paramMap.at("blue_mul"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("red_mul", std::to_string(m_red_mul)));
            map.insert(std::make_pair("green_mul", std::to_string(m_green_mul)));
            map.insert(std::make_pair("blue_mul", std::to_string(m_blue_mul)));
            return map;
        }

    public:
        float m_red_mul = 1.0f;
        float m_green_mul = 1.0f;
        float m_blue_mul = 1.0f;
};

//-----------------------//
//----- COcvColorchange -----//
//-----------------------//
class COcvColorchange : public CImageProcess2d
{
    public:

        COcvColorchange() : CImageProcess2d()
        {
        }
        COcvColorchange(const std::string name, const std::shared_ptr<COcvColorchangeParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<COcvColorchangeParam>(*pParam);
        }

        size_t     getProgressSteps() override
        {
            return 3;
        }

        void    run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pGraphics = std::dynamic_pointer_cast<CGraphicsProcessInput>(getInput(1));

            auto pParam = std::dynamic_pointer_cast<COcvColorchangeParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgDst;
            CMat imgSrc = pInput->getImage();

            createGraphicsMask(imgSrc.getNbCols(), imgSrc.getNbRows(), pGraphics);

            emit m_signalHandler->doProgress();

            if(imgSrc.channels() != 3)
                throw CException(CoreExCode::INVALID_PARAMETER, "Color image required", __func__, __FILE__, __LINE__);

            try
            {
                CMat mask;
                if(isMaskAvailable(0))
                    mask = getGraphicsMask(0);
                else
                    mask = CMat(imgSrc.size(), CV_8U, 255);

                cv::colorChange(imgSrc, mask, imgDst, pParam->m_blue_mul, pParam->m_green_mul, pParam->m_red_mul);
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

class COcvColorchangeFactory : public CProcessFactory
{
    public:

        COcvColorchangeFactory()
        {
            m_info.m_name = QObject::tr("Colorchange").toStdString();
            m_info.m_description = QObject::tr("Given an original color image, two differently colored versions of this image can be mixed seamlessly.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Computational photography/Seamless cloning").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Color,Mixing,Seamless";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/df/da0/group__photo__clone.html#ga6684f35dc669ff6196a7c340dc73b98e";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pColorchangeParam = std::dynamic_pointer_cast<COcvColorchangeParam>(pParam);
            if(pColorchangeParam != nullptr)
                return std::make_shared<COcvColorchange>(m_info.m_name, pColorchangeParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pColorchangeParam = std::make_shared<COcvColorchangeParam>();
            assert(pColorchangeParam != nullptr);
            return std::make_shared<COcvColorchange>(m_info.m_name, pColorchangeParam);
        }
};

#endif // COCVCOLORCHANGE_HPP
