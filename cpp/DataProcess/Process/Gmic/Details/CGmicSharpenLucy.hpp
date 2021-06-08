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

#ifndef CGMICSHARPENLUCY_HPP
#define CGMICSHARPENLUCY_HPP

#include "Core/C2dImageTask.h"
#include "Process/Gmic/CGmicTaskParam.hpp"
#include "Process/Gmic/CGmicImageConverter.h"

//---------------------------------//
//----- CGmicSharpenLucyParam -----//
//---------------------------------//
class CGmicSharpenLucyParam: public CGmicTaskParam
{
    public:

        enum BlurMethods { EXPONENTIAL, GAUSSIAN };

        CGmicSharpenLucyParam() : CGmicTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_sigma = std::stod(paramMap.at("sigma"));
            m_iteration = std::stoi(paramMap.at("iteration"));
            m_blur = std::stoi(paramMap.at("blur"));
            m_bCut = std::stoi(paramMap.at("isCut"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("sigma", std::to_string(m_sigma)));
            map.insert(std::make_pair("iteration", std::to_string(m_iteration)));
            map.insert(std::make_pair("blur", std::to_string(m_blur)));
            map.insert(std::make_pair("isCut", std::to_string(m_bCut)));
            return map;
        }

        std::string getParamString() const override
        {
            //Build string of parameters, take care of order.
            return std::to_string(m_sigma) + "," + std::to_string(m_iteration) + "," + std::to_string(m_blur) + "," +
                    std::to_string(m_bCut);
        }

    public:

        float   m_sigma = 1;
        int     m_iteration = 10;
        int     m_blur = GAUSSIAN;
        bool    m_bCut = true;
};

//-----------------------//
//----- CGmicLucy -----//
//-----------------------//
class CGmicSharpenLucy : public C2dImageTask
{
    public:

        CGmicSharpenLucy() : C2dImageTask()
        {
        }
        CGmicSharpenLucy(const std::string name, const std::shared_ptr<CGmicSharpenLucyParam>& pParam) : C2dImageTask(name)
        {
            m_pParam = std::make_shared<CGmicSharpenLucyParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<CGmicSharpenLucyParam>(m_pParam);

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
                std::string command = "fx_unsharp_richardsonlucy[0] " + pParam->getParamString();
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

class CGmicSharpenLucyFactory : public CTaskFactory
{
    public:

        CGmicSharpenLucyFactory()
        {
            m_info.m_name = QObject::tr("Sharpen [Richardson-Lucy]").toStdString();
            m_info.m_description = QObject::tr("This process performs deblurring on images with Richardon-Lucy algorithm.").toStdString();
            m_info.m_path = QObject::tr("Gmic/Details").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/gmic.png").toStdString();
            m_info.m_keywords = "enhance,contrast,deconvolution";
            m_info.m_authors = "Jerôme Boulanger";
            m_info.m_year = 2013;
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<CGmicSharpenLucyParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<CGmicSharpenLucy>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pParam = std::make_shared<CGmicSharpenLucyParam>();
            assert(pParam != nullptr);
            return std::make_shared<CGmicSharpenLucy>(m_info.m_name, pParam);
        }
};

#endif // CGMICSHARPENLUCY_HPP
