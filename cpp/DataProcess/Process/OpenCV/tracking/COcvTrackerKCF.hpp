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

#ifndef COCVTRACKERKCF_HPP
#define COCVTRACKERKCF_HPP

#include "Core/CVideoTrackingTask.h"
#include "IO/CImageIO.h"
#include <opencv2/tracking.hpp>

//--------------------------------//
//----- COcvTrackerKCFParam -----//
//--------------------------------//
class COcvTrackerKCFParam: public CWorkflowTaskParam
{
    public:

        COcvTrackerKCFParam() : CWorkflowTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            Q_UNUSED(paramMap)
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            return map;
        }

    public:
};

//---------------------------//
//----- COcvTrackerKCF -----//
//---------------------------//
class COcvTrackerKCF : public CVideoTrackingTask
{
    public:

        COcvTrackerKCF() : CVideoTrackingTask()
        {
        }
        COcvTrackerKCF(const std::string name, const std::shared_ptr<COcvTrackerKCFParam>& pParam) : CVideoTrackingTask(name)
        {
            m_pParam = std::make_shared<COcvTrackerKCFParam>(*pParam);
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        void run() override
        {
            beginTaskRun();

            if(getInputCount() < 2)
                throw CException(CoreExCode::INVALID_PARAMETER, "Not enough inputs", __func__, __FILE__, __LINE__);

            auto pInput = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            auto pParam = std::dynamic_pointer_cast<COcvTrackerKCFParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(m_bVideoStarted == false)
                throw CException(CoreExCode::INVALID_USAGE, QObject::tr("The tracking starts when video begins playing").toStdString(), __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty images", __func__, __FILE__, __LINE__);

            emit m_signalHandler->doProgress();

            CMat imgSrc = pInput->getImage();
            try
            {
                if(m_bInitRoi == false)
                {
                    setRoiToTrack();
                    if(m_bInitRoi == false)
                        throw CException(CoreExCode::INVALID_PARAMETER, QObject::tr("The process needs a single rectangle as graphics input").toStdString(), __func__, __FILE__, __LINE__);
                    // Create tracker
                    m_pTracker = cv::TrackerKCF::create();
                    if(!m_pTracker->init(imgSrc, m_trackedRect))
                        throw CException(CoreExCode::INVALID_PARAMETER, QObject::tr("KCF tracker init failed").toStdString(), __func__, __FILE__, __LINE__);
                }
                else
                {
                    // Update tracked rectangle on the fly
                    if(!m_pTracker->update(imgSrc, m_trackedRect))
                        throw CException(CoreExCode::INVALID_PARAMETER, QObject::tr("KCF tracker update failed").toStdString(), __func__, __FILE__, __LINE__);
                }

                manageOutputs();
                emit m_signalHandler->doProgress();
                endTaskRun();
                emit m_signalHandler->doProgress();
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }
        }

    private:
        cv::Ptr<cv::TrackerKCF>  m_pTracker;
};

class COcvTrackerKCFFactory : public CTaskFactory
{
    public:

        COcvTrackerKCFFactory()
        {
            m_info.m_name = QObject::tr("TrackerKCF").toStdString();
            m_info.m_description = QObject::tr("KCF is a novel tracking framework that utilizes properties of circulant matrix to enhance the processing speed.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Tracking API").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Tracking";
            m_info.m_authors = "J. F. Henriques, R. Caseiro, P. Martins, and J. Batista";
            m_info.m_article = "Exploiting the circulant structure of tracking-by-detection with kernels";
            m_info.m_journal = "ECCV";
            m_info.m_year = 2012;
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d2/dff/classcv_1_1TrackerKCF.html";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvTrackerKCFParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvTrackerKCF>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvTrackerKCFParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvTrackerKCF>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVTRACKERKCF_HPP
