#ifndef COCVGRAYWORLDWB_HPP
#define COCVGRAYWORLDWB_HPP

#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"
#include <opencv2/xphoto.hpp>

//------------------------------//
//----- COcvGrayworldWBParam -----//
//------------------------------//
class COcvGrayworldWBParam: public CProtocolTaskParam
{
    public:

        COcvGrayworldWBParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_satThreshold = std::stof(paramMap.at("satThreshold"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("satThreshold", std::to_string(m_satThreshold)));

            return map;
        }

    public:
        float   m_satThreshold = 1.0f;
};

//-------------------------//
//----- COcvGrayworldWB -----//
//-------------------------//
class COcvGrayworldWB : public CImageProcess2d
{
    public:

        COcvGrayworldWB() : CImageProcess2d()
        {
        }
        COcvGrayworldWB(const std::string name, const std::shared_ptr<COcvGrayworldWBParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<COcvGrayworldWBParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvGrayworldWBParam>(m_pParam);

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
                auto pGWB = cv::xphoto::createGrayworldWB();
                pGWB->setSaturationThreshold(pParam->m_satThreshold);
                pGWB->balanceWhite(imgSrc, imgDst);
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

class COcvGrayworldWBFactory : public CProcessFactory
{
    public:

        COcvGrayworldWBFactory()
        {
            m_info.m_name = QObject::tr("GrayworldWB").toStdString();
            m_info.m_description = QObject::tr("This algorithm scales the values of pixels based on a gray-world assumption which states that the average of all channels should result in a gray image.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Additional photo processing algorithms").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "White,Balance,AutoWhiteBalance";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d7/d71/classcv_1_1xphoto_1_1GrayworldWB.html";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvGrayworldWBParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvGrayworldWB>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvGrayworldWBParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvGrayworldWB>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVGRAYWORLDWB_HPP
