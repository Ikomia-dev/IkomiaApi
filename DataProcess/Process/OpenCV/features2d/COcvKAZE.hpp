#ifndef COCVKAZE_HPP
#define COCVKAZE_HPP

#include "Core/CFeatureImageProcess2d.h"
#include "IO/CImageProcessIO.h"

//--------------------------------//
//----- COcvKAZEParam -----//
//--------------------------------//
class COcvKAZEParam: public CProtocolTaskParam
{
    public:

        COcvKAZEParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_bExtended = std::stoi(paramMap.at("bExtended"));
            m_bUpright = std::stoi(paramMap.at("bUpright"));
            m_threshold = std::stof(paramMap.at("threshold"));
            m_nOctaves = std::stoi(paramMap.at("nOctaves"));
            m_nOctaveLayers = std::stoi(paramMap.at("nOctaveLayers"));
            m_diffusivity = std::stoi(paramMap.at("diffusivity"));
            m_bUseProvidedKeypoints = std::stoi(paramMap.at("bUseProvidedKeypoints"));
            m_bDetect = std::stoi(paramMap.at("bDetect"));
            m_bCompute = std::stoi(paramMap.at("bCompute"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("bExtended", std::to_string(m_bExtended)));
            map.insert(std::make_pair("bUpright", std::to_string(m_bUpright)));
            map.insert(std::make_pair("threshold", std::to_string(m_threshold)));
            map.insert(std::make_pair("nOctaves", std::to_string(m_nOctaves)));
            map.insert(std::make_pair("nOctaveLayers", std::to_string(m_nOctaveLayers)));
            map.insert(std::make_pair("diffusivity", std::to_string(m_diffusivity)));
            map.insert(std::make_pair("bUseProvidedKeypoints", std::to_string(m_bUseProvidedKeypoints)));
            map.insert(std::make_pair("bDetect", std::to_string(m_bDetect)));
            map.insert(std::make_pair("bCompute", std::to_string(m_bCompute)));
            return map;
        }

    public:

        bool    m_bExtended = false;
        bool    m_bUpright = false;
        float   m_threshold = 0.001f;
        int     m_nOctaves = 4;
        int     m_nOctaveLayers = 4;
        int     m_diffusivity = cv::KAZE::DIFF_PM_G2;
        bool    m_bUseProvidedKeypoints = false;
        bool    m_bDetect = true;
        bool    m_bCompute = true;
};

//---------------------------//
//----- COcvKAZE -----//
//---------------------------//
class COcvKAZE : public CFeatureImageProcess2d
{
    public:

        COcvKAZE() : CFeatureImageProcess2d()
        {
            addKeypointInput();
            addDescriptorAndKeypointOuputs();
        }
        COcvKAZE(const std::string name, const std::shared_ptr<COcvKAZEParam>& pParam) : CFeatureImageProcess2d(name)
        {
            addKeypointInput();
            addDescriptorAndKeypointOuputs();
            m_pParam = std::make_shared<COcvKAZEParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvKAZEParam>(m_pParam);

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
                m_pFeatures = cv::KAZE::create(pParam->m_bExtended, pParam->m_bUpright, pParam->m_threshold, pParam->m_nOctaves,
                                               pParam->m_nOctaveLayers, static_cast<cv::KAZE::DiffusivityType>(pParam->m_diffusivity));
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

class COcvKAZEFactory : public CProcessFactory
{
    public:

        COcvKAZEFactory()
        {
            m_info.m_name = QObject::tr("KAZE").toStdString();
            m_info.m_description = QObject::tr("This process implements the KAZE keypoint detector and descriptor extractor.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/2D features framework/Feature detection and description").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Features,Keypoints,Descriptors,Interest";
            m_info.m_authors = "Pablo Fernández Alcantarilla, Adrien Bartoli, and Andrew J Davison";
            m_info.m_article = "Kaze features";
            m_info.m_journal = "ECCV 2012, pages 214–227";
            m_info.m_year = 2012;
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d3/d61/classcv_1_1KAZE.html";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvKAZEParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvKAZE>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvKAZEParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvKAZE>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVKAZE_HPP
