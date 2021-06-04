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

#include "CVideoOFTask.h"

CVideoOFTask::CVideoOFTask() : CVideoTask()
{
}

CVideoOFTask::CVideoOFTask(const std::string& name) : CVideoTask(name)
{
}

void CVideoOFTask::drawOptFlowMap(const CMat& flow, CMat& cflowmap, int step)
{
    using namespace cv;
    Scalar color;

    if(m_graphicsContextPtr)
    {
        auto pProperty = m_graphicsContextPtr->getPolylinePropertyPtr();
        color = Scalar(pProperty->m_penColor[0], pProperty->m_penColor[1], pProperty->m_penColor[2]);
    }
    else
        color = Scalar(255,0,0);

    for(int y = 0; y < cflowmap.rows; y += step)
    {
        for(int x = 0; x < cflowmap.cols; x += step)
        {
            const Point2f& fxy = flow.at<Point2f>(y, x);
            line(cflowmap, Point(x,y), Point(cvRound(x+fxy.x), cvRound(y+fxy.y)), color);
            circle(cflowmap, Point(x,y), 2, color, -1);
        }
    }
}

CMat CVideoOFTask::flowToDisplay(const CMat& flow)
{
    using namespace cv;
    CMat flow_split[2];
    CMat magnitude, angle;
    CMat hsv_split[3], hsv, rgb;
    split(flow, flow_split);
    cartToPolar(flow_split[0], flow_split[1], magnitude, angle, true);
    normalize(magnitude, magnitude, 0, 1, NORM_MINMAX);
    hsv_split[0] = angle; // already in degrees - no normalization needed
    hsv_split[1] = Mat::ones(angle.size(), angle.type());
    hsv_split[2] = magnitude;
    merge(hsv_split, 3, hsv);
    cvtColor(hsv, rgb, COLOR_HSV2BGR);
    return rgb;
}

void CVideoOFTask::makeOF(const CMat& nextGray)
{
    if(m_bUseOpenCL)
        m_pOF->calc(m_imgPrev, nextGray, m_imgFlow.getUMat(cv::ACCESS_RW));
    else
        m_pOF->calc(m_imgPrev, nextGray, m_imgFlow);

    // Store current gray image for next calculus
    m_imgPrev = nextGray;
}

CMat CVideoOFTask::manageInputs(const CMat& imgSrc)
{
    cv::ocl::setUseOpenCL(m_bUseOpenCL);
    if(m_bUseOpenCL && !cv::ocl::useOpenCL())
    {
        throw CException(CoreExCode::INVALID_PARAMETER, "OpenCL is not available on this platform.", __func__, __FILE__, __LINE__);
    }

    cv::Mat nextGray;
    if(imgSrc.channels() == 3)
    {
        CMat imgLab, Lab[3];
        cv::cvtColor(imgSrc, imgLab, cv::COLOR_RGB2Lab);
        cv::split(imgLab,Lab);
        nextGray = Lab[0];
    }
    else
        nextGray = imgSrc;

    // For first video image, we apply optical flow on same images
    if(m_imgPrev.empty())
        m_imgPrev = nextGray;

    return nextGray;
}

void CVideoOFTask::manageOuputs(const CMat& imgSrc)
{
    CMat imgDst = flowToDisplay(m_imgFlow);
    cv::normalize(imgDst, imgDst, 0, 255, cv::NORM_MINMAX);
    imgDst.convertTo(imgDst, CV_8UC3);

    applyGraphicsMask(imgSrc, imgDst, 0);

    auto pOutput = std::dynamic_pointer_cast<CVideoIO>(getOutput(0));
    if(pOutput)
        pOutput->setImage(imgDst);
}
