#ifndef COCVBCKGNDSUBMOG_HPP
#define COCVBCKGNDSUBMOG_HPP

#include "opencv2/bgsegm.hpp"
#include "Core/CVideoProcess.h"

//---------------------------------//
//----- COcvBckgndSubMogParam -----//
//---------------------------------//
class COcvBckgndSubMogParam: public CProtocolTaskParam
{
    public:

        COcvBckgndSubMogParam()
        {
        }

        void        setParamMap(const UMapString& paramMap) override
        {
            m_history = std::stoi(paramMap.at("history"));
            m_mixturesCount = std::stoi(paramMap.at("mixturesCount"));
            m_bckRatio = std::stod(paramMap.at("bckRatio"));
            m_noiseSigma = std::stod(paramMap.at("noiseSigma"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("history", std::to_string(m_history)));
            map.insert(std::make_pair("mixturesCount", std::to_string(m_mixturesCount)));
            map.insert(std::make_pair("bckRatio", std::to_string(m_bckRatio)));
            map.insert(std::make_pair("noiseSigma", std::to_string(m_noiseSigma)));
            return map;
        }

    public:

        int     m_history = 200;
        int  	m_mixturesCount = 5;
        double  m_bckRatio = 0.7;
        double  m_noiseSigma = 0;
};

//----------------------------//
//----- COcvBckgndSubMog -----//
//----------------------------//
class COcvBckgndSubMog : public CVideoProcess
{
    public:

        COcvBckgndSubMog() : CVideoProcess()
        {
            setOutputDataType(IODataType::IMAGE_BINARY, 0);
            addOutput(std::make_shared<CImageProcessIO>());
            setOutputColorMap(1, 0, {{255,0,0}});
        }
        COcvBckgndSubMog(const std::string name, const std::shared_ptr<COcvBckgndSubMogParam>& pParam) : CVideoProcess(name)
        {
            setOutputDataType(IODataType::IMAGE_BINARY, 0);
            addOutput(std::make_shared<CImageProcessIO>());
            setOutputColorMap(1, 0, {{255,0,0}});
            m_pParam = std::make_shared<COcvBckgndSubMogParam>(*pParam);
        }

        size_t     getProgressSteps() override
        {
            return 3;
        }

        void    notifyVideoStart(int frameCount) override
        {
            Q_UNUSED(frameCount);

            auto pParam = std::dynamic_pointer_cast<COcvBckgndSubMogParam>(m_pParam);
            if(pParam == nullptr)
                return;

            m_pBgSubtractor = cv::bgsegm::createBackgroundSubtractorMOG(pParam->m_history, pParam->m_mixturesCount, pParam->m_bckRatio, pParam->m_noiseSigma);
        }

        void    run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pParam = std::dynamic_pointer_cast<COcvBckgndSubMogParam>(m_pParam);

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
                    m_pBgSubtractor = cv::bgsegm::createBackgroundSubtractorMOG(pParam->m_history, pParam->m_mixturesCount, pParam->m_bckRatio, pParam->m_noiseSigma);

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

        cv::Ptr<cv::bgsegm::BackgroundSubtractorMOG>    m_pBgSubtractor;
};

//-----------------------------------//
//----- COcvBckgndSubMogFactory -----//
//-----------------------------------//
class COcvBckgndSubMogFactory : public CProcessFactory
{
    public:

        COcvBckgndSubMogFactory()
        {
            m_info.m_name = QObject::tr("MOG background substractor").toStdString();
            m_info.m_description = QObject::tr("This process removes background on video sequence based on mixture of gaussian.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Improved Background-Foreground Segmentation Methods").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "video,background,substractor,segmentation,binary,mixture,gaussian";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d6/da7/classcv_1_1bgsegm_1_1BackgroundSubtractorMOG.html";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvBckgndSubMogParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvBckgndSubMog>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvBckgndSubMogParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvBckgndSubMog>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVBCKGNDSUBMOG_HPP
