#ifndef COCVTHRESHOLD_HPP
#define COCVTHRESHOLD_HPP

#include "DataProcessTools.hpp"
#include "Main/CoreTools.hpp"
#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"

//------------------------------//
//----- COcvThresholdParam -----//
//------------------------------//
class COcvThresholdParam: public CProtocolTaskParam
{
    public:

        COcvThresholdParam() : CProtocolTaskParam()
        {
        }

        void        setParamMap(const UMapString& paramMap) override
        {
            m_thresholdType = std::stoi(paramMap.at("thresholdType"));
            m_threshold = std::stod(paramMap.at("threshold"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("thresholdType", std::to_string(m_thresholdType)));
            map.insert(std::make_pair("threshold", std::to_string(m_threshold)));
            return map;
        }

    public:

        int     m_thresholdType = cv::THRESH_BINARY;
        double  m_threshold = 0.0;
};

//------------------------------//
//----- COcvThresholdParam -----//
//------------------------------//
class COcvThreshold : public CImageProcess2d
{
    public:

        COcvThreshold() : CImageProcess2d()
        {
            addOutput(std::make_shared<CImageProcessIO>());
            getOutput(0)->setDataType(IODataType::IMAGE_BINARY);
            setOutputColorMap(1, 0, {{255,0,0}});
        }
        COcvThreshold(const std::string name, const std::shared_ptr<COcvThresholdParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<COcvThresholdParam>(*pParam);
            addOutput(std::make_shared<CImageProcessIO>());
            getOutput(0)->setDataType(IODataType::IMAGE_BINARY);
            setOutputColorMap(1, 0, {{255,0,0}});
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        void    updateStaticOutputs() override
        {
            CImageProcess2d::updateStaticOutputs();
            auto pImgOutput =  std::dynamic_pointer_cast<CImageProcessIO>(getOutput(1));
            assert(pImgOutput);
            pImgOutput->setChannelCount(1);
        }

        void    run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsProcessInput>(getInput(1));
            auto pParam = std::dynamic_pointer_cast<COcvThresholdParam>(m_pParam);

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
                imgSrcTmp = conformInput(imgSrc);
                cv::threshold(imgSrcTmp, imgDst, pParam->m_threshold, Utils::Image::getMaxValue(imgSrc), pParam->m_thresholdType);
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

            emit m_signalHandler->doProgress();
            endTaskRun();
        }

        CMat    conformInput(CMat source)
        {
            CMat conformImg;
            auto pParam = std::dynamic_pointer_cast<COcvThresholdParam>(m_pParam);

            if(pParam->m_thresholdType & cv::THRESH_OTSU || pParam->m_thresholdType & cv::THRESH_TRIANGLE)
            {
                //Image must be 8 bits monochrome
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
            }
            else
                conformImg = source;

            return conformImg;
        }
};

class COcvThresholdFactory : public CProcessFactory
{
    public:

        COcvThresholdFactory()
        {
            m_info.m_name = QObject::tr("Threshold").toStdString();
            m_info.m_description = QObject::tr("The function applies fixed-level thresholding").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Image processing/Miscellaneous image transformations").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "segmentation,threshold,otsu,triangle,binary";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d7/d1b/group__imgproc__misc.html#gae8a4a146d1ca78c626a53577199e9c57";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvThresholdParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvThreshold>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvThresholdParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvThreshold>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVTHRESHOLD_HPP
