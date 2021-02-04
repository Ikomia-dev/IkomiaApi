#ifndef COCVDUALTVL1OPTICALFLOW_HPP
#define COCVDUALTVL1OPTICALFLOW_HPP

#include "Core/CVideoProcessOF.h"
#include "IO/CVideoProcessIO.h"
#include "opencv2/optflow.hpp"

//------------------------------//
//----- COcvDualTVL1OFParam -----//
//------------------------------//
class COcvDualTVL1OFParam: public CProtocolTaskParam
{
    public:

        COcvDualTVL1OFParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_tau = std::stoi(paramMap.at("tau"));
            m_lambda = std::stoi(paramMap.at("lambda"));
            m_theta = std::stoi(paramMap.at("theta"));
            m_nscales = std::stoi(paramMap.at("nscales"));
            m_warps = std::stoi(paramMap.at("warps"));
            m_epsilon = std::stoi(paramMap.at("epsilon"));
            m_innerIterations = std::stoi(paramMap.at("innerIterations"));
            m_outerIterations = std::stoi(paramMap.at("outerIterations"));
            m_scaleStep = std::stoi(paramMap.at("scaleStep"));
            m_gamma = std::stoi(paramMap.at("gamma"));
            m_medianFiltering = std::stoi(paramMap.at("medianFiltering"));
            m_useInitialFlow = std::stoi(paramMap.at("useInitialFlow"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("tau", std::to_string(m_tau)));
            map.insert(std::make_pair("lambda", std::to_string(m_lambda)));
            map.insert(std::make_pair("theta", std::to_string(m_theta)));
            map.insert(std::make_pair("nscales", std::to_string(m_nscales)));
            map.insert(std::make_pair("warps", std::to_string(m_warps)));
            map.insert(std::make_pair("epsilon", std::to_string(m_epsilon)));
            map.insert(std::make_pair("innerIterations", std::to_string(m_innerIterations)));
            map.insert(std::make_pair("outerIterations", std::to_string(m_outerIterations)));
            map.insert(std::make_pair("scaleStep", std::to_string(m_scaleStep)));
            map.insert(std::make_pair("gamma", std::to_string(m_gamma)));
            map.insert(std::make_pair("medianFiltering", std::to_string(m_medianFiltering)));
            map.insert(std::make_pair("useInitialFlow", std::to_string(m_useInitialFlow)));

            return map;
        }

    public:
        double  	m_tau = 0.25;
        double  	m_lambda = 0.15;
        double  	m_theta = 0.3;
        int         m_nscales = 5;
        int         m_warps = 5;
        double  	m_epsilon = 0.01;
        int         m_innerIterations = 30;
        int         m_outerIterations = 10;
        double  	m_scaleStep = 0.8;
        double  	m_gamma = 0.0;
        int         m_medianFiltering = 5;
        bool        m_useInitialFlow = false;
        bool        m_bUseOCL = false;

};

//-------------------------//
//----- COcvDualTVL1OF -----//
//-------------------------//
class COcvDualTVL1OF : public CVideoProcessOF
{
    public:

        COcvDualTVL1OF() : CVideoProcessOF()
        {
        }
        COcvDualTVL1OF(const std::string name, const std::shared_ptr<COcvDualTVL1OFParam>& pParam) : CVideoProcessOF(name)
        {
            m_pParam = std::make_shared<COcvDualTVL1OFParam>(*pParam);
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        virtual void notifyVideoStart(int /*frameCount*/) override
        {
            m_imgPrev.release();
        }

        void run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CVideoProcessIO>(getInput(0));
            auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsProcessInput>(getInput(1));
            auto pParam = std::dynamic_pointer_cast<COcvDualTVL1OFParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            m_bUseOpenCL = pParam->m_bUseOCL;

            CMat imgSrc = pInput->getImage();

            // Manage when we reach the end of video by copying the last frame => optical flow doesn't change
            if(imgSrc.empty())
                imgSrc = m_imgPrev.clone();

            createGraphicsMask(imgSrc.getNbCols(), imgSrc.getNbRows(), pGraphicsInput);
            emit m_signalHandler->doProgress();

            m_imgFlow = cv::Mat(imgSrc.size[0], imgSrc.size[1], CV_32FC2);
            try
            {
                cv::Mat nextGray = manageInputs(imgSrc);
                m_pOF = cv::optflow::DualTVL1OpticalFlow::create(pParam->m_tau, pParam->m_lambda, pParam->m_theta, pParam->m_nscales, pParam->m_warps, pParam->m_epsilon, pParam->m_innerIterations, pParam->m_outerIterations, pParam->m_scaleStep, pParam->m_gamma, pParam->m_medianFiltering, pParam->m_useInitialFlow);
                makeOF(nextGray);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            endTaskRun();

            emit m_signalHandler->doProgress();

            manageOuputs(imgSrc);

            emit m_signalHandler->doProgress();
        }
};

class COcvDualTVL1OFFactory : public CProcessFactory
{
    public:

        COcvDualTVL1OFFactory()
        {
            m_info.m_name = QObject::tr("Dual TVL1 Optical Flow").toStdString();
            m_info.m_description = QObject::tr("This process computes a dense optical flow using the Dual TV L1 algorithm.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Optical flow algorithms").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Farneback, Dense, Optical, Flow";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/dc/d47/classcv_1_1DualTVL1OpticalFlow.html";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDualTVL1OFParam = std::dynamic_pointer_cast<COcvDualTVL1OFParam>(pParam);
            if(pDualTVL1OFParam != nullptr)
                return std::make_shared<COcvDualTVL1OF>(m_info.m_name, pDualTVL1OFParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDualTVL1OFParam = std::make_shared<COcvDualTVL1OFParam>();
            assert(pDualTVL1OFParam != nullptr);
            return std::make_shared<COcvDualTVL1OF>(m_info.m_name, pDualTVL1OFParam);
        }
};

#endif // COCVDUALTVL1OPTICALFLOW_HPP
