#ifndef COCVRETINATONEMAPPING_HPP
#define COCVRETINATONEMAPPING_HPP

#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"
#include <opencv2/bioinspired.hpp>

//----------------------------//
//----- COcvRetinaToneMappingParam -----//
//----------------------------//
class COcvRetinaToneMappingParam: public CProtocolTaskParam
{
    public:

        COcvRetinaToneMappingParam() : CProtocolTaskParam(){}

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
//----- COcvRetinaToneMapping -----//
//-----------------------//
class COcvRetinaToneMapping : public CImageProcess2d
{
    public:

        COcvRetinaToneMapping() : CImageProcess2d()
        {
        }
        COcvRetinaToneMapping(const std::string name, const std::shared_ptr<COcvRetinaToneMappingParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<COcvRetinaToneMappingParam>(*pParam);
        }

        size_t     getProgressSteps() override
        {
            return 3;
        }

        void    run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pParam = std::dynamic_pointer_cast<COcvRetinaToneMappingParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgDst;
            CMat imgSrc = pInput->getImage();

            emit m_signalHandler->doProgress();

            try
            {
                auto pRetina = cv::bioinspired::RetinaFastToneMapping::create(imgSrc.size());
                //pRetina->setup(pParam->m_photoreceptorsNeighborhoodRadius, pParam->m_ganglioncellsNeighborhoodRadius, pParam->m_meanLuminanceModulatorK);
                pRetina->applyFastToneMapping(imgSrc, imgDst);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            endTaskRun();
            emit m_signalHandler->doProgress();

            auto pOutput = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(imgDst);

            emit m_signalHandler->doProgress();
        }
};

class COcvRetinaToneMappingFactory : public CProcessFactory
{
    public:

        COcvRetinaToneMappingFactory()
        {
            m_info.m_name = QObject::tr("RetinaToneMapping").toStdString();
            m_info.m_description = QObject::tr("This RetinaToneMapping model allows spatio-temporal image processing (applied on still images, video sequences).").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Biologically inspired vision models and derivated tools").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "conversion,convert,color,gray,contrast,preservation";
            m_info.m_authors = "Alexandre Benoit, Alice Caplier, Barthélémy Durette, and Jeanny Hérault";
            m_info.m_article = "Using human visual system modeling for bio-inspired low level image processing";
            m_info.m_journal = "Computer vision and Image understanding, 114(7):758–773";
            m_info.m_year = 2010;
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/dd/d73/classcv_1_1bioinspired_1_1RetinaFastToneMapping.html";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pRetinaToneMappingParam = std::dynamic_pointer_cast<COcvRetinaToneMappingParam>(pParam);
            if(pRetinaToneMappingParam != nullptr)
                return std::make_shared<COcvRetinaToneMapping>(m_info.m_name, pRetinaToneMappingParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pRetinaToneMappingParam = std::make_shared<COcvRetinaToneMappingParam>();
            assert(pRetinaToneMappingParam != nullptr);
            return std::make_shared<COcvRetinaToneMapping>(m_info.m_name, pRetinaToneMappingParam);
        }
};

#endif // COCVRETINATONEMAPPING_HPP
