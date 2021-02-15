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

#ifndef COCVCOPYMAKEBORDER_HPP
#define COCVCOPYMAKEBORDER_HPP

#include "Core/CImageProcess2d.h"

//-----------------------------------//
//----- COcvCopyMakeBorderParam -----//
//-----------------------------------//
class COcvCopyMakeBorderParam: public CProtocolTaskParam
{
    public:

        COcvCopyMakeBorderParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_top = std::stoi(paramMap.at("top"));
            m_bottom = std::stoi(paramMap.at("bottom"));
            m_left = std::stoi(paramMap.at("left"));
            m_right = std::stoi(paramMap.at("right"));
            m_borderType = std::stoi(paramMap.at("borderType"));
            m_value[0] = std::stoi(paramMap.at("value0"));
            m_value[1] = std::stoi(paramMap.at("value1"));
            m_value[2] = std::stoi(paramMap.at("value2"));
            m_value[3] = std::stoi(paramMap.at("value3"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("top", std::to_string(m_top)));
            map.insert(std::make_pair("bottom", std::to_string(m_bottom)));
            map.insert(std::make_pair("left", std::to_string(m_left)));
            map.insert(std::make_pair("right", std::to_string(m_right)));
            map.insert(std::make_pair("borderType", std::to_string(m_borderType)));
            map.insert(std::make_pair("value0", std::to_string(m_value[0])));
            map.insert(std::make_pair("value1", std::to_string(m_value[1])));
            map.insert(std::make_pair("value2", std::to_string(m_value[2])));
            map.insert(std::make_pair("value3", std::to_string(m_value[3])));
            return map;
        }

    public:

        int         m_top = 0;
        int         m_bottom = 0;
        int         m_left = 0;
        int         m_right = 0;
        int         m_borderType = cv::BORDER_REFLECT;
        cv::Scalar  m_value = {0, 0, 0, 0};
};

//------------------------------//
//----- COcvCopyMakeBorder -----//
//------------------------------//
class COcvCopyMakeBorder : public CImageProcess2d
{
    public:

        COcvCopyMakeBorder() : CImageProcess2d(false)
        {
        }
        COcvCopyMakeBorder(const std::string name, const std::shared_ptr<COcvCopyMakeBorderParam>& pParam) : CImageProcess2d(name, false)
        {
            m_pParam = std::make_shared<COcvCopyMakeBorderParam>(*pParam);
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        void run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pParam = std::dynamic_pointer_cast<COcvCopyMakeBorderParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty images", __func__, __FILE__, __LINE__);

            CMat imgDst;
            emit m_signalHandler->doProgress();

            try
            {
                auto imgSrc = pInput->getImage();
                cv::copyMakeBorder(imgSrc, imgDst, pParam->m_top, pParam->m_bottom, pParam->m_left, pParam->m_right, pParam->m_borderType, pParam->m_value);
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

class COcvCopyMakeBorderFactory : public CProcessFactory
{
    public:

        COcvCopyMakeBorderFactory()
        {
            m_info.m_name = QObject::tr("CopyMakeBorder").toStdString();
            m_info.m_description = QObject::tr("The function copies the source image into the middle of the destination image. The areas to the left, to the right, above and below the copied source image will be filled with extrapolated pixels.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Core functionality/Operations on arrays").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "padding,copy,border";
            m_info.m_docLink = "https://docs.opencv.org/4.0.1/d2/de8/group__core__array.html#ga2ac1049c2c3dd25c2b41bffe17658a36";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvCopyMakeBorderParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvCopyMakeBorder>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvCopyMakeBorderParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvCopyMakeBorder>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVCOPYMAKEBORDER_HPP
