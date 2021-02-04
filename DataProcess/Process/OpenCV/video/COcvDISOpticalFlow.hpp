#ifndef COCVDISOPTICALFLOW_HPP
#define COCVDISOPTICALFLOW_HPP

#include "Core/CVideoProcessOF.h"
#include "IO/CVideoProcessIO.h"
#include "opencv2/tracking.hpp"

//------------------------------//
//----- COcvDISOFParam -----//
//------------------------------//
class COcvDISOFParam: public CProtocolTaskParam
{
    public:

        COcvDISOFParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_preset = std::stoi(paramMap.at("preset"));
            m_bUseOCL = std::stoi(paramMap.at("bUseOCL"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("bUseOCL", std::to_string(m_bUseOCL)));

            return map;
        }

    public:
        int  	m_preset = cv::DISOpticalFlow::PRESET_FAST;
        bool    m_bUseOCL = false;


};

//-------------------------//
//----- COcvDISOF -----//
//-------------------------//
class COcvDISOF : public CVideoProcessOF
{
    public:

        COcvDISOF() : CVideoProcessOF()
        {
        }
        COcvDISOF(const std::string name, const std::shared_ptr<COcvDISOFParam>& pParam) : CVideoProcessOF(name)
        {
            m_pParam = std::make_shared<COcvDISOFParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvDISOFParam>(m_pParam);

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
                m_pOF = cv::DISOpticalFlow::create(pParam->m_preset);
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

class COcvDISOFFactory : public CProcessFactory
{
    public:

        COcvDISOFFactory()
        {
            m_info.m_name = QObject::tr("DIS Optical Flow").toStdString();
            m_info.m_description = QObject::tr("This process computes a dense optical flow using the Dense Inverse Search (DIS) algorithm.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Video analysis/Object tracking").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "DIS, Dense, Optical, Flow";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/da/d06/classcv_1_1optflow_1_1DISOpticalFlow.html";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDISOFParam = std::dynamic_pointer_cast<COcvDISOFParam>(pParam);
            if(pDISOFParam != nullptr)
                return std::make_shared<COcvDISOF>(m_info.m_name, pDISOFParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDISOFParam = std::make_shared<COcvDISOFParam>();
            assert(pDISOFParam != nullptr);
            return std::make_shared<COcvDISOF>(m_info.m_name, pDISOFParam);
        }
};

#endif // COCVDISOPTICALFLOW_HPP
