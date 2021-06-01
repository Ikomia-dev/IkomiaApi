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

#ifndef COCVHOUGHLINES_HPP
#define COCVHOUGHLINES_HPP

#include "Core/CImageProcess2d.h"

//-------------------------------//
//----- COcvHoughLinesParam -----//
//-------------------------------//
class COcvHoughLinesParam: public CProtocolTaskParam
{
    public:

        COcvHoughLinesParam() : CProtocolTaskParam()
        {
        }

        void        setParamMap(const UMapString& paramMap) override
        {
            m_bProbabilistic = std::stoi(paramMap.at("probabilistic"));
            m_rho = std::stod(paramMap.at("rho"));
            m_theta = std::stod(paramMap.at("theta"));
            m_threshold = std::stoi(paramMap.at("threshold"));
            m_srn = std::stod(paramMap.at("srn"));
            m_stn = std::stod(paramMap.at("stn"));
            m_minTheta = std::stod(paramMap.at("minTheta"));
            m_maxTheta = std::stod(paramMap.at("maxTheta"));
            m_minLength = std::stod(paramMap.at("minLength"));
            m_maxGap = std::stod(paramMap.at("maxGap"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("probabilistic", std::to_string(m_bProbabilistic)));
            map.insert(std::make_pair("rho", std::to_string(m_rho)));
            map.insert(std::make_pair("theta", std::to_string(m_theta)));
            map.insert(std::make_pair("threshold", std::to_string(m_threshold)));
            map.insert(std::make_pair("srn", std::to_string(m_srn)));
            map.insert(std::make_pair("stn", std::to_string(m_stn)));
            map.insert(std::make_pair("minTheta", std::to_string(m_minTheta)));
            map.insert(std::make_pair("maxTheta", std::to_string(m_maxTheta)));
            map.insert(std::make_pair("minLength", std::to_string(m_minLength)));
            map.insert(std::make_pair("maxGap", std::to_string(m_maxGap)));
            return map;
        }

    public:

        bool    m_bProbabilistic = false;
        double  m_rho = 1.0;
        double  m_theta = CV_PI / 180.0;
        int     m_threshold = 150;
        double  m_srn = 0.0;
        double  m_stn = 0.0;
        double  m_minTheta = 0.0;
        double  m_maxTheta = CV_PI;
        double  m_minLength = 0;
        double  m_maxGap = 0;
};

//--------------------------//
//----- COcvHoughLines -----//
//--------------------------//
class COcvHoughLines : public CImageProcess2d
{
    public:

        COcvHoughLines() : CImageProcess2d()
        {
            addOutput(std::make_shared<CImageProcessIO>());
            addOutput(std::make_shared<CGraphicsProcessOutput>());
            getOutput(1)->setDataType(IODataType::IMAGE_BINARY);
        }
        COcvHoughLines(const std::string name, const std::shared_ptr<COcvHoughLinesParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<COcvHoughLinesParam>(*pParam);
            addOutput(std::make_shared<CImageProcessIO>());
            addOutput(std::make_shared<CGraphicsProcessOutput>());
            getOutput(1)->setDataType(IODataType::IMAGE_BINARY);
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        void    run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsProcessInput>(getInput(1));
            auto pParam = std::dynamic_pointer_cast<COcvHoughLinesParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            std::vector<cv::Vec2f> lines;
            std::vector<cv::Vec4i> linesP;
            cv::Mat imgSrc = pInput->getImage();
            createGraphicsMask(imgSrc.cols, imgSrc.rows, pGraphicsInput);
            emit m_signalHandler->doProgress();

            try
            {
                if(pParam->m_bProbabilistic == false)
                    cv::HoughLines(imgSrc, lines, pParam->m_rho, pParam->m_theta, pParam->m_threshold, pParam->m_srn, pParam->m_stn, pParam->m_minTheta, pParam->m_maxTheta);
                else
                    cv::HoughLinesP(imgSrc, linesP, pParam->m_rho, pParam->m_theta, pParam->m_threshold, pParam->m_minLength, pParam->m_maxGap);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }
            emit m_signalHandler->doProgress();

            forwardInputImage(0, 0);

            if(pParam->m_bProbabilistic == false)
                manageGraphicsOutput(lines);
            else
                manageGraphicsOutput(linesP);

            manageBinaryOutput();
            emit m_signalHandler->doProgress();
            endTaskRun();
        }

