#ifndef CGMICDISTANCETRANSFORM_HPP
#define CGMICDISTANCETRANSFORM_HPP

#include "Core/CImageProcess2d.h"
#include "Process/Gmic/CGmicTaskParam.hpp"
#include "Process/Gmic/CGmicImageConverter.h"

//---------------------------------------//
//----- CGmicDistanceTransformParam -----//
//---------------------------------------//
class CGmicDistanceTransformParam: public CGmicTaskParam
{
    public:

        enum Metric { CHEBYSHEV, MANHATTAN, EUCLIDEAN, SQUARE_EUCLIDEAN };
        enum Normalization { CUT, NORMALIZE, MODULO };

        CGmicDistanceTransformParam() : CGmicTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_value = std::stoi(paramMap.at("value"));
            m_metric = std::stoi(paramMap.at("metric"));
            m_normalization = std::stoi(paramMap.at("normalization"));
            m_modulo = std::stoi(paramMap.at("modulo"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("value", std::to_string(m_value)));
            map.insert(std::make_pair("metric", std::to_string(m_metric)));
            map.insert(std::make_pair("normalization", std::to_string(m_normalization)));
            map.insert(std::make_pair("modulo", std::to_string(m_modulo)));
            return map;
        }

        std::string getParamString() const override
        {
            //Build string of parameters, take care of order.
            return std::to_string(m_value) + "," + std::to_string(m_metric) + "," + std::to_string(m_normalization) + "," +
                    std::to_string(m_modulo);
        }

    public:

        int     m_value = 128;
        int     m_metric = EUCLIDEAN;
        int     m_normalization = MODULO;
        int     m_modulo = 32;
};

//----------------------------------//
//----- CGmicDistanceTransform -----//
//----------------------------------//
class CGmicDistanceTransform : public CImageProcess2d
{
    public:

        CGmicDistanceTransform() : CImageProcess2d()
        {
        }
        CGmicDistanceTransform(const std::string name, const std::shared_ptr<CGmicDistanceTransformParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<CGmicDistanceTransformParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<CGmicDistanceTransformParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid input", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgSrc = pInput->getImage();
            CMat imgDst(imgSrc.rows, imgSrc.cols, imgSrc.type());
            createGraphicsMask(imgSrc.getNbCols(), imgSrc.getNbRows(), pGraphicsInput);
            emit m_signalHandler->doProgress();

            try
            {
                //Put image inputs into a gmic_list object
                gmic_list<float> imageList;
                gmic_list<char> image_names;

                //Allocate list, parameter is the number of image inputs
                imageList.assign(1);
                //Conversion from CMat(cv::Mat) to gmic_image(CImg)
                CGmicImageConverter::convert(imgSrc, imageList[0]);
                //Build command for gmic interpreter
                std::string command = "fx_distance[0] " + pParam->getParamString();
                //Call interpreter
                gmic(command.c_str(), imageList, image_names);
                //Conversion from gmic_image to CMat
                CGmicImageConverter::convert(imageList[0], imgDst);
                //Free memory
                imageList.assign(0);
            }
            catch(gmic_exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e.what(), __func__, __FILE__, __LINE__);
            }

            endTaskRun();
            emit m_signalHandler->doProgress();

            applyGraphicsMask(imgSrc, imgDst, 0);

            auto pOutput = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(imgDst);

            emit m_signalHandler->doProgress();
        }
};

class CGmicDistanceTransformFactory : public CProcessFactory
{
    public:

        CGmicDistanceTransformFactory()
        {
            m_info.m_name = QObject::tr("Distance transform").toStdString();
            m_info.m_description = QObject::tr("This process performs Distance Transform algorithm.").toStdString();
            m_info.m_path = QObject::tr("Gmic/Contours").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/gmic.png").toStdString();
            m_info.m_keywords = "binary,edge,separate";
            m_info.m_authors = "David Tschumperlé";
            m_info.m_year = 2011;
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<CGmicDistanceTransformParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<CGmicDistanceTransform>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pParam = std::make_shared<CGmicDistanceTransformParam>();
            assert(pParam != nullptr);
            return std::make_shared<CGmicDistanceTransform>(m_info.m_name, pParam);
        }
};

#endif // CGMICDISTANCETRANSFORM_HPP
