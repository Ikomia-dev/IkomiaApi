#ifndef COCVDETAILENHANCE_H
#define COCVDETAILENHANCE_H

#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"

//----------------------------------//
//----- COcvDetailEnhanceParam -----//
//----------------------------------//
class COcvDetailEnhanceParam : public CProtocolTaskParam
{
    public:

        COcvDetailEnhanceParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_sigma_s = std::stof(paramMap.at("sigmaS"));
            m_sigma_r = std::stof(paramMap.at("sigmaR"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("sigmaS", std::to_string(m_sigma_s)));
            map.insert(std::make_pair("sigmaR", std::to_string(m_sigma_r)));
            return map;
        }

    public:

        float m_sigma_s = 10.0f;
        float m_sigma_r = 0.15f;
};

//-----------------------------//
//----- COcvDetailEnhance -----//
//-----------------------------//
class COcvDetailEnhance : public CImageProcess2d
{
    public:

        COcvDetailEnhance() : CImageProcess2d()
        {
        }
        COcvDetailEnhance(const std::string name, const std::shared_ptr<COcvDetailEnhanceParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<COcvDetailEnhanceParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvDetailEnhanceParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            auto image = pInput->getImage();
            if(image.channels() < 3 || image.depth() != CV_8U)
                throw CException(CoreExCode::INVALID_PARAMETER, "8 bits color image required", __func__, __FILE__, __LINE__);

            CMat imgDst;
            createGraphicsMask(image.getNbCols(), image.getNbRows(), pGraphicsInput);
            emit m_signalHandler->doProgress();

            try
            {
                cv::detailEnhance(image, imgDst, pParam->m_sigma_s, pParam->m_sigma_r);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            endTaskRun();
            applyGraphicsMask(image, imgDst, 0);
            emit m_signalHandler->doProgress();

            auto pOutput = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(imgDst);

            emit m_signalHandler->doProgress();
        }
};

class COcvDetailEnhanceFactory : public CProcessFactory
{
    public:

        COcvDetailEnhanceFactory()
        {
            m_info.m_name = QObject::tr("Detail Enhance Filter").toStdString();
            m_info.m_description = QObject::tr("This process enhances the details of a particular image.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Computational photography/Non-photorealistic rendering").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Enhancement";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/df/dac/group__photo__render.html#ga0de660cb6f371a464a74c7b651415975";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDetailEnhanceParam = std::dynamic_pointer_cast<COcvDetailEnhanceParam>(pParam);
            if(pDetailEnhanceParam != nullptr)
                return std::make_shared<COcvDetailEnhance>(m_info.m_name, pDetailEnhanceParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDetailEnhanceParam = std::make_shared<COcvDetailEnhanceParam>();
            assert(pDetailEnhanceParam != nullptr);
            return std::make_shared<COcvDetailEnhance>(m_info.m_name, pDetailEnhanceParam);
        }
};

#endif // COCVDETAILENHANCE_H
