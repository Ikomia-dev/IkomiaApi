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

#ifndef COCVCAMSHIFT_HPP
#define COCVCAMSHIFT_HPP

#include "Core/CVideoTask.h"
#include "Graphics/CGraphicsPolygon.h"

//------------------------------//
//----- COcvCamShiftParam -----//
//-----------------------------//
class COcvCamShiftParam: public CWorkflowTaskParam
{
    public:

        COcvCamShiftParam()
        {
        }

        void        setParamMap(const UMapString& paramMap) override
        {
            m_termType = std::stoi(paramMap.at("termType"));
            m_termMaxCount = std::stoi(paramMap.at("termMaxCount"));
            m_termEpsilon = std::stod(paramMap.at("termEpsilon"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("termType", std::to_string(m_termType)));
            map.insert(std::make_pair("termMaxCount", std::to_string(m_termMaxCount)));
            map.insert(std::make_pair("termEpsilon", std::to_string(m_termEpsilon)));
            return map;
        }

    public:

        int     m_termType = cv::TermCriteria::COUNT+cv::TermCriteria::EPS;
        int     m_termMaxCount = 10;
        double  m_termEpsilon = 1.0;
};

//------------------------//
//----- COcvCamShift -----//
//------------------------//
class COcvCamShift : public CVideoTask
{
    public:

        COcvCamShift() : CVideoTask()
        {
            setOutputDataType(IODataType::IMAGE_BINARY, 0);
            addOutput(std::make_shared<CImageIO>());
            addOutput(std::make_shared<CBlobMeasureIO>());
            setOutputColorMap(1, 0, {{255,0,0}});
        }
        COcvCamShift(const std::string name, const std::shared_ptr<COcvCamShiftParam>& pParam) : CVideoTask(name)
        {
            setOutputDataType(IODataType::IMAGE_BINARY, 0);
            addOutput(std::make_shared<CImageIO>());
            addOutput(std::make_shared<CBlobMeasureIO>());
            setOutputColorMap(1, 0, {{255,0,0}});
            m_pParam = std::make_shared<COcvCamShiftParam>(*pParam);            
        }

        size_t  getProgressSteps() override
        {
            return 6;
        }

        void    notifyVideoStart(int frameCount) override
        {
            Q_UNUSED(frameCount);
            m_bVideoStarted = true;
            m_bInitRoi = false;
            m_bInitSearch = false;
            initSearchProperties();
        }
        void    notifyVideoEnd() override
        {
            m_bVideoStarted = false;
        }

        void    run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            auto pParam = std::dynamic_pointer_cast<COcvCamShiftParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            if(m_bVideoStarted == false)
                throw CException(CoreExCode::INVALID_USAGE, QObject::tr("The tracking starts when video begins playing").toStdString(), __func__, __FILE__, __LINE__);

            if(m_bInitSearch == false)
                throw CException(CoreExCode::INVALID_PARAMETER, QObject::tr("The process needs a single rectangle as graphics input").toStdString(), __func__, __FILE__, __LINE__);

