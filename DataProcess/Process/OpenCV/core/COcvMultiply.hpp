#ifndef COCVMULTIPLY_HPP
#define COCVMULTIPLY_HPP

#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"

//--------------------------------//
//----- COcvMultiplyParam -----//
//--------------------------------//
class COcvMultiplyParam: public CProtocolTaskParam
{
    public:

        COcvMultiplyParam() : CProtocolTaskParam(){}

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
//----- COcvMultiply -----//
//---------------------------//
class COcvMultiply : public CImageProcess2d
{
    public:

        COcvMultiply() : CImageProcess2d()
        {
            insertInput(std::make_shared<CImageProcessIO>(), 1);
        }
        COcvMultiply(const std::string name, const std::shared_ptr<COcvMultiplyParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<COcvMultiplyParam>(*pParam);
            insertInput(std::make_shared<CImageProcessIO>(), 1);
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        void run() override
        {
            beginTaskRun();

            if(getInputCount() < 2)
                throw CException(CoreExCode::INVALID_PARAMETER, "Not enough inputs", __func__, __FILE__, __LINE__);

            auto pInput1 = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pInput2 = std::dynamic_pointer_cast<CImageProcessIO>(getInput(1));
            auto pParam = std::dynamic_pointer_cast<COcvMultiplyParam>(m_pParam);

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
                cv::multiply(imgSrc1, imgSrc2, imgDst, pParam->m_scale, pParam->m_dtype);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }
            emit m_signalHandler->doProgress();

            auto pOutput = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(imgDst);

            applyInputGraphicsMask(2, 0, 0, MaskMode::MASK_ONLY);
            endTaskRun();
            emit m_signalHandler->doProgress();
        }
};

class COcvMultiplyFactory : public CProcessFactory
{
    public:

        COcvMultiplyFactory()
        {
            m_info.m_name = QObject::tr("Multiply").toStdString();
            m_info.m_description = QObject::tr("The function Multiply calculates the per-element product of two arrays: with the given formula:<br> <center><i>dst = saturate( scale * src1 * src2 )</i></center>").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Core functionality/Operations on arrays").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Arithmetic,Multiply,Per-element";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d2/de8/group__core__array.html#ga979d898a58d7f61c53003e162e7ad89f";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvMultiplyParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvMultiply>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvMultiplyParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvMultiply>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVMULTIPLY_HPP
