#ifndef COCVSUPERPIXELLSC_HPP
#define COCVSUPERPIXELLSC_HPP

#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"
#include <opencv2/ximgproc.hpp>

//----------------------------//
//----- COcvSuperpixelLSCParam -----//
//----------------------------//
class COcvSuperpixelLSCParam: public CProtocolTaskParam
{
    public:

        COcvSuperpixelLSCParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
             m_regions_size = std::stoi(paramMap.at("regions_size"));
             m_ratio = std::stof(paramMap.at("ratio"));
             m_num_iterations = std::stoi(paramMap.at("num_iterations"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("regions_size", std::to_string(m_regions_size)));
            map.insert(std::make_pair("ratio", std::to_string(m_ratio)));
            map.insert(std::make_pair("num_iterations", std::to_string(m_num_iterations)));
            return map;
        }

    public:
        int     m_regions_size = 10;
        float   m_ratio = 0.075f;
        int     m_num_iterations = 10;
};

//-----------------------------//
//----- COcvSuperpixelLSC -----//
//-----------------------------//
class COcvSuperpixelLSC : public CImageProcess2d
{
    public:

        COcvSuperpixelLSC() : CImageProcess2d()
        {
            setOutputDataType(IODataType::IMAGE_LABEL, 0);
            addOutput(std::make_shared<CImageProcessIO>(IODataType::IMAGE_BINARY));
            addOutput(std::make_shared<CImageProcessIO>());
            setOutputColorMap(2, 0);
        }
        COcvSuperpixelLSC(const std::string name, const std::shared_ptr<COcvSuperpixelLSCParam>& pParam) : CImageProcess2d(name)
        {
            setOutputDataType(IODataType::IMAGE_LABEL, 0);
            addOutput(std::make_shared<CImageProcessIO>(IODataType::IMAGE_BINARY));
            addOutput(std::make_shared<CImageProcessIO>());
            setOutputColorMap(2, 0);
            m_pParam = std::make_shared<COcvSuperpixelLSCParam>(*pParam);
        }

        size_t     getProgressSteps() override
        {
            return 3;
        }

        void    run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsProcessInput>(getInput(1));
            auto pParam = std::dynamic_pointer_cast<COcvSuperpixelLSCParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgDst, imgBin;
            CMat imgSrc = pInput->getImage();

            createGraphicsMask(imgSrc.getNbCols(), imgSrc.getNbRows(), pGraphicsInput);
            emit m_signalHandler->doProgress();

            try
            {
                m_pSuperpixel = cv::ximgproc::createSuperpixelLSC(imgSrc, pParam->m_regions_size, pParam->m_ratio);
                m_pSuperpixel->iterate(pParam->m_num_iterations);
                m_pSuperpixel->getLabels(imgDst);
                m_pSuperpixel->getLabelContourMask(imgBin);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            emit m_signalHandler->doProgress();
            applyGraphicsMask(imgSrc, imgDst, 0);
            forwardInputImage(0, 2);

            auto pOutput1 = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(0));
            if(pOutput1)
                pOutput1->setImage(imgDst);

            auto pOutput2 = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(1));
            if(pOutput2)
                pOutput2->setImage(imgBin);

            endTaskRun();
            emit m_signalHandler->doProgress();
        }
    private:
        cv::Ptr<cv::ximgproc::SuperpixelLSC> m_pSuperpixel;
};

class COcvSuperpixelLSCFactory : public CProcessFactory
{
    public:

        COcvSuperpixelLSCFactory()
        {
            m_info.m_name = QObject::tr("SuperpixelLSC").toStdString();
            m_info.m_description = QObject::tr("LSC (Linear Spectral Clustering) produces compact and uniform superpixels with low computational costs. Basically, a normalized cuts formulation of the superpixel segmentation is adopted based on a similarity metric that measures the color similarity and space proximity between image pixels. LSC is of linear computational complexity and high memory efficiency and is able to preserve global properties of images ").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Extended Image Processing/Superpixels").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Segmentation,Edges,Contours";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d5/da0/classcv_1_1ximgproc_1_1SuperpixelLSC.html";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pSuperpixelLSCParam = std::dynamic_pointer_cast<COcvSuperpixelLSCParam>(pParam);
            if(pSuperpixelLSCParam != nullptr)
                return std::make_shared<COcvSuperpixelLSC>(m_info.m_name, pSuperpixelLSCParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pSuperpixelLSCParam = std::make_shared<COcvSuperpixelLSCParam>();
            assert(pSuperpixelLSCParam != nullptr);
            return std::make_shared<COcvSuperpixelLSC>(m_info.m_name, pSuperpixelLSCParam);
        }
};

#endif // COCVSUPERPIXELLSC_HPP
