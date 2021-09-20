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

#ifndef COCVCOLORMAP_HPP
#define COCVCOLORMAP_HPP

#include "Core/C2dImageTask.h"
#include "IO/CImageIO.h"

//-----------------------------//
//----- COcvColorMapParam -----//
//-----------------------------//
class COcvColorMapParam: public CWorkflowTaskParam
{
    public:

        COcvColorMapParam() : CWorkflowTaskParam()
        {

        }

        void        setParamMap(const UMapString& paramMap) override
        {
            m_type = std::stoi(paramMap.at("type"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("type", std::to_string(m_type)));
            return map;
        }

    public:

        int m_type = cv::COLORMAP_AUTUMN;
};

//------------------------//
//----- COcvColorMap -----//
//------------------------//
class COcvColorMap : public C2dImageTask
{
    public:

        COcvColorMap() : C2dImageTask()
        {
        }
        COcvColorMap(const std::string name, const std::shared_ptr<COcvColorMapParam>& pParam) : C2dImageTask(name)
        {
            m_pParam = std::make_shared<COcvColorMapParam>(*pParam);
        }

        size_t     getProgressSteps() override
        {
            return 3;
        }

        void    run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsInput>(getInput(1));
            auto pParam = std::dynamic_pointer_cast<COcvColorMapParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgSrc, imgDst;
            imgSrc = pInput->getImage();
            createGraphicsMask(imgSrc.getNbCols(), imgSrc.getNbRows(), pGraphicsInput);
            emit m_signalHandler->doProgress();

            try
            {
                cv::applyColorMap(imgSrc, imgDst, pParam->m_type);
                cv::cvtColor(imgDst, imgDst, cv::COLOR_RGB2BGR);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }            

            emit m_signalHandler->doProgress();
            applyGraphicsMask(imgSrc, imgDst, 0);

            auto pOutput = std::dynamic_pointer_cast<CImageIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(imgDst);

            endTaskRun();
            emit m_signalHandler->doProgress();
        }
};

class COcvColorMapFactory : public CTaskFactory
{
    public:

        COcvColorMapFactory()
        {
            m_info.m_name = "ocv_colormap";
            m_info.m_description = QObject::tr("The human perception isn't built for observing fine changes in grayscale images. Human eyes are more sensitive to observing changes between colors, so you often need to recolor your grayscale images to get a clue about them. OpenCV now comes with various colormaps to enhance the visualization in your computer vision application.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Image processing/ColorMaps").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "color,colormap,grayscale,lut";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d3/d50/group__imgproc__colormap.html";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pColorMapParam = std::dynamic_pointer_cast<COcvColorMapParam>(pParam);
            if(pColorMapParam != nullptr)
                return std::make_shared<COcvColorMap>(m_info.m_name, pColorMapParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pColorMapParam = std::make_shared<COcvColorMapParam>();
            assert(pColorMapParam != nullptr);
            return std::make_shared<COcvColorMap>(m_info.m_name, pColorMapParam);
        }
};

#endif // COCVCOLORMAP_HPP
