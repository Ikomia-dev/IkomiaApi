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

#ifndef COCVSTRUCTUREDEDGEDETECTION_HPP
#define COCVSTRUCTUREDEDGEDETECTION_HPP

#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"
#include <opencv2/ximgproc.hpp>

//----------------------------//
//----- COcvStructuredEdgeDetectionParam -----//
//----------------------------//
class COcvStructuredEdgeDetectionParam: public CProtocolTaskParam
{
    public:

        COcvStructuredEdgeDetectionParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
             m_modelPath = paramMap.at("algorithm");
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("algorithm", m_modelPath));
            return map;
        }

    public:

        std::string     m_modelPath = Utils::IkomiaApp::getAppFolder() + "/Resources/StructuredEdgeDetection/model.yml.gz";
};

//-----------------------//
//----- COcvStructuredEdgeDetection -----//
//-----------------------//
class COcvStructuredEdgeDetection : public CImageProcess2d
{
    public:

        COcvStructuredEdgeDetection() : CImageProcess2d()
        {
        }
        COcvStructuredEdgeDetection(const std::string name, const std::shared_ptr<COcvStructuredEdgeDetectionParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<COcvStructuredEdgeDetectionParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvStructuredEdgeDetectionParam>(m_pParam);

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
                CMat img;
                if(imgSrc.depth() != CV_32F)
                    cv::normalize(imgSrc, img, 0, 1, cv::NORM_MINMAX, CV_32F);

                m_pSuperpixel = cv::ximgproc::createStructuredEdgeDetection(pParam->m_modelPath);
                m_pSuperpixel->detectEdges(img, imgDst);
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

            emit m_signalHandler->doProgress();
        }
    private:
        cv::Ptr<cv::ximgproc::StructuredEdgeDetection> m_pSuperpixel;
};

class COcvStructuredEdgeDetectionFactory : public CProcessFactory
{
    public:

        COcvStructuredEdgeDetectionFactory()
        {
            m_info.m_name = QObject::tr("StructuredEdgeDetection").toStdString();
            m_info.m_description = QObject::tr("The function detects edges and is much more robust to texture presence, than common approaches, e.g. Sobel.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Extended Image Processing/Structured forests for fast edge detection").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Segmentation,Edges,Contours";
            m_info.m_authors = "Piotr Dollár and C Lawrence Zitnick";
            m_info.m_article = "Structured forests for fast edge detection";
            m_info.m_journal = "IEEE ICCV, pages 1841–184";
            m_info.m_year = 2013;
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d8/d54/classcv_1_1ximgproc_1_1StructuredEdgeDetection.html";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pStructuredEdgeDetectionParam = std::dynamic_pointer_cast<COcvStructuredEdgeDetectionParam>(pParam);
            if(pStructuredEdgeDetectionParam != nullptr)
                return std::make_shared<COcvStructuredEdgeDetection>(m_info.m_name, pStructuredEdgeDetectionParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pStructuredEdgeDetectionParam = std::make_shared<COcvStructuredEdgeDetectionParam>();
            assert(pStructuredEdgeDetectionParam != nullptr);
            return std::make_shared<COcvStructuredEdgeDetection>(m_info.m_name, pStructuredEdgeDetectionParam);
        }
};

#endif // COCVSTRUCTUREDEDGEDETECTION_HPP
