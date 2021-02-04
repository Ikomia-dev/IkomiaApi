#ifndef COCVADAPTIVETHRESHOLD_HPP
#define COCVADAPTIVETHRESHOLD_HPP

#include "DataProcessTools.hpp"

#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"

//--------------------------------------//
//----- COcvAdaptiveThresholdParam -----//
//--------------------------------------//
class COcvAdaptiveThresholdParam: public CProtocolTaskParam
{
    public:

        COcvAdaptiveThresholdParam() : CProtocolTaskParam()
        {
        }

        void        setParamMap(const UMapString& paramMap) override
        {
            m_adaptiveMethod = std::stoi(paramMap.at("adaptiveMethod"));
            m_thresholdType = std::stoi(paramMap.at("thresholdType"));
            m_blockSize = std::stoi(paramMap.at("blockSize"));
            m_offset = std::stod(paramMap.at("offset"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("adaptiveMethod", std::to_string(m_adaptiveMethod)));
            map.insert(std::make_pair("thresholdType", std::to_string(m_thresholdType)));
            map.insert(std::make_pair("blockSize", std::to_string(m_blockSize)));
            map.insert(std::make_pair("offset", std::to_string(m_offset)));
            return map;
        }

    public:

        int     m_adaptiveMethod = cv::ADAPTIVE_THRESH_MEAN_C;
        int     m_thresholdType = cv::THRESH_BINARY;
        int     m_blockSize = 33;
        double  m_offset = 10.0;
};

//--------------------------------------//
//----- COcvAdaptiveThresholdParam -----//
//--------------------------------------//
class COcvAdaptiveThreshold : public CImageProcess2d
{
    public:

        COcvAdaptiveThreshold() : CImageProcess2d()
        {
            getOutput(0)->setDataType(IODataType::IMAGE_BINARY);
            addOutput(std::make_shared<CImageProcessIO>());
            setOutputColorMap(1, 0, {{255,0,0}});
        }
        COcvAdaptiveThreshold(const std::string name, const std::shared_ptr<COcvAdaptiveThresholdParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<COcvAdaptiveThresholdParam>(*pParam);
            getOutput(0)->setDataType(IODataType::IMAGE_BINARY);
            addOutput(std::make_shared<CImageProcessIO>());
            setOutputColorMap(1, 0, {{255,0,0}});
        }

        size_t     getProgressSteps() override
        {
            return 3;
        }

        void    updateStaticOutputs() override
        {
            CImageProcess2d::updateStaticOutputs();
            auto pImgOutput =  std::dynamic_pointer_cast<CImageProcessIO>(getOutput(0));
            assert(pImgOutput);
            pImgOutput->setChannelCount(1);
        }

        void    run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsProcessInput>(getInput(1));
            auto pParam = std::dynamic_pointer_cast<COcvAdaptiveThresholdParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgSrcTmp, imgDst;
            CMat imgSrc = pInput->getImage();
            createGraphicsMask(imgSrc.getNbCols(), imgSrc.getNbRows(), pGraphicsInput);
            emit m_signalHandler->doProgress();

            try
            {
                //Require 8 bits monochrome source image
                imgSrcTmp = conformInput(imgSrc);
                cv::adaptiveThreshold(imgSrcTmp, imgDst, 255, pParam->m_adaptiveMethod, pParam->m_thresholdType, pParam->m_blockSize, pParam->m_offset);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            forwardInputImage(0, 1);
            applyGraphicsMaskToBinary(imgDst, imgDst, 0);
            emit m_signalHandler->doProgress();

            auto pOutput = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(imgDst);

            endTaskRun();
            emit m_signalHandler->doProgress();
        }

        CMat    conformInput(CMat source)
        {
            CMat conformImg;

            if(source.depth() != CV_8S && source.depth() != CV_8U)
            {
                source.convertTo(conformImg, CV_8U);
                if(source.channels() != 1)
                    cv::cvtColor(source, conformImg, cv::COLOR_RGB2GRAY, 0);
            }
            else if(source.channels() != 1)
                cv::cvtColor(source, conformImg, cv::COLOR_RGB2GRAY, 0);
            else
                conformImg = source;

            return conformImg;
        }
};

class COcvAdaptiveThresholdFactory : public CProcessFactory
{
    public:

        COcvAdaptiveThresholdFactory()
        {
            m_info.m_name = QObject::tr("Adaptive Threshold").toStdString();
            m_info.m_description = QObject::tr("Adaptive thresholding according to local background estimation").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Image processing/Miscellaneous image transformations").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "segmentation,adaptive,threshold,mean,gaussian";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d7/d1b/group__imgproc__misc.html#ga72b913f352e4a1b1b397736707afcde3";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvAdaptiveThresholdParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvAdaptiveThreshold>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvAdaptiveThresholdParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvAdaptiveThreshold>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVADAPTIVETHRESHOLD_HPP
