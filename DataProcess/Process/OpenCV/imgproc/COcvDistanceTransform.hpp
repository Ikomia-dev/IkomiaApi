#ifndef COCVDISTANCETRANSFORM_HPP
#define COCVDISTANCETRANSFORM_HPP

#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"
#include "opencv2/ximgproc.hpp"

//-----------------------------//
//----- COcvDistanceTransformParam -----//
//-----------------------------//
class COcvDistanceTransformParam: public CProtocolTaskParam
{
    public:

        COcvDistanceTransformParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_bVoronoi = std::stoi(paramMap.at("isVoronoi"));
            m_type = std::stoi(paramMap.at("type"));
            m_maskSize = std::stoi(paramMap.at("maskSize"));
            m_labelType = std::stoi(paramMap.at("labelType"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("isVoronoi", std::to_string(m_bVoronoi)));
            map.insert(std::make_pair("type", std::to_string(m_type)));
            map.insert(std::make_pair("maskSize", std::to_string(m_maskSize)));
            map.insert(std::make_pair("labelType", std::to_string(m_labelType)));
            return map;
        }

    public:

        bool    m_bVoronoi = false;
        int     m_type = cv::DIST_L1;
        int     m_maskSize = cv::DIST_MASK_3;
        int     m_labelType = cv::DIST_LABEL_CCOMP;
};

//-------------------------//
//----- COcvDistanceTransform -----//
//-------------------------//
class COcvDistanceTransform : public CImageProcess2d
{
    public:

        COcvDistanceTransform() : CImageProcess2d()
        {
            getInput(0)->setDataType(IODataType::IMAGE_BINARY);
        }
        COcvDistanceTransform(const std::string name, const std::shared_ptr<COcvDistanceTransformParam>& pParam) : CImageProcess2d(name)
        {
            getInput(0)->setDataType(IODataType::IMAGE_BINARY);
            m_pParam = std::make_shared<COcvDistanceTransformParam>(*pParam);
            parametersModified();
        }

        void    setParam(const ProtocolTaskParamPtr &pParam) override
        {
            m_pParam = pParam;
            parametersModified();
        }

        void    parametersModified() override
        {
            auto pParam = std::dynamic_pointer_cast<COcvDistanceTransformParam>(m_pParam);
            if(pParam == nullptr)
                return;

            if(pParam->m_bVoronoi == true && getOutputCount() == 1)
                addOutput(std::make_shared<CImageProcessIO>());
            else if(pParam->m_bVoronoi == false && getOutputCount() == 2)
                removeOutput(getOutputCount() - 1);
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        void    run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsProcessInput>(getInput(1));
            auto pParam = std::dynamic_pointer_cast<COcvDistanceTransformParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgDst, labels;
            CMat imgSrc = pInput->getImage();
            createGraphicsMask(imgSrc.getNbCols(), imgSrc.getNbRows(), pGraphicsInput);
            emit m_signalHandler->doProgress();

            try
            {
                if(pParam->m_bVoronoi == false)
                    cv::distanceTransform(imgSrc, imgDst, pParam->m_type, pParam->m_maskSize);
                else
                    cv::distanceTransform(imgSrc, imgDst, labels, pParam->m_type, pParam->m_maskSize, pParam->m_labelType);
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

            if(pParam->m_bVoronoi ==  true)
            {
                auto pOutputVoronoi = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(1));
                if(pOutputVoronoi)
                    pOutputVoronoi->setImage(labels);
            }

            emit m_signalHandler->doProgress();
        }
};

class COcvDistanceTransformFactory : public CProcessFactory
{
    public:

        COcvDistanceTransformFactory()
        {
            m_info.m_name = QObject::tr("Distance Transform").toStdString();
            m_info.m_description = QObject::tr("The function calculates the approximate or precise distance from every binary image pixel to the nearest zero pixel. For zero image pixels, the distance will obviously be zero.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Image processing/Miscellaneous image transformations").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "distance transform,binary,voronoi";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d7/d1b/group__imgproc__misc.html#ga8a0b7fdfcb7a13dde018988ba3a43042";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvDistanceTransformParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvDistanceTransform>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvDistanceTransformParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvDistanceTransform>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVDISTANCETRANSFORM_HPP
