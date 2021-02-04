#ifndef COCVCOUNTNONZERO_HPP
#define COCVCOUNTNONZERO_HPP

#include "Core/CImageProcess2d.h"

//---------------------------------//
//----- COcvCountNonZeroParam -----//
//---------------------------------//
class COcvCountNonZeroParam: public CProtocolTaskParam
{
    public:

        COcvCountNonZeroParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            Q_UNUSED(paramMap);
        }

        UMapString  getParamMap() const override
        {
            return UMapString();
        }
};

//----------------------------//
//----- COcvCountNonZero -----//
//----------------------------//
class COcvCountNonZero : public CImageProcess2d
{
    public:

        COcvCountNonZero() : CImageProcess2d(false)
        {
            removeOutput(0);
            addOutput(std::make_shared<CFeatureProcessIO<int>>());
        }
        COcvCountNonZero(const std::string name, const std::shared_ptr<COcvCountNonZeroParam>& pParam) : CImageProcess2d(name, false)
        {
            m_pParam = std::make_shared<COcvCountNonZeroParam>(*pParam);
            removeOutput(0);
            addOutput(std::make_shared<CFeatureProcessIO<int>>());
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        void run() override
        {
            beginTaskRun();

            if(getInputCount() < 1)
                throw CException(CoreExCode::INVALID_PARAMETER, "Not enough inputs", __func__, __FILE__, __LINE__);

            auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            if(pInput == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            int nonZeroCount = 0;
            emit m_signalHandler->doProgress();

            try
            {
                nonZeroCount = cv::countNonZero(pInput->getImage());
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            endTaskRun();
            emit m_signalHandler->doProgress();

            auto pOutput = std::dynamic_pointer_cast<CFeatureProcessIO<int>>(getOutput(0));
            if(pOutput)
            {
                std::vector<int> values;
                values.push_back(nonZeroCount);
                pOutput->addValueList(values, "Number of non-zero elements");
            }
            emit m_signalHandler->doProgress();
        }
};

class COcvCountNonZeroFactory : public CProcessFactory
{
    public:

        COcvCountNonZeroFactory()
        {
            m_info.m_name = QObject::tr("CountNonZero").toStdString();
            m_info.m_description = QObject::tr("The function returns the number of non-zero elements in source image.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Core functionality/Operations on arrays").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "count,zero";
            m_info.m_docLink = "https://docs.opencv.org/4.0.1/d2/de8/group__core__array.html#gaa4b89393263bb4d604e0fe5986723914";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvCountNonZeroParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvCountNonZero>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvCountNonZeroParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvCountNonZero>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVCOUNTNONZERO_HPP
