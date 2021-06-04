// Copyright (C) 2021 Ikomia SAS
// Contact: https://www.ikomia.com
//
// This file is part of the Ikomia API libraries.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#ifndef COCVGRAPHSEGMENTATION_HPP
#define COCVGRAPHSEGMENTATION_HPP

#include "Core/C2dImageTask.h"
#include "IO/CImageIO.h"
#include <opencv2/ximgproc.hpp>

//--------------------------------------//
//----- COcvGraphSegmentationParam -----//
//--------------------------------------//
class COcvGraphSegmentationParam: public CWorkflowTaskParam
{
    public:

        COcvGraphSegmentationParam() : CWorkflowTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_sigma = std::stod(paramMap.at("sigma"));
            m_k = std::stod(paramMap.at("k"));
            m_minSize = std::stoi(paramMap.at("minSize"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("sigma", std::to_string(m_sigma)));
            map.insert(std::make_pair("k", std::to_string(m_k)));
            map.insert(std::make_pair("minSize", std::to_string(m_minSize)));
            return map;
        }

    public:

        double  m_sigma = 0.8;
        double  m_k = 300;
        int     m_minSize = 100;
};

//---------------------------------//
//----- COcvGraphSegmentation -----//
//---------------------------------//
class COcvGraphSegmentation : public C2dImageTask
{
    public:

        COcvGraphSegmentation() : C2dImageTask()
        {
            setOutputDataType(IODataType::IMAGE_LABEL, 0);
            addOutput(std::make_shared<CImageIO>());
            setOutputColorMap(1, 0);
        }
        COcvGraphSegmentation(const std::string name, const std::shared_ptr<COcvGraphSegmentationParam>& pParam) : C2dImageTask(name)
        {
            m_pParam = std::make_shared<COcvGraphSegmentationParam>(*pParam);
            setOutputDataType(IODataType::IMAGE_LABEL, 0);
            addOutput(std::make_shared<CImageIO>());
            setOutputColorMap(1, 0);
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        void run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            auto pParam = std::dynamic_pointer_cast<COcvGraphSegmentationParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgDst;
            CMat imgSrc = pInput->getImage();
            emit m_signalHandler->doProgress();

            try
            {
                cv::Ptr<cv::ximgproc::segmentation::GraphSegmentation> gs = cv::ximgproc::segmentation::createGraphSegmentation();
                gs->setSigma(pParam->m_sigma);
                gs->setK(pParam->m_k);
                gs->setMinSize(pParam->m_minSize);
                gs->processImage(imgSrc, imgDst);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            emit m_signalHandler->doProgress();
            forwardInputImage(0, 1);

            auto pOutput = std::dynamic_pointer_cast<CImageIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(imgDst);

            endTaskRun();
            emit m_signalHandler->doProgress();
        }
};

class COcvGraphSegmentationFactory : public CTaskFactory
{
    public:

        COcvGraphSegmentationFactory()
        {
            m_info.m_name = QObject::tr("Graph Segmentation").toStdString();
            m_info.m_description = QObject::tr("This process performs a graph based segmentation.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Extended Image Processing/Image segmentation").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "segmentation,graph";
            m_info.m_authors = "Pedro F Felzenszwalb and Daniel P Huttenlocher";
            m_info.m_article = "Efficient graph-based image segmentation";
            m_info.m_journal = "Springer, volume 59, pages 167–181";
            m_info.m_year = 2004;
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d5/df0/group__ximgproc__segmentation.html";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvGraphSegmentationParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvGraphSegmentation>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvGraphSegmentationParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvGraphSegmentation>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVGRAPHSEGMENTATION_HPP
