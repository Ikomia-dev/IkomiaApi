#ifndef COCVBFMATCHER_HPP
#define COCVBFMATCHER_HPP

#include "Core/CFeatureMatcher2d.h"
#include "IO/CImageProcessIO.h"

//--------------------------------//
//----- COcvBFMatcherParam -----//
//--------------------------------//
class COcvBFMatcherParam: public CProtocolTaskParam
{
    public:

        COcvBFMatcherParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_normType = std::stoi(paramMap.at("normType"));
            m_bCrossCheck = std::stoi(paramMap.at("bCrossCheck"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("normType", std::to_string(m_normType)));
            map.insert(std::make_pair("bCrossCheck", std::to_string(m_bCrossCheck)));
            return map;
        }

    public:

        int     m_normType = cv::NORM_L2;
        bool    m_bCrossCheck = false;
};

//---------------------------//
//----- COcvBFMatcher -----//
//---------------------------//
class COcvBFMatcher : public CFeatureMatcher2d
{
    public:

        COcvBFMatcher() : CFeatureMatcher2d()
        {
        }
        COcvBFMatcher(const std::string name, const std::shared_ptr<COcvBFMatcherParam>& pParam) : CFeatureMatcher2d(name)
        {
            m_pParam = std::make_shared<COcvBFMatcherParam>(*pParam);
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        void run() override
        {
            beginTaskRun();

            if(getInputCount() < 6)
                throw CException(CoreExCode::INVALID_PARAMETER, "Not enough inputs", __func__, __FILE__, __LINE__);

            auto pImg1 = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pDesc1 = std::dynamic_pointer_cast<CImageProcessIO>(getInput(1));
            auto pFeatures1 = std::dynamic_pointer_cast<CFeatureProcessIO<cv::KeyPoint>>(getInput(2));

            auto pImg2 = std::dynamic_pointer_cast<CImageProcessIO>(getInput(3));
            auto pDesc2 = std::dynamic_pointer_cast<CImageProcessIO>(getInput(4));
            auto pFeatures2 = std::dynamic_pointer_cast<CFeatureProcessIO<cv::KeyPoint>>(getInput(5));

            auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsProcessInput>(getInput(6));
            auto pParam = std::dynamic_pointer_cast<COcvBFMatcherParam>(m_pParam);

            if( pDesc1 == nullptr || pImg1 == nullptr || pFeatures1 == nullptr ||
                pDesc2 == nullptr || pImg2 == nullptr || pFeatures2 == nullptr ||
                pParam == nullptr)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);
            }

            if( pDesc1->isDataAvailable() == false || pImg1->isDataAvailable() == false ||
                pDesc2->isDataAvailable() == false || pImg2->isDataAvailable() == false )
            {
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty images", __func__, __FILE__, __LINE__);
            }

            emit m_signalHandler->doProgress();

            CMat descriptor1 = pDesc1->getImage();
            CMat descriptor2 = pDesc2->getImage();
            CMat img1 = pImg1->getImage();

            std::vector< cv::DMatch > matches;
            createGraphicsMask(img1.getNbCols(), img1.getNbRows(), pGraphicsInput);
            try
            {
                auto mask = getGraphicsMask(0);
                m_pMatcher = cv::BFMatcher::create(pParam->m_normType, pParam->m_bCrossCheck);
                m_pMatcher->match(descriptor1, descriptor2, matches, mask);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            endTaskRun();
            emit m_signalHandler->doProgress();

            CMat img2 = pImg2->getImage();

            auto key1 = pFeatures1->getValueList(0);
            auto key2 = pFeatures2->getValueList(0);

            manageOutputs(img1, key1, img2, key2, matches);

            emit m_signalHandler->doProgress();
        }
};

class COcvBFMatcherFactory : public CProcessFactory
{
    public:

        COcvBFMatcherFactory()
        {
            m_info.m_name = QObject::tr("BFMatcher").toStdString();
            m_info.m_description = QObject::tr("Finds the best match for each descriptor from a query set.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/2D features framework/Descriptor matchers").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Features,Keypoints,Descriptors,Interest";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d3/da1/classcv_1_1BFMatcher.html";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvBFMatcherParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvBFMatcher>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvBFMatcherParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvBFMatcher>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVBFMATCHER_HPP
