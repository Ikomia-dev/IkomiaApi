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

#ifndef COCVINPAINT_HPP
#define COCVINPAINT_HPP

#include "Core/CImageProcess2d.h"

//----------------------------//
//----- COcvInpaintParam -----//
//----------------------------//
class COcvInpaintParam: public CProtocolTaskParam
{
    public:

        COcvInpaintParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_radius = std::stod(paramMap.at("radius"));
            m_method = std::stoi(paramMap.at("method"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("radius", std::to_string(m_radius)));
            map.insert(std::make_pair("method", std::to_string(m_method)));
            return map;
        }

    public:

        double  m_radius = 10;
        int     m_method = cv::INPAINT_NS;
};

//-----------------------//
//----- COcvInpaint -----//
//-----------------------//
class COcvInpaint : public CImageProcess2d
{
    public:

        COcvInpaint() : CImageProcess2d()
        {
        }
        COcvInpaint(const std::string name, const std::shared_ptr<COcvInpaintParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<COcvInpaintParam>(*pParam);
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        void run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsProcessInput>(getInput(1));
            auto pParam = std::dynamic_pointer_cast<COcvInpaintParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgDst;
            CMat imgSrc = pInput->getImage();
            createGraphicsMask(imgSrc.getNbCols(), imgSrc.getNbRows(), pGraphicsInput);
            emit m_signalHandler->doProgress();

            auto mask = getGraphicsMask(0);
            if(mask.data == nullptr)
                throw CException(CoreExCode::NULL_POINTER, "Empty region of interest", __func__, __FILE__, __LINE__);

            try
            {
                cv::inpaint(imgSrc, mask, imgDst, pParam->m_radius, pParam->m_method);
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

//------------------------------//
//----- COcvInpaintFactory -----//
//------------------------------//
class COcvInpaintFactory : public CProcessFactory
{
    public:

        COcvInpaintFactory()
        {
            m_info.m_name = QObject::tr("Inpaint filter").toStdString();
            m_info.m_description = QObject::tr("This process performs inpainting from region of interest drawn in original image.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Computational photography").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "inpainting,photo,deletion";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d1/d0d/group__photo.html#gaedd30dfa0214fec4c88138b51d678085";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvInpaintParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvInpaint>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvInpaintParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvInpaint>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVINPAINT_HPP