            try
            {
                cv::Mat hsv, mask, hue, backproj;
                CMat imgSrc = pInput->getImage();

                cv::cvtColor(imgSrc, hsv, cv::COLOR_RGB2HSV);
                emit m_signalHandler->doProgress();
                cv::inRange(hsv, cv::Scalar(m_hMin, m_sMin, m_vMin), cv::Scalar(m_hMax, m_sMax, m_vMax), mask);
                int ch[] = {0, 0};
                hue.create(hsv.size(), hsv.depth());
                cv::mixChannels(&hsv, 1, &hue, 1, ch, 1);
                const float* phRange = m_hRange.data();
                emit m_signalHandler->doProgress();
                cv::calcBackProject(&hue, 1, 0, m_hist, backproj, &phRange);
                backproj &= mask;
                emit m_signalHandler->doProgress();
                auto orienterBbox = cv::CamShift(backproj, m_trackedRect, cv::TermCriteria(pParam->m_termType, pParam->m_termMaxCount, pParam->m_termEpsilon));
                emit m_signalHandler->doProgress();                
                manageOutputs(orienterBbox);
                emit m_signalHandler->doProgress();
                endTaskRun();
                emit m_signalHandler->doProgress();
            }
            catch(std::exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e.what(), __func__, __FILE__, __LINE__);
            }
        }

    private:

        void    setRoiToTrack()
        {
            auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsInput>(getInput(1));
            if(pGraphicsInput == nullptr)
                return;

            auto items = pGraphicsInput->getItems();
            if(items.size() == 0)
            {
                Utils::print(QObject::tr("CamShift algorithm requires rectangular object").toStdString(), QtCriticalMsg);
                return;
            }

            if(items.size() > 1)
                Utils::print(QObject::tr("CamShift algorithm can only track one rectangular object").toStdString(), QtWarningMsg);

            auto pItem = std::dynamic_pointer_cast<CProxyGraphicsRect>(items.back());
            if(pItem == nullptr)
            {
                Utils::print(QObject::tr("CamShift algorithm can only track rectangular object").toStdString(), QtCriticalMsg);
                return;
            }

            m_trackedRect = cv::Rect(pItem->m_x, pItem->m_y, pItem->m_width, pItem->m_height);
            m_bInitRoi = true;
        }

        void    initSearchProperties()
        {
            setRoiToTrack();
            if(m_bInitRoi == false)
                return;

            const float* phRange = m_hRange.data();
            cv::Mat hsv, mask, hue;
            auto pInput = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            cv::cvtColor(pInput->getImage(), hsv, cv::COLOR_RGB2HSV);
            cv::inRange(hsv, cv::Scalar(m_hMin, m_sMin, m_vMin), cv::Scalar(m_hMax, m_sMax, m_vMax), mask);
            int ch[] = {0, 0};
            hue.create(hsv.size(), hsv.depth());
            cv::mixChannels(&hsv, 1, &hue, 1, ch, 1);
            cv::Mat roi(hue, m_trackedRect);
            cv::Mat maskRoi(mask, m_trackedRect);
            cv::calcHist(&roi, 1, 0, maskRoi, m_hist, 1, &m_histSize, &phRange);
            cv::normalize(m_hist, m_hist, 0, 255, cv::NORM_MINMAX);
            m_bInitSearch = true;
        }

        void    manageOutputs(const cv::RotatedRect& bbox)
        {
            forwardInputImage(0, 1);

            //Generate binary mask
            auto pInput = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            auto pOutput = std::dynamic_pointer_cast<CImageIO>(getOutput(0));

            if(pOutput)
            {
                CMat imgSrc = pInput->getImage();
                CGraphicsPolygon poly;
                cv::Point2f vertices[4];
                bbox.points(vertices);

                for (int i = 0; i < 4; i++)
                    poly.addPoint(QPointF(vertices[i].x, vertices[i].y));

                QList<QGraphicsItem*> pList;
                pList.append(&poly);

                CGraphicsConversion conv(imgSrc.cols, imgSrc.rows);
                CMat imgDst = conv.graphicsToBinaryMask(pList);
                pOutput->setImage(imgDst);
            }

            //Tracked oriented rectangle coordinates
            auto pMeasureOutput = std::dynamic_pointer_cast<CBlobMeasureIO>(getOutput(2));
            if(pMeasureOutput)
            {
                CMeasure bboxMeasure(CMeasure::ORIENTED_BBOX, QObject::tr("Tracked ROI").toStdString());
                pMeasureOutput->setObjectMeasure(0, CObjectMeasure(bboxMeasure, {bbox.center.x, bbox.center.y, bbox.size.width, bbox.size.height, bbox.angle}, -1, ""));
            }
        }

    private:

        cv::Mat             m_hist;
        cv::Rect            m_trackedRect;
        bool                m_bVideoStarted = false;
        bool                m_bInitRoi = false;
        bool                m_bInitSearch = false;
        std::vector<float>  m_hRange = {0, 180};
        int                 m_histSize = 16;
        int                 m_hMin = 0;
        int                 m_sMin = 60;
        int                 m_vMin = 32;
        int                 m_hMax = 180;
        int                 m_sMax = 255;
        int                 m_vMax = 255;
};

//-------------------------------//
//----- COcvCamShiftFactory -----//
//-------------------------------//
class COcvCamShiftFactory : public CTaskFactory
{
    public:

        COcvCamShiftFactory()
        {
            m_info.m_name = "ocv_cam_shift";
            m_info.m_description = QObject::tr("This process computes object tracking using CamShift algorithm based on the back project of the histogram.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Video analysis/Object tracking").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "camshift,meanshift,tracking,video,back projection,histogram";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/dc/d6b/group__video__track.html#gaef2bd39c8356f423124f1fe7c44d54a1";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvCamShiftParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvCamShift>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvCamShiftParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvCamShift>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVCAMSHIFT_HPP
