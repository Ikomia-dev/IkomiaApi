#include "CVideoProcessOF.h"

CVideoProcessOF::CVideoProcessOF() : CVideoProcess()
{
}

CVideoProcessOF::CVideoProcessOF(const std::string& name) : CVideoProcess(name)
{
}

void CVideoProcessOF::drawOptFlowMap(const CMat& flow, CMat& cflowmap, int step)
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

CMat CVideoProcessOF::flowToDisplay(const CMat& flow)
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

void CVideoProcessOF::makeOF(const CMat& nextGray)
{
    if(m_bUseOpenCL)
        m_pOF->calc(m_imgPrev, nextGray, m_imgFlow.getUMat(cv::ACCESS_RW));
    else
        m_pOF->calc(m_imgPrev, nextGray, m_imgFlow);

    // Store current gray image for next calculus
    m_imgPrev = nextGray;
}

CMat CVideoProcessOF::manageInputs(const CMat& imgSrc)
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

void CVideoProcessOF::manageOuputs(const CMat& imgSrc)
{
    CMat imgDst = flowToDisplay(m_imgFlow);
    cv::normalize(imgDst, imgDst, 0, 255, cv::NORM_MINMAX);
    imgDst.convertTo(imgDst, CV_8UC3);

    applyGraphicsMask(imgSrc, imgDst, 0);

    auto pOutput = std::dynamic_pointer_cast<CVideoProcessIO>(getOutput(0));
    if(pOutput)
        pOutput->setImage(imgDst);
}
