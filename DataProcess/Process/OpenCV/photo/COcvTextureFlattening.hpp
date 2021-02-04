#ifndef COCVTEXTUREFLATTENING_HPP
#define COCVTEXTUREFLATTENING_HPP

#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"
#include <opencv2/photo.hpp>

//----------------------------------//
//----- COcvTextureFlatteningParam -----//
//----------------------------------//
class COcvTextureFlatteningParam : public CProtocolTaskParam
{
    public:

        COcvTextureFlatteningParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_lowThresh = std::stof(paramMap.at("lowThresh"));
            m_highThresh = std::stof(paramMap.at("highThresh"));
            m_kernelSize = std::stof(paramMap.at("kernelSize"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("lowThresh", std::to_string(m_lowThresh)));
            map.insert(std::make_pair("highThresh", std::to_string(m_highThresh)));
            map.insert(std::make_pair("kernelSize", std::to_string(m_kernelSize)));
            return map;
        }

    public:
        float m_lowThresh = 30.0f;
        float m_highThresh = 45.0f;
        int m_kernelSize = 3;
};

//-----------------------------//
//----- COcvTextureFlattening -----//
//-----------------------------//
class COcvTextureFlattening : public CImageProcess2d
{
    public:

        COcvTextureFlattening() : CImageProcess2d()
        {
        }
        COcvTextureFlattening(const std::string name, const std::shared_ptr<COcvTextureFlatteningParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<COcvTextureFlatteningParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvTextureFlatteningParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            auto image = pInput->getImage();

            if(image.channels() != 3)
                throw CException(CoreExCode::INVALID_PARAMETER, "Color image required", __func__, __FILE__, __LINE__);

            CMat imgDst;
            createGraphicsMask(image.getNbCols(), image.getNbRows(), pGraphicsInput);
            CMat mask;
            if(isMaskAvailable(0))
                mask = getGraphicsMask(0);
            else
                mask = CMat(image.size(), CV_8U, 255);

            emit m_signalHandler->doProgress();

            try
            {
                cv::textureFlattening(image, mask, imgDst, pParam->m_lowThresh, pParam->m_highThresh, pParam->m_kernelSize);
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

class COcvTextureFlatteningFactory : public CProcessFactory
{
    public:

        COcvTextureFlatteningFactory()
        {
            m_info.m_name = QObject::tr("Texture flattening").toStdString();
            m_info.m_description = QObject::tr("By retaining only the gradients at edge locations, before integrating with the Poisson solver, one washes out the texture of the selected region, giving its contents a flat aspect. Here Canny Edge Detector is used.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Computational photography/Seamless cloning").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Seamless,Cloning,Texture,Flattening,Bilateral";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/df/da0/group__photo__clone.html#gad55df6aa53797365fa7cc23959a54004";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pTextureFlatteningParam = std::dynamic_pointer_cast<COcvTextureFlatteningParam>(pParam);
            if(pTextureFlatteningParam != nullptr)
                return std::make_shared<COcvTextureFlattening>(m_info.m_name, pTextureFlatteningParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pTextureFlatteningParam = std::make_shared<COcvTextureFlatteningParam>();
            assert(pTextureFlatteningParam != nullptr);
            return std::make_shared<COcvTextureFlattening>(m_info.m_name, pTextureFlatteningParam);
        }
};

#endif // COCVTEXTUREFLATTENING_HPP
