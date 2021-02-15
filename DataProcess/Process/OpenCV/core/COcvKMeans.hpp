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

#ifndef COCVKMEANS_HPP
#define COCVKMEANS_HPP

#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"
#include "IO/CMeasureProcessIO.h"

//------------------------------//
//----- COcvKMeansParam -----//
//------------------------------//
class COcvKMeansParam: public CProtocolTaskParam
{
    public:

        COcvKMeansParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_k = std::stoi(paramMap.at("k"));
            m_termType = std::stoi(paramMap.at("termType"));
            m_termMaxCount = std::stoi(paramMap.at("termMaxCount"));
            m_termEpsilon = std::stod(paramMap.at("termEpsilon"));
            m_attempts = std::stoi(paramMap.at("attempts"));
            m_flags = std::stoi(paramMap.at("flags"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("k", std::to_string(m_k)));
            map.insert(std::make_pair("termType", std::to_string(m_termType)));
            map.insert(std::make_pair("termMaxCount", std::to_string(m_termMaxCount)));
            map.insert(std::make_pair("termEpsilon", std::to_string(m_termEpsilon)));
            map.insert(std::make_pair("attempts", std::to_string(m_attempts)));
            map.insert(std::make_pair("flags", std::to_string(m_flags)));
            return map;
        }

    public:

        int     m_k = 3;
        int     m_termType = cv::TermCriteria::COUNT+cv::TermCriteria::EPS;
        int     m_termMaxCount = 10;
        double  m_termEpsilon = 1.0;
        int     m_attempts = 3;
        int     m_flags = cv::KmeansFlags::KMEANS_PP_CENTERS;
};

//----------------------//
//----- COcvKMeans -----//
//----------------------//
class COcvKMeans : public CImageProcess2d
{
    public:

        COcvKMeans() : CImageProcess2d()
        {
            addOutput(std::make_shared<CImageProcessIO>(IODataType::IMAGE_LABEL));
            addOutput(std::make_shared<CImageProcessIO>());
            addOutput(std::make_shared<CMeasureProcessIO>());
            setOutputColorMap(2, 1);
        }
        COcvKMeans(const std::string name, const std::shared_ptr<COcvKMeansParam>& pParam) : CImageProcess2d(name)
        {
            addOutput(std::make_shared<CImageProcessIO>(IODataType::IMAGE_LABEL));
            addOutput(std::make_shared<CImageProcessIO>());
            addOutput(std::make_shared<CMeasureProcessIO>());
            setOutputColorMap(2, 1);
            m_pParam = std::make_shared<COcvKMeansParam>(*pParam);
        }

        size_t  getProgressSteps() override
        {
            return 6;
        }

        void run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsProcessInput>(getInput(1));
            auto pParam = std::dynamic_pointer_cast<COcvKMeansParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            double compactness;
            cv::Mat1f centers;
            CMat imgDst1, imgDst2;
            CMat imgSrc = pInput->getImage();
            createGraphicsMask(imgSrc.getNbCols(), imgSrc.getNbRows(), pGraphicsInput);
            emit m_signalHandler->doProgress();

