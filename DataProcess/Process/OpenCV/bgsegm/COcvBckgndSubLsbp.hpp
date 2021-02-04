#ifndef COCVBCKGNDSUBLSBP_HPP
#define COCVBCKGNDSUBLSBP_HPP

#include "opencv2/bgsegm.hpp"
#include "Core/CVideoProcess.h"

//----------------------------------//
//----- COcvBckgndSubLsbpParam -----//
//----------------------------------//
class COcvBckgndSubLsbpParam: public CProtocolTaskParam
{
    public:

        COcvBckgndSubLsbpParam()
        {
        }

        void        setParamMap(const UMapString& paramMap) override
        {
            m_motionComp = std::stoi(paramMap.at("motionComp"));
            m_samples = std::stoi(paramMap.at("samples"));
            m_lsbpRadius = std::stoi(paramMap.at("lsbpRadius"));
            m_TLower = std::stod(paramMap.at("TLower"));
            m_TUpper = std::stod(paramMap.at("TUpper"));
            m_TInc = std::stod(paramMap.at("TInc"));
            m_TDec = std::stod(paramMap.at("TDec"));
            m_RScale = std::stod(paramMap.at("RScale"));
            m_RIncDec = std::stod(paramMap.at("RIncDec"));
            m_noiseRemovalThresholdFacBG = std::stod(paramMap.at("noiseRemovalThresholdFacBG"));
            m_noiseRemovalThresholdFacFG = std::stod(paramMap.at("noiseRemovalThresholdFacFG"));
            m_lsbpThreshold = std::stoi(paramMap.at("lsbpThreshold"));
            m_minCount = std::stoi(paramMap.at("minCount"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("motionComp", std::to_string(m_motionComp)));
            map.insert(std::make_pair("samples", std::to_string(m_samples)));
            map.insert(std::make_pair("lsbpRadius", std::to_string(m_lsbpRadius)));
            map.insert(std::make_pair("TLower", std::to_string(m_TLower)));
            map.insert(std::make_pair("TUpper", std::to_string(m_TUpper)));
            map.insert(std::make_pair("TInc", std::to_string(m_TInc)));
            map.insert(std::make_pair("TDec", std::to_string(m_TDec)));
            map.insert(std::make_pair("RScale", std::to_string(m_RScale)));
            map.insert(std::make_pair("RIncDec", std::to_string(m_RIncDec)));
            map.insert(std::make_pair("noiseRemovalThresholdFacBG", std::to_string(m_noiseRemovalThresholdFacBG)));
            map.insert(std::make_pair("noiseRemovalThresholdFacFG", std::to_string(m_noiseRemovalThresholdFacFG)));
            map.insert(std::make_pair("lsbpThreshold", std::to_string(m_lsbpThreshold)));
            map.insert(std::make_pair("minCount", std::to_string(m_minCount)));
            return map;
        }

    public:

        int     m_motionComp = cv::bgsegm::LSBP_CAMERA_MOTION_COMPENSATION_NONE;
        int     m_samples = 20;
        int     m_lsbpRadius = 16;
        float  	m_TLower = 2.0f;
        float  	m_TUpper = 32.0f;
        float  	m_TInc = 1.0f;
        float  	m_TDec = 0.05f;
        float  	m_RScale = 10.0f;
        float  	m_RIncDec = 0.005f;
        float  	m_noiseRemovalThresholdFacBG = 0.0004f;
        float  	m_noiseRemovalThresholdFacFG = 0.0008f;
        int  	m_lsbpThreshold = 8;
        int  	m_minCount = 2;
};

//-----------------------------//
//----- COcvBckgndSubLsbp -----//
//-----------------------------//
class COcvBckgndSubLsbp : public CVideoProcess
{
    public:

