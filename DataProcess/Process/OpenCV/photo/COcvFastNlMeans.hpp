#ifndef COCVNLMEANS_HPP
#define COCVNLMEANS_HPP

#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"
#include "opencv2/photo/cuda.hpp"
#include "UtilsTools.hpp"

//----------------------------//
//----- COcvNlMeansParam -----//
//----------------------------//
class COcvFastNlMeansParam: public CProtocolTaskParam
{
    public:

        COcvFastNlMeansParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_h = std::stof(paramMap.at("h"));
            m_blockSize = std::stoi(paramMap.at("blockSize"));
            m_searchSize = std::stoi(paramMap.at("searchSize"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("h", std::to_string(m_h)));
            map.insert(std::make_pair("blockSize", std::to_string(m_blockSize)));
            map.insert(std::make_pair("searchSize", std::to_string(m_searchSize)));
            return map;
        }

    public:

        float   m_h = 3.0;
        int     m_blockSize = 7;
        int     m_searchSize = 21;
};

//-----------------------//
//----- COcvNlMeans -----//
//-----------------------//
class COcvFastNlMeans : public CImageProcess2d
{
    public:

        COcvFastNlMeans() : CImageProcess2d()
        {
        }
        COcvFastNlMeans(const std::string name, const std::shared_ptr<COcvFastNlMeansParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<COcvFastNlMeansParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvFastNlMeansParam>(m_pParam);

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
                bool bCuda = Utils::Gpu::isCudaAvailable();
                bCuda = false;
                if(bCuda == true)
                {
                    cv::cuda::GpuMat cuImgSrc, cuImgDst;
                    cuImgSrc.upload(imgSrc);

                    if(pInput->m_channelCount == 1)
                        cv::cuda::fastNlMeansDenoising(cuImgSrc, cuImgDst, pParam->m_h, pParam->m_blockSize, pParam->m_searchSize);
                    else
                        cv::cuda::fastNlMeansDenoisingColored(cuImgSrc, cuImgDst, pParam->m_h, pParam->m_blockSize, pParam->m_searchSize);

                    cuImgDst.download(imgDst);
                }
                else
                {
                    if(pInput->m_channelCount == 1)
                        cv::fastNlMeansDenoising(imgSrc, imgDst, pParam->m_h, pParam->m_blockSize, pParam->m_searchSize);
                    else
                        cv::fastNlMeansDenoisingColored(imgSrc, imgDst, pParam->m_h, pParam->m_blockSize, pParam->m_searchSize);
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

class COcvFastNlMeansFactory : public CProcessFactory
{
    public:

        COcvFastNlMeansFactory()
        {
            m_info.m_name = QObject::tr("Non Local Means Filter").toStdString();
            m_info.m_description = QObject::tr("This process smoothes your image with an edge-preserving filter (blur homogeneous area and keep borders intact).").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Computational photography/Denoising").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "smooth,blur,anisotropic,filter,Gaussian,edge preserving,fast";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d1/d79/group__photo__denoise.html#ga4c6b0031f56ea3f98f768881279ffe93";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvFastNlMeansParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvFastNlMeans>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvFastNlMeansParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvFastNlMeans>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVNLMEANS_HPP
