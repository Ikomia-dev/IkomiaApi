#ifndef COCVSIFT_HPP
#define COCVSIFT_HPP

#include "Core/CFeatureImageProcess2d.h"

//-------------------------//
//----- COcvSIFTParam -----//
//-------------------------//
class COcvSIFTParam: public CProtocolTaskParam
{
    public:

        COcvSIFTParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_featuresCount = std::stoi(paramMap.at("featuresCount"));
            m_octaveLayersCount = std::stoi(paramMap.at("octaveLayersCount"));
            m_contrastThreshold = std::stod(paramMap.at("contrastThreshold"));
            m_edgeThreshold = std::stod(paramMap.at("edgeThreshold"));
            m_sigma = std::stod(paramMap.at("sigma"));
            m_bUseProvidedKeypoints = std::stoi(paramMap.at("bUseProvidedKeypoints"));
            m_bDetect = std::stoi(paramMap.at("bDetect"));
            m_bCompute = std::stoi(paramMap.at("bCompute"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("featuresCount", std::to_string(m_featuresCount)));
            map.insert(std::make_pair("octaveLayersCount", std::to_string(m_octaveLayersCount)));
            map.insert(std::make_pair("contrastThreshold", std::to_string(m_contrastThreshold)));
            map.insert(std::make_pair("edgeThreshold", std::to_string(m_edgeThreshold)));
            map.insert(std::make_pair("sigma", std::to_string(m_sigma)));
            map.insert(std::make_pair("bUseProvidedKeypoints", std::to_string(m_bUseProvidedKeypoints)));
            map.insert(std::make_pair("bDetect", std::to_string(m_bDetect)));
            map.insert(std::make_pair("bCompute", std::to_string(m_bCompute)));
            return map;
        }

    public:

        int     m_featuresCount = 0;
        int     m_octaveLayersCount = 3;
        double  m_contrastThreshold = 0.04;
        double  m_edgeThreshold = 10.0;
        double  m_sigma = 1.6;
        bool    m_bUseProvidedKeypoints = false;
        bool    m_bDetect = true;
        bool    m_bCompute = true;
};

//--------------------//
//----- COcvSIFT -----//
//--------------------//
class COcvSIFT : public CFeatureImageProcess2d
{
    public:

        COcvSIFT() : CFeatureImageProcess2d()
        {
            addKeypointInput();
            addDescriptorAndKeypointOuputs();
        }
        COcvSIFT(const std::string name, const std::shared_ptr<COcvSIFTParam>& pParam) : CFeatureImageProcess2d(name)
        {
            addKeypointInput();
            addDescriptorAndKeypointOuputs();
            m_pParam = std::make_shared<COcvSIFTParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvSIFTParam>(m_pParam);

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
                m_pFeatures = cv::SIFT::create(pParam->m_featuresCount, pParam->m_octaveLayersCount, pParam->m_contrastThreshold,
                                               pParam->m_edgeThreshold, pParam->m_sigma);
                makeFeatures(imgSrc, pParam->m_bDetect, pParam->m_bCompute);
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

class COcvSIFTFactory : public CProcessFactory
{
    public:

        COcvSIFTFactory()
        {
            m_info.m_name = QObject::tr("SIFT").toStdString();
            m_info.m_description = QObject::tr("This process implements the SIFT keypoint detector and descriptor extractor.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/2D features framework/Feature detection and description").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Features,Keypoints,Descriptors,Interest";
            m_info.m_authors = "David G. Lowe";
            m_info.m_article = "Distinctive image features from scale-invariant keypoints";
            m_info.m_journal = "International Journal of Computer Vision, 60, 91-110";
            m_info.m_year = 2004;
            m_info.m_docLink = "https://docs.opencv.org/4.5.0/d7/d60/classcv_1_1SIFT.html";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvSIFTParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvSIFT>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvSIFTParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvSIFT>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVSIFT_HPP
