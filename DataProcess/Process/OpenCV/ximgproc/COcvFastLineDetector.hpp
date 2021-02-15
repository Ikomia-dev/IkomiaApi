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

#ifndef COCVFASTLINEDETECTOR_HPP
#define COCVFASTLINEDETECTOR_HPP

#include "opencv2/ximgproc.hpp"
#include "Core/CImageProcess2d.h"

//-------------------------------------//
//----- COcvFastLineDetectorParam -----//
//-------------------------------------//
class COcvFastLineDetectorParam: public CProtocolTaskParam
{
    public:

        COcvFastLineDetectorParam() : CProtocolTaskParam()
        {
        }

        void        setParamMap(const UMapString& paramMap) override
        {
            m_lengthThreshold = std::stoi(paramMap.at("lengthThreshold"));
            m_distanceThreshold = std::stof(paramMap.at("distanceThreshold"));
            m_cannyThreshold1 = std::stod(paramMap.at("cannyThreshold1"));
            m_cannyThreshold1 = std::stod(paramMap.at("cannyThreshold2"));
            m_cannyApertureSize = std::stoi(paramMap.at("cannyApertureSize"));
            m_bMerge = std::stoi(paramMap.at("isMerge"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("lengthThreshold", std::to_string(m_lengthThreshold)));
            map.insert(std::make_pair("distanceThreshold", std::to_string(m_distanceThreshold)));
            map.insert(std::make_pair("cannyThreshold1", std::to_string(m_cannyThreshold1)));
            map.insert(std::make_pair("cannyThreshold2", std::to_string(m_cannyThreshold1)));
            map.insert(std::make_pair("cannyApertureSize", std::to_string(m_cannyApertureSize)));
            map.insert(std::make_pair("isMerge", std::to_string(m_bMerge)));
            return map;
        }

    public:

        int     m_lengthThreshold = 10;
        float   m_distanceThreshold = 1.414213562f;
        double  m_cannyThreshold1 = 50;
        double  m_cannyThreshold2 = 50;
        int     m_cannyApertureSize = 3;
        bool    m_bMerge = false;
};

//--------------------------------//
//----- COcvFastLineDetector -----//
//--------------------------------//
class COcvFastLineDetector : public CImageProcess2d
{
    public:

        COcvFastLineDetector() : CImageProcess2d()
        {
            addOutput(std::make_shared<CGraphicsProcessOutput>());
        }
        COcvFastLineDetector(const std::string name, const std::shared_ptr<COcvFastLineDetectorParam>& pParam) : CImageProcess2d(name)
        {
            addOutput(std::make_shared<CGraphicsProcessOutput>());
            m_pParam = std::make_shared<COcvFastLineDetectorParam>(*pParam);
        }

        size_t  getProgressSteps() override
        {
            return 4;
        }

        void    run() override
        {
            beginTaskRun();

            auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pParam = std::dynamic_pointer_cast<COcvFastLineDetectorParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            std::vector<cv::Vec4f> lines;
            CMat imgSrc = pInput->getImage();
            emit m_signalHandler->doProgress();

            try
            {
                CMat imgTmp;
                if(imgSrc.channels() > 1)
                    cv::cvtColor(imgSrc, imgTmp, cv::COLOR_RGB2GRAY);
                else
                    imgTmp = imgSrc;

                auto ptr = cv::ximgproc::createFastLineDetector(pParam->m_lengthThreshold, pParam->m_distanceThreshold, pParam->m_cannyThreshold1,
                                                                pParam->m_cannyThreshold2, pParam->m_cannyApertureSize, pParam->m_bMerge);
                ptr->detect(imgTmp, lines);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            endTaskRun();
            emit m_signalHandler->doProgress();

            manageLinesOutput(lines);
            emit m_signalHandler->doProgress();

            forwardInputImage();
            emit m_signalHandler->doProgress();
        }

        void    manageLinesOutput(const std::vector<cv::Vec4f>& lines)
        {
            auto pOutput = std::dynamic_pointer_cast<CGraphicsProcessOutput>(getOutput(1));
            if(pOutput == nullptr)
                throw CException(CoreExCode::NULL_POINTER, "Invalid graphics output", __func__, __FILE__, __LINE__);

            pOutput->setNewLayer(getName());
            pOutput->setImageIndex(0);

            for(size_t i=0; i<lines.size(); ++i)
            {
                std::vector<CPointF> line;
                line.push_back(CPointF(lines[i][0], lines[i][1]));
                line.push_back(CPointF(lines[i][2], lines[i][3]));
                pOutput->addPolyline(line);
            }
        }
};

class COcvFastLineDetectorFactory : public CProcessFactory
{
    public:

        COcvFastLineDetectorFactory()
        {
            m_info.m_name = QObject::tr("Fast Line Detector").toStdString();
            m_info.m_description = QObject::tr("This process finds lines in input image and creates graphics layer with lines as output).").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Extended Image Processing/Fast line detector").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Canny,line,fast,detector,hysteresis";
            m_info.m_authors = "Jin Han Lee, Sehyung Lee, Guoxuan Zhang, Jongwoo Lim, Wan Kyun Chung, and Il Hong Suh";
            m_info.m_article = "Outdoor place recognition in urban environments using straight lines.";
            m_info.m_journal = "In 2014 IEEE International Conference on Robotics and Automation (ICRA), pages 5550–5557. IEEE.";
            m_info.m_year = 2014;
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/df/d4c/classcv_1_1ximgproc_1_1FastLineDetector.html";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvFastLineDetectorParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvFastLineDetector>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvFastLineDetectorParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvFastLineDetector>(m_info.m_name, pDerivedParam);

        }
};

#endif // COCVFASTLINEDETECTOR_HPP
