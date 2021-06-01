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

#ifndef COCVSUPERPIXELSLIC_HPP
#define COCVSUPERPIXELSLIC_HPP

#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"
#include <opencv2/ximgproc.hpp>

//----------------------------//
//----- COcvSuperpixelSLICParam -----//
//----------------------------//
class COcvSuperpixelSLICParam: public CProtocolTaskParam
{
    public:

        COcvSuperpixelSLICParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
             m_algorithm = std::stoi(paramMap.at("algorithm"));
             m_region_size = std::stoi(paramMap.at("region_size"));
             m_ruler = std::stof(paramMap.at("ruler"));
             m_num_iterations = std::stoi(paramMap.at("num_iterations"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("algorithm", std::to_string(m_algorithm)));
            map.insert(std::make_pair("region_size", std::to_string(m_region_size)));
            map.insert(std::make_pair("ruler", std::to_string(m_ruler)));
            map.insert(std::make_pair("num_iterations", std::to_string(m_num_iterations)));
            return map;
        }

    public:
        int     m_algorithm = cv::ximgproc::SLICO;
        int     m_region_size = 10;
        float   m_ruler = 10.0f;
        int     m_num_iterations = 10;
};

//-----------------------//
//----- COcvSuperpixelSLIC -----//
//-----------------------//
class COcvSuperpixelSLIC : public CImageProcess2d
{
    public:

        COcvSuperpixelSLIC() : CImageProcess2d()
        {
            setOutputDataType(IODataType::IMAGE_LABEL, 0);
            addOutput(std::make_shared<CImageProcessIO>(IODataType::IMAGE_BINARY));
            addOutput(std::make_shared<CImageProcessIO>());
            setOutputColorMap(2, 0);
        }
        COcvSuperpixelSLIC(const std::string name, const std::shared_ptr<COcvSuperpixelSLICParam>& pParam) : CImageProcess2d(name)
        {
            setOutputDataType(IODataType::IMAGE_LABEL, 0);
            addOutput(std::make_shared<CImageProcessIO>(IODataType::IMAGE_BINARY));
            addOutput(std::make_shared<CImageProcessIO>());
            setOutputColorMap(2, 0);
            m_pParam = std::make_shared<COcvSuperpixelSLICParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvSuperpixelSLICParam>(m_pParam);

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
                m_pSuperpixel = cv::ximgproc::createSuperpixelSLIC(imgSrc, pParam->m_algorithm, pParam->m_region_size, pParam->m_ruler);
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
        cv::Ptr<cv::ximgproc::SuperpixelSLIC> m_pSuperpixel;
};

class COcvSuperpixelSLICFactory : public CProcessFactory
{
    public:

        COcvSuperpixelSLICFactory()
        {
            m_info.m_name = QObject::tr("SuperpixelSLIC").toStdString();
            m_info.m_description = QObject::tr("LSC (Linear Spectral Clustering) produces compact and uniform superpixels with low computational costs. Basically, a normalized cuts formulation of the superpixel segmentation is adopted based on a similarity metric that measures the color similarity and space proximity between image pixels. LSC is of linear computational complexity and high memory efficiency and is able to preserve global properties of images ").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Extended Image Processing/Superpixels").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Segmentation,Edges,Contours";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d3/da9/classcv_1_1ximgproc_1_1SuperpixelSLIC.html";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pSuperpixelSLICParam = std::dynamic_pointer_cast<COcvSuperpixelSLICParam>(pParam);
            if(pSuperpixelSLICParam != nullptr)
                return std::make_shared<COcvSuperpixelSLIC>(m_info.m_name, pSuperpixelSLICParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pSuperpixelSLICParam = std::make_shared<COcvSuperpixelSLICParam>();
            assert(pSuperpixelSLICParam != nullptr);
            return std::make_shared<COcvSuperpixelSLIC>(m_info.m_name, pSuperpixelSLICParam);
        }
};

#endif // COCVSUPERPIXELSLIC_HPP
