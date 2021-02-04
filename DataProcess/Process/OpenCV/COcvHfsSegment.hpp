#ifndef COCVHFSSEGMENT_HPP
#define COCVHFSSEGMENT_HPP

#include <opencv2/hfs.hpp>
#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"

//------------------------------//
//----- COcvHfsSegmentParam -----//
//------------------------------//
class COcvHfsSegmentParam: public CProtocolTaskParam
{
    public:

        COcvHfsSegmentParam() : CProtocolTaskParam(){}

        void setParamMap(const UMapString& paramMap) override
        {
            m_minRegionSizeI = std::stoi(paramMap.at("minRegionSizeI"));
            m_minRegionSizeII = std::stoi(paramMap.at("minRegionSizeII"));
            m_numSlicIter = std::stoi(paramMap.at("numSlicIter"));
            m_segEgbThresholdI = std::stof(paramMap.at("segEgbThresholdI"));
            m_segEgbThresholdII = std::stof(paramMap.at("segEgbThresholdII"));
            m_slicSpixelSize = std::stoi(paramMap.at("slicSpixelSize"));
            m_spatialWeight = std::stof(paramMap.at("spatialWeight"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("minRegionSizeI", std::to_string(m_minRegionSizeI)));
            map.insert(std::make_pair("minRegionSizeII", std::to_string(m_minRegionSizeII)));
            map.insert(std::make_pair("numSlicIter", std::to_string(m_numSlicIter)));
            map.insert(std::make_pair("segEgbThresholdI", std::to_string(m_segEgbThresholdI)));
            map.insert(std::make_pair("segEgbThresholdII", std::to_string(m_segEgbThresholdII)));
            map.insert(std::make_pair("slicSpixelSize", std::to_string(m_slicSpixelSize)));
            map.insert(std::make_pair("spatialWeight", std::to_string(m_spatialWeight)));
            return map;
        }

    public:
        int     m_minRegionSizeI = 100;
        int     m_minRegionSizeII = 200;
        int     m_numSlicIter = 5;
        float   m_segEgbThresholdI = 0.08f;
        float   m_segEgbThresholdII = 0.28f;
        int     m_slicSpixelSize = 8;
        float   m_spatialWeight = 0.6f;

};

//-------------------------//
//----- COcvHfsSegment -----//
//-------------------------//
class COcvHfsSegment : public CImageProcess2d
{
    public:

        COcvHfsSegment() : CImageProcess2d()
        {
            setOutputDataType(IODataType::IMAGE_LABEL, 0);
            addOutput(std::make_shared<CImageProcessIO>());
            setOutputColorMap(1, 0);
        }
        COcvHfsSegment(const std::string name, const std::shared_ptr<COcvHfsSegmentParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<COcvHfsSegmentParam>(*pParam);
            setOutputDataType(IODataType::IMAGE_LABEL, 0);
            addOutput(std::make_shared<CImageProcessIO>());
            setOutputColorMap(1, 0);
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
            auto pParam = std::dynamic_pointer_cast<COcvHfsSegmentParam>(m_pParam);

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
                // create engine
                cv::Ptr<cv::hfs::HfsSegment> seg = cv::hfs::HfsSegment::create( imgSrc.rows, imgSrc.cols );

                // perform segmentation
                // now "res" is a matrix of indices
                // change the second parameter to "True" to get a rgb image for "res"
                imgDst = seg->performSegmentGpu(imgSrc, false);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            forwardInputImage(0, 1);
            applyGraphicsMask(imgSrc, imgDst, 0);
            emit m_signalHandler->doProgress();

            auto pOutput = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(imgDst);

            endTaskRun();
            emit m_signalHandler->doProgress();
        }
};

class COcvHfsSegmentFactory : public CProcessFactory
{
    public:

        COcvHfsSegmentFactory()
        {
            m_info.m_name = QObject::tr("Hierarchical Feature Selection Segmentation").toStdString();
            m_info.m_description = QObject::tr("This process is a real-time system for image segmentation.The algorithm is executed in 3 stages. In the first stage, it obtains an over-segmented image using SLIC(simple linear iterative clustering). In the last 2 stages, it iteratively merges the over-segmented image with merging method mentioned in EGB(Efficient Graph-based Image Segmentation) and learned SVM parameters.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Hierarchical Feature Selection for Efficient Image Segmentation").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Segmentation, hfs";
            m_info.m_authors = "M. Cheng et al.";
            m_info.m_article = "HFS: Hierarchical Feature Selection for Efficient Image Segmentation";
            m_info.m_journal = "ECCV";
            m_info.m_year = 2016;
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/dc/d29/group__hfs.html";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pHfsSegmentParam = std::dynamic_pointer_cast<COcvHfsSegmentParam>(pParam);
            if(pHfsSegmentParam != nullptr)
                return std::make_shared<COcvHfsSegment>(m_info.m_name, pHfsSegmentParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pHfsSegmentParam = std::make_shared<COcvHfsSegmentParam>();
            assert(pHfsSegmentParam != nullptr);
            return std::make_shared<COcvHfsSegment>(m_info.m_name, pHfsSegmentParam);
        }
};

#endif // COCVHFSSEGMENT_HPP
