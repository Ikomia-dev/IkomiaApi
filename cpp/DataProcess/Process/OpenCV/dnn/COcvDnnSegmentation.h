/*
 * Copyright (C) 2021 Ikomia SAS
 * Contact: https://www.ikomia.com
 *
 * This file is part of the Ikomia API libraries.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef COCVDNNSEGMENTATION_HPP
#define COCVDNNSEGMENTATION_HPP

#include "COcvDnnProcess.h"

//------------------------------------//
//----- COcvDnnSegmentationParam -----//
//------------------------------------//
class COcvDnnSegmentationParam: public COcvDnnProcessParam
{
    public:

        enum NetworkType {ENET, FCN, MASK_RCNN, UNET};

        COcvDnnSegmentationParam();

        void        setParamMap(const UMapString& paramMap) override;

        UMapString  getParamMap() const override;

    public:

        int     m_netType = NetworkType::ENET;
        double  m_confidence = 0.5;
        double  m_maskThreshold = 0.3;
};

//-------------------------------//
//----- COcvDnnSegmentation -----//
//-------------------------------//
class COcvDnnSegmentation: public COcvDnnProcess
{
    public:

        COcvDnnSegmentation();
        COcvDnnSegmentation(const std::string name, const std::shared_ptr<COcvDnnSegmentationParam> &pParam);

        size_t                  getProgressSteps() override;
        int                     getNetworkInputSize() const override;
        double                  getNetworkInputScaleFactor() const override;
        cv::Scalar              getNetworkInputMean() const;

        std::vector<cv::String> getOutputsNames() const override;

        bool                    isBgr();

        void                    run();

    private:

        void                    manageOutput(std::vector<cv::Mat> &netOutputs);
        void                    manageMaskRCNNOutput(std::vector<cv::Mat> &netOutputs);

        void                    createLabelImageOutput(const cv::Mat &netOutput);

        std::vector<cv::Vec3b>  generateColorMap(const cv::Mat &netOutput, bool bWithBackgroundClass);

        void                    createLegendImage(const cv::Mat &netOutput, const std::vector<cv::Vec3b>& colors, bool bWithBackgroundClass);
};

//----------------------------------//
//----- COcvDnnSegmentationFactory -----//
//----------------------------------//
class COcvDnnSegmentationFactory : public CTaskFactory
{
    public:

        COcvDnnSegmentationFactory()
        {
            m_info.m_name = "ocv_dnn_segmentation";
            m_info.m_description = QObject::tr("This process gives the possibility to launch inference from already trained networks for segmentation purpose (CAFFE, TENSORFLOW, DARKNET and TORCH)).").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Deep neural network").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "cnn,dnn,deep,neural,network,segmentation";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d6/d0f/group__dnn.html";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvDnnSegmentationParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvDnnSegmentation>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvDnnSegmentationParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvDnnSegmentation>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVDNNSEGMENTATION_HPP
