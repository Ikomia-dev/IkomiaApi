#ifndef COCVSIMPLEWB_HPP
#define COCVSIMPLEWB_HPP

#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"
#include <opencv2/xphoto.hpp>

//------------------------------//
//----- COcvSimpleWBParam -----//
//------------------------------//
class COcvSimpleWBParam: public CProtocolTaskParam
{
    public:

        COcvSimpleWBParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_inputMin = std::stof(paramMap.at("inputMin"));
            m_inputMax = std::stof(paramMap.at("inputMax"));
            m_outputMin = std::stof(paramMap.at("outputMin"));
            m_outputMax = std::stof(paramMap.at("outputMax"));
            m_P = std::stof(paramMap.at("P"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("inputMin", std::to_string(m_inputMin)));
            map.insert(std::make_pair("inputMax", std::to_string(m_inputMax)));
            map.insert(std::make_pair("outputMin", std::to_string(m_outputMin)));
            map.insert(std::make_pair("outputMax", std::to_string(m_outputMax)));
            map.insert(std::make_pair("P", std::to_string(m_P)));

            return map;
        }

    public:
        float   m_inputMin = 0.0f;
        float   m_inputMax = 255.0f;
        float   m_outputMin = 0.0f;
        float   m_outputMax = 255.0f;
        float   m_P = 2.0f;
};

//-------------------------//
//----- COcvSimpleWB -----//
//-------------------------//
class COcvSimpleWB : public CImageProcess2d
{
    public:

        COcvSimpleWB() : CImageProcess2d()
        {
        }
        COcvSimpleWB(const std::string name, const std::shared_ptr<COcvSimpleWBParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<COcvSimpleWBParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvSimpleWBParam>(m_pParam);

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
                auto pSWB = cv::xphoto::createSimpleWB();
                pSWB->setInputMin(pParam->m_inputMin);
                pSWB->setInputMax(pParam->m_inputMax);
                pSWB->setOutputMin(pParam->m_outputMin);
                pSWB->setOutputMax(pParam->m_outputMax);
                pSWB->setP(pParam->m_P);
                pSWB->balanceWhite(imgSrc, imgDst);
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

class COcvSimpleWBFactory : public CProcessFactory
{
    public:

        COcvSimpleWBFactory()
        {
            m_info.m_name = QObject::tr("SimpleWB").toStdString();
            m_info.m_description = QObject::tr("This algorithm scales the values of pixels based on a gray-world assumption which states that the average of all channels should result in a gray image.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Additional photo processing algorithms").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "White,Balance,AutoWhiteBalance";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d1/d8b/classcv_1_1xphoto_1_1SimpleWB.html";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvSimpleWBParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvSimpleWB>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvSimpleWBParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvSimpleWB>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVSIMPLEWB_HPP
