#ifndef COCVINPAINTFUZZY_HPP
#define COCVINPAINTFUZZY_HPP

#include "Core/CImageProcess2d.h"
#include "opencv2/fuzzy/fuzzy_image.hpp"

//---------------------------------//
//----- COcvInpaintFuzzyParam -----//
//---------------------------------//
class COcvInpaintFuzzyParam: public CProtocolTaskParam
{
    public:

        COcvInpaintFuzzyParam() : CProtocolTaskParam()
        {
        }

        void        setParamMap(const UMapString& paramMap) override
        {
            m_radius = std::stod(paramMap.at("radius"));
            m_method = std::stoi(paramMap.at("method"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("radius", std::to_string(m_radius)));
            map.insert(std::make_pair("method", std::to_string(m_method)));
            return map;
        }

    public:

        double  m_radius = 10;
        int     m_method = cv::ft::ONE_STEP;
};

//----------------------------//
//----- COcvInpaintFuzzy -----//
//----------------------------//
class COcvInpaintFuzzy: public CImageProcess2d
{
    public:

        COcvInpaintFuzzy() : CImageProcess2d()
        {
        }
        COcvInpaintFuzzy(const std::string name, const std::shared_ptr<COcvInpaintFuzzyParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<COcvInpaintFuzzyParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvInpaintFuzzyParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgDst;
            CMat imgSrc = pInput->getImage();
            createGraphicsMask(imgSrc.getNbCols(), imgSrc.getNbRows(), pGraphicsInput);
            emit m_signalHandler->doProgress();

            auto mask = getGraphicsMask(0);
            if(mask.data == nullptr)
                throw CException(CoreExCode::NULL_POINTER, "Empty region of interest", __func__, __FILE__, __LINE__);

            try
            {
                cv::bitwise_not(mask, mask);
                cv::ft::inpaint(imgSrc, mask, imgDst, pParam->m_radius, cv::ft::LINEAR, pParam->m_method);
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

//-----------------------------------//
//----- COcvInpaintFuzzyFactory -----//
//-----------------------------------//
class COcvInpaintFuzzyFactory : public CProcessFactory
{
    public:

        COcvInpaintFuzzyFactory()
        {
            m_info.m_name = QObject::tr("Inpaint fuzzy filter").toStdString();
            m_info.m_description = QObject::tr("This process performs inpainting from region of interest drawn in original image.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Fuzzy mathematics/Fuzzy image processing").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "inpainting,photo,deletion,fuzzy,mathematics";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d1/dfc/group__f__image.html#ga62fa7703db24939ad65d7a799e8e599e";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvInpaintFuzzyParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvInpaintFuzzy>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvInpaintFuzzyParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvInpaintFuzzy>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVINPAINTFUZZY_HPP
