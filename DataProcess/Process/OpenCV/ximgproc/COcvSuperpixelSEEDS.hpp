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

#ifndef COCVSUPERPIXELSEEDS_HPP
#define COCVSUPERPIXELSEEDS_HPP

#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"
#include <opencv2/ximgproc.hpp>

//----------------------------//
//----- COcvSuperpixelSEEDSParam -----//
//----------------------------//
class COcvSuperpixelSEEDSParam: public CProtocolTaskParam
{
    public:

        COcvSuperpixelSEEDSParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
             m_num_superpixels = std::stoi(paramMap.at("num_superpixels"));
             m_num_levels = std::stoi(paramMap.at("num_levels"));
             m_prior = std::stoi(paramMap.at("prior"));
             m_histogram_bins = std::stoi(paramMap.at("histogram_bins"));
             m_double_step = std::stoi(paramMap.at("double_step"));
             m_num_iterations = std::stoi(paramMap.at("num_iterations"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("num_superpixels", std::to_string(m_num_superpixels)));
            map.insert(std::make_pair("num_levels", std::to_string(m_num_levels)));
            map.insert(std::make_pair("prior", std::to_string(m_prior)));
            map.insert(std::make_pair("histogram_bins", std::to_string(m_histogram_bins)));
            map.insert(std::make_pair("double_step", std::to_string(m_double_step)));
            map.insert(std::make_pair("num_iterations", std::to_string(m_num_iterations)));
            return map;
        }

    public:
        int     m_num_superpixels = 400;
        int     m_num_levels = 4;
        int     m_prior = 2;
        int     m_histogram_bins = 5;
        bool    m_double_step = false;
        int     m_num_iterations = 4;
};

//-----------------------//
//----- COcvSuperpixelSEEDS -----//
//-----------------------//
class COcvSuperpixelSEEDS : public CImageProcess2d
{
    public:

        COcvSuperpixelSEEDS() : CImageProcess2d()
        {
            setOutputDataType(IODataType::IMAGE_LABEL, 0);
            addOutput(std::make_shared<CImageProcessIO>(IODataType::IMAGE_BINARY));
            addOutput(std::make_shared<CImageProcessIO>());
            setOutputColorMap(2, 0);
        }
        COcvSuperpixelSEEDS(const std::string name, const std::shared_ptr<COcvSuperpixelSEEDSParam>& pParam) : CImageProcess2d(name)
        {
            setOutputDataType(IODataType::IMAGE_LABEL, 0);
            addOutput(std::make_shared<CImageProcessIO>(IODataType::IMAGE_BINARY));
            addOutput(std::make_shared<CImageProcessIO>());
            setOutputColorMap(2, 0);
            m_pParam = std::make_shared<COcvSuperpixelSEEDSParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvSuperpixelSEEDSParam>(m_pParam);

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
                m_pSuperpixel = cv::ximgproc::createSuperpixelSEEDS(imgSrc.cols, imgSrc.rows, imgSrc.channels(), pParam->m_num_superpixels, pParam->m_num_levels, pParam->m_prior, pParam->m_histogram_bins, pParam->m_double_step);
                m_pSuperpixel->iterate(imgSrc, pParam->m_num_iterations);
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
        cv::Ptr<cv::ximgproc::SuperpixelSEEDS> m_pSuperpixel;
};

class COcvSuperpixelSEEDSFactory : public CProcessFactory
{
    public:

        COcvSuperpixelSEEDSFactory()
        {
            m_info.m_name = QObject::tr("SuperpixelSEEDS").toStdString();
            m_info.m_description = QObject::tr("The algorithm uses an efficient hill-climbing algorithm to optimize the superpixels' energy function that is based on color histograms and a boundary term, which is optional. The energy function encourages superpixels to be of the same color, and if the boundary term is activated, the superpixels have smooth boundaries and are of similar shape. In practice it starts from a regular grid of superpixels and moves the pixels or blocks of pixels at the boundaries to refine the solution. The algorithm runs in real-time using a single CPU.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Extended Image Processing/Superpixels").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Segmentation,Edges,Contours";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/df/d81/classcv_1_1ximgproc_1_1SuperpixelSEEDS.html";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pSuperpixelSEEDSParam = std::dynamic_pointer_cast<COcvSuperpixelSEEDSParam>(pParam);
            if(pSuperpixelSEEDSParam != nullptr)
                return std::make_shared<COcvSuperpixelSEEDS>(m_info.m_name, pSuperpixelSEEDSParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pSuperpixelSEEDSParam = std::make_shared<COcvSuperpixelSEEDSParam>();
            assert(pSuperpixelSEEDSParam != nullptr);
            return std::make_shared<COcvSuperpixelSEEDS>(m_info.m_name, pSuperpixelSEEDSParam);
        }
};

#endif // COCVSUPERPIXELSEEDS_HPP
