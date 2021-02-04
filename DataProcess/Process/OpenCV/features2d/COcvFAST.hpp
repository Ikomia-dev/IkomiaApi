#ifndef COCVFAST_HPP
#define COCVFAST_HPP

#include "Core/CFeatureImageProcess2d.h"
#include "IO/CImageProcessIO.h"

//--------------------------------//
//----- COcvFASTParam -----//
//--------------------------------//
class COcvFASTParam: public CProtocolTaskParam
{
    public:

        COcvFASTParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_threshold = std::stoi(paramMap.at("thresh"));
            m_bNonmaxSuppression = std::stoi(paramMap.at("nonmaxsupp"));
            m_type = std::stoi(paramMap.at("type"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("thresh", std::to_string(m_threshold)));
            map.insert(std::make_pair("nonmaxsupp", std::to_string(m_bNonmaxSuppression)));
            map.insert(std::make_pair("type", std::to_string(m_type)));
            return map;
        }

    public:

        int     m_threshold = 10;
        bool    m_bNonmaxSuppression = true;
        int     m_type = cv::FastFeatureDetector::TYPE_9_16;
};

//---------------------------//
//----- COcvFAST -----//
//---------------------------//
class COcvFAST : public CFeatureImageProcess2d
{
    public:

        COcvFAST() : CFeatureImageProcess2d()
        {
            addKeypointOutput();
        }
        COcvFAST(const std::string name, const std::shared_ptr<COcvFASTParam>& pParam) : CFeatureImageProcess2d(name)
        {
            addKeypointOutput();
            m_pParam = std::make_shared<COcvFASTParam>(*pParam);
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
            auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsProcessInput>(getInput(1));
            auto pParam = std::dynamic_pointer_cast<COcvFASTParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty images", __func__, __FILE__, __LINE__);

            emit m_signalHandler->doProgress();

            CMat imgSrc = pInput->getImage();
            createGraphicsMask(imgSrc.getNbCols(), imgSrc.getNbRows(), pGraphicsInput);
            try
            {
                manageInputs();
                m_pFeatures = cv::FastFeatureDetector::create(pParam->m_threshold, pParam->m_bNonmaxSuppression,
                                                              static_cast<cv::FastFeatureDetector::DetectorType>(pParam->m_type));
                makeFeatures(imgSrc, true, false);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            endTaskRun();

            emit m_signalHandler->doProgress();
            manageOutputs();
            emit m_signalHandler->doProgress();
        }
};

class COcvFASTFactory : public CProcessFactory
{
    public:

        COcvFASTFactory()
        {
            m_info.m_name = QObject::tr("FAST").toStdString();
            m_info.m_description = QObject::tr("This process implements the FAST keypoint detector and descriptor extractor.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/2D features framework/Feature detection and description").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Features,Keypoints,Descriptors,Interest";
            m_info.m_authors = "Edward Rosten and Tom Drummond";
            m_info.m_article = "Machine learning for high-speed corner detection";
            m_info.m_journal = "ECCV 2006, pages 430-443";
            m_info.m_year = 2006;
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d5/d51/group__features2d__main.html#gaf3185c9bd7496ba7a629add75fb371ad";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvFASTParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvFAST>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvFASTParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvFAST>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVFAST_HPP
