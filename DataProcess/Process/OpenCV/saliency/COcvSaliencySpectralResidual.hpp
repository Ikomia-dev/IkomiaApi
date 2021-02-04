#ifndef COCVSALIENCYSPECTRALRESIDUAL_HPP
#define COCVSALIENCYSPECTRALRESIDUAL_HPP

#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"
#include <opencv2/saliency.hpp>

//------------------------------//
//----- COcvSaliencySpectralResidualParam -----//
//------------------------------//
class COcvSaliencySpectralResidualParam: public CProtocolTaskParam
{
    public:

        COcvSaliencySpectralResidualParam() : CProtocolTaskParam(){}

        void setParamMap(const UMapString& paramMap) override
        {
            Q_UNUSED(paramMap)
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            return map;
        }
};

//-------------------------//
//----- COcvSaliencySpectralResidual -----//
//-------------------------//
class COcvSaliencySpectralResidual : public CImageProcess2d
{
    public:

        COcvSaliencySpectralResidual() : CImageProcess2d()
        {
            addOutput(std::make_shared<CImageProcessIO>(IODataType::IMAGE_BINARY));
        }
        COcvSaliencySpectralResidual(const std::string name, const std::shared_ptr<COcvSaliencySpectralResidualParam>& pParam) : CImageProcess2d(name)
        {
            addOutput(std::make_shared<CImageProcessIO>(IODataType::IMAGE_BINARY));
            m_pParam = std::make_shared<COcvSaliencySpectralResidualParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvSaliencySpectralResidualParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat saliencyMap, binaryMap;
            CMat imgSrc = pInput->getImage();
            createGraphicsMask(imgSrc.getNbCols(), imgSrc.getNbRows(), pGraphicsInput);
            emit m_signalHandler->doProgress();

            try
            {
                if(m_pSaliency.get() == nullptr)
                    m_pSaliency = cv::saliency::StaticSaliencySpectralResidual::create();

                if(m_pSaliency->computeSaliency(imgSrc, saliencyMap))
                {
                    if(!m_pSaliency->computeBinaryMap( saliencyMap, binaryMap ))
                        throw CException(CoreExCode::INVALID_PARAMETER, "Compute binary map has failed", __func__, __FILE__, __LINE__);
                }
                else
                {
                    throw CException(CoreExCode::INVALID_PARAMETER, "Compute saliency has failed", __func__, __FILE__, __LINE__);
                }

            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }


            applyGraphicsMask(imgSrc, saliencyMap, 0);
            applyGraphicsMask(imgSrc, binaryMap, 0);
            emit m_signalHandler->doProgress();

            auto pOutput1 = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(0));
            if(pOutput1)
                pOutput1->setImage(saliencyMap);

            auto pOutput2 = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(1));
            if(pOutput2)
                pOutput2->setImage(binaryMap);

            endTaskRun();
            emit m_signalHandler->doProgress();
        }

    private:
        cv::Ptr<cv::saliency::StaticSaliencySpectralResidual> m_pSaliency;
};

class COcvSaliencySpectralResidualFactory : public CProcessFactory
{
    public:

        COcvSaliencySpectralResidualFactory()
        {
            m_info.m_name = QObject::tr("StaticSaliencySpectralResidual").toStdString();
            m_info.m_description = QObject::tr("Starting from the principle of natural image statistics, this method simulate the behavior of pre-attentive visual search. The algorithm analyze the log spectrum of each image and obtain the spectral residual. Then transform the spectral residual to spatial domain to obtain the saliency map, which suggests the positions of proto-objects.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Saliency API").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Saliency,Features,Detection";
            m_info.m_authors = "Xiaodi Hou and Liqing Zhang";
            m_info.m_article = "Saliency detection: A spectral residual approach";
            m_info.m_journal = "CVPR, pages 1–8, IEEE";
            m_info.m_year = 2007;
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/df/d37/classcv_1_1saliency_1_1StaticSaliencySpectralResidual.html";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pSaliencySpectralResidualParam = std::dynamic_pointer_cast<COcvSaliencySpectralResidualParam>(pParam);
            if(pSaliencySpectralResidualParam != nullptr)
                return std::make_shared<COcvSaliencySpectralResidual>(m_info.m_name, pSaliencySpectralResidualParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pSaliencySpectralResidualParam = std::make_shared<COcvSaliencySpectralResidualParam>();
            assert(pSaliencySpectralResidualParam != nullptr);
            return std::make_shared<COcvSaliencySpectralResidual>(m_info.m_name, pSaliencySpectralResidualParam);
        }
};

#endif // COCVSALIENCYSPECTRALRESIDUAL_HPP
