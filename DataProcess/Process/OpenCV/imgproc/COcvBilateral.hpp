#ifndef COCVBILATERAL_H
#define COCVBILATERAL_H

#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"

//------------------------------//
//----- COcvBilateralParam -----//
//------------------------------//
class COcvBilateralParam: public CProtocolTaskParam
{
    public:

        COcvBilateralParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_diameter = std::stoi(paramMap.at("diameter"));
            m_sigmaColor = std::stod(paramMap.at("sigmaColor"));
            m_sigmaSpace = std::stod(paramMap.at("sigmaSpace"));
            m_borderType = std::stoi(paramMap.at("borderType"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("diameter", std::to_string(m_diameter)));
            map.insert(std::make_pair("sigmaColor", std::to_string(m_sigmaColor)));
            map.insert(std::make_pair("sigmaSpace", std::to_string(m_sigmaSpace)));
            map.insert(std::make_pair("borderType", std::to_string(m_borderType)));
            return map;
        }

    public:

        int     m_diameter = 5;
        double  m_sigmaColor = 10;
        double  m_sigmaSpace = 10;
        int     m_borderType = cv::BORDER_DEFAULT;
};

//-------------------------//
//----- COcvBilateral -----//
//-------------------------//
class COcvBilateral : public CImageProcess2d
{
    public:

        COcvBilateral() : CImageProcess2d()
        {
        }
        COcvBilateral(const std::string name, const std::shared_ptr<COcvBilateralParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<COcvBilateralParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvBilateralParam>(m_pParam);

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
                cv::bilateralFilter(imgSrc, imgDst, pParam->m_diameter, pParam->m_sigmaColor, pParam->m_sigmaSpace, pParam->m_borderType);
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

class COcvBilateralFactory : public CProcessFactory
{
    public:

        COcvBilateralFactory()
        {            
            m_info.m_name = QObject::tr("Bilateral Filter").toStdString();
            m_info.m_description = QObject::tr("This process smoothes your image with an edge-preserving filter (blur homogeneous area and keep borders intact).").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Image processing/Image filtering").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Smooth,Blur,Anisotropic,Filter,Gaussian,Edge-preserving";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d4/d86/group__imgproc__filter.html#ga9d7064d478c95d60003cf839430737ed";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvBilateralParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvBilateral>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvBilateralParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvBilateral>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVBILATERAL_H
