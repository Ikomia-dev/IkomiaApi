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

#ifndef COCVPEILINNORMALIZATION_HPP
#define COCVPEILINNORMALIZATION_HPP


#include "Core/C2dImageTask.h"
#include "IO/CImageIO.h"
#include <opencv2/ximgproc.hpp>

//----------------------------------------//
//----- COcvPeiLinNormalizationParam -----//
//----------------------------------------//
class COcvPeiLinNormalizationParam: public CWorkflowTaskParam
{
    public:

        COcvPeiLinNormalizationParam() : CWorkflowTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_interpolation = std::stoi(paramMap.at("interpolation"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("interpolation", std::to_string(m_interpolation)));
            return map;
        }

    public:

        int     m_interpolation = cv::INTER_LINEAR;
};

//-----------------------------------//
//----- COcvPeiLinNormalization -----//
//-----------------------------------//
class COcvPeiLinNormalization : public C2dImageTask
{
    public:

        COcvPeiLinNormalization() : C2dImageTask()
        {
        }
        COcvPeiLinNormalization(const std::string name, const std::shared_ptr<COcvPeiLinNormalizationParam>& pParam) : C2dImageTask(name)
        {
            m_pParam = std::make_shared<COcvPeiLinNormalizationParam>(*pParam);
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        void run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsInput>(getInput(1));
            auto pParam = std::dynamic_pointer_cast<COcvPeiLinNormalizationParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgTmp, imgDst;
            CMat imgSrc = pInput->getImage();
            createGraphicsMask(imgSrc.getNbCols(), imgSrc.getNbRows(), pGraphicsInput);
            emit m_signalHandler->doProgress();

            try
            {
                if(imgSrc.channels() > 1)
                    cv::cvtColor(imgSrc, imgTmp, cv::COLOR_RGB2GRAY);
                else
                    imgTmp = imgSrc;

                cv::Matx23d transform = cv::ximgproc::PeiLinNormalization(imgTmp);
                cv::warpAffine(imgSrc, imgDst, transform, imgSrc.size(), pParam->m_interpolation);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            endTaskRun();
            applyGraphicsMask(imgSrc, imgDst, 0);
            emit m_signalHandler->doProgress();

            auto pOutput = std::dynamic_pointer_cast<CImageIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(imgDst);

            emit m_signalHandler->doProgress();
        }
};

class COcvPeiLinNormalizationFactory : public CTaskFactory
{
    public:

        COcvPeiLinNormalizationFactory()
        {
            m_info.m_name = QObject::tr("Pei-Lin Normalization").toStdString();
            m_info.m_description = QObject::tr("The process calculates and applies an affine transformation that normalize the given image using Pei & Lin Normalization.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Extended Image Processing").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "normalization,distortion,scaling,rotation,translation,skew";
            m_info.m_authors = "Soo-Chang Pei and Chao-Nan Lin";
            m_info.m_article = "Image normalization for pattern recognition";
            m_info.m_journal = "Image and Vision Computing, Vol. 13, N.10, pp. 711-723";
            m_info.m_year = 1995;
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/df/d2d/group__ximgproc.html#ga50d064b92f63916f4162474eea22d656";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvPeiLinNormalizationParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvPeiLinNormalization>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvPeiLinNormalizationParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvPeiLinNormalization>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVPEILINNORMALIZATION_HPP
