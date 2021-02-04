#ifndef COCVSELECTIVESEARCHSEGMENTATION_HPP
#define COCVSELECTIVESEARCHSEGMENTATION_HPP

#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"
#include <opencv2/ximgproc.hpp>

//------------------------------------------------//
//----- COcvSelectiveSearchSegmentationParam -----//
//------------------------------------------------//
class COcvSelectiveSearchSegmentationParam: public CProtocolTaskParam
{
    public:

        enum InitializationMethod{SINGLE, FAST, QUALITY};

        COcvSelectiveSearchSegmentationParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_sigma = std::stod(paramMap.at("sigma"));
            m_baseK = std::stoi(paramMap.at("baseK"));
            m_stepK = std::stoi(paramMap.at("stepK"));
            m_initMethod = std::stoi(paramMap.at("initMethod"));
            m_bDefaultStrategy = std::stoi(paramMap.at("isDefaultStrategy"));
            m_bColorStrategy = std::stoi(paramMap.at("isColorStrategy"));
            m_colorWeight = std::stod(paramMap.at("colorWeight"));
            m_bFillStrategy = std::stoi(paramMap.at("isFillStrategy"));
            m_fillWeight = std::stod(paramMap.at("fillWeight"));
            m_bSizeStrategy = std::stoi(paramMap.at("isSizeStrategy"));
            m_sizeWeight = std::stod(paramMap.at("sizeWeight"));
            m_bTextureStrategy = std::stoi(paramMap.at("isTextureStrategy"));
            m_textureWeight = std::stod(paramMap.at("textureWeight"));
            m_nbRects = std::stoi(paramMap.at("nbRects"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("sigma", std::to_string(m_sigma)));
            map.insert(std::make_pair("baseK", std::to_string(m_baseK)));
            map.insert(std::make_pair("stepK", std::to_string(m_stepK)));
            map.insert(std::make_pair("initMethod", std::to_string(m_initMethod)));
            map.insert(std::make_pair("isDefaultStrategy", std::to_string(m_bDefaultStrategy)));
            map.insert(std::make_pair("isColorStrategy", std::to_string(m_bColorStrategy)));
            map.insert(std::make_pair("colorWeight", std::to_string(m_colorWeight)));
            map.insert(std::make_pair("isFillStrategy", std::to_string(m_bFillStrategy)));
            map.insert(std::make_pair("fillWeight", std::to_string(m_fillWeight)));
            map.insert(std::make_pair("isSizeStrategy", std::to_string(m_bSizeStrategy)));
            map.insert(std::make_pair("sizeWeight", std::to_string(m_sizeWeight)));
            map.insert(std::make_pair("isTextureStrategy", std::to_string(m_bTextureStrategy)));
            map.insert(std::make_pair("textureWeight", std::to_string(m_textureWeight)));
            map.insert(std::make_pair("nbRects", std::to_string(m_nbRects)));
            return map;
        }

    public:

        double  m_sigma = 0.8;
        int     m_baseK = 150;
        int     m_stepK = 150;
        int     m_initMethod = SINGLE;
        bool    m_bDefaultStrategy = true;
        bool    m_bColorStrategy = false;
        float   m_colorWeight = 0.25;
        bool    m_bFillStrategy = false;
        float   m_fillWeight = 0.25;
        bool    m_bSizeStrategy = false;
        float   m_sizeWeight = 0.25;
        bool    m_bTextureStrategy = false;
        float   m_textureWeight = 0.25;
        int     m_nbRects = 50;
};

//-------------------------------------------//
//----- COcvSelectiveSearchSegmentation -----//
//-------------------------------------------//
class COcvSelectiveSearchSegmentation : public CImageProcess2d
{
    public:

        COcvSelectiveSearchSegmentation() : CImageProcess2d()
        {
            addOutput(std::make_shared<CGraphicsProcessOutput>());
        }
        COcvSelectiveSearchSegmentation(const std::string name, const std::shared_ptr<COcvSelectiveSearchSegmentationParam>& pParam) : CImageProcess2d(name)
        {
            addOutput(std::make_shared<CGraphicsProcessOutput>());
            m_pParam = std::make_shared<COcvSelectiveSearchSegmentationParam>(*pParam);
        }

        size_t  getProgressSteps() override
        {
            return 4;
        }

        void    run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pParam = std::dynamic_pointer_cast<COcvSelectiveSearchSegmentationParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            std::vector<cv::Rect> rects;
            CMat imgSrc = pInput->getImage();
            emit m_signalHandler->doProgress();

            try
            {
                auto sssPtr = cv::ximgproc::segmentation::createSelectiveSearchSegmentation();
                sssPtr->setBaseImage(imgSrc);

                switch(pParam->m_initMethod)
                {
                    case COcvSelectiveSearchSegmentationParam::SINGLE:
                        sssPtr->switchToSingleStrategy(pParam->m_baseK, pParam->m_sigma);
                        break;
                    case COcvSelectiveSearchSegmentationParam::FAST:
                        sssPtr->switchToSelectiveSearchFast(pParam->m_baseK, pParam->m_stepK, pParam->m_sigma);
                        break;
                    case COcvSelectiveSearchSegmentationParam::QUALITY:
                        sssPtr->switchToSelectiveSearchQuality(pParam->m_baseK, pParam->m_stepK, pParam->m_sigma);
                        break;
                }

                if(pParam->m_bDefaultStrategy == false)
                {
                    auto multiStrategyPtr = cv::ximgproc::segmentation::createSelectiveSearchSegmentationStrategyMultiple();
                    if(pParam->m_bColorStrategy == true)
                    {
                        auto colorStrategyPtr = cv::ximgproc::segmentation::createSelectiveSearchSegmentationStrategyColor();
                        multiStrategyPtr->addStrategy(colorStrategyPtr, pParam->m_colorWeight);
                    }

                    if(pParam->m_bFillStrategy == true)
                    {
                        auto fillStrategyPtr = cv::ximgproc::segmentation::createSelectiveSearchSegmentationStrategyFill();
                        multiStrategyPtr->addStrategy(fillStrategyPtr, pParam->m_fillWeight);
                    }

                    if(pParam->m_bSizeStrategy == true)
                    {
                        auto sizeStrategyPtr = cv::ximgproc::segmentation::createSelectiveSearchSegmentationStrategySize();
                        multiStrategyPtr->addStrategy(sizeStrategyPtr, pParam->m_sizeWeight);
                    }

                    if(pParam->m_bTextureStrategy == true)
                    {
                        auto textureStrategyPtr = cv::ximgproc::segmentation::createSelectiveSearchSegmentationStrategyTexture();
                        multiStrategyPtr->addStrategy(textureStrategyPtr, pParam->m_textureWeight);
                    }
                    sssPtr->addStrategy(multiStrategyPtr);
                }
                sssPtr->process(rects);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            endTaskRun();
            emit m_signalHandler->doProgress();
            manageOutputs(rects);
            emit m_signalHandler->doProgress();
            forwardInputImage();
            emit m_signalHandler->doProgress();
        }

        void    manageOutputs(const std::vector<cv::Rect>& rects)
        {
            auto pGraphicsOutput = std::dynamic_pointer_cast<CGraphicsProcessOutput>(getOutput(1));
            if(pGraphicsOutput == nullptr)
                throw CException(CoreExCode::NULL_POINTER, "No valid graphics output found", __func__, __FILE__, __LINE__);

            pGraphicsOutput->setNewLayer("Selective search segmentation");
            pGraphicsOutput->setImageIndex(0);
            auto pParam = std::dynamic_pointer_cast<COcvSelectiveSearchSegmentationParam>(m_pParam);
            size_t nbRects = std::min((size_t)pParam->m_nbRects, rects.size());

            //Create rectangle graphics of bbox
            for(size_t i=0; i<nbRects; ++i)
                pGraphicsOutput->addRectangle(rects[i].x, rects[i].y, rects[i].width, rects[i].height);
        }
};

class COcvSelectiveSearchSegmentationFactory : public CProcessFactory
{
    public:

        COcvSelectiveSearchSegmentationFactory()
        {
            m_info.m_name = QObject::tr("Selective Search Segmentation").toStdString();
            m_info.m_description = QObject::tr("This process performs a selective search segmentation.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Extended Image Processing/Image segmentation").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "segmentation,graph,selective,search,multiple,strategy,color,texture,size,fill";
            m_info.m_authors = "Jasper RR Uijlings, Koen EA van de Sande, Theo Gevers, and Arnold WM Smeulders";
            m_info.m_article = "Selective search for object recognition";
            m_info.m_journal = "International journal of computer vision, 104(2):154–171";
            m_info.m_year = 2013;
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d5/df0/group__ximgproc__segmentation.html";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvSelectiveSearchSegmentationParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvSelectiveSearchSegmentation>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvSelectiveSearchSegmentationParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvSelectiveSearchSegmentation>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVSELECTIVESEARCHSEGMENTATION_HPP