        COcvBckgndSubLsbp() : CVideoProcess()
        {
            setOutputDataType(IODataType::IMAGE_BINARY, 0);
            addOutput(std::make_shared<CImageProcessIO>());
            setOutputColorMap(1, 0, {{255,0,0}});
        }
        COcvBckgndSubLsbp(const std::string name, const std::shared_ptr<COcvBckgndSubLsbpParam>& pParam) : CVideoProcess(name)
        {
            setOutputDataType(IODataType::IMAGE_BINARY, 0);
            addOutput(std::make_shared<CImageProcessIO>());
            setOutputColorMap(1, 0, {{255,0,0}});
            m_pParam = std::make_shared<COcvBckgndSubLsbpParam>(*pParam);
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        void    notifyVideoStart(int frameCount) override
        {
            Q_UNUSED(frameCount);

            auto pParam = std::dynamic_pointer_cast<COcvBckgndSubLsbpParam>(m_pParam);
            if(pParam == nullptr)
                return;

            m_pBgSubtractor = cv::bgsegm::createBackgroundSubtractorLSBP(pParam->m_motionComp, pParam->m_samples, pParam->m_lsbpRadius,
                                                                         pParam->m_TLower, pParam->m_TUpper, pParam->m_TInc, pParam->m_TDec,
                                                                         pParam->m_RScale, pParam->m_RIncDec,
                                                                         pParam->m_noiseRemovalThresholdFacBG, pParam->m_noiseRemovalThresholdFacFG,
                                                                         pParam->m_lsbpThreshold, pParam->m_minCount);
        }

        void    run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pParam = std::dynamic_pointer_cast<COcvBckgndSubLsbpParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgDst;
            CMat imgSrc = pInput->getImage();
            emit m_signalHandler->doProgress();

            try
            {
                if(m_pBgSubtractor.empty())
                {
                    m_pBgSubtractor = cv::bgsegm::createBackgroundSubtractorLSBP(pParam->m_motionComp, pParam->m_samples, pParam->m_lsbpRadius,
                                                                                 pParam->m_TLower, pParam->m_TUpper, pParam->m_TInc, pParam->m_TDec,
                                                                                 pParam->m_RScale, pParam->m_RIncDec,
                                                                                 pParam->m_noiseRemovalThresholdFacBG, pParam->m_noiseRemovalThresholdFacFG,
                                                                                 pParam->m_lsbpThreshold, pParam->m_minCount);
                }
                m_pBgSubtractor->apply(imgSrc, imgDst);
            }
            catch(std::exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e.what(), __func__, __FILE__, __LINE__);
            }
            emit m_signalHandler->doProgress();

            auto pOutput = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(imgDst);

            applyInputGraphicsMask(1, 0, 0, MaskMode::MASK_ONLY);
            forwardInputImage(0, 1);
            endTaskRun();
            emit m_signalHandler->doProgress();
        }

    private:

        cv::Ptr<cv::bgsegm::BackgroundSubtractorLSBP>    m_pBgSubtractor;
};

//------------------------------------//
//----- COcvBckgndSubLsbpFactory -----//
//------------------------------------//
class COcvBckgndSubLsbpFactory : public CProcessFactory
{
    public:

        COcvBckgndSubLsbpFactory()
        {
            m_info.m_name = QObject::tr("LSBP background substractor").toStdString();
            m_info.m_description = QObject::tr("This process removes background on video sequence using Local SVD Binary Pattern.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Improved Background-Foreground Segmentation Methods").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "video,background,substractor,segmentation,binary,svd";
            m_info.m_authors = "L. Guo, D. Xu, and Z. Qiang";
            m_info.m_article = "Background subtraction using local svd binary pattern.";
            m_info.m_journal = "In 2016 IEEE Conference on Computer Vision and Pattern Recognition Workshops (CVPRW), pages 1159–1167.";
            m_info.m_year = 2016;
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/de/d4c/classcv_1_1bgsegm_1_1BackgroundSubtractorLSBP.html";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvBckgndSubLsbpParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvBckgndSubLsbp>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvBckgndSubLsbpParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvBckgndSubLsbp>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVBCKGNDSUBLSBP_HPP
