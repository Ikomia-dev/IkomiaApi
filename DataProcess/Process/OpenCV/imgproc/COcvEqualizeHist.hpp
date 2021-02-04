#ifndef COCVEQUALIZEHIST_HPP
#define COCVEQUALIZEHIST_HPP

#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"

//------------------------------//
//----- COcvEqualizeHistParam -----//
//------------------------------//
class COcvEqualizeHistParam: public CProtocolTaskParam
{
    public:

        COcvEqualizeHistParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            Q_UNUSED(paramMap);
        }

        UMapString  getParamMap() const override
        {
            return UMapString();
        }
};

//-------------------------//
//----- COcvEqualizeHist -----//
//-------------------------//
class COcvEqualizeHist : public CImageProcess2d
{
    public:

        COcvEqualizeHist() : CImageProcess2d()
        {
        }
        COcvEqualizeHist(const std::string name, const std::shared_ptr<COcvEqualizeHistParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<COcvEqualizeHistParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvEqualizeHistParam>(m_pParam);

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
                if(imgSrc.channels() == 1)
                    cv::equalizeHist(imgSrc, imgDst);
                else
                {
                    cv::cvtColor(imgSrc, imgDst, cv::COLOR_RGB2Lab);
                    CMat channels[3];
                    cv::split(imgDst, channels);
                    cv::equalizeHist(channels[0], channels[0]);
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

class COcvEqualizeHistFactory : public CProcessFactory
{
    public:

        COcvEqualizeHistFactory()
        {
            m_info.m_name = QObject::tr("Equalize histogram").toStdString();
            m_info.m_description = QObject::tr("This process normalizes the brightness and increases the contrast of the image.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Image processing/Histograms").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "brightness,contrast,histogram,normalization,equalization";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d6/dc7/group__imgproc__hist.html#ga7e54091f0c937d49bf84152a16f76d6e";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pEqualizeHistParam = std::dynamic_pointer_cast<COcvEqualizeHistParam>(pParam);
            if(pEqualizeHistParam != nullptr)
                return std::make_shared<COcvEqualizeHist>(m_info.m_name, pEqualizeHistParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pEqualizeHistParam = std::make_shared<COcvEqualizeHistParam>();
            assert(pEqualizeHistParam != nullptr);
            return std::make_shared<COcvEqualizeHist>(m_info.m_name, pEqualizeHistParam);
        }
};

#endif // COCVEQUALIZEHIST_HPP
