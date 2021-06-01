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

#ifndef CGMICDYNAMICRANGEINCREASE_HPP
#define CGMICDYNAMICRANGEINCREASE_HPP

#include "Core/CImageProcess2d.h"
#include "Process/Gmic/CGmicTaskParam.hpp"
#include "Process/Gmic/CGmicImageConverter.h"

//------------------------------------------//
//----- CGmicDynamicRangeIncreaseParam -----//
//------------------------------------------//
class CGmicDynamicRangeIncreaseParam: public CGmicTaskParam
{
    public:

        CGmicDynamicRangeIncreaseParam() : CGmicTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_mapTones = std::stod(paramMap.at("mapTones"));
            m_recoverShadows = std::stod(paramMap.at("recoverShadows"));
            m_recoverHighligths = std::stod(paramMap.at("recoverHighligths"));
            m_enhanceDetails = std::stod(paramMap.at("enhanceDetails"));
            m_detailStrength = std::stod(paramMap.at("detailStrength"));
            m_bMapTones = std::stoi(paramMap.at("isMapTones"));
            m_bEnhanceDetails = std::stoi(paramMap.at("isEnhanceDetails"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("mapTones", std::to_string(m_mapTones)));
            map.insert(std::make_pair("recoverShadows", std::to_string(m_recoverShadows)));
            map.insert(std::make_pair("recoverHighligths", std::to_string(m_recoverHighligths)));
            map.insert(std::make_pair("enhanceDetails", std::to_string(m_enhanceDetails)));
            map.insert(std::make_pair("detailStrength", std::to_string(m_detailStrength)));
            map.insert(std::make_pair("isMapTones", std::to_string(m_bMapTones)));
            map.insert(std::make_pair("isEnhanceDetails", std::to_string(m_bEnhanceDetails)));
            return map;
        }

        std::string getParamString() const override
        {
            //Build string of parameters, take care of order.
            return std::to_string(m_mapTones) + "," + std::to_string(m_recoverShadows) + "," + std::to_string(m_recoverHighligths) + "," +
                    std::to_string(m_enhanceDetails) + "," + std::to_string(m_detailStrength) + "," + std::to_string(m_bMapTones) + "," +
                    std::to_string(m_bEnhanceDetails);
        }

    public:

        float   m_mapTones = 0;
        float   m_recoverShadows = 0;
        float   m_recoverHighligths = 0;
        float   m_enhanceDetails = 1;
        float   m_detailStrength = 0.5;
        bool    m_bMapTones = true;
        bool    m_bEnhanceDetails = true;
};

//-------------------------------------//
//----- CGmicDynamicRangeIncrease -----//
//-------------------------------------//
class CGmicDynamicRangeIncrease : public CImageProcess2d
{
    public:

        CGmicDynamicRangeIncrease() : CImageProcess2d()
        {
        }
        CGmicDynamicRangeIncrease(const std::string name, const std::shared_ptr<CGmicDynamicRangeIncreaseParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<CGmicDynamicRangeIncreaseParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<CGmicDynamicRangeIncreaseParam>(m_pParam);

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
                std::string command = "fx_tk_dri[0] " + pParam->getParamString();
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

class CGmicDynamicRangeIncreaseFactory : public CProcessFactory
{
    public:

        CGmicDynamicRangeIncreaseFactory()
        {
            m_info.m_name = QObject::tr("Dynamic range increase").toStdString();
            m_info.m_description = QObject::tr("This process increases dynamic range of images.").toStdString();
            m_info.m_path = QObject::tr("Gmic/Details").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/gmic.png").toStdString();
            m_info.m_keywords = "enhance,contrast,hdr";
            m_info.m_authors = "Tom Keil";
            m_info.m_year = 2011;
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<CGmicDynamicRangeIncreaseParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<CGmicDynamicRangeIncrease>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pParam = std::make_shared<CGmicDynamicRangeIncreaseParam>();
            assert(pParam != nullptr);
            return std::make_shared<CGmicDynamicRangeIncrease>(m_info.m_name, pParam);
        }
};

#endif // CGMICDYNAMICRANGEINCREASE_HPP
