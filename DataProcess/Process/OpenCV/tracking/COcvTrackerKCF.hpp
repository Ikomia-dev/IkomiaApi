#ifndef COCVTRACKERKCF_HPP
#define COCVTRACKERKCF_HPP

#include "Core/CVideoProcessTracking.h"
#include "IO/CImageProcessIO.h"
#include <opencv2/tracking.hpp>

//--------------------------------//
//----- COcvTrackerKCFParam -----//
//--------------------------------//
class COcvTrackerKCFParam: public CProtocolTaskParam
{
    public:

        COcvTrackerKCFParam() : CProtocolTaskParam(){}

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
class COcvTrackerKCF : public CVideoProcessTracking
{
    public:

        COcvTrackerKCF() : CVideoProcessTracking()
        {
        }
        COcvTrackerKCF(const std::string name, const std::shared_ptr<COcvTrackerKCFParam>& pParam) : CVideoProcessTracking(name)
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

            auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
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

class COcvTrackerKCFFactory : public CProcessFactory
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

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvTrackerKCFParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvTrackerKCF>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvTrackerKCFParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvTrackerKCF>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVTRACKERKCF_HPP
