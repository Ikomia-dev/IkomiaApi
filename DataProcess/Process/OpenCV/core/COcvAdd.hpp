#ifndef COCVADD_HPP
#define COCVADD_HPP

#include "Core/CImageProcess2d.h"

//--------------------------------//
//----- COcvAddParam -----//
//--------------------------------//
class COcvAddParam: public CProtocolTaskParam
{
    public:

        COcvAddParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_dtype = std::stod(paramMap.at("dtype"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("dtype", std::to_string(m_dtype)));
            return map;
        }

    public:

        int     m_dtype = -1;
};

//---------------------------//
//----- COcvAdd -----//
//---------------------------//
class COcvAdd : public CImageProcess2d
{
    public:

        COcvAdd() : CImageProcess2d()
        {
            insertInput(std::make_shared<CImageProcessIO>(), 0);
        }
        COcvAdd(const std::string name, const std::shared_ptr<COcvAddParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<COcvAddParam>(*pParam);
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
            auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsProcessInput>(getInput(2));
            auto pParam = std::dynamic_pointer_cast<COcvAddParam>(m_pParam);

            if(pInput1 == nullptr || pInput2 == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput1->isDataAvailable() == false || pInput2->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty images", __func__, __FILE__, __LINE__);

            emit m_signalHandler->doProgress();

            CMat imgDst;
            CMat imgSrc1 = pInput1->getImage();
            CMat imgSrc2 = pInput2->getImage();
            createGraphicsMask(imgSrc1.getNbCols(), imgSrc1.getNbRows(), pGraphicsInput);

            try
            {
                auto mask = getGraphicsMask(0);
                cv::add(imgSrc1, imgSrc2, imgDst, mask, pParam->m_dtype);
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
            emit m_signalHandler->doProgress();
        }
};

class COcvAddFactory : public CProcessFactory
{
    public:

        COcvAddFactory()
        {
            m_info.m_name = QObject::tr("Add").toStdString();
            m_info.m_description = QObject::tr("The function calculates the per-element sum of two arrays with the given formula:<br> <center><i>dst = saturate( src1 + src2 )</i></center>").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Core functionality/Operations on arrays").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Arithmetic,Add,Per-element";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d2/de8/group__core__array.html#ga10ac1bfb180e2cfda1701d06c24fdbd6";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvAddParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvAdd>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvAddParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvAdd>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVADD_HPP
