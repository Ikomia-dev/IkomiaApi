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

#ifndef CGMICBOOSTFADE_HPP
#define CGMICBOOSTFADE_HPP

#include "Core/C2dImageTask.h"
#include "Process/Gmic/CGmicTaskParam.hpp"
#include "Process/Gmic/CGmicImageConverter.h"

//-------------------------------//
//----- CGmicBoostFadeParam -----//
//-------------------------------//
class CGmicBoostFadeParam: public CGmicTaskParam
{
    public:

        enum ColorSpaces { YCBCR, LAB };

        CGmicBoostFadeParam() : CGmicTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_amplitude = std::stod(paramMap.at("amplitude"));
            m_colorSpace = std::stoi(paramMap.at("colorSpace"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("amplitude", std::to_string(m_amplitude)));
            map.insert(std::make_pair("colorSpace", std::to_string(m_colorSpace)));
            return map;
        }

        std::string getParamString() const override
        {
            //Build string of parameters, take care of order.
            return std::to_string(m_amplitude) + "," + std::to_string(m_colorSpace);
        }

    public:

        float   m_amplitude = 5;
        int     m_colorSpace = YCBCR;
};

//----------------------------//
//----- CGmicBoostFade -----//
//----------------------------//
class CGmicBoostFade : public C2dImageTask
{
    public:

        CGmicBoostFade() : C2dImageTask()
        {
        }
        CGmicBoostFade(const std::string name, const std::shared_ptr<CGmicBoostFadeParam>& pParam) : C2dImageTask(name)
        {
            m_pParam = std::make_shared<CGmicBoostFadeParam>(*pParam);
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        void run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsInput>(getInput(1));
            auto pParam = std::dynamic_pointer_cast<CGmicBoostFadeParam>(m_pParam);

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
                std::string command = "fx_boost_fade[0] " + pParam->getParamString();
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

            auto pOutput = std::dynamic_pointer_cast<CImageIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(imgDst);

            emit m_signalHandler->doProgress();
        }
};

class CGmicBoostFadeFactory : public CTaskFactory
{
    public:

        CGmicBoostFadeFactory()
        {
            m_info.m_name = "gmic_boost_fade";
            m_info.m_description = QObject::tr("This process fades chromaticity in images.").toStdString();
            m_info.m_path = QObject::tr("Gmic/Colors").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/gmic.png").toStdString();
            m_info.m_keywords = "color,enhance";
            m_info.m_authors = "David Tschumperlé";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<CGmicBoostFadeParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<CGmicBoostFade>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pParam = std::make_shared<CGmicBoostFadeParam>();
            assert(pParam != nullptr);
            return std::make_shared<CGmicBoostFade>(m_info.m_name, pParam);
        }
};

#endif // CGMICBOOSTFADE_HPP
