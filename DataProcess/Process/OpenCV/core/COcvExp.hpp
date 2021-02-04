#ifndef COCVEXP_HPP
#define COCVEXP_HPP


#include "Core/CImageProcess2d.h"

//-------------------//
//----- COcvExp -----//
//-------------------//
class COcvExp : public CImageProcess2d
{
    public:

        COcvExp() : CImageProcess2d()
        {
        }
        COcvExp(const std::string name, const std::shared_ptr<CProtocolTaskParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<CProtocolTaskParam>(*pParam);
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
            if(pInput == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty images", __func__, __FILE__, __LINE__);

            emit m_signalHandler->doProgress();

            CMat imgDst;
            CMat imgSrc = pInput->getImage();

            try
            {
                CMat imgTmp;
                if(imgSrc.depth() != CV_32F || imgSrc.depth() != CV_64F)
                    imgSrc.convertTo(imgTmp, CV_32F);
                else
                    imgTmp = imgSrc;

                cv::exp(imgTmp, imgDst);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            emit m_signalHandler->doProgress();
            auto pOutput = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(imgDst);

            endTaskRun();
            applyInputGraphicsMask(1, 0, 0, MaskMode::MERGE_SOURCE);
            emit m_signalHandler->doProgress();
        }
};

class COcvExpFactory : public CProcessFactory
{
    public:

        COcvExpFactory()
        {
            m_info.m_name = QObject::tr("Exponent").toStdString();
            m_info.m_description = QObject::tr("Calculates the exponent of every element of the input image").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Core functionality/Operations on arrays").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "exp, exponent, exponential";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d2/de8/group__core__array.html#ga3e10108e2162c338f1b848af619f39e5";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            if(pParam != nullptr)
                return std::make_shared<COcvExp>(m_info.m_name, pParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pParam = std::make_shared<CProtocolTaskParam>();
            assert(pParam != nullptr);
            return std::make_shared<COcvExp>(m_info.m_name, pParam);
        }
};

#endif // COCVEXP_HPP
