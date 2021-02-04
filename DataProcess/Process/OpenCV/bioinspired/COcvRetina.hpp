#ifndef COCVRETINA_HPP
#define COCVRETINA_HPP

#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"
#include <opencv2/bioinspired.hpp>

//----------------------------//
//----- COcvRetinaParam -----//
//----------------------------//
class COcvRetinaParam: public CProtocolTaskParam
{
    public:

        COcvRetinaParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_bUseOCL = std::stoi(paramMap.at("useOCL"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("useOCL", std::to_string(m_bUseOCL)));
            return map;
        }

    public:
        bool    m_bUseOCL = false;
};

//-----------------------//
//----- COcvRetina -----//
//-----------------------//
class COcvRetina : public CImageProcess2d
{
    public:

        COcvRetina() : CImageProcess2d()
        {
            addOutput(std::make_shared<CImageProcessIO>());
        }
        COcvRetina(const std::string name, const std::shared_ptr<COcvRetinaParam>& pParam) : CImageProcess2d(name)
        {
            addOutput(std::make_shared<CImageProcessIO>());
            m_pParam = std::make_shared<COcvRetinaParam>(*pParam);
        }

        size_t     getProgressSteps() override
        {
            return 3;
        }

        void    run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pParam = std::dynamic_pointer_cast<COcvRetinaParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgParvo, imgMagno;
            CMat imgSrc = pInput->getImage();

            try
            {
                if(m_pRetina.get() == nullptr)
                {
                    m_pRetina = cv::bioinspired::Retina::create(imgSrc.size());
                    m_width = imgSrc.cols;
                    m_height = imgSrc.rows;
                }
                else
                {
                    if(imgSrc.cols != m_width || imgSrc.rows != m_height)
                    {
                        m_pRetina = cv::bioinspired::Retina::create(imgSrc.size());
                        m_width = imgSrc.cols;
                        m_height = imgSrc.rows;
                    }
                }
                emit m_signalHandler->doProgress();

                /* retina parameters management methods use sample
                   -> save current (here default) retina parameters to a xml file (you may use it only one time to get the file and modify it)
                */
                //pRetina->write("RetinaDefaultParameters.xml");
                // -> load parameters if file exists
                //pRetina->setup("RetinaSpecificParameters.xml");
                // reset all retina buffers (open your eyes)
                //pRetina->clearBuffers();

                // run retina on the input image
                m_pRetina->run(imgSrc);

                if(pParam->m_bUseOCL)
                {
                    // Enable OpenCL and test success
                    cv::ocl::setUseOpenCL(true);
                    if(!cv::ocl::useOpenCL())
                    {
                        throw CException(CoreExCode::INVALID_PARAMETER, "OpenCL is not available on this platform.", __func__, __FILE__, __LINE__);
                    }
                    // Use UMat for gpu calculations
                    cv::UMat parvo, magno;
                    m_pRetina->getParvo(parvo);
                    m_pRetina->getMagno(magno);
                    // Get Mat from UMat
                    imgParvo = parvo.getMat(cv::ACCESS_RW).clone();
                    imgMagno = magno.getMat(cv::ACCESS_RW).clone();
                }
                else
                {
                    m_pRetina->getParvo(imgParvo);
                    m_pRetina->getMagno(imgMagno);
                }
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }
            emit m_signalHandler->doProgress();

            auto pOutput1 = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(0));
            if(pOutput1)
                pOutput1->setImage(imgParvo);

            auto pOutput2 = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(1));
            if(pOutput2)
                pOutput2->setImage(imgMagno);

            endTaskRun();
            applyInputGraphicsMask(1, 0, 0, MaskMode::MERGE_SOURCE);
            applyInputGraphicsMask(1, 0, 1, MaskMode::MASK_ONLY);
            emit m_signalHandler->doProgress();
        }

    private:

        cv::Ptr<cv::bioinspired::Retina>    m_pRetina;
        int                                 m_width = 0;
        int                                 m_height = 0;
};

class COcvRetinaFactory : public CProcessFactory
{
    public:

        COcvRetinaFactory()
        {
            m_info.m_name = QObject::tr("Retina").toStdString();
            m_info.m_description = QObject::tr("This retina model allows spatio-temporal image processing (applied on still images, video sequences).").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Biologically inspired vision models and derivated tools").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "conversion,convert,color,gray,contrast,preservation";
            m_info.m_authors = "Alexandre Benoit, Alice Caplier, Barthélémy Durette, and Jeanny Hérault";
            m_info.m_article = "Using human visual system modeling for bio-inspired low level image processing";
            m_info.m_journal = "Computer vision and Image understanding, 114(7):758–773";
            m_info.m_year = 2010;
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/dc/d54/classcv_1_1bioinspired_1_1Retina.html";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pRetinaParam = std::dynamic_pointer_cast<COcvRetinaParam>(pParam);
            if(pRetinaParam != nullptr)
                return std::make_shared<COcvRetina>(m_info.m_name, pRetinaParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pRetinaParam = std::make_shared<COcvRetinaParam>();
            assert(pRetinaParam != nullptr);
            return std::make_shared<COcvRetina>(m_info.m_name, pRetinaParam);
        }
};

#endif // COCVRETINA_HPP
