#ifndef COCVHOUGHCIRCLES_HPP
#define COCVHOUGHCIRCLES_HPP

#include "Core/CImageProcess2d.h"

//---------------------------------//
//----- COcvHoughCirclesParam -----//
//---------------------------------//
class COcvHoughCirclesParam: public CProtocolTaskParam
{
    public:

        COcvHoughCirclesParam() : CProtocolTaskParam()
        {
        }

        void        setParamMap(const UMapString& paramMap) override
        {
            m_method = std::stoi(paramMap.at("method"));
            m_dp = std::stod(paramMap.at("dp"));
            m_minDist = std::stod(paramMap.at("minDistance"));
            m_param1 = std::stod(paramMap.at("param1"));
            m_param2 = std::stod(paramMap.at("param2"));
            m_minRadius = std::stoi(paramMap.at("minRadius"));
            m_maxRadius = std::stoi(paramMap.at("maxRadius"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("method", std::to_string(m_method)));
            map.insert(std::make_pair("dp", std::to_string(m_dp)));
            map.insert(std::make_pair("minDistance", std::to_string(m_minDist)));
            map.insert(std::make_pair("param1", std::to_string(m_param1)));
            map.insert(std::make_pair("param2", std::to_string(m_param2)));
            map.insert(std::make_pair("minRadius", std::to_string(m_minRadius)));
            map.insert(std::make_pair("maxRadius", std::to_string(m_maxRadius)));
            return map;
        }

    public:

        int     m_method = cv::HOUGH_GRADIENT;
        double  m_dp = 1.0;
        double  m_minDist = 10.0;
        double  m_param1 = 100;
        double  m_param2 = 100;
        int     m_minRadius = 0;
        int     m_maxRadius = 0;
};

//----------------------------//
//----- COcvHoughCircles -----//
//----------------------------//
class COcvHoughCircles : public CImageProcess2d
{
    public:

        COcvHoughCircles() : CImageProcess2d()
        {
            addOutput(std::make_shared<CImageProcessIO>());
            addOutput(std::make_shared<CGraphicsProcessOutput>());
            getOutput(1)->setDataType(IODataType::IMAGE_BINARY);
        }
        COcvHoughCircles(const std::string name, const std::shared_ptr<COcvHoughCirclesParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<COcvHoughCirclesParam>(*pParam);
            addOutput(std::make_shared<CImageProcessIO>());
            addOutput(std::make_shared<CGraphicsProcessOutput>());
            getOutput(1)->setDataType(IODataType::IMAGE_BINARY);
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        void    run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsProcessInput>(getInput(1));
            auto pParam = std::dynamic_pointer_cast<COcvHoughCirclesParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            std::vector<cv::Vec3f> circles;
            cv::Mat imgSrc = pInput->getImage();
            createGraphicsMask(imgSrc.cols, imgSrc.rows, pGraphicsInput);
            emit m_signalHandler->doProgress();

            try
            {
                cv::HoughCircles(imgSrc, circles, pParam->m_method, pParam->m_dp, pParam->m_minDist,
                                 pParam->m_param1, pParam->m_param2, pParam->m_minRadius, pParam->m_maxRadius);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }
            emit m_signalHandler->doProgress();

            forwardInputImage(0, 0);
            manageGraphicsOutput(circles);
            manageBinaryOutput();
            emit m_signalHandler->doProgress();
            endTaskRun();
        }

        void    manageGraphicsOutput(const std::vector<cv::Vec3f>& circles)
        {
            assert(m_graphicsContextPtr);
            auto pOutput = std::dynamic_pointer_cast<CGraphicsProcessOutput>(getOutput(2));
            assert(pOutput);
            pOutput->setNewLayer(getName());
            pOutput->setImageIndex(0);

            for(size_t i=0; i<circles.size(); ++i)
            {
                cv::Point center(circles[i][0], circles[i][1]);
                int radius = circles[i][2];
                pOutput->addEllipse(center.x - radius, center.y - radius, radius*2, radius*2);
            }
        }
        void    manageBinaryOutput()
        {
            auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pOutputGraphics = std::dynamic_pointer_cast<CGraphicsProcessOutput>(getOutput(2));

            cv::Mat imgSrc = pInput->getImage();
            CGraphicsConversion graphicsConverter(imgSrc.cols, imgSrc.rows);
            CMat binary = graphicsConverter.graphicsToBinaryMask(pOutputGraphics->getItems());
            applyGraphicsMaskToBinary(binary, binary, 0);

            auto pOutput = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(1));
            if(pOutput)
                pOutput->setImage(binary);
        }
};

class COcvHoughCirclesFactory : public CProcessFactory
{
    public:

        COcvHoughCirclesFactory()
        {
            m_info.m_name = QObject::tr("Hough circles").toStdString();
            m_info.m_description = QObject::tr("The function finds circles in a grayscale image using the Hough transform.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Image processing/Feature detection").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "circle,radius";
            m_info.m_docLink = "https://docs.opencv.org/4.0.1/dd/d1a/group__imgproc__feature.html#ga47849c3be0d0406ad3ca45db65a25d2d";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvHoughCirclesParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvHoughCircles>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvHoughCirclesParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvHoughCircles>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVHOUGHCIRCLES_HPP