            try
            {
                std::vector<int> labels;
                CMat data = imgSrc.reshape(1, imgSrc.cols*imgSrc.rows);
                data.convertTo(data, CV_32F);
                CMat data2 = CMat(imgSrc.cols*imgSrc.rows, 1, CV_32F);
                int maxVal = getMaxValueFromDepth(imgSrc.depth());
                emit m_signalHandler->doProgress();

                compactness = cv::kmeans(data, pParam->m_k, labels,
                                         cv::TermCriteria(pParam->m_termType, pParam->m_termMaxCount, pParam->m_termEpsilon),
                                         pParam->m_attempts, pParam->m_flags, centers);

                emit m_signalHandler->doProgress();

                if(imgSrc.channels() == 1)
                {
                    for(int i=0; i<imgSrc.cols*imgSrc.rows; ++i)
                    {
                        data.at<float>(i, 0) = centers(labels[i], 0);
                        data2.at<float>(i, 0) = (float)labels[i] / (float)pParam->m_k * (float)maxVal;
                    }
                }
                else if(imgSrc.channels() == 3)
                {
                    for(int i=0; i<imgSrc.cols*imgSrc.rows; ++i)
                    {
                        data.at<float>(i, 0) = centers(labels[i], 0);
                        data.at<float>(i, 1) = centers(labels[i], 1);
                        data.at<float>(i, 2) = centers(labels[i], 2);
                        data2.at<float>(i, 0) = (float)(labels[i] + 1) / (float)pParam->m_k * (float)maxVal;
                    }
                }
                emit m_signalHandler->doProgress();
                imgDst1 = data.reshape(imgSrc.channels(), imgSrc.rows);
                imgDst1.convertTo(imgDst1, imgSrc.type());
                imgDst2 = data2.reshape(1, imgSrc.rows);
                imgDst2.convertTo(imgDst2, imgSrc.type());
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            emit m_signalHandler->doProgress();
            forwardInputImage(0, 2);

            //Color or monochrome image according to source image
            auto pOutput1 = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(0));
            if(pOutput1)
            {
                applyGraphicsMask(imgSrc, imgDst1, 0);
                pOutput1->setImage(imgDst1);
            }

            //Label image monochrome
            auto pOutput2 = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(1));
            if(pOutput2)
            {
                applyGraphicsMaskToBinary(imgDst2, imgDst2, 0);
                pOutput2->setImage(imgDst2);
            }

            //Compactness and center values
            auto pMeasureOutput = std::dynamic_pointer_cast<CMeasureProcessIO>(getOutput(3));
            if(pMeasureOutput)
            {
                pMeasureOutput->addObjectMeasure(CObjectMeasure(CMeasure(CMeasure::CUSTOM, QObject::tr("Compactness").toStdString()), compactness, -1, ""));

                for(int i=0; i<centers.rows; ++i)
                {
                    if(centers.cols == 1)
                        pMeasureOutput->addObjectMeasure(CObjectMeasure(CMeasure(CMeasure::CUSTOM+1, QObject::tr("Centers").toStdString()), centers(i, 0), i, ""));
                    else if(centers.cols == 3)
                    {
                        pMeasureOutput->addObjectMeasure(CObjectMeasure(CMeasure(CMeasure::CUSTOM+1, QObject::tr("Centers R").toStdString()), centers(i, 0), i, ""));
                        pMeasureOutput->addObjectMeasure(CObjectMeasure(CMeasure(CMeasure::CUSTOM+2, QObject::tr("Centers G").toStdString()), centers(i, 1), i, ""));
                        pMeasureOutput->addObjectMeasure(CObjectMeasure(CMeasure(CMeasure::CUSTOM+3, QObject::tr("Centers B").toStdString()), centers(i, 2), i, ""));
                    }
                }
            }

            endTaskRun();
            emit m_signalHandler->doProgress();
        }

    private:

        int getMaxValueFromDepth(int depth)
        {
            int value = 1;
            switch(depth)
            {
                case CV_8U: value = 255; break;
                case CV_8S: value = 127; break;
                case CV_16U: value = 65535; break;
                case CV_16S: value = 32767; break;
                case CV_32S: value = 2147483647; break;
                case CV_32F: value = 1; break;
                case CV_64F: value = 1; break;
            }
            return value;
        }
};

class COcvKMeansFactory : public CProcessFactory
{
    public:

        COcvKMeansFactory()
        {
            m_info.m_name = QObject::tr("K-means").toStdString();
            m_info.m_description = QObject::tr("This process implements a k-means algorithm that finds the centers of k clusters and groups the input samples around the clusters.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Core functionality/Clustering").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "kmeans, clustering, cluster, center, classification";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d5/d38/group__core__cluster.html#ga9a34dc06c6ec9460e90860f15bcd2f88";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvKMeansParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvKMeans>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvKMeansParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvKMeans>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVKMEANS_HPP
