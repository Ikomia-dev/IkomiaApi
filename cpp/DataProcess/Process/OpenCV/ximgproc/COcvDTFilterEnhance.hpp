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

#ifndef COCVDTFILTERENHANCE_HPP
#define COCVDTFILTERENHANCE_HPP
#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"
#include <opencv2/ximgproc.hpp>

//----------------------------//
//----- COcvDTFilterEnhanceParam -----//
//----------------------------//
class COcvDTFilterEnhanceParam: public CProtocolTaskParam
{
    public:

        COcvDTFilterEnhanceParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
             m_sigmaColor = std::stod(paramMap.at("sigmaColor"));
             m_sigmaSpatial = std::stod(paramMap.at("sigmaSpatial"));
             m_contrastBase = std::stoi(paramMap.at("contrastBase"));
             m_detailsLevel = std::stoi(paramMap.at("detailsLevel"));
             m_numLayer = std::stoi(paramMap.at("numLayer"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("sigmaColor", std::to_string(m_sigmaColor)));
            map.insert(std::make_pair("sigmaSpatial", std::to_string(m_sigmaSpatial)));
            map.insert(std::make_pair("contrastBase", std::to_string(m_contrastBase)));
            map.insert(std::make_pair("detailsLevel", std::to_string(m_detailsLevel)));
            map.insert(std::make_pair("numLayer", std::to_string(m_numLayer)));
            return map;
        }

    public:
        double  m_sigmaColor = 0.07;
        double  m_sigmaSpatial = 15;
        int     m_contrastBase = 100;
        int     m_detailsLevel = 10;
        int     m_numLayer = 1;
};

//-----------------------//
//----- COcvDTFilterEnhance -----//
//-----------------------//
class COcvDTFilterEnhance : public CImageProcess2d
{
    public:

        COcvDTFilterEnhance() : CImageProcess2d()
        {
        }
        COcvDTFilterEnhance(const std::string name, const std::shared_ptr<COcvDTFilterEnhanceParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<COcvDTFilterEnhanceParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvDTFilterEnhanceParam>(m_pParam);

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
                using namespace cv;
                using namespace cv::ximgproc;

                if(imgSrc.channels() != 3)
                    throw CException(CoreExCode::INVALID_PARAMETER, "Input image must have 3 channels (color image)", __func__, __FILE__, __LINE__);

                if(imgSrc.type() == CV_8U || imgSrc.type() == CV_8UC3)
                    imgSrc.convertTo(imgSrc, CV_32F, 1.0f/255.0f);

                //Decompose image to 3 Lab channels
                Mat frameLab, frameLabCn[3];
                cvtColor(imgSrc, frameLab, COLOR_RGB2Lab);
                split(frameLab, frameLabCn);

                //Generate progressively smoother versions of the lightness channel
                Mat layer0 = frameLabCn[0]; //first channel is original lightness
                double meanLigtness = mean(frameLabCn[0])[0]/100;
                double cBase = pParam->m_contrastBase / 100.0;
                Mat layer1, layer2, res = cv::Mat::zeros(layer0.size(), layer0.type());
                layer0 /= 100; // normalize to [0 1]  --  J0
                // init
                layer1 = layer0;
                for(int i=0; i<pParam->m_numLayer; ++i)
                {
                    dtFilter(layer1, layer1, layer2, std::pow(2,i)*pParam->m_sigmaSpatial, pParam->m_sigmaColor, DTF_IC);
                    Mat Di = layer1 - layer2;
                    Mat expTerm;
                    exp(-pParam->m_detailsLevel*Di, expTerm);
                    Mat Si = 2.0/(1.0+expTerm)-1.0;
                    res += Si;
                    layer1 = layer2;
                }
                frameLabCn[0]  = cBase*(layer2 - meanLigtness) + meanLigtness; //fit contrast of base (most blurred) layer
                frameLabCn[0] += res; //add weighted sum of detail layers to new lightness
                frameLabCn[0] *= 100; // back to [0 100]

                //Update new lightness
                merge(frameLabCn, 3, frameLab);
                cvtColor(frameLab, imgSrc, COLOR_Lab2RGB);
                imgSrc.convertTo(imgDst, CV_8U, 255);
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
};

class COcvDTFilterEnhanceFactory : public CProcessFactory
{
    public:

        COcvDTFilterEnhanceFactory()
        {
            m_info.m_name = QObject::tr("DTFilterEnhance").toStdString();
            m_info.m_description = QObject::tr("This process produces enhanced image.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Extended Image Processing/Filters").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Details,Enhancement,DTFilterEnhance";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDTFilterEnhanceParam = std::dynamic_pointer_cast<COcvDTFilterEnhanceParam>(pParam);
            if(pDTFilterEnhanceParam != nullptr)
                return std::make_shared<COcvDTFilterEnhance>(m_info.m_name, pDTFilterEnhanceParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDTFilterEnhanceParam = std::make_shared<COcvDTFilterEnhanceParam>();
            assert(pDTFilterEnhanceParam != nullptr);
            return std::make_shared<COcvDTFilterEnhance>(m_info.m_name, pDTFilterEnhanceParam);
        }
};

#endif // COCVDTFILTERENHANCE_HPP
