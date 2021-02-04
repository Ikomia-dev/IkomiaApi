#ifndef COCVBOXFILTER_H
#define COCVBOXFILTER_H

#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"

//------------------------------//
//----- COcvBoxFilterParam -----//
//------------------------------//
class COcvBoxFilterParam: public CProtocolTaskParam
{
    public:

        COcvBoxFilterParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_ddepth = std::stoi(paramMap.at("ddepth"));
            m_ksize.width = std::stoi(paramMap.at("kSizeWidth"));
            m_ksize.height = std::stoi(paramMap.at("kSizeHeight"));
            m_anchor.x = std::stoi(paramMap.at("anchorX"));
            m_anchor.y = std::stoi(paramMap.at("anchorY"));
            m_bNormalize = std::stoi(paramMap.at("bNormalize"));
            m_borderType = std::stoi(paramMap.at("borderType"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("ddepth", std::to_string(m_ddepth)));
            map.insert(std::make_pair("kSizeWidth", std::to_string(m_ksize.width)));
            map.insert(std::make_pair("kSizeHeight", std::to_string(m_ksize.height)));
            map.insert(std::make_pair("anchorX", std::to_string(m_anchor.x)));
            map.insert(std::make_pair("anchorY", std::to_string(m_anchor.y)));
            map.insert(std::make_pair("bNormalize", std::to_string(m_bNormalize)));
            map.insert(std::make_pair("borderType", std::to_string(m_borderType)));
            return map;
        }

    public:

        int         m_ddepth = -1;
        cv::Size    m_ksize = cv::Size(5, 5);
        cv::Point   m_anchor = cv::Point(-1, -1);
        bool        m_bNormalize = true;
        int         m_borderType = cv::BORDER_DEFAULT;
};

//-------------------------//
//----- COcvBoxFilter -----//
//-------------------------//
class COcvBoxFilter : public CImageProcess2d
{
    public:

        COcvBoxFilter() : CImageProcess2d()
        {
        }
        COcvBoxFilter(const std::string name, const std::shared_ptr<COcvBoxFilterParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<COcvBoxFilterParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvBoxFilterParam>(m_pParam);

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
                cv::boxFilter(imgSrc, imgDst, pParam->m_ddepth, pParam->m_ksize, pParam->m_anchor, pParam->m_bNormalize, pParam->m_borderType);
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

class COcvBoxFilterFactory : public CProcessFactory
{
    public:

        COcvBoxFilterFactory()
        {
            m_info.m_name = QObject::tr("Box Filter").toStdString();
            m_info.m_description = QObject::tr("This process smoothes your image with a box filter.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Image processing/Image filtering").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Smooth,Blur,Isotropic,Filter,Gaussian";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d4/d86/group__imgproc__filter.html#gad533230ebf2d42509547d514f7d3fbc3";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pBoxFilterParam = std::dynamic_pointer_cast<COcvBoxFilterParam>(pParam);
            if(pBoxFilterParam != nullptr)
                return std::make_shared<COcvBoxFilter>(m_info.m_name, pBoxFilterParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pBoxFilterParam = std::make_shared<COcvBoxFilterParam>();
            assert(pBoxFilterParam != nullptr);
            return std::make_shared<COcvBoxFilter>(m_info.m_name, pBoxFilterParam);
        }
};

#endif // COCVBOXFILTER_H
