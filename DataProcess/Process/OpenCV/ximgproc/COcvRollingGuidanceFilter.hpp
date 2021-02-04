#ifndef COCVROLLINGGUIDANCEFILTER_HPP
#define COCVROLLINGGUIDANCEFILTER_HPP

#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"
#include <opencv2/ximgproc.hpp>

//------------------------------//
//----- COcvRollingGuidanceFilterParam -----//
//------------------------------//
class COcvRollingGuidanceFilterParam: public CProtocolTaskParam
{
    public:

        COcvRollingGuidanceFilterParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_d = std::stoi(paramMap.at("d"));
            m_sigmaColor = std::stod(paramMap.at("sigmaColor"));
            m_sigmaSpace = std::stod(paramMap.at("sigmaSpace"));
            m_numOfIter = std::stoi(paramMap.at("iter"));
            m_border = std::stoi(paramMap.at("border"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("d", std::to_string(m_d)));
            map.insert(std::make_pair("sigmaColor", std::to_string(m_sigmaColor)));
            map.insert(std::make_pair("sigmaSpace", std::to_string(m_sigmaSpace)));
            map.insert(std::make_pair("iter", std::to_string(m_numOfIter)));
            map.insert(std::make_pair("border", std::to_string(m_border)));
            return map;
        }

    public:

        int     m_d = -1;
        double  m_sigmaColor = 25;
        double  m_sigmaSpace = 3;
        int     m_numOfIter = 4;
        int     m_border = cv::BORDER_DEFAULT;
};

//-------------------------//
//----- COcvRollingGuidanceFilter -----//
//-------------------------//
class COcvRollingGuidanceFilter : public CImageProcess2d
{
    public:

        COcvRollingGuidanceFilter() : CImageProcess2d()
        {
        }
        COcvRollingGuidanceFilter(const std::string name, const std::shared_ptr<COcvRollingGuidanceFilterParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<COcvRollingGuidanceFilterParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvRollingGuidanceFilterParam>(m_pParam);

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
                cv::ximgproc::rollingGuidanceFilter(imgSrc, imgDst, pParam->m_d, pParam->m_sigmaColor, pParam->m_sigmaSpace, pParam->m_numOfIter, pParam->m_border);
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

class COcvRollingGuidanceFilterFactory : public CProcessFactory
{
    public:

        COcvRollingGuidanceFilterFactory()
        {
            m_info.m_name = QObject::tr("Rolling guidance filter").toStdString();
            m_info.m_description = QObject::tr("Applies the rolling guidance filter to an image.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Extended Image Processing/Filters").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Smooth,Blur,Anisotropic,Filter,Gaussian,Edge-preserving";
            m_info.m_authors = "Qi Zhang, Xiaoyong Shen, Li Xu, and Jiaya Jia";
            m_info.m_article = "Rolling guidance filter";
            m_info.m_journal = "ECCV 2014, pages 815–830";
            m_info.m_year = 2014;
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/da/d17/group__ximgproc__filters.html#gacef7fba156b072851dae79e42c39001f";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvRollingGuidanceFilterParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvRollingGuidanceFilter>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvRollingGuidanceFilterParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvRollingGuidanceFilter>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVROLLINGGUIDANCEFILTER_HPP
