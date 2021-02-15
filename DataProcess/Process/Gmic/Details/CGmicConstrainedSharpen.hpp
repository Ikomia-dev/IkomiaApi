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

#ifndef CGMICCONSTRAINEDSHARPEN_HPP
#define CGMICCONSTRAINEDSHARPEN_HPP

#include "Core/CImageProcess2d.h"
#include "Process/Gmic/CGmicTaskParam.hpp"
#include "Process/Gmic/CGmicImageConverter.h"

//----------------------------------------//
//----- CGmicConstrainedSharpenParam -----//
//----------------------------------------//
class CGmicConstrainedSharpenParam: public CGmicTaskParam
{
    public:

        enum ValueAction { NONE, CUT, NORMALIZE };

        CGmicConstrainedSharpenParam() : CGmicTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_radius = std::stod(paramMap.at("radius"));
            m_amount = std::stod(paramMap.at("amount"));
            m_threshold = std::stod(paramMap.at("threshold"));
            m_constraintRadius = std::stoi(paramMap.at("constraintRadius"));
            m_overshoot = std::stod(paramMap.at("overshoot"));
            m_channel = std::stoi(paramMap.at("channel"));
            m_valueAction = std::stoi(paramMap.at("valueAction"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("radius", std::to_string(m_radius)));
            map.insert(std::make_pair("amount", std::to_string(m_amount)));
            map.insert(std::make_pair("threshold", std::to_string(m_threshold)));
            map.insert(std::make_pair("constraintRadius", std::to_string(m_constraintRadius)));
            map.insert(std::make_pair("overshoot", std::to_string(m_overshoot)));
            map.insert(std::make_pair("channel", std::to_string(m_channel)));
            map.insert(std::make_pair("valueAction", std::to_string(m_valueAction)));
            return map;
        }

        std::string getParamString() const override
        {
            //Build string of parameters, take care of order.
            return std::to_string(m_radius) + "," + std::to_string(m_amount) + "," + std::to_string(m_threshold) + "," +
                    std::to_string(m_constraintRadius) + "," + std::to_string(m_overshoot) + "," + std::to_string(m_channel) + "," +
                    std::to_string(m_valueAction);
        }

    public:

        const std::vector<std::string> m_channels =
        {
            "All", "RGBA [all]", "RGB [all]", "RGB [red]", "RGB [green]", "RGB [blue]", "RGBA [alpha]",
            "Linear RGB [all]", "Linear RGB [red]", "Linear RGB [green]", "Linear RGB [blue]",
            "YCbCr [luminance]", "YCbCr [blue-red chrominances]", "YCbCr [blue chrominance]", "YCbCr [red chrominance]", "YCbCr [green chrominance]",
            "Lab [lightness]", "Lab [ab-chrominances]", "Lab [a-chrominance]", "Lab [b-chrominance]",
            "Lch [ch-chrominances]", "Lch [c-chrominance]", "Lch [h-chrominance]",
            "HSV [hue]", "HSV [saturation]", "HSV [value]",
            "HSI [intensity]", "HSL [lightness]", "CMYK [cyan]", "CMYK [magenta]", "CMYK [yellow]", "CMYK [key]"
        };

        float   m_radius = 0.75;
        float   m_amount = 2;
        float   m_threshold = 1;
        int     m_constraintRadius = 5;
        float   m_overshoot = 0;
        int     m_channel = 11;
        int     m_valueAction = CUT;
};

//-----------------------------------//
//----- CGmicConstrainedSharpen -----//
//-----------------------------------//
class CGmicConstrainedSharpen : public CImageProcess2d
{
    public:

        CGmicConstrainedSharpen() : CImageProcess2d()
        {
        }
        CGmicConstrainedSharpen(const std::string name, const std::shared_ptr<CGmicConstrainedSharpenParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<CGmicConstrainedSharpenParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<CGmicConstrainedSharpenParam>(m_pParam);

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
                std::string command = "iain_constrained_sharpen[0] " + pParam->getParamString();
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

class CGmicConstrainedSharpenFactory : public CProcessFactory
{
    public:

        CGmicConstrainedSharpenFactory()
        {
            m_info.m_name = QObject::tr("Constrained sharpen").toStdString();
            m_info.m_description = QObject::tr("This process sharpens image but restricts pixels values to local minimum and maximum values to significantly reduce halos."
                                               "<b>Sharpen Radius</b>, <b>Amount</b> and <b>Threshold</b> are the normal unsharp controls."
                                               "A high <b>Amount</b> will produce aliasing"
                                               "<b>Constraint Radius</b> sets the radius to find the local minimum and maximum."
                                               "<b>Overshoot</b> sets how far the sharpening is allowed to go past the local minimum and maximum values.").toStdString();
            m_info.m_path = QObject::tr("Gmic/Details").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/gmic.png").toStdString();
            m_info.m_keywords = "enhance,contrast";
            m_info.m_authors = "Iain Fergusson";
            m_info.m_year = 2016;
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<CGmicConstrainedSharpenParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<CGmicConstrainedSharpen>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pParam = std::make_shared<CGmicConstrainedSharpenParam>();
            assert(pParam != nullptr);
            return std::make_shared<CGmicConstrainedSharpen>(m_info.m_name, pParam);
        }
};

#endif // CGMICCONSTRAINEDSHARPEN_HPP
