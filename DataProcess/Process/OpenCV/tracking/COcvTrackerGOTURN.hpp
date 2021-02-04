#ifndef COCVTRACKERGOTURN_HPP
#define COCVTRACKERGOTURN_HPP

#include "Core/CVideoProcessTracking.h"
#include "IO/CImageProcessIO.h"
#include <opencv2/tracking.hpp>

//--------------------------------//
//----- COcvTrackerGOTURNParam -----//
//--------------------------------//
class COcvTrackerGOTURNParam: public CProtocolTaskParam
{
    public:

        COcvTrackerGOTURNParam() : CProtocolTaskParam(){}

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
//----- COcvTrackerGOTURN -----//
//---------------------------//
class COcvTrackerGOTURN : public CVideoProcessTracking
{
    public:

        COcvTrackerGOTURN() : CVideoProcessTracking()
        {
        }
        COcvTrackerGOTURN(const std::string name, const std::shared_ptr<COcvTrackerGOTURNParam>& pParam) : CVideoProcessTracking(name)
        {
            m_pParam = std::make_shared<COcvTrackerGOTURNParam>(*pParam);
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

            auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pParam = std::dynamic_pointer_cast<COcvTrackerGOTURNParam>(m_pParam);

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
                    m_pTracker = cv::TrackerGOTURN::create();
                    if(!m_pTracker->init(imgSrc, m_trackedRect))
                        throw CException(CoreExCode::INVALID_PARAMETER, QObject::tr("GOTURN tracker init failed").toStdString(), __func__, __FILE__, __LINE__);
                }
                else
                {
                    // Update tracked rectangle on the fly
                    if(!m_pTracker->update(imgSrc, m_trackedRect))
                        throw CException(CoreExCode::INVALID_PARAMETER, QObject::tr("GOTURN tracker update failed").toStdString(), __func__, __FILE__, __LINE__);
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
        cv::Ptr<cv::TrackerGOTURN>  m_pTracker;
};

class COcvTrackerGOTURNFactory : public CProcessFactory
{
    public:

        COcvTrackerGOTURNFactory()
        {
            m_info.m_name = QObject::tr("TrackerGOTURN").toStdString();
            m_info.m_description = QObject::tr("GOTURN is a kind of trackers based on Convolutional Neural Networks (CNN).").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Tracking API").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Tracking,CNN,Deep,Learning";
            m_info.m_authors = "David Held, Sebastian Thrun, and Silvio Savarese";
            m_info.m_article = "Learning to track at 100 fps with deep regression networks";
            m_info.m_journal = "ECCV";
            m_info.m_year = 2016;
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d7/d4c/classcv_1_1TrackerGOTURN.html";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvTrackerGOTURNParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvTrackerGOTURN>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvTrackerGOTURNParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvTrackerGOTURN>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVTRACKERGOTURN_HPP