        void    manageGraphicsOutput(const std::vector<cv::Vec2f>& lines)
        {
            assert(m_graphicsContextPtr);
            auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            CMat imgSrc = pInput->getImage();
            auto pOutput = std::dynamic_pointer_cast<CGraphicsProcessOutput>(getOutput(2));
            assert(pOutput);
            pOutput->setNewLayer(getName());
            pOutput->setImageIndex(0);

            for(size_t i=0; i<lines.size(); ++i)
            {
                double rho = lines[i][0];
                double theta = lines[i][1];
                double cosTheta = cos(theta);
                double sinTheta = sin(theta);
                double x0 = rho * cosTheta;
                double y0 = rho * sinTheta;
                CPointF p1(x0 + imgSrc.cols*(-sinTheta), y0 + imgSrc.rows*(cosTheta));
                CPointF p2(x0 - imgSrc.cols*(-sinTheta), y0 - imgSrc.rows*(cosTheta));
                clipPoint(p1, imgSrc.cols, imgSrc.rows);
                clipPoint(p2, imgSrc.cols, imgSrc.rows);

                PolygonF poly;
                poly.push_back(p1);
                poly.push_back(p2);
                pOutput->addPolyline(poly);
            }
        }
        void    manageGraphicsOutput(const std::vector<cv::Vec4i>& lines)
        {
            assert(m_graphicsContextPtr);
            auto pOutput = std::dynamic_pointer_cast<CGraphicsProcessOutput>(getOutput(2));
            assert(pOutput);
            pOutput->setNewLayer(getName());
            pOutput->setImageIndex(0);

            for(size_t i=0; i<lines.size(); ++i)
            {
                PolygonF polyline;
                polyline.push_back(CPointF(lines[i][0], lines[i][1]));
                polyline.push_back(CPointF(lines[i][2], lines[i][3]));
                pOutput->addPolyline(polyline);
            }
        }

        void    manageBinaryOutput()
        {
            auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pOutputGraphics = std::dynamic_pointer_cast<CGraphicsProcessOutput>(getOutput(2));

            cv::Mat imgSrc = pInput->getImage();
            CGraphicsConversion graphicsConverter(imgSrc.cols, imgSrc.rows);
            CMat binary = graphicsConverter.graphicsToBinaryMask(pOutputGraphics->getItems());
            applyGraphicsMaskToBinary(binary, binary, 0);

            auto pOutput = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(1));
            if(pOutput)
                pOutput->setImage(binary);
        }

        void    clipPoint(CPointF& pt, int width, int height)
        {
            if(pt.m_x < 0)
                pt.m_x = 0;
            else if(pt.m_x >= width)
                pt.m_x = width - 1;

            if(pt.m_y < 0)
                pt.m_y = 0;
            else if(pt.m_y >= height)
                pt.m_y = height - 1;
        }
};

class COcvHoughLinesFactory : public CProcessFactory
{
    public:

        COcvHoughLinesFactory()
        {
            m_info.m_name = QObject::tr("Hough lines").toStdString();
            m_info.m_description = QObject::tr("The function implements the standard or standard multi-scale Hough transform algorithm and the probabilistic Hough transform for line detection.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Image processing/Feature detection").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "line,polar";
            m_info.m_docLink = "https://docs.opencv.org/4.0.1/dd/d1a/group__imgproc__feature.html#ga46b4e588934f6c8dfd509cc6e0e4545a";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvHoughLinesParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvHoughLines>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvHoughLinesParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvHoughLines>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVHOUGHLINES_HPP
