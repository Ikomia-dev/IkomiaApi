#ifndef COCVOCRTESSERACT_HPP
#define COCVOCRTESSERACT_HPP

#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"
#include <opencv2/text.hpp>

//------------------------------//
//----- COcvOCRTesseractParam -----//
//------------------------------//
class COcvOCRTesseractParam: public CProtocolTaskParam
{
    public:

        COcvOCRTesseractParam() : CProtocolTaskParam(){}

        void setParamMap(const UMapString& paramMap) override
        {
            /*m_datapath = paramMap.at("datapath").c_str();
            m_language = paramMap.at("language").c_str();
            m_char_whitelist = paramMap.at("char_whitelist").c_str();*/
            m_oem = std::stoi(paramMap.at("oem"));
            m_psmode = std::stoi(paramMap.at("psmode"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            /*map.insert(std::make_pair("datapath", std::to_string(m_datapath)));
            map.insert(std::make_pair("language", std::to_string(m_language)));
            map.insert(std::make_pair("char_whitelist", std::to_string(m_char_whitelist)));*/
            map.insert(std::make_pair("oem", std::to_string(m_oem)));
            map.insert(std::make_pair("psmode", std::to_string(m_psmode)));
            return map;
        }

    public:
        const char*  	m_datapath = nullptr;
        const char*  	m_language = nullptr;
        const char*  	m_char_whitelist = nullptr;
        int             m_oem = cv::text::OEM_DEFAULT;
        int             m_psmode = cv::text::PSM_AUTO;

};

//-------------------------//
//----- COcvOCRTesseract -----//
//-------------------------//
class COcvOCRTesseract : public CImageProcess2d
{
    public:

        COcvOCRTesseract() : CImageProcess2d()
        {
            addOutput(std::make_shared<CGraphicsProcessOutput>());
        }
        COcvOCRTesseract(const std::string name, const std::shared_ptr<COcvOCRTesseractParam>& pParam) : CImageProcess2d(name)
        {
            addOutput(std::make_shared<CGraphicsProcessOutput>());
            m_pParam = std::make_shared<COcvOCRTesseractParam>(*pParam);
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        void    makeRecognition(CMat& imgSrc)
        {
            auto pGraphicsOutput = std::dynamic_pointer_cast<CGraphicsProcessOutput>(getOutput(getOutputCount() - 1));
            if(pGraphicsOutput == nullptr)
                throw CException(CoreExCode::NULL_POINTER, "Invalid graphics output", __func__, __FILE__, __LINE__);

            pGraphicsOutput->setNewLayer(getName());
            pGraphicsOutput->setImageIndex(0);

            auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsProcessInput>(getInput(1));
            if(pGraphicsInput == nullptr)
                return;

            int compLevel = 0;
            auto items = pGraphicsInput->getItems();

            // Load graphics properties
            GraphicsRectProperty rectProp = m_graphicsContextPtr->getRectProperty();
            GraphicsTextProperty textProp = m_graphicsContextPtr->getTextProperty();

            // Custom properties
            rectProp.m_lineSize = 4;
            rectProp.m_penColor = {0,255,0,255};
            int ftSize = std::max(10.0, 0.03*imgSrc.rows);
            textProp.m_fontSize = ftSize;
            textProp.m_color = {0,100,200,255};

            // Loop over bounding boxes
            for(auto&& it : items)
            {
                auto rect = it->getBoundingRect();
                int x = rect.x();
                int y = rect.y();
                int w = rect.width();
                int h = rect.height();

                // Check if whole bb is inside image domain
                if(x >= 0 && y >= 0 && x+w < imgSrc.cols && y+h < imgSrc.rows)
                {
                    std::string outputText;
                    std::vector<cv::Rect> compRects;
                    std::vector<std::string> compTexts;
                    std::vector<float> compConfidences;
                    cv::Rect roi(x, y, w, h);
                    CMat crop = imgSrc(roi);
                    m_pTesseract->run(crop, outputText, &compRects, &compTexts, &compConfidences, compLevel);
                    pGraphicsOutput->addRectangle(roi.x, roi.y, roi.width, roi.height, rectProp);
                    pGraphicsOutput->addText(outputText, rect.right()+10, rect.top()+10);
                }
            }
        }

        void    run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pParam = std::dynamic_pointer_cast<COcvOCRTesseractParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgSrc = pInput->getImage();

            emit m_signalHandler->doProgress();

            try
            {
                m_pTesseract = cv::text::OCRTesseract::create(pParam->m_datapath, pParam->m_language, pParam->m_char_whitelist, pParam->m_oem, pParam->m_psmode);
                makeRecognition(imgSrc);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            endTaskRun();
            emit m_signalHandler->doProgress();
            forwardInputImage();
            emit m_signalHandler->doProgress();
        }

    private:
        cv::Ptr<cv::text::OCRTesseract> m_pTesseract;
};

class COcvOCRTesseractFactory : public CProcessFactory
{
    public:

        COcvOCRTesseractFactory()
        {
            m_info.m_name = QObject::tr("OCRTesseract").toStdString();
            m_info.m_description = QObject::tr("Text recognition.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Scene text detection and recognition/Scene text recognition").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Text,Recognition";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d7/ddc/classcv_1_1text_1_1OCRTesseract.html";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pOCRTesseractParam = std::dynamic_pointer_cast<COcvOCRTesseractParam>(pParam);
            if(pOCRTesseractParam != nullptr)
                return std::make_shared<COcvOCRTesseract>(m_info.m_name, pOCRTesseractParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pOCRTesseractParam = std::make_shared<COcvOCRTesseractParam>();
            assert(pOCRTesseractParam != nullptr);
            return std::make_shared<COcvOCRTesseract>(m_info.m_name, pOCRTesseractParam);
        }
};

#endif // COCVOCRTESSERACT_HPP
