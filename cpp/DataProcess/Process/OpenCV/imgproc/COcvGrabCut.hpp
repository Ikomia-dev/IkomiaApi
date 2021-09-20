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

#ifndef COCVGRABCUT_HPP
#define COCVGRABCUT_HPP

#include "Core/C2dImageInteractiveTask.h"

//----------------------------//
//----- COcvGrabCutParam -----//
//----------------------------//
class COcvGrabCutParam: public CWorkflowTaskParam
{
    public:

        enum ActionFlags
        {
            CLEAR_SEGMENTATION = 0x00000001,
            SET_FOREGND = 0x00000002,
            SET_POSSIBLE_FOREGND = 0x00000004,
            SET_BCKGND = 0x00000008,
            SET_POSSIBLE_BCKGND = 0x000000016
        };

        COcvGrabCutParam()
        {
        }

        void        setParamMap(const UMapString& paramMap) override
        {
            m_iterationCount = std::stoi(paramMap.at("iterationCount"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("iterationCount", std::to_string(m_iterationCount)));
            return map;
        }

    public:

        int m_iterationCount = 1;
};

//-----------------------//
//----- COcvGrabCut -----//
//-----------------------//
class COcvGrabCut : public C2dImageInteractiveTask
{
    public:

        COcvGrabCut() : C2dImageInteractiveTask()
        {
            //Remove graphics input
            removeInput(1);
            setOutputDataType(IODataType::IMAGE_BINARY, 0);
            addOutput(std::make_shared<CImageIO>());
            setOutputColorMap(1, 0, {{255,0,0}});
        }
        COcvGrabCut(const std::string name, const std::shared_ptr<COcvGrabCutParam>& pParam) : C2dImageInteractiveTask(name)
        {
            //Remove graphics input
            removeInput(1);
            setOutputDataType(IODataType::IMAGE_BINARY, 0);
            addOutput(std::make_shared<CImageIO>());
            setOutputColorMap(1, 0, {{255,0,0}});
            m_pParam = std::make_shared<COcvGrabCutParam>(*pParam);
        }

        size_t  getProgressSteps() override
        {
            return 4;
        }

