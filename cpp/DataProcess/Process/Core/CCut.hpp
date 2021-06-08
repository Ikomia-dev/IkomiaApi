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

#ifndef CCUT_HPP
#define CCUT_HPP

#include "Core/C2dImageTask.h"
#include "IO/CImageIO.h"

//---------------------//
//----- CCutParam -----//
//---------------------//
class CCutParam: public CWorkflowTaskParam
{
    public:

        CCutParam() : CWorkflowTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_size = std::stoi(paramMap.at("size"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("size", std::to_string(m_size)));
            return map;
        }

    public:

        int m_size = 10;
};

//----------------//
//----- CCut -----//
//----------------//
class CCut : public C2dImageTask
{
    public:

        CCut() : C2dImageTask(true)
        {
            getInput(0)->setDataType(IODataType::IMAGE_BINARY);
            setOutputDataType(IODataType::IMAGE_BINARY, 0);
        }
        CCut(const std::string name, const std::shared_ptr<CCutParam>& pParam) : C2dImageTask(name, true)
        {
            getInput(0)->setDataType(IODataType::IMAGE_BINARY);
            setOutputDataType(IODataType::IMAGE_BINARY, 0);
            m_pParam = std::make_shared<CCutParam>(*pParam);
        }

        size_t getProgressSteps() override
        {
            return 3;
        }

        void run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsInput>(getInput(1));
            auto pParam = std::dynamic_pointer_cast<CCutParam>(m_pParam);

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
                //Distance map
                CMat distance;
                cv::distanceTransform(imgSrc, distance, cv::DIST_L2, cv::DIST_MASK_3, CV_32F);

                //Gaussian filter to merge centers away by less than size/2
                double sigma = (pParam->m_size - 1) / 6.0;
                cv::GaussianBlur(distance, distance, cv::Size(3,3), sigma, sigma);

                //Dilation
                CMat distanceDilated;
                auto kernel = cv::getStructuringElement(cv::MorphShapes::MORPH_ELLIPSE, cv::Size(pParam->m_size, pParam->m_size));
                cv::dilate(distance, distanceDilated, kernel);

                //Local maximum are where distanceMap == dilated distanceMap
                cv::Mat localMaxima = (distance != 0) & (distance == distanceDilated);

                //Label each center
                std::vector<cv::Vec4i> hierarchy;
                std::vector<std::vector<cv::Point>> contours;
                cv::findContours(localMaxima, contours, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE);
                cv::Mat markers(localMaxima.size(), CV_32S);
                markers = cv::Scalar::all(0);
                int idx=0, compCount=0;

                for( ; idx >= 0; idx = hierarchy[idx][0], compCount++ )
                    cv::drawContours(markers, contours, idx, cv::Scalar::all(compCount+1), -1, cv::LINE_8, hierarchy);

                //Watershed with local max as seeds
                CMat colorSrc;
                cv::cvtColor(imgSrc, colorSrc, cv::COLOR_GRAY2RGB);
                cv::watershed(colorSrc, markers);
                imgDst = imgSrc & (markers > 0);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            endTaskRun();
            emit m_signalHandler->doProgress();
            applyGraphicsMask(imgSrc, imgDst, 0);

            auto pOutput = std::dynamic_pointer_cast<CImageIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(imgDst);

            emit m_signalHandler->doProgress();
        }
};

class CCutFactory : public CTaskFactory
{
    public:

        CCutFactory()
        {
            m_info.m_name = QObject::tr("Cut").toStdString();
            m_info.m_description = QObject::tr("This process cut connected objects on binary images according to a reference size.").toStdString();
            m_info.m_path = QObject::tr("Core/Utils").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/default-process.png").toStdString();
            m_info.m_keywords = "cut,connected,separate,binary";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<CCutParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<CCut>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<CCutParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<CCut>(m_info.m_name, pDerivedParam);
        }
};

#endif // CCUT_HPP
