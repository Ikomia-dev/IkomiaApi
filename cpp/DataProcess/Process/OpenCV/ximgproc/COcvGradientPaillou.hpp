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

#ifndef COCVGRADIENTPAILLOU_HPP
#define COCVGRADIENTPAILLOU_HPP

#include "Core/C2dImageTask.h"
#include "IO/CImageIO.h"
#include <opencv2/ximgproc.hpp>

//------------------------------//
//----- COcvGradientPaillouParam -----//
//------------------------------//
class COcvGradientPaillouParam: public CWorkflowTaskParam
{
    public:
        enum orientation : int {
          X,Y
        };
        COcvGradientPaillouParam() : CWorkflowTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_orientation = static_cast<orientation>(std::stoi(paramMap.at("orientation")));
            m_alpha = std::stod(paramMap.at("alpha"));
            m_omega = std::stod(paramMap.at("omega"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("orientation", std::to_string(m_orientation)));
            map.insert(std::make_pair("alpha", std::to_string(m_alpha)));
            map.insert(std::make_pair("omega", std::to_string(m_omega)));
            return map;
        }

    public:

        orientation     m_orientation = orientation::X;
        double          m_alpha = 1;
        double          m_omega = 0.1;
};

//-------------------------//
//----- COcvGradientPaillou -----//
//-------------------------//
class COcvGradientPaillou : public C2dImageTask
{
    public:

        COcvGradientPaillou() : C2dImageTask()
        {
        }
        COcvGradientPaillou(const std::string name, const std::shared_ptr<COcvGradientPaillouParam>& pParam) : C2dImageTask(name)
        {
            m_pParam = std::make_shared<COcvGradientPaillouParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvGradientPaillouParam>(m_pParam);

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
                if(pParam->m_orientation == COcvGradientPaillouParam::orientation::X)
                    cv::ximgproc::GradientPaillouX(imgSrc, imgDst, pParam->m_alpha, pParam->m_omega);
                else
                    cv::ximgproc::GradientPaillouY(imgSrc, imgDst, pParam->m_alpha, pParam->m_omega);
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

class COcvGradientPaillouFactory : public CTaskFactory
{
    public:

        COcvGradientPaillouFactory()
        {
            m_info.m_name = "ocv_paillou_gradient_filter";
            m_info.m_description = QObject::tr("Applies X or Y Paillou filter to an image").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Extended Image Processing/Filters").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Gradient,Derivation,X,Y";
            m_info.m_authors = "Philippe Paillou";
            m_info.m_article = "Detecting step edges in noisy sar images: a new linear operator.";
            m_info.m_journal = "IEEE transactions on geoscience and remote sensing, 35(1):191–196";
            m_info.m_year = 1997;
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/da/d17/group__ximgproc__filters.html#ga72624ecc016ba7b637374de088fe9cc2";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvGradientPaillouParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvGradientPaillou>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvGradientPaillouParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvGradientPaillou>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVGRADIENTPAILLOU_HPP
