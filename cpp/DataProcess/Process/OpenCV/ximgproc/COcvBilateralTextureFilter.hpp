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

#ifndef COCVBILATERALTEXTUREFILTER_HPP
#define COCVBILATERALTEXTUREFILTER_HPP

#include "Core/C2dImageTask.h"
#include "IO/CImageIO.h"
#include <opencv2/ximgproc.hpp>

//------------------------------//
//----- COcvBilateralTextureFilterParam -----//
//------------------------------//
class COcvBilateralTextureFilterParam: public CWorkflowTaskParam
{
    public:

        COcvBilateralTextureFilterParam() : CWorkflowTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_fr = std::stoi(paramMap.at("fr"));
            m_numIter = std::stoi(paramMap.at("numIter"));
            m_sigmaAlpha = std::stod(paramMap.at("sigmaAlpha"));
            m_sigmaAvg = std::stod(paramMap.at("sigmaAvg"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("fr", std::to_string(m_fr)));
            map.insert(std::make_pair("numIter", std::to_string(m_numIter)));
            map.insert(std::make_pair("sigmaAlpha", std::to_string(m_sigmaAlpha)));
            map.insert(std::make_pair("sigmaAvg", std::to_string(m_sigmaAvg)));
            return map;
        }

    public:

        int     m_fr = 3;
        int     m_numIter = 1;
        double  m_sigmaAlpha = -1;
        double  m_sigmaAvg = -1;
};

//-------------------------//
//----- COcvBilateralTextureFilter -----//
//-------------------------//
class COcvBilateralTextureFilter : public C2dImageTask
{
    public:

        COcvBilateralTextureFilter() : C2dImageTask()
        {
        }
        COcvBilateralTextureFilter(const std::string name, const std::shared_ptr<COcvBilateralTextureFilterParam>& pParam) : C2dImageTask(name)
        {
            m_pParam = std::make_shared<COcvBilateralTextureFilterParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvBilateralTextureFilterParam>(m_pParam);

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
                cv::ximgproc::bilateralTextureFilter(imgSrc, imgDst, pParam->m_fr, pParam->m_numIter, pParam->m_sigmaAlpha, pParam->m_sigmaAvg);
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

class COcvBilateralTextureFilterFactory : public CTaskFactory
{
    public:

        COcvBilateralTextureFilterFactory()
        {
            m_info.m_name = "ocv_bilateral_texture_filter";
            m_info.m_description = QObject::tr("Applies the bilateral texture filter to an image. It performs structure-preserving texture filter.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Extended Image Processing/Filters").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Smooth,Blur,Anisotropic,Filter,Gaussian,Edge-preserving";
            m_info.m_authors = "Hojin Cho, Hyunjoon Lee, Henry Kang, and Seungyong Lee";
            m_info.m_article = "Bilateral texture filtering";
            m_info.m_journal = "ACM Transactions on Graphics, 33(4):128:1–128:8";
            m_info.m_year = 2014;
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvBilateralTextureFilterParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvBilateralTextureFilter>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvBilateralTextureFilterParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvBilateralTextureFilter>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVBILATERALTEXTUREFILTER_HPP
