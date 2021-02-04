#ifndef COCVRETINASEGMENTATION_HPP
#define COCVRETINASEGMENTATION_HPP

#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"
#include <opencv2/bioinspired.hpp>

//----------------------------//
//----- COcvRetinaSegmentationParam -----//
//----------------------------//
class COcvRetinaSegmentationParam: public CProtocolTaskParam
{
    public:

        COcvRetinaSegmentationParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            Q_UNUSED(paramMap)
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            return map;
        }
};

//-----------------------//
//----- COcvRetinaSegmentation -----//
//-----------------------//
class COcvRetinaSegmentation : public CImageProcess2d
{
    public:

        COcvRetinaSegmentation() : CImageProcess2d()
        {
            setOutputDataType(IODataType::IMAGE_BINARY, 0);
            addOutput(std::make_shared<CImageProcessIO>());
            setOutputColorMap(1, 0, {{255,0,0}});
        }
        COcvRetinaSegmentation(const std::string name, const std::shared_ptr<COcvRetinaSegmentationParam>& pParam) : CImageProcess2d(name)
        {
            setOutputDataType(IODataType::IMAGE_BINARY, 0);
            addOutput(std::make_shared<CImageProcessIO>());
            setOutputColorMap(1, 0, {{255,0,0}});
            m_pParam = std::make_shared<COcvRetinaSegmentationParam>(*pParam);
        }

        size_t     getProgressSteps() override
        {
            return 3;
        }

        void    run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pParam = std::dynamic_pointer_cast<COcvRetinaSegmentationParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgDst;
            CMat imgSrc = pInput->getImage();
            emit m_signalHandler->doProgress();

            try
            {
                CMat img;
                if(imgSrc.channels() == 3)
                {
                    CMat imgLab, Lab[3];
                    cv::cvtColor(imgSrc, imgLab, cv::COLOR_RGB2Lab);
                    cv::split(imgLab,Lab);
                    img = Lab[0];
                }
                else
                    img = imgSrc;

                if(m_pRetina.get() == nullptr)
                    m_pRetina = cv::bioinspired::TransientAreasSegmentationModule::create(imgSrc.size());

                //pRetina->setup(pParam->m_photoreceptorsNeighborhoodRadius, pParam->m_ganglioncellsNeighborhoodRadius, pParam->m_meanLuminanceModulatorK);
                m_pRetina->run(img);
                m_pRetina->getSegmentationPicture(imgDst);
                imgDst *= 255;
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }
            emit m_signalHandler->doProgress();


            auto pOutput = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(imgDst);

            applyInputGraphicsMask(1, 0, 0, MaskMode::MASK_ONLY);
            forwardInputImage(0, 1);
            endTaskRun();
            emit m_signalHandler->doProgress();
        }

    private:
        cv::Ptr<cv::bioinspired::TransientAreasSegmentationModule> m_pRetina;
};

class COcvRetinaSegmentationFactory : public CProcessFactory
{
    public:

        COcvRetinaSegmentationFactory()
        {
            m_info.m_name = QObject::tr("RetinaSegmentation").toStdString();
            m_info.m_description = QObject::tr("This process provides a transient/moving areas segmentation using the retina magno.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Biologically inspired vision models and derivated tools").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "retina,segmentation";
            m_info.m_authors = "Alexandre Benoit";
            m_info.m_article = "Le système visuel humain au secours de la vision par ordinateur";
            m_info.m_journal = "PhD thesis";
            m_info.m_year = 2007;
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/da/d6e/classcv_1_1bioinspired_1_1TransientAreasSegmentationModule.html";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pRetinaSegmentationParam = std::dynamic_pointer_cast<COcvRetinaSegmentationParam>(pParam);
            if(pRetinaSegmentationParam != nullptr)
                return std::make_shared<COcvRetinaSegmentation>(m_info.m_name, pRetinaSegmentationParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pRetinaSegmentationParam = std::make_shared<COcvRetinaSegmentationParam>();
            assert(pRetinaSegmentationParam != nullptr);
            return std::make_shared<COcvRetinaSegmentation>(m_info.m_name, pRetinaSegmentationParam);
        }
};

#endif // COCVRETINASEGMENTATION_HPP
