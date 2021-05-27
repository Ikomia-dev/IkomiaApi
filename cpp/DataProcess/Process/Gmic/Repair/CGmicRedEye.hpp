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

#ifndef CGMICREDEYE_HPP
#define CGMICREDEYE_HPP

#include "Core/CImageProcess2d.h"
#include "Process/Gmic/CGmicTaskParam.hpp"
#include "Process/Gmic/CGmicImageConverter.h"

//-----------------------------//
//----- CGmicRedEyeParam -----//
//-----------------------------//
class CGmicRedEyeParam: public CGmicTaskParam
{
    public:

        CGmicRedEyeParam() : CGmicTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_threshold = std::stoi(paramMap.at("threshold"));
            m_smoothness = std::stod(paramMap.at("smoothness"));
            m_attenuation = std::stod(paramMap.at("attenuation"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("threshold", std::to_string(m_threshold)));
            map.insert(std::make_pair("smoothness", std::to_string(m_smoothness)));
            map.insert(std::make_pair("attenuation", std::to_string(m_attenuation)));
            return map;
        }

        std::string getParamString() const override
        {
            //Build string of parameters, take care of order.
            return std::to_string(m_threshold) + "," + std::to_string(m_smoothness) + "," +  std::to_string(m_attenuation);
        }

    public:

        int     m_threshold = 75;
        double  m_smoothness = 3.5;
        double  m_attenuation = 0.1;
};

//-----------------------//
//----- CGmicRedEye -----//
//-----------------------//
class CGmicRedEye : public CImageProcess2d
{
    public:

        CGmicRedEye() : CImageProcess2d()
        {
        }
        CGmicRedEye(const std::string name, const std::shared_ptr<CGmicRedEyeParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<CGmicRedEyeParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<CGmicRedEyeParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid input", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgSrc = pInput->getImage();
            CMat imgDst(imgSrc.rows, imgSrc.cols, imgSrc.type());
            createGraphicsMask(imgSrc.getNbCols(), imgSrc.getNbRows(), pGraphicsInput);
            emit m_signalHandler->doProgress();

            try
            {
                //Put image inputs into a gmic_list object
                gmic_list<float> imageList;
                gmic_list<char> image_names;

                //Allocate list, parameter is the number of image inputs
                imageList.assign(1);
                //Conversion from CMat(cv::Mat) to gmic_image(CImg)
                CGmicImageConverter::convert(imgSrc, imageList[0]);
                //Build command for gmic interpreter
                std::string command = "red_eye[0] " + pParam->getParamString();
                //Call interpreter
                gmic(command.c_str(), imageList, image_names);
                //Conversion from gmic_image to CMat
                CGmicImageConverter::convert(imageList[0], imgDst);
                //Free memory
                imageList.assign(0);
            }
            catch(gmic_exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e.what(), __func__, __FILE__, __LINE__);
            }

            endTaskRun();
            emit m_signalHandler->doProgress();

            applyGraphicsMask(imgSrc, imgDst, 0);

            auto pOutput = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(imgDst);

            emit m_signalHandler->doProgress();
        }
};

class CGmicRedEyeFactory : public CProcessFactory
{
    public:

        CGmicRedEyeFactory()
        {
            m_info.m_name = QObject::tr("Red-eye").toStdString();
            m_info.m_description = QObject::tr("This process automatically attenuates red-eye effect in images.").toStdString();
            m_info.m_docLink = "https://gmic.eu/reference.shtml#red_eye";
            m_info.m_path = QObject::tr("Gmic/Repair").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/gmic.png").toStdString();
            m_info.m_keywords = "attenuation,photo,red,eye,eyes";
            m_info.m_authors = "David Tschumperlé";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<CGmicRedEyeParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<CGmicRedEye>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pParam = std::make_shared<CGmicRedEyeParam>();
            assert(pParam != nullptr);
            return std::make_shared<CGmicRedEye>(m_info.m_name, pParam);
        }
};

#endif // CGMICREDEYE_HPP
