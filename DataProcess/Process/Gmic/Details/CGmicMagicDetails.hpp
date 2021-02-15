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

#ifndef CGMICMAGICDETAILS_HPP
#define CGMICMAGICDETAILS_HPP

#include "Core/CImageProcess2d.h"
#include "Process/Gmic/CGmicTaskParam.hpp"
#include "Process/Gmic/CGmicImageConverter.h"

//----------------------------------//
//----- CGmicMagicDetailsParam -----//
//----------------------------------//
class CGmicMagicDetailsParam: public CGmicTaskParam
{
    public:

        CGmicMagicDetailsParam() : CGmicTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_amplitude = std::stod(paramMap.at("amplitude"));
            m_spatialScale = std::stod(paramMap.at("spatialScale"));
            m_valueScale = std::stod(paramMap.at("valueScale"));
            m_edges = std::stod(paramMap.at("edges"));
            m_smoothness = std::stod(paramMap.at("smoothness"));
            m_channel = std::stoi(paramMap.at("channel"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("amplitude", std::to_string(m_amplitude)));
            map.insert(std::make_pair("spatialScale", std::to_string(m_spatialScale)));
            map.insert(std::make_pair("valueScale", std::to_string(m_valueScale)));
            map.insert(std::make_pair("edges", std::to_string(m_edges)));
            map.insert(std::make_pair("smoothness", std::to_string(m_smoothness)));
            map.insert(std::make_pair("channel", std::to_string(m_channel)));
            return map;
        }

        std::string getParamString() const override
        {
            //Build string of parameters, take care of order.
            return std::to_string(m_amplitude) + "," + std::to_string(m_spatialScale) + "," + std::to_string(m_valueScale) + "," +
                    std::to_string(m_edges) + "," + std::to_string(m_smoothness) + "," + std::to_string(m_channel);
        }

    public:

        const std::vector<std::string> m_channels =
        {
            "All", "RGBA [All]","RGB [All]","RGB [Red]","RGB [Green]","RGB [Blue]","RGBA [Alpha]",
            "Linear RGB [All]","Linear RGB [Red]","Linear RGB [Green]","Linear RGB [Blue]","YCbCr [Luminance]",
            "YCbCr [Blue-Red Chrominances]","YCbCr [Blue Chrominance]","YCbCr [Red Chrominance]",
            "YCbCr [Green Chrominance]","Lab [Lightness]","Lab [ab-Chrominances]","Lab [a-Chrominance]",
            "Lab [b-Chrominance]","Lch [ch-Chrominances]","Lch [c-Chrominance]","Lch [h-Chrominance]","HSV [Hue]",
            "HSV [Saturation]","HSV [Value]","HSI [Intensity]","HSL [Lightness]","CMYK [Cyan]","CMYK [Magenta]",
            "CMYK [Yellow]","CMYK [Key]","YIQ [Luma]","YIQ [Chromas]"
        };

        float   m_amplitude = 6;
        float   m_spatialScale = 3;
        float   m_valueScale = 15;
        float   m_edges = -0.5;
        float   m_smoothness = 2;
        int     m_channel = 27;
};

//-----------------------------//
//----- CGmicMagicDetails -----//
//-----------------------------//
class CGmicMagicDetails : public CImageProcess2d
{
    public:

        CGmicMagicDetails() : CImageProcess2d()
        {
        }
        CGmicMagicDetails(const std::string name, const std::shared_ptr<CGmicMagicDetailsParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<CGmicMagicDetailsParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<CGmicMagicDetailsParam>(m_pParam);

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
                std::string command = "fx_magic_details[0] " + pParam->getParamString();
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

class CGmicMagicDetailsFactory : public CProcessFactory
{
    public:

        CGmicMagicDetailsFactory()
        {
            m_info.m_name = QObject::tr("Magic details").toStdString();
            m_info.m_description = QObject::tr("This process increases details in images.").toStdString();
            m_info.m_path = QObject::tr("Gmic/Details").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/gmic.png").toStdString();
            m_info.m_keywords = "enhance,contrast";
            m_info.m_authors = "David Tschumperlé";
            m_info.m_year = 2018;
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<CGmicMagicDetailsParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<CGmicMagicDetails>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pParam = std::make_shared<CGmicMagicDetailsParam>();
            assert(pParam != nullptr);
            return std::make_shared<CGmicMagicDetails>(m_info.m_name, pParam);
        }
};

#endif // CGMICMAGICDETAILS_HPP
