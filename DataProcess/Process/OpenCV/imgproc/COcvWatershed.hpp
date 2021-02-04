#ifndef COCVWATERSHED_HPP
#define COCVWATERSHED_HPP

#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"

//------------------------------//
//----- COcvWatershedParam -----//
//------------------------------//
class COcvWatershedParam: public CProtocolTaskParam
{
    public:

        COcvWatershedParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            Q_UNUSED(paramMap);
        }

        UMapString  getParamMap() const override
        {
            return UMapString();
        }
};

//----------------------//
//----- COcvWatershed -----//
//----------------------//
class COcvWatershed : public CImageProcess2d
{
    public:

        COcvWatershed() : CImageProcess2d()
        {
            insertInput(std::make_shared<CImageProcessIO>(IODataType::IMAGE), 1);
            setOutputDataType(IODataType::IMAGE_LABEL, 0);
            addOutput(std::make_shared<CImageProcessIO>());
            setOutputColorMap(1, 0);
        }
        COcvWatershed(const std::string name, const std::shared_ptr<COcvWatershedParam>& pParam) : CImageProcess2d(name)
        {
            insertInput(std::make_shared<CImageProcessIO>(IODataType::IMAGE), 1);
            setOutputDataType(IODataType::IMAGE_LABEL, 0);
            addOutput(std::make_shared<CImageProcessIO>());
            setOutputColorMap(1, 0);
            m_pParam = std::make_shared<COcvWatershedParam>(*pParam);
        }

        size_t  getProgressSteps() override
        {
            return 4;
        }

        void run() override
        {
            beginTaskRun();
            auto pInput1 = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pInput2 = std::dynamic_pointer_cast<CImageProcessIO>(getInput(1));
            auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsProcessInput>(getInput(2));

            if(pInput1 == nullptr || pInput2 == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput1->isDataAvailable() == false || pInput2->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgSrc = pInput1->getImage();
            CMat markersMask = pInput2->getImage();
            CMat imgDst;

            //Source image must be 3 channels
            if(imgSrc.channels() == 1)
                cv::cvtColor(imgSrc, imgSrc, cv::COLOR_GRAY2RGB);

            //Marker image must be 1 channel 32 bits integer
            if(markersMask.channels() > 1)
            {
                cv::cvtColor(markersMask, imgDst, cv::COLOR_RGB2GRAY);
                if(imgDst.depth() != CV_32S)
                    imgDst.convertTo(imgDst, CV_32S);
            }
            else
            {
                if(markersMask.depth() != CV_32S)
                    markersMask.convertTo(imgDst, CV_32S);
            }
            emit m_signalHandler->doProgress();

            createGraphicsMask(imgSrc.getNbCols(), imgSrc.getNbRows(), pGraphicsInput);
            emit m_signalHandler->doProgress();

            try
            {
                cv::watershed(imgSrc, imgDst);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }


            emit m_signalHandler->doProgress();
            applyGraphicsMask(imgSrc, imgDst, 0);
            forwardInputImage(0, 1);

            auto pOutput = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(imgDst);

            endTaskRun();
            emit m_signalHandler->doProgress();
        }
};

class COcvWatershedFactory : public CProcessFactory
{
    public:

        COcvWatershedFactory()
        {
            m_info.m_name = QObject::tr("Watershed").toStdString();
            m_info.m_description = QObject::tr("This process implements one of the variants of watershed, non-parametric marker-based segmentation algorithm.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Image processing/Miscellaneous image transformations").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Watershed, segmentation, markers, label";
            m_info.m_authors = "Fernand Meyer";
            m_info.m_article = "Color image segmentation";
            m_info.m_journal = "Image Processing and its Applications";
            m_info.m_year = 1992;
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d7/d1b/group__imgproc__misc.html#ga3267243e4d3f95165d55a618c65ac6e1";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvWatershedParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvWatershed>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvWatershedParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvWatershed>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVWATERSHED_HPP
