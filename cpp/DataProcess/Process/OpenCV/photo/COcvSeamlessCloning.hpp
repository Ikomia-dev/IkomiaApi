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

#ifndef COCVSEAMLESSCLONING_HPP
#define COCVSEAMLESSCLONING_HPP

#include "Core/C2dImageTask.h"
#include "IO/CImageIO.h"

//----------------------------//
//----- COcvSeamlessCloningParam -----//
//----------------------------//
class COcvSeamlessCloningParam: public CWorkflowTaskParam
{
    public:

        COcvSeamlessCloningParam() : CWorkflowTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_flags = std::stoi(paramMap.at("flags"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("flags", std::to_string(m_flags)));
            return map;
        }

    public:
        int m_flags = cv::NORMAL_CLONE;
};

//-----------------------//
//----- COcvSeamlessCloning -----//
//-----------------------//
class COcvSeamlessCloning : public C2dImageTask
{
    public:

        COcvSeamlessCloning() : C2dImageTask()
        {
            clearInputs();
            addInput(std::make_shared<CImageIO>());
            addInput(std::make_shared<CGraphicsInput>());
            addInput(std::make_shared<CImageIO>());
            addInput(std::make_shared<CGraphicsInput>());
        }
        COcvSeamlessCloning(const std::string name, const std::shared_ptr<COcvSeamlessCloningParam>& pParam) : C2dImageTask(name)
        {
            clearInputs();
            addInput(std::make_shared<CImageIO>());
            addInput(std::make_shared<CGraphicsInput>());
            addInput(std::make_shared<CImageIO>());
            addInput(std::make_shared<CGraphicsInput>());
            m_pParam = std::make_shared<COcvSeamlessCloningParam>(*pParam);
        }

        size_t     getProgressSteps() override
        {
            return 3;
        }

        void    run() override
        {
            beginTaskRun();
            auto pInput1 = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            auto pGraphics1 = std::dynamic_pointer_cast<CGraphicsInput>(getInput(1));
            auto pInput2 = std::dynamic_pointer_cast<CImageIO>(getInput(2));
            auto pGraphics2 = std::dynamic_pointer_cast<CGraphicsInput>(getInput(3));
            auto pParam = std::dynamic_pointer_cast<COcvSeamlessCloningParam>(m_pParam);

            if( pInput1 == nullptr || pGraphics1 == nullptr ||
                pInput2 == nullptr || pGraphics2 == nullptr ||
                pParam == nullptr)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);
            }

            if(pInput1->isDataAvailable() == false || pInput2->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            if(pGraphics1->isDataAvailable() == false || pGraphics2->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty graphics", __func__, __FILE__, __LINE__);

            CMat imgDst;
            CMat img1 = pInput1->getImage();
            CMat img2 = pInput2->getImage();

            createGraphicsMask(img1.getNbCols(), img1.getNbRows(), pGraphics1);

            auto items = pGraphics2->getItems();
            QPointF center = items[0]->getBoundingRect().center();
            cv::Point pt(center.x(), center.y());
            emit m_signalHandler->doProgress();

            if(img1.channels() != 3 || img2.channels() != 3)
                throw CException(CoreExCode::INVALID_PARAMETER, "Color image required", __func__, __FILE__, __LINE__);

            try
            {
                auto mask = getGraphicsMask(0);
                cv::seamlessClone(img1, img2, mask, pt, imgDst, pParam->m_flags);
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
};

class COcvSeamlessCloningFactory : public CTaskFactory
{
    public:

        COcvSeamlessCloningFactory()
        {
            m_info.m_name = "ocv_seamless_cloning";
            m_info.m_description = QObject::tr("Image editing tasks concern either global changes (color/intensity corrections, filters, deformations) or local changes concerned to a selection. Here we are interested in achieving local changes, ones that are restricted to a region manually selected (ROI), in a seamless and effortless manner. The extent of the changes ranges from slight distortions to complete replacement by novel content.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Computational photography/Seamless cloning").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Poisson,Editing,Fusion,Blending";
            m_info.m_authors = "Patrick Pérez, Michel Gangnet, and Andrew Blake";
            m_info.m_article = "Poisson image editing";
            m_info.m_journal = "ACM Transactions on Graphics, volume 22, pages 313–318";
            m_info.m_year = 2003;
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/df/da0/group__photo__clone.html#ga2bf426e4c93a6b1f21705513dfeca49d";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pSeamlessCloningParam = std::dynamic_pointer_cast<COcvSeamlessCloningParam>(pParam);
            if(pSeamlessCloningParam != nullptr)
                return std::make_shared<COcvSeamlessCloning>(m_info.m_name, pSeamlessCloningParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pSeamlessCloningParam = std::make_shared<COcvSeamlessCloningParam>();
            assert(pSeamlessCloningParam != nullptr);
            return std::make_shared<COcvSeamlessCloning>(m_info.m_name, pSeamlessCloningParam);
        }
};

#endif // COCVSEAMLESSCLONING_HPP
