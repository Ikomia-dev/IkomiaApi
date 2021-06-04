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

#ifndef COCVJOINTBILATERALFILTER_HPP
#define COCVJOINTBILATERALFILTER_HPP

#include "opencv2/ximgproc.hpp"
#include "Core/C2dImageTask.h"

//-----------------------------------------//
//----- COcvJointBilateralFilterParam -----//
//-----------------------------------------//
class COcvJointBilateralFilterParam: public CWorkflowTaskParam
{
    public:

        COcvJointBilateralFilterParam() : CWorkflowTaskParam()
        {
        }

        void        setParamMap(const UMapString& paramMap) override
        {
            m_diameter = std::stoi(paramMap.at("diameter"));
            m_sigmaColor = std::stod(paramMap.at("sigmaColor"));
            m_sigmaSpace = std::stod(paramMap.at("sigmaSpace"));
            m_borderType = std::stoi(paramMap.at("borderType"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("diameter", std::to_string(m_diameter)));
            map.insert(std::make_pair("sigmaColor", std::to_string(m_sigmaColor)));
            map.insert(std::make_pair("sigmaSpace", std::to_string(m_sigmaSpace)));
            map.insert(std::make_pair("borderType", std::to_string(m_borderType)));
            return map;
        }

    public:

        int         m_diameter = 5;
        double      m_sigmaColor = 10.0;
        double      m_sigmaSpace = 10.0;
        int         m_borderType = cv::BORDER_DEFAULT;
};

//------------------------------------//
//----- COcvJointBilateralFilter -----//
//------------------------------------//
class COcvJointBilateralFilter : public C2dImageTask
{
    public:

        COcvJointBilateralFilter() : C2dImageTask()
        {
            insertInput(std::make_shared<CImageIO>(), 1);
        }
        COcvJointBilateralFilter(const std::string name, const std::shared_ptr<COcvJointBilateralFilterParam>& pParam) : C2dImageTask(name)
        {
            insertInput(std::make_shared<CImageIO>(), 1);
            m_pParam = std::make_shared<COcvJointBilateralFilterParam>(*pParam);
        }

        size_t     getProgressSteps() override
        {
            return 3;
        }

        void    run() override
        {
            beginTaskRun();

            auto pInput1 = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            auto pInput2 = std::dynamic_pointer_cast<CImageIO>(getInput(1));
            auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsInput>(getInput(2));
            auto pParam = std::dynamic_pointer_cast<COcvJointBilateralFilterParam>(m_pParam);

            if(pInput1 == nullptr || pInput2 == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput1->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgDst, imgJoint;
            CMat imgSrc = pInput1->getImage();

            if(pInput2->isDataAvailable() == false)
                imgJoint = imgSrc;
            else
                imgJoint = pInput2->getImage();

            createGraphicsMask(imgSrc.getNbCols(), imgSrc.getNbRows(), pGraphicsInput);
            emit m_signalHandler->doProgress();

            try
            {
                cv::ximgproc::jointBilateralFilter(imgJoint, imgSrc, imgDst, pParam->m_diameter, pParam->m_sigmaColor, pParam->m_sigmaSpace, pParam->m_borderType);
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

class COcvJointBilateralFilterFactory : public CTaskFactory
{
    public:

        COcvJointBilateralFilterFactory()
        {
            m_info.m_name = QObject::tr("Joint Bilateral Filter").toStdString();
            m_info.m_description = QObject::tr("This process applies the Joint Bilateral filter.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Extended Image Processing/Filters").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "smooth,blur,anisotropic,filter,edge preserving,guide";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/da/d17/group__ximgproc__filters.html#ga80b9b58fb85dd069691b709285ab985c";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pManifoldParam = std::dynamic_pointer_cast<COcvJointBilateralFilterParam>(pParam);
            if(pManifoldParam != nullptr)
                return std::make_shared<COcvJointBilateralFilter>(m_info.m_name, pManifoldParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pManifoldParam = std::make_shared<COcvJointBilateralFilterParam>();
            assert(pManifoldParam != nullptr);
            return std::make_shared<COcvJointBilateralFilter>(m_info.m_name, pManifoldParam);

        }
};

#endif // COCVJOINTBILATERALFILTER_HPP
