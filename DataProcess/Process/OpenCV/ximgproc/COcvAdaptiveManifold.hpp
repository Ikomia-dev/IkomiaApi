#ifndef COCVADAPTIVEMANIFOLD_H
#define COCVADAPTIVEMANIFOLD_H

#include "opencv2/ximgproc.hpp"
#include "Core/CImageProcess2d.h"

//-------------------------------------//
//----- COcvAdaptiveManifoldParam -----//
//-------------------------------------//
class COcvAdaptiveManifoldParam: public CProtocolTaskParam
{
    public:

        COcvAdaptiveManifoldParam() : CProtocolTaskParam()
        {
        }

        void        setParamMap(const UMapString& paramMap) override
        {
            m_sigmaS = std::stod(paramMap.at("sigmaS"));
            m_sigmaR = std::stod(paramMap.at("sigmaR"));
            m_treeHeight = std::stoi(paramMap.at("treeHeight"));
            m_numPcaIterations = std::stoi(paramMap.at("numPcaIterations"));
            m_bAdjustOutliers = std::stoi(paramMap.at("bAdjustOutliers"));
            m_bUseRNG = std::stoi(paramMap.at("bUseRNG"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("sigmaS", std::to_string(m_sigmaS)));
            map.insert(std::make_pair("sigmaR", std::to_string(m_sigmaR)));
            map.insert(std::make_pair("treeHeight", std::to_string(m_treeHeight)));
            map.insert(std::make_pair("numPcaIterations", std::to_string(m_numPcaIterations)));
            map.insert(std::make_pair("bAdjustOutliers", std::to_string(m_bAdjustOutliers)));
            map.insert(std::make_pair("bUseRNG", std::to_string(m_bUseRNG)));
            return map;
        }

    public:

        double  m_sigmaS = 16.0;
        double  m_sigmaR = 0.2;
        int     m_treeHeight = -1;
        int     m_numPcaIterations = 1;
        bool    m_bAdjustOutliers = false;
        bool    m_bUseRNG = true;
};

//--------------------------------//
//----- COcvAdaptiveManifold -----//
//--------------------------------//
class COcvAdaptiveManifold : public CImageProcess2d
{
    public:

        COcvAdaptiveManifold() : CImageProcess2d()
        {
        }
        COcvAdaptiveManifold(const std::string name, const std::shared_ptr<COcvAdaptiveManifoldParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<COcvAdaptiveManifoldParam>(*pParam);
        }

        size_t     getProgressSteps() override
        {
            return 4;
        }

        void    run() override
        {
            beginTaskRun();

            auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsProcessInput>(getInput(1));
            auto pParam = std::dynamic_pointer_cast<COcvAdaptiveManifoldParam>(m_pParam);

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
                auto ptr = cv::ximgproc::createAMFilter(pParam->m_sigmaS, pParam->m_sigmaR, pParam->m_bAdjustOutliers);
                ptr->setUseRNG(pParam->m_bUseRNG);
                ptr->setTreeHeight(pParam->m_treeHeight);
                ptr->setPCAIterations(pParam->m_numPcaIterations);
                emit m_signalHandler->doProgress();
                ptr->filter(imgSrc, imgDst);
                ptr->collectGarbage();
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

class COcvAdaptiveManifoldFactory : public CProcessFactory
{
    public:

        COcvAdaptiveManifoldFactory()
        {
            m_info.m_name = QObject::tr("Adaptive Manifold Filter").toStdString();
            m_info.m_description = QObject::tr("This process smoothes your image with an edge-preserving filter (blur homogeneous area and keep borders intact).").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Extended Image Processing/Filters").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Smooth, Blur,Anisotropic,Filter,Gaussian,Edge preserving";
            m_info.m_authors = "Eduardo SL Gastal, Manuel M Oliveira";
            m_info.m_article = "Adaptive manifolds for real-time high-dimensional filtering.";
            m_info.m_journal = "ACM Transactions on Graphics (TOG), 31(4):33";
            m_info.m_year = 2012;
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/de/db7/classcv_1_1ximgproc_1_1AdaptiveManifoldFilter.html";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pManifoldParam = std::dynamic_pointer_cast<COcvAdaptiveManifoldParam>(pParam);
            if(pManifoldParam != nullptr)
                return std::make_shared<COcvAdaptiveManifold>(m_info.m_name, pManifoldParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pManifoldParam = std::make_shared<COcvAdaptiveManifoldParam>();
            assert(pManifoldParam != nullptr);
            return std::make_shared<COcvAdaptiveManifold>(m_info.m_name, pManifoldParam);

        }
};

#endif // COCVADAPTIVEMANIFOLD_H
