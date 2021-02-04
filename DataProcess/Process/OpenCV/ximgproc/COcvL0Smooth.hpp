#ifndef COCVL0SMOOTH_HPP
#define COCVL0SMOOTH_HPP

#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"
#include <opencv2/ximgproc.hpp>

//------------------------------//
//----- COcvL0SmoothParam -----//
//------------------------------//
class COcvL0SmoothParam: public CProtocolTaskParam
{
    public:

        COcvL0SmoothParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_lambda = std::stod(paramMap.at("lambda"));
            m_kappa = std::stod(paramMap.at("kappa"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("lambda", std::to_string(m_lambda)));
            map.insert(std::make_pair("kappa", std::to_string(m_kappa)));
            return map;
        }

    public:

        double  m_lambda = 0.02;
        double  m_kappa = 2.0;
};

//-------------------------//
//----- COcvL0Smooth -----//
//-------------------------//
class COcvL0Smooth : public CImageProcess2d
{
    public:

        COcvL0Smooth() : CImageProcess2d()
        {
        }
        COcvL0Smooth(const std::string name, const std::shared_ptr<COcvL0SmoothParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<COcvL0SmoothParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvL0SmoothParam>(m_pParam);

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
                cv::ximgproc::l0Smooth(imgSrc, imgDst, pParam->m_lambda, pParam->m_kappa);
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

class COcvL0SmoothFactory : public CProcessFactory
{
    public:

        COcvL0SmoothFactory()
        {
            m_info.m_name = QObject::tr("L0 smooth filter").toStdString();
            m_info.m_description = QObject::tr("Global image smoothing via L0 gradient minimization.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Extended Image Processing/Filters").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Smooth,Blur,Anisotropic,Filter,Gaussian,Edge-preserving,Cartooning";
            m_info.m_authors = "Li Xu, Cewu Lu, Yi Xu, and Jiaya Jia";
            m_info.m_article = "Image smoothing via l 0 gradient minimization";
            m_info.m_journal = "ACM Transactions on Graphics (TOG), volume 30, page 174";
            m_info.m_year = 2011;
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/da/d17/group__ximgproc__filters.html#gac13015b6a8a7e99554f4affa7b6896ae";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvL0SmoothParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvL0Smooth>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvL0SmoothParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvL0Smooth>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVL0SMOOTH_HPP
