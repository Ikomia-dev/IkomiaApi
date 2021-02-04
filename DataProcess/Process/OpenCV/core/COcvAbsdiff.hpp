#ifndef COCVABSDIFF_HPP
#define COCVABSDIFF_HPP

#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"

//--------------------------------//
//----- COcvAbsdiffParam -----//
//--------------------------------//
class COcvAbsdiffParam: public CProtocolTaskParam
{
    public:

        COcvAbsdiffParam() : CProtocolTaskParam(){}

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

//---------------------------//
//----- COcvAbsdiff -----//
//---------------------------//
class COcvAbsdiff : public CImageProcess2d
{
    public:

        COcvAbsdiff() : CImageProcess2d()
        {
            insertInput(std::make_shared<CImageProcessIO>(), 0);
        }
        COcvAbsdiff(const std::string name, const std::shared_ptr<COcvAbsdiffParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<COcvAbsdiffParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvAbsdiffParam>(m_pParam);

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
                cv::absdiff(imgSrc1, imgSrc2, imgDst);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            auto pOutput = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(imgDst);

            endTaskRun();
            applyInputGraphicsMask(2, 0, 0, MaskMode::MASK_ONLY);
            emit m_signalHandler->doProgress();

            emit m_signalHandler->doProgress();
        }
};

class COcvAbsdiffFactory : public CProcessFactory
{
    public:

        COcvAbsdiffFactory()
        {
            m_info.m_name = QObject::tr("Abs diff").toStdString();
            m_info.m_description = QObject::tr("The function calculates the per-element absolute difference between two arrays or between an array and a scalar with the given formula:<br> <center><i>dst = saturate( |src1 - src2| )</i></center>").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Core functionality/Operations on arrays").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Arithmetic,Absdiff,Per-element";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d2/de8/group__core__array.html#ga6fef31bc8c4071cbc114a758a2b79c14";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvAbsdiffParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvAbsdiff>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvAbsdiffParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvAbsdiff>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVABSDIFF_HPP
