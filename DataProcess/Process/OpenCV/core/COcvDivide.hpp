#ifndef COCVDIVIDE_HPP
#define COCVDIVIDE_HPP

#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"

//--------------------------------//
//----- COcvDivideParam -----//
//--------------------------------//
class COcvDivideParam: public CProtocolTaskParam
{
    public:

        COcvDivideParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_scale = std::stod(paramMap.at("scale"));
            m_dtype = std::stod(paramMap.at("dtype"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("scale", std::to_string(m_scale)));
            map.insert(std::make_pair("dtype", std::to_string(m_dtype)));
            return map;
        }

    public:

        double  m_scale = 1;
        int     m_dtype = -1;
};

//---------------------------//
//----- COcvDivide -----//
//---------------------------//
class COcvDivide : public CImageProcess2d
{
    public:

        COcvDivide() : CImageProcess2d()
        {
            insertInput(std::make_shared<CImageProcessIO>(), 0);
        }
        COcvDivide(const std::string name, const std::shared_ptr<COcvDivideParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<COcvDivideParam>(*pParam);
            insertInput(std::make_shared<CImageProcessIO>(), 0);
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        void    run() override
        {
            beginTaskRun();

            if(getInputCount() < 2)
                throw CException(CoreExCode::INVALID_PARAMETER, "Not enough inputs", __func__, __FILE__, __LINE__);

            auto pInput1 = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pInput2 = std::dynamic_pointer_cast<CImageProcessIO>(getInput(1));
            auto pParam = std::dynamic_pointer_cast<COcvDivideParam>(m_pParam);

            if(pInput1 == nullptr || pInput2 == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput1->isDataAvailable() == false || pInput2->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty images", __func__, __FILE__, __LINE__);

            emit m_signalHandler->doProgress();

            CMat imgDst;
            CMat imgSrc1 = pInput1->getImage();
            CMat imgSrc2 = pInput2->getImage();
            try
            {
                cv::divide(imgSrc1, imgSrc2, imgDst, pParam->m_scale, pParam->m_dtype);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            emit m_signalHandler->doProgress();
            auto pOutput = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(imgDst);

            endTaskRun();
            applyInputGraphicsMask(2, 0, 0, MaskMode::MASK_ONLY);
            emit m_signalHandler->doProgress();
        }
};

class COcvDivideFactory : public CProcessFactory
{
    public:

        COcvDivideFactory()
        {
            m_info.m_name = QObject::tr("Divide").toStdString();
            m_info.m_description = QObject::tr("The function divides one array by another with the given formula:<br> <center><i>dst = saturate( scale * src1 / src2 )</i></center>").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Core functionality/Operations on arrays").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Arithmetic,Divide,Per-element";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d2/de8/group__core__array.html#ga6db555d30115642fedae0cda05604874";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvDivideParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvDivide>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvDivideParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvDivide>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVDIVIDE_HPP
