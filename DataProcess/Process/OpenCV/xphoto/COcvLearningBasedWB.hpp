#ifndef COCVLEARNINGBASEDWB_HPP
#define COCVLEARNINGBASEDWB_HPP

#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"
#include <opencv2/xphoto.hpp>

//------------------------------//
//----- COcvLearningBasedWBParam -----//
//------------------------------//
class COcvLearningBasedWBParam: public CProtocolTaskParam
{
    public:

        COcvLearningBasedWBParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_histBinNum = std::stoi(paramMap.at("histBinNum"));
            m_rangeMaxVal = std::stoi(paramMap.at("rangeMaxVal"));
            m_satThreshold = std::stof(paramMap.at("satThreshold"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("histBinNum", std::to_string(m_histBinNum)));
            map.insert(std::make_pair("rangeMaxVal", std::to_string(m_rangeMaxVal)));
            map.insert(std::make_pair("satThreshold", std::to_string(m_satThreshold)));

            return map;
        }

    public:
        int     m_histBinNum = 64;
        int     m_rangeMaxVal = 255;
        float   m_satThreshold = 0.98f;
};

//-------------------------//
//----- COcvLearningBasedWB -----//
//-------------------------//
class COcvLearningBasedWB : public CImageProcess2d
{
    public:

        COcvLearningBasedWB() : CImageProcess2d()
        {
        }
        COcvLearningBasedWB(const std::string name, const std::shared_ptr<COcvLearningBasedWBParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<COcvLearningBasedWBParam>(*pParam);
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        void run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsProcessInput>(getInput(1));
            auto pParam = std::dynamic_pointer_cast<COcvLearningBasedWBParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgDst;
            CMat imgSrc = pInput->getImage();
            createGraphicsMask(imgSrc.getNbCols(), imgSrc.getNbRows(), pGraphicsInput);
            emit m_signalHandler->doProgress();

            try
            {
                auto pLWB = cv::xphoto::createLearningBasedWB();
                pLWB->setHistBinNum(pParam->m_histBinNum);
                pLWB->setRangeMaxVal(pParam->m_rangeMaxVal);
                pLWB->setSaturationThreshold(pParam->m_satThreshold);
                pLWB->balanceWhite(imgSrc, imgDst);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            endTaskRun();
            applyGraphicsMask(imgSrc, imgDst, 0);
            emit m_signalHandler->doProgress();

            auto pOutput = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(imgDst);

            emit m_signalHandler->doProgress();
        }
};

class COcvLearningBasedWBFactory : public CProcessFactory
{
    public:

        COcvLearningBasedWBFactory()
        {
            m_info.m_name = QObject::tr("LearningBasedWB").toStdString();
            m_info.m_description = QObject::tr("More sophisticated learning-based automatic white balance algorithm.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Additional photo processing algorithms").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "White,Balance,AutoWhiteBalance";
            m_info.m_authors = "Dongliang Cheng, Brian Price, Scott Cohen, and Michael S Brown";
            m_info.m_article = "Effective learning-based illuminant estimation using simple features";
            m_info.m_journal = "IEEE CVPR, pages 1000–1008";
            m_info.m_year = 2015;
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d4/d3b/classcv_1_1xphoto_1_1LearningBasedWB.html";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvLearningBasedWBParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvLearningBasedWB>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvLearningBasedWBParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvLearningBasedWB>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVLEARNINGBASEDWB_HPP
