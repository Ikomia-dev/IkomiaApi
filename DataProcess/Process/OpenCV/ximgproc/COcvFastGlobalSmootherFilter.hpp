#ifndef COCVFASTGLOBALSMOOTHERFILTER_HPP
#define COCVFASTGLOBALSMOOTHERFILTER_HPP

#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"
#include <opencv2/ximgproc.hpp>

//---------------------------------------------//
//----- COcvFastGlobalSmootherFilterParam -----//
//---------------------------------------------//
class COcvFastGlobalSmootherFilterParam: public CProtocolTaskParam
{
    public:

        COcvFastGlobalSmootherFilterParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_lambda = std::stod(paramMap.at("lambda"));
            m_sigmaColor = std::stod(paramMap.at("sigmaColor"));
            m_lambdaAttenuation = std::stod(paramMap.at("lambdaAttenuation"));
            m_nbIter = std::stoi(paramMap.at("nbIteration"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("lambda", std::to_string(m_lambda)));
            map.insert(std::make_pair("sigmaColor", std::to_string(m_sigmaColor)));
            map.insert(std::make_pair("lambdaAttenuation", std::to_string(m_lambdaAttenuation)));
            map.insert(std::make_pair("nbIteration", std::to_string(m_nbIter)));
            return map;
        }

    public:

        double  m_lambda = 5.0;
        double  m_sigmaColor = 9.0;
        double  m_lambdaAttenuation = 0.25;
        int     m_nbIter = 3;
};

//----------------------------------------//
//----- COcvFastGlobalSmootherFilter -----//
//----------------------------------------//
class COcvFastGlobalSmootherFilter : public CImageProcess2d
{
    public:

        COcvFastGlobalSmootherFilter() : CImageProcess2d()
        {
        }
        COcvFastGlobalSmootherFilter(const std::string name, const std::shared_ptr<COcvFastGlobalSmootherFilterParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<COcvFastGlobalSmootherFilterParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvFastGlobalSmootherFilterParam>(m_pParam);

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
                cv::ximgproc::fastGlobalSmootherFilter(imgSrc, imgSrc, imgDst, pParam->m_lambda, pParam->m_sigmaColor, pParam->m_lambdaAttenuation, pParam->m_nbIter);
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

class COcvFastGlobalSmootherFilterFactory : public CProcessFactory
{
    public:

        COcvFastGlobalSmootherFilterFactory()
        {
            m_info.m_name = QObject::tr("Fast Global Smoother Filter").toStdString();
            m_info.m_description = QObject::tr("The process applies fast global image smoothing based on weighted least squares.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Extended Image Processing/Filters").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "smooth,global,fast,filter,weighted,least squares";
            m_info.m_authors = "Dongbo Min, Sunghwan Choi, Jiangbo Lu, Bumsub Ham, Kwanghoon Sohn, and Minh N Do";
            m_info.m_article = "Fast global image smoothing based on weighted least squares";
            m_info.m_journal = "Image Processing, IEEE Transactions on, 23(12):5638–5653";
            m_info.m_year = 2014;
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/da/d17/group__ximgproc__filters.html#gaf8673fe9147160ad96ac6053fac3c106";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvFastGlobalSmootherFilterParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvFastGlobalSmootherFilter>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvFastGlobalSmootherFilterParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvFastGlobalSmootherFilter>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVFASTGLOBALSMOOTHERFILTER_HPP
