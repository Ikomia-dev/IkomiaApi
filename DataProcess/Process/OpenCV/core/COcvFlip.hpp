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

#ifndef COCVFLIP_HPP
#define COCVFLIP_HPP


#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"

//-------------------------//
//----- COcvFlipParam -----//
//-------------------------//
class COcvFlipParam: public CProtocolTaskParam
{
    public:

        COcvFlipParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_flipCode = std::stoi(paramMap.at("flipCode"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("flipCode", std::to_string(m_flipCode)));
            return map;
        }

    public:

        //Flip vertical by default
        int m_flipCode = 0;
};

//--------------------//
//----- COcvFlip -----//
//--------------------//
class COcvFlip : public CImageProcess2d
{
    public:

        COcvFlip() : CImageProcess2d()
        {
        }
        COcvFlip(const std::string name, const std::shared_ptr<COcvFlipParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<COcvFlipParam>(*pParam);
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        void run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pParam = std::dynamic_pointer_cast<COcvFlipParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgDst;
            CMat imgSrc = pInput->getImage();
            emit m_signalHandler->doProgress();

            try
            {
                cv::flip(imgSrc, imgDst, pParam->m_flipCode);
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
            applyInputGraphicsMask(1, 0, 0, MaskMode::MERGE_SOURCE);
            emit m_signalHandler->doProgress();
        }
};

class COcvFlipFactory : public CProcessFactory
{
    public:

        COcvFlipFactory()
        {
            m_info.m_name = QObject::tr("Flip").toStdString();
            m_info.m_description = QObject::tr("This process flip your image around vertical, horizontal or both axes.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Core functionality/Operations on arrays").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "flip";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d2/de8/group__core__array.html#gaca7be533e3dac7feb70fc60635adf441";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pFlipParam = std::dynamic_pointer_cast<COcvFlipParam>(pParam);
            if(pFlipParam != nullptr)
                return std::make_shared<COcvFlip>(m_info.m_name, pFlipParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pFlipParam = std::make_shared<COcvFlipParam>();
            assert(pFlipParam != nullptr);
            return std::make_shared<COcvFlip>(m_info.m_name, pFlipParam);
        }
};

#endif // COCVFLIP_HPP
