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

#ifndef CGMICAUTOBALANCE_HPP
#define CGMICAUTOBALANCE_HPP

#include "Core/C2dImageTask.h"
#include "Process/Gmic/CGmicTaskParam.hpp"
#include "Process/Gmic/CGmicImageConverter.h"

//---------------------------------//
//----- CGmicAutoBalanceParam -----//
//---------------------------------//
class CGmicAutoBalanceParam: public CGmicTaskParam
{
    public:

        enum GMIC_COLOR_SPACE { HSI, HSV, LAB, LINEAR_RGB, RGB, YCBCR };

        CGmicAutoBalanceParam() : CGmicTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_area = std::stoi(paramMap.at("area"));
            m_smooth = std::stoi(paramMap.at("smooth"));
            m_colorSpace = std::stod(paramMap.at("colorSpace"));
            m_bBalance = std::stoi(paramMap.at("isBalance"));
            m_bReduceRAM = std::stoi(paramMap.at("isReduceRAM"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("area", std::to_string(m_area)));
            map.insert(std::make_pair("smooth", std::to_string(m_smooth)));
            map.insert(std::make_pair("colorSpace", std::to_string(m_colorSpace)));
            map.insert(std::make_pair("isBalance", std::to_string(m_bBalance)));
            map.insert(std::make_pair("isReduceRAM", std::to_string(m_bReduceRAM)));
            return map;
        }

        std::string getParamString() const override
        {
            //Build string of parameters, take care of order.
            return std::to_string(m_area) + "," + std::to_string(m_smooth) + "," +  std::to_string(m_colorSpace) + "," +
                    std::to_string(m_bBalance) + "," + std::to_string(m_bReduceRAM);
        }

    public:

        int     m_area = 30;
        float   m_smooth = 0;
        int     m_colorSpace = HSV;
        bool    m_bBalance = true;
        bool    m_bReduceRAM = false;
};

//----------------------------//
//----- CGmicAutoBalance -----//
//----------------------------//
class CGmicAutoBalance : public C2dImageTask
{
    public:

        CGmicAutoBalance() : C2dImageTask()
        {
        }
        CGmicAutoBalance(const std::string name, const std::shared_ptr<CGmicAutoBalanceParam>& pParam) : C2dImageTask(name)
        {
            m_pParam = std::make_shared<CGmicAutoBalanceParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<CGmicAutoBalanceParam>(m_pParam);

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
                std::string command = "gcd_auto_balance " + pParam->getParamString();
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

class CGmicAutoBalanceFactory : public CTaskFactory
{
    public:

        CGmicAutoBalanceFactory()
        {
            m_info.m_name = "gmic_auto_balance";
            m_info.m_description = QObject::tr("This process performs a map based tone and color balance.").toStdString();
            m_info.m_path = QObject::tr("Gmic/Colors").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/gmic.png").toStdString();
            m_info.m_keywords = "color,tone,homogeneous";
            m_info.m_authors = "GarageCoder";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<CGmicAutoBalanceParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<CGmicAutoBalance>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pParam = std::make_shared<CGmicAutoBalanceParam>();
            assert(pParam != nullptr);
            return std::make_shared<CGmicAutoBalance>(m_info.m_name, pParam);
        }
};

#endif // CGMICAUTOBALANCE_HPP
