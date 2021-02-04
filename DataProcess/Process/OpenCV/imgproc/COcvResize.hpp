#ifndef COCVRESIZE_HPP
#define COCVRESIZE_HPP

#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"
#include "opencv2/cudawarping.hpp"

//---------------------------//
//----- COcvResizeParam -----//
//---------------------------//
class COcvResizeParam: public CProtocolTaskParam
{
    public:

        COcvResizeParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_bPixelUnit = std::stoi(paramMap.at("isInPixel"));
            m_newWidth = std::stoi(paramMap.at("newWidth"));
            m_newHeight = std::stoi(paramMap.at("newHeight"));
            m_interpolation = std::stoi(paramMap.at("interpolation"));
            m_fx = std::stod(paramMap.at("fx"));
            m_fy = std::stod(paramMap.at("fy"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("isInPixel", std::to_string(m_bPixelUnit)));
            map.insert(std::make_pair("newWidth", std::to_string(m_newWidth)));
            map.insert(std::make_pair("newHeight", std::to_string(m_newHeight)));
            map.insert(std::make_pair("interpolation", std::to_string(m_interpolation)));
            map.insert(std::make_pair("fx", std::to_string(m_fx)));
            map.insert(std::make_pair("fy", std::to_string(m_fy)));
            return map;
        }

    public:

        bool    m_bPixelUnit = true;
        int     m_newWidth = 0;
        int     m_newHeight = 0;
        int     m_interpolation = cv::INTER_LINEAR;
        double  m_fx = 0.0;
        double  m_fy = 0.0;
};

//----------------------//
//----- COcvResize -----//
//----------------------//
class COcvResize : public CImageProcess2d
{
    public:

        COcvResize() : CImageProcess2d(false)
        {
        }
        COcvResize(const std::string name, const std::shared_ptr<COcvResizeParam>& pParam) : CImageProcess2d(name, false)
        {
            m_pParam = std::make_shared<COcvResizeParam>(*pParam);
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        void run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pParam = std::dynamic_pointer_cast<COcvResizeParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgDst;
            emit m_signalHandler->doProgress();

            try
            {

                bool bCuda = Utils::Gpu::isCudaAvailable();
                if(bCuda == true)
                {
                    cv::cuda::GpuMat cuImgSrc, cuImgDst;
                    cuImgSrc.upload(pInput->getImage());
                    cv::cuda::resize(cuImgSrc, cuImgDst, cv::Size(pParam->m_newWidth,pParam->m_newHeight), pParam->m_fx, pParam->m_fy, pParam->m_interpolation);
                    cuImgDst.download(imgDst);
                }
                cv::resize(pInput->getImage(), imgDst, cv::Size(pParam->m_newWidth,pParam->m_newHeight), pParam->m_fx, pParam->m_fy, pParam->m_interpolation);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            endTaskRun();
            emit m_signalHandler->doProgress();

            auto pOutput = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(imgDst);

            emit m_signalHandler->doProgress();
        }
};

class COcvResizeFactory : public CProcessFactory
{
    public:

        COcvResizeFactory()
        {
            m_info.m_name = QObject::tr("Resize").toStdString();
            m_info.m_description = QObject::tr("This process resize your image.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Image processing/Geometric image transformations").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "resize, interpolation";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/da/d54/group__imgproc__transform.html#ga47a974309e9102f5f08231edc7e7529d";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pFlipParam = std::dynamic_pointer_cast<COcvResizeParam>(pParam);
            if(pFlipParam != nullptr)
                return std::make_shared<COcvResize>(m_info.m_name, pFlipParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pFlipParam = std::make_shared<COcvResizeParam>();
            assert(pFlipParam != nullptr);
            return std::make_shared<COcvResize>(m_info.m_name, pFlipParam);
        }
};

#endif // COCVRESIZE_HPP
