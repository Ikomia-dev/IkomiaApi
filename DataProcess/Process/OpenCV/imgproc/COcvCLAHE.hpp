#ifndef COCVCLAHE_HPP
#define COCVCLAHE_HPP

#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"

//--------------------------//
//----- COcvCLAHEParam -----//
//--------------------------//
class COcvCLAHEParam: public CProtocolTaskParam
{
    public:

        COcvCLAHEParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_clipLimit = std::stod(paramMap.at("clipLimit"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("clipLimit", std::to_string(m_clipLimit)));
            return map;
        }

    public:

        double  m_clipLimit = 4.0;
};

//-----------------------------------------------------------------------//
//----- COcvCLAHE: Contrast Limited Adaptive Histogram Equalization -----//
//-----------------------------------------------------------------------//
class COcvCLAHE : public CImageProcess2d
{
    public:

        COcvCLAHE() : CImageProcess2d()
        {
        }
        COcvCLAHE(const std::string name, const std::shared_ptr<COcvCLAHEParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<COcvCLAHEParam>(*pParam);
        }

        size_t getProgressSteps() override
        {
            return 3;
        }

        void run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsProcessInput>(getInput(1));
            auto pParam = std::dynamic_pointer_cast<COcvCLAHEParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgDst;
            CMat imgSrc = pInput->getImage();

            if(imgSrc.depth() != CV_8S || imgSrc.depth() != CV_8U)
                imgSrc.convertTo(imgSrc, CV_8U);

            createGraphicsMask(imgSrc.getNbCols(), imgSrc.getNbRows(), pGraphicsInput);
            emit m_signalHandler->doProgress();

            try
            {
                cv::Ptr<cv::CLAHE> clahePtr = cv::createCLAHE();
                clahePtr->setClipLimit(pParam->m_clipLimit);

                if(imgSrc.channels() == 1)
                    clahePtr->apply(imgSrc, imgDst);
                else
                {
                    cv::cvtColor(imgSrc, imgDst, cv::COLOR_RGB2Lab);
                    CMat channels[3];
                    cv::split(imgDst, channels);
                    clahePtr->apply(channels[0], channels[0]);
                    cv::merge(channels, 3, imgDst);
                    cv::cvtColor(imgDst, imgDst, cv::COLOR_Lab2RGB);
                }
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

class COcvCLAHEFactory : public CProcessFactory
{
    public:

        COcvCLAHEFactory()
        {
            m_info.m_name = QObject::tr("CLAHE").toStdString();
            m_info.m_description = QObject::tr("This process equalizes the histogram of image using Contrast Limited Adaptive Histogram Equalization.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Image processing/Histograms").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "brightness,contrast,histogram,normalization,equalization,adaptive";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d6/db6/classcv_1_1CLAHE.html";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pCLAHEParam = std::dynamic_pointer_cast<COcvCLAHEParam>(pParam);
            if(pCLAHEParam != nullptr)
                return std::make_shared<COcvCLAHE>(m_info.m_name, pCLAHEParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pCLAHEParam = std::make_shared<COcvCLAHEParam>();
            assert(pCLAHEParam != nullptr);
            return std::make_shared<COcvCLAHE>(m_info.m_name, pCLAHEParam);
        }
};

#endif // COCVCLAHE_HPP
