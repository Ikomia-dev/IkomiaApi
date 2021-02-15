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

#ifndef COCVDENOISETVL1_HPP
#define COCVDENOISETVL1_HPP

#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"

//-----------------------------------------//
//----- COcvDenoiseTVL1Param -----//
//-----------------------------------------//
class COcvDenoiseTVL1Param: public CProtocolTaskParam
{
    public:

        COcvDenoiseTVL1Param() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_lambda = std::stod(paramMap.at("lambda"));
            m_niters = std::stoi(paramMap.at("niters"));

        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("lambda", std::to_string(m_lambda)));
            map.insert(std::make_pair("niters", std::to_string(m_niters)));
            return map;
        }

    public:

        double  	m_lambda = 1.0;
        int         m_niters = 30;
};

//------------------------------------//
//----- COcvDenoiseTVL1 -----//
//------------------------------------//
class COcvDenoiseTVL1 : public CImageProcess2d
{
    public:

        COcvDenoiseTVL1() : CImageProcess2d()
        {
        }
        COcvDenoiseTVL1(const std::string name, const std::shared_ptr<COcvDenoiseTVL1Param>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<COcvDenoiseTVL1Param>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvDenoiseTVL1Param>(m_pParam);

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
                std::vector<cv::Mat> Lab(3);
                if(imgSrc.channels() == 3)
                {
                    CMat imgLab;
                    cv::cvtColor(imgSrc, imgLab, cv::COLOR_RGB2Lab);
                    cv::split(imgLab, Lab);
                    img = Lab[0];
                }
                else
                    img = imgSrc;

                std::vector<cv::Mat> matList = {img};
                cv::denoise_TVL1(matList, Lab[0], pParam->m_lambda, pParam->m_niters);

                if(imgSrc.channels() == 3)
                {
                    CMat imgLab;
                    cv::merge(Lab, imgLab);
                    cv::cvtColor(imgLab, imgDst, cv::COLOR_Lab2RGB);
                }
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

class COcvDenoiseTVL1Factory : public CProcessFactory
{
    public:

        COcvDenoiseTVL1Factory()
        {
            m_info.m_name = QObject::tr("Denoise TV L1").toStdString();
            m_info.m_description = QObject::tr("Primal-dual algorithm is an algorithm for solving special types of variational problems (that is, finding a function to minimize some functional). As the image denoising, in particular, may be seen as the variational problem, primal-dual algorithm then can be used to perform denoising and this is exactly what is implemented.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Computational photography/Denoising").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Smooth,Variational";
            m_info.m_authors = "Antonin Chambolle, Vicent Caselles, Daniel Cremers, Matteo Novaga, and Thomas Pock";
            m_info.m_article = "An introduction to total variation for image analysis";
            m_info.m_journal = "Theoretical foundations and numerical methods for sparse recovery, 9:263–340";
            m_info.m_year = 2010;
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d1/d79/group__photo__denoise.html#ga7602ed5ae17b7de40152b922227c4e4f";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDenoiseTVL1Param = std::dynamic_pointer_cast<COcvDenoiseTVL1Param>(pParam);
            if(pDenoiseTVL1Param != nullptr)
                return std::make_shared<COcvDenoiseTVL1>(m_info.m_name, pDenoiseTVL1Param);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDenoiseTVL1Param = std::make_shared<COcvDenoiseTVL1Param>();
            assert(pDenoiseTVL1Param != nullptr);
            return std::make_shared<COcvDenoiseTVL1>(m_info.m_name, pDenoiseTVL1Param);
        }
};

#endif // COCVDENOISETVL1_HPP
