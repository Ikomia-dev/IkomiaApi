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

#ifndef COCVCROP_HPP
#define COCVCROP_HPP

#include "Core/C2dImageTask.h"
#include "IO/CImageIO.h"
#include "Graphics/CGraphicsRectangle.h"

//-------------------------//
//----- COcvCropParam -----//
//-------------------------//
class COcvCropParam: public CWorkflowTaskParam
{
    public:

        COcvCropParam() : CWorkflowTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_cropRect.x = std::stoi(paramMap.at("left"));
            m_cropRect.y = std::stoi(paramMap.at("top"));
            m_cropRect.width = std::stoi(paramMap.at("width"));
            m_cropRect.height = std::stoi(paramMap.at("height"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("left", std::to_string(m_cropRect.x)));
            map.insert(std::make_pair("top", std::to_string(m_cropRect.y)));
            map.insert(std::make_pair("width", std::to_string(m_cropRect.width)));
            map.insert(std::make_pair("height", std::to_string(m_cropRect.height)));
            return map;
        }

    public:

        cv::Rect    m_cropRect;
};

//--------------------//
//----- COcvCrop -----//
//--------------------//
class COcvCrop : public C2dImageTask
{
    public:

        COcvCrop() : C2dImageTask()
        {
        }
        COcvCrop(const std::string name, const std::shared_ptr<COcvCropParam>& pParam) : C2dImageTask(name)
        {
            m_pParam = std::make_shared<COcvCropParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvCropParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgDst;
            emit m_signalHandler->doProgress();

            try
            {
                cv::Rect cropRoi;
                auto imgSrc = pInput->getImage();

                if(pGraphicsInput->isDataAvailable() == true)
                {
                    cropRoi = getGraphicsRoi(pGraphicsInput);
                    if(cropRoi.empty() == true)
                        cropRoi = cv::Rect(0, 0, imgSrc.cols, imgSrc.rows);
                }
                else if(pParam->m_cropRect.width != 0 && pParam->m_cropRect.height != 0)
                    cropRoi = pParam->m_cropRect;
                else
                    cropRoi = cv::Rect(0, 0, imgSrc.cols, imgSrc.rows);

                CMat cropImg = CMat(imgSrc, cropRoi);
                imgDst = cropImg.clone();
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            endTaskRun();
            emit m_signalHandler->doProgress();

            auto pOutput = std::dynamic_pointer_cast<CImageIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(imgDst);

            emit m_signalHandler->doProgress();
        }

    private:

        cv::Rect    getGraphicsRoi(const GraphicsInputPtr& pInput)
        {
            auto items = pInput->getItems();
            for(size_t i=0; i<items.size(); ++i)
            {
                auto pItem = std::dynamic_pointer_cast<CProxyGraphicsRect>(items[i]);
                if(pItem)
                    return cv::Rect(pItem->m_x, pItem->m_y, pItem->m_width, pItem->m_height);
            }
            return cv::Rect();
        }
};

class COcvCropFactory : public CTaskFactory
{
    public:

        COcvCropFactory()
        {
            m_info.m_name = "ocv_crop";
            m_info.m_description = QObject::tr("This process crops your image.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Core functionality/Operations on arrays").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "crop";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pCropParam = std::dynamic_pointer_cast<COcvCropParam>(pParam);
            if(pCropParam != nullptr)
                return std::make_shared<COcvCrop>(m_info.m_name, pCropParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pCropParam = std::make_shared<COcvCropParam>();
            assert(pCropParam != nullptr);
            return std::make_shared<COcvCrop>(m_info.m_name, pCropParam);
        }
};

#endif // COCVCROP_HPP
