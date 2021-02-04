#ifndef COCVPCAOPTICALFLOW_HPP
#define COCVPCAOPTICALFLOW_HPP

#include "Core/CVideoProcessOF.h"
#include "IO/CVideoProcessIO.h"
#include "opencv2/optflow.hpp"

//------------------------------//
//----- COcvPCAOFParam -----//
//------------------------------//
class COcvPCAOFParam: public CProtocolTaskParam
{
    public:

        COcvPCAOFParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_basisSize = cv::Size(std::stoi(paramMap.at("width")), std::stoi(paramMap.at("height")));
            m_sparseRate = std::stof(paramMap.at("sparseRate"));
            m_retainedCornersFraction = std::stof(paramMap.at("retainedCornersFraction"));
            m_occlusionsThreshold = std::stof(paramMap.at("occlusionsThreshold"));
            m_dampingFactor = std::stof(paramMap.at("dampingFactor"));
            m_claheClip = std::stof(paramMap.at("claheClip"));
            m_bUseOCL = std::stof(paramMap.at("bUseOCL"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("width", std::to_string(m_basisSize.width)));
            map.insert(std::make_pair("height", std::to_string(m_basisSize.height)));
            map.insert(std::make_pair("sparseRate", std::to_string(m_sparseRate)));
            map.insert(std::make_pair("retainedCornersFraction", std::to_string(m_retainedCornersFraction)));
            map.insert(std::make_pair("occlusionsThreshold", std::to_string(m_occlusionsThreshold)));
            map.insert(std::make_pair("dampingFactor", std::to_string(m_dampingFactor)));
            map.insert(std::make_pair("claheClip", std::to_string(m_claheClip)));
            map.insert(std::make_pair("bUseOCL", std::to_string(m_bUseOCL)));

            return map;
        }

    public:
        cv::Ptr< const cv::optflow::PCAPrior >  	m_prior = cv::Ptr< const cv::optflow::PCAPrior >();
        cv::Size                                    m_basisSize = cv::Size(18, 14);
        float                                       m_sparseRate = 0.024f;
        float                                       m_retainedCornersFraction = 0.2f;
        float                                       m_occlusionsThreshold = 0.0003f;
        float                                       m_dampingFactor = 0.00002f;
        float                                       m_claheClip = 14;
        bool                                        m_bUseOCL = false;


};

//-------------------------//
//----- COcvPCAOF -----//
//-------------------------//
class COcvPCAOF : public CVideoProcessOF
{
    public:

        COcvPCAOF() : CVideoProcessOF()
        {
        }
        COcvPCAOF(const std::string name, const std::shared_ptr<COcvPCAOFParam>& pParam) : CVideoProcessOF(name)
        {
            m_pParam = std::make_shared<COcvPCAOFParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvPCAOFParam>(m_pParam);

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
                m_pOF = cv::Ptr<cv::optflow::OpticalFlowPCAFlow>(new cv::optflow::OpticalFlowPCAFlow(pParam->m_prior, pParam->m_basisSize, pParam->m_sparseRate, pParam->m_retainedCornersFraction, pParam->m_occlusionsThreshold, pParam->m_dampingFactor, pParam->m_claheClip));
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

class COcvPCAOFFactory : public CProcessFactory
{
    public:

        COcvPCAOFFactory()
        {
            m_info.m_name = QObject::tr("PCA Optical Flow").toStdString();
            m_info.m_description = QObject::tr("This process computes a dense optical flow using the PCA algorithm.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Optical flow algorithms").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "PCA, Dense, Optical, Flow";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d1/da2/classcv_1_1optflow_1_1OpticalFlowPCAFlow.html";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pPCAOFParam = std::dynamic_pointer_cast<COcvPCAOFParam>(pParam);
            if(pPCAOFParam != nullptr)
                return std::make_shared<COcvPCAOF>(m_info.m_name, pPCAOFParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pPCAOFParam = std::make_shared<COcvPCAOFParam>();
            assert(pPCAOFParam != nullptr);
            return std::make_shared<COcvPCAOF>(m_info.m_name, pPCAOFParam);
        }
};

#endif // COCVPCAOPTICALFLOW_HPP
