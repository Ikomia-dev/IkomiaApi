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

#ifndef COCVCALCHIST_HPP
#define COCVCALCHIST_HPP

#include "Core/C2dImageTask.h"
#include "IO/CImageIO.h"
#include "IO/CFeatureIO.hpp"

//------------------------------//
//----- COcvCalcHistParam -----//
//------------------------------//
class COcvCalcHistParam: public CWorkflowTaskParam
{
    public:

        COcvCalcHistParam() : CWorkflowTaskParam(){}

        void setParamMap(const UMapString& paramMap) override
        {
            m_bUniform = std::stoi(paramMap.at("bUniform"));
            m_bAccu = std::stoi(paramMap.at("bAccu"));
            auto nBins = std::stoi(paramMap.at("nBins"));
            for(int i=0; i<nBins; ++i)
                m_histSize.push_back(std::stoi(paramMap.at("histSize"+std::to_string(i))));
            auto nRanges = std::stoi(paramMap.at("nRanges"));
            for(int i=0; i<nRanges; ++i)
                m_ranges.push_back(std::stoi(paramMap.at("ranges"+std::to_string(i))));
            auto nChannels = std::stoi(paramMap.at("nChannels"));
            for(int i=0; i<nChannels; ++i)
                m_channels.push_back(std::stoi(paramMap.at("channels"+std::to_string(i))));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("bUniform", std::to_string(m_bUniform)));
            map.insert(std::make_pair("bAccu", std::to_string(m_bAccu)));
            map.insert(std::make_pair("nBins", std::to_string(m_histSize.size())));
            for(size_t i=0; i<m_histSize.size(); ++i)
                map.insert(std::make_pair("histSize"+std::to_string(i), std::to_string(m_histSize[i])));
            map.insert(std::make_pair("nRanges", std::to_string(m_ranges.size())));
            for(size_t i=0; i<m_ranges.size(); ++i)
                map.insert(std::make_pair("ranges"+std::to_string(i), std::to_string(m_ranges[i])));
            map.insert(std::make_pair("nChannels", std::to_string(m_channels.size())));
            for(size_t i=0; i<m_channels.size(); ++i)
                map.insert(std::make_pair("channels"+std::to_string(i), std::to_string(m_channels[i])));
            return map;
        }

    public:

        std::vector<int>    m_histSize = {64};
        std::vector<float>  m_ranges;
        std::vector<int>    m_channels = {0};
        bool                m_bUniform = true;
        bool                m_bAccu = false;
};

//-------------------------//
//----- COcvCalcHist -----//
//-------------------------//
class COcvCalcHist : public C2dImageTask
{
    public:

        COcvCalcHist() : C2dImageTask()
        {
            addOutput(std::make_shared<CFeatureIO<double>>());
        }
        COcvCalcHist(const std::string name, const std::shared_ptr<COcvCalcHistParam>& pParam) : C2dImageTask(name)
        {
            m_pParam = std::make_shared<COcvCalcHistParam>(*pParam);
            addOutput(std::make_shared<CFeatureIO<double>>());
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        void    forwardInputImage()
        {
            if(getOutputCount() == 0)
                throw CException(CoreExCode::INVALID_SIZE, "Wrong outputs count", __func__, __FILE__, __LINE__);

            auto pInput = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            auto pOutput = std::dynamic_pointer_cast<CImageIO>(getOutput(0));

            if(pOutput)
                pOutput->setImage(pInput->getImage());
        }

        void    run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsInput>(getInput(1));
            auto pParam = std::dynamic_pointer_cast<COcvCalcHistParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgDst;
            CMat imgSrc = pInput->getImage();
            createGraphicsMask(imgSrc.getNbCols(), imgSrc.getNbRows(), pGraphicsInput);
            emit m_signalHandler->doProgress();

            std::vector<std::vector<double>> valuesByChannels;
            std::vector<std::vector<std::string>> namesByChannels;

            try
            {
                auto mask = getGraphicsMask(0);
                for(int i=0; i<imgSrc.channels(); ++i)
                {
                    pParam->m_channels[0] = i;
                    cv::calcHist(std::vector<cv::Mat>{imgSrc}, pParam->m_channels, mask, imgDst, pParam->m_histSize, pParam->m_ranges, pParam->m_bAccu);
                    std::vector<double> values;
                    std::vector<std::string> names;

                    for(int j=0; j<imgDst.rows; ++j)
                    {
                        values.push_back(static_cast<double>(imgDst.at<float>(j)));
                        names.push_back(std::to_string((j+1)*qRound(255.0/pParam->m_histSize[0])));
                    }
                    valuesByChannels.push_back(values);
                    namesByChannels.push_back(names);
                }
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            endTaskRun();
            emit m_signalHandler->doProgress();

            auto pOutput = std::dynamic_pointer_cast<CFeatureIO<double>>(getOutput(1));
            if(pOutput)
            {
                assert(valuesByChannels.size() == namesByChannels.size());
                pOutput->clearData();

                for(size_t i=0; i<valuesByChannels.size(); ++i)
                    pOutput->addValueList(valuesByChannels[i], namesByChannels[i]);

                pOutput->setOutputType(NumericOutputType::PLOT);
                pOutput->setPlotType(PlotType::HISTOGRAM);
            }
            forwardInputImage();
            emit m_signalHandler->doProgress();
        }
};

class COcvCalcHistFactory : public CTaskFactory
{
    public:

        COcvCalcHistFactory()
        {
            m_info.m_name = QObject::tr("CalcHist").toStdString();
            m_info.m_description = QObject::tr("This process calculates the histogram of the input image.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Image processing/Histograms").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "histogram";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d6/dc7/group__imgproc__hist.html#ga4b2b5fd75503ff9e6844cc4dcdaed35d";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvCalcHistParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvCalcHist>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvCalcHistParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvCalcHist>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVCALCHIST_HPP
