#ifndef COCVGRADIENTDERICHE_HPP
#define COCVGRADIENTDERICHE_HPP

#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"
#include <opencv2/ximgproc.hpp>

//------------------------------//
//----- COcvGradientDericheParam -----//
//------------------------------//
class COcvGradientDericheParam: public CProtocolTaskParam
{
    public:
        enum orientation : int {
          X,Y
        };
        COcvGradientDericheParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_orientation = static_cast<orientation>(std::stoi(paramMap.at("orientation")));
            m_alphaDerive = std::stod(paramMap.at("alphaDerive"));
            m_alphaMean = std::stod(paramMap.at("alphaMean"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("orientation", std::to_string(m_orientation)));
            map.insert(std::make_pair("alphaDerive", std::to_string(m_alphaDerive)));
            map.insert(std::make_pair("alphaMean", std::to_string(m_alphaMean)));
            return map;
        }

    public:

        orientation     m_orientation = orientation::X;
        double          m_alphaDerive = 1;
        double          m_alphaMean = 1;
};

//-------------------------//
//----- COcvGradientDeriche -----//
//-------------------------//
class COcvGradientDeriche : public CImageProcess2d
{
    public:

        COcvGradientDeriche() : CImageProcess2d()
        {
        }
        COcvGradientDeriche(const std::string name, const std::shared_ptr<COcvGradientDericheParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<COcvGradientDericheParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvGradientDericheParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgDst;
            CMat imgSrc = pInput->getImage();
            createGraphicsMask(imgSrc.getNbCols(), imgSrc.getNbRows(), pGraphicsInput);
            emit m_signalHandler->doProgress();

            try
            {
                if(pParam->m_orientation == COcvGradientDericheParam::orientation::X)
                    cv::ximgproc::GradientDericheX(imgSrc, imgDst, pParam->m_alphaDerive, pParam->m_alphaMean);
                else
                    cv::ximgproc::GradientDericheY(imgSrc, imgDst, pParam->m_alphaDerive, pParam->m_alphaMean);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            endTaskRun();
            applyGraphicsMask(imgSrc, imgDst, 0);
            emit m_signalHandler->doProgress();

            auto pOutput = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(imgDst);

            emit m_signalHandler->doProgress();
        }
};

class COcvGradientDericheFactory : public CProcessFactory
{
    public:

        COcvGradientDericheFactory()
        {
            m_info.m_name = QObject::tr("Deriche gradient filter").toStdString();
            m_info.m_description = QObject::tr("Applies X or Y Deriche filter to an image").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Extended Image Processing/Filters").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Gradient,Derivation";
            m_info.m_authors = "Rachid Deriche";
            m_info.m_article = "Using Canny's criteria to derive a recursively implemented optimal edge detector.";
            m_info.m_journal = "IJCV, 167-187";
            m_info.m_year = 1987;
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/da/d17/group__ximgproc__filters.html#gaa897f2b095cf81458d91211a768bea70";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvGradientDericheParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvGradientDeriche>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvGradientDericheParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvGradientDeriche>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVGRADIENTDERICHE_HPP