        void    graphicsChanged() override
        {
            auto pLayer = getInteractionLayer();
            if(pLayer == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Usage : draw a rectangle around the object you want to segment", __func__, __FILE__, __LINE__);

            auto pLastItem = getLastItem();
            if(pLastItem == nullptr)
                reset();
            else
            {
                auto graphicsCount = pLayer->getChildItemCount();
                if(graphicsCount == 1)
                {
                    if(m_status != Status::NOT_INIT)
                        reset();

                    if(pLastItem->type() == QGraphicsItem::UserType + static_cast<size_t>(GraphicsItem::RECTANGLE))
                        initRect(pLastItem);
                    else
                    {
                        clearInteractionLayer();
                        throw CException(CoreExCode::INVALID_PARAMETER, "Usage : draw a rectangle around the object you want to segment", __func__, __FILE__, __LINE__);
                    }
                }
                else
                    initMask(pLastItem);
            }
            run();
            emit m_signalHandler->doOutputChanged();
        }

        void    run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsInput>(getInput(1));
            auto pParam = std::dynamic_pointer_cast<COcvGrabCutParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgDst;
            CMat imgSrc = pInput->getImage();
            createGraphicsMask(imgSrc.getNbCols(), imgSrc.getNbRows(), pGraphicsInput);
            emit m_signalHandler->doProgress();

            try
            {
                imgDst.create(imgSrc.size(), CV_8UC1);
                switch(m_status)
                {
                    case Status::NOT_INIT:
                        imgDst.setTo(cv::Scalar(0));
                        break;

                    case Status::RECT_INIT:
                        cv::grabCut(imgSrc, m_mask, m_rc, m_bckgdModel, m_foregdModel, pParam->m_iterationCount, cv::GC_INIT_WITH_RECT);
                        imgDst = m_mask & cv::GC_FGD;
                        break;

                    case Status::MASK_INIT:
                        cv::grabCut(imgSrc, m_mask, m_rc, m_bckgdModel, m_foregdModel, pParam->m_iterationCount, cv::GC_INIT_WITH_MASK);
                        imgDst = m_mask & cv::GC_FGD;
                        break;
                }
                emit m_signalHandler->doProgress();
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            forwardInputImage(0, 1);
            applyGraphicsMaskToBinary(imgDst, imgDst, 0);
            emit m_signalHandler->doProgress();

            auto pOutput = std::dynamic_pointer_cast<CImageIO>(getOutput(0));
            if(pOutput)
            {
                imgDst *= 255;
                pOutput->setImage(imgDst);
            }
            endTaskRun();
            emit m_signalHandler->doProgress();
        }

        void    executeActions(int flags) override
        {
            if(flags & COcvGrabCutParam::ActionFlags::CLEAR_SEGMENTATION)
            {
                reset();
                clearInteractionLayer();
                run();
                emit m_signalHandler->doOutputChanged();
            }
            else if(flags & COcvGrabCutParam::ActionFlags::SET_BCKGND)
                m_currentClass = cv::GC_BGD;
            else if(flags & COcvGrabCutParam::ActionFlags::SET_FOREGND)
                m_currentClass = cv::GC_FGD;
            else if(flags & COcvGrabCutParam::ActionFlags::SET_POSSIBLE_BCKGND)
                m_currentClass = cv::GC_PR_BGD;
            else if(flags & COcvGrabCutParam::ActionFlags::SET_POSSIBLE_FOREGND)
                m_currentClass = cv::GC_PR_FGD;
        }

    private:

        void    initRect(QGraphicsItem* pItem)
        {
            if(m_mask.empty())
            {
                auto pInput = std::dynamic_pointer_cast<CImageIO>(getInput(0));
                CMat imgSrc = pInput->getImage();
                m_mask.create(imgSrc.size(), CV_8UC1);
            }

            m_mask.setTo(cv::Scalar::all(cv::GC_BGD));
            auto itemRc = pItem->mapRectToScene(pItem->boundingRect());
            m_rc = cv::Rect(itemRc.x(), itemRc.y(), itemRc.width(), itemRc.height());
            m_status = Status::RECT_INIT;
        }

        void    initMask(QGraphicsItem* pItem)
        {
            if(m_mask.empty())
            {
                auto pInput = std::dynamic_pointer_cast<CImageIO>(getInput(0));
                CMat imgSrc = pInput->getImage();
                m_mask.create(imgSrc.size(), CV_8UC1);
            }

            auto pItemInterface = dynamic_cast<CGraphicsItem*>(pItem);
            if(pItemInterface)
            {
                auto pInput = std::dynamic_pointer_cast<CImageIO>(getInput(0));
                CMat imgSrc = pInput->getImage();
                CGraphicsConversion converter((int)imgSrc.getNbRows(), (int)imgSrc.getNbCols());
                pItemInterface->insertToImage(m_mask, converter, true, true);
                m_status = Status::MASK_INIT;
            }
        }

        void    reset()
        {
            m_mask.setTo(cv::Scalar::all(cv::GC_BGD));
            m_status = Status::NOT_INIT;
            m_bckgdModel.release();
            m_foregdModel.release();
        }

    private:

        enum class Status : size_t { NOT_INIT, RECT_INIT, MASK_INIT };

        Status              m_status = Status::NOT_INIT;
        CMat                m_mask;
        CMat                m_bckgdModel;
        CMat                m_foregdModel;
        cv::Rect            m_rc;
        cv::GrabCutClasses  m_currentClass = cv::GC_BGD;
};

//------------------------------//
//----- COcvGrabCutFactory -----//
//------------------------------//
class COcvGrabCutFactory : public CTaskFactory
{
    public:

        COcvGrabCutFactory()
        {
            m_info.m_name = "ocv_grabcut_segmentation";
            m_info.m_description = QObject::tr("This process computes image segmentation based on graph cuts method.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Image processing/Miscellaneous image transformations").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Color segmentation, grabcut, gaussian mixture model, graph cuts";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d7/d1b/group__imgproc__misc.html#ga909c1dda50efcbeaa3ce126be862b37f";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvGrabCutParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvGrabCut>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvGrabCutParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvGrabCut>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVGRABCUT_HPP
