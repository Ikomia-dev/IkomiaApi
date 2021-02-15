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

#ifndef CGMICDOG_HPP
#define CGMICDOG_HPP

#include "Core/CImageProcess2d.h"
#include "Process/Gmic/CGmicTaskParam.hpp"
#include "Process/Gmic/CGmicImageConverter.h"

//-------------------------//
//----- CGmicDoGParam -----//
//-------------------------//
class CGmicDoGParam: public CGmicTaskParam
{
    public:

        CGmicDoGParam() : CGmicTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_variance1 = std::stod(paramMap.at("variance1"));
            m_variance2 = std::stod(paramMap.at("variance2"));
            m_threshold = std::stod(paramMap.at("threshold"));
            m_bNegativeColors = std::stoi(paramMap.at("isNegativeColors"));
            m_bMonochrome = std::stoi(paramMap.at("isMonochrome"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("variance1", std::to_string(m_variance1)));
            map.insert(std::make_pair("variance2", std::to_string(m_variance2)));
            map.insert(std::make_pair("threshold", std::to_string(m_threshold)));
            map.insert(std::make_pair("isNegativeColors", std::to_string(m_bNegativeColors)));
            map.insert(std::make_pair("isMonochrome", std::to_string(m_bMonochrome)));
            return map;
        }

        std::string getParamString() const override
        {
            //Build string of parameters, take care of order.
            return std::to_string(m_variance1) + "," + std::to_string(m_variance2) + "," + std::to_string(m_threshold) + "," +
                    std::to_string(m_bNegativeColors) + "," + std::to_string(m_bMonochrome);
        }

    public:

        float   m_variance1 = 1.4;
        float   m_variance2 = 1.5;
        float   m_threshold = 0;
        bool    m_bNegativeColors = false;
        bool    m_bMonochrome = true;
};

//--------------------//
//----- CGmicDoG -----//
//--------------------//
class CGmicDoG : public CImageProcess2d
{
    public:

        CGmicDoG() : CImageProcess2d()
        {
        }
        CGmicDoG(const std::string name, const std::shared_ptr<CGmicDoGParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<CGmicDoGParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<CGmicDoGParam>(m_pParam);

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
                std::string command = "fx_dog[0] " + pParam->getParamString();
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

class CGmicDoGFactory : public CProcessFactory
{
    public:

        CGmicDoGFactory()
        {
            m_info.m_name = QObject::tr("Difference of Gaussians").toStdString();
            m_info.m_description = QObject::tr("This process performs DoG algorithm.").toStdString();
            m_info.m_path = QObject::tr("Gmic/Contours").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/gmic.png").toStdString();
            m_info.m_keywords = "edge,frequency";
            m_info.m_authors = "David Tschumperlé";
            m_info.m_year = 2010;
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<CGmicDoGParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<CGmicDoG>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pParam = std::make_shared<CGmicDoGParam>();
            assert(pParam != nullptr);
            return std::make_shared<CGmicDoG>(m_info.m_name, pParam);
        }
};

#endif // CGMICDOG_HPP
