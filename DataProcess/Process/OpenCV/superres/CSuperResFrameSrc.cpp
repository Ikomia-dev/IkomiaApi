#include "CSuperResFrameSrc.h"

CSuperResFrameSrc::CSuperResFrameSrc(bool bCuda)
{
    m_bCuda = bCuda;
}

CSuperResFrameSrc::~CSuperResFrameSrc()
{
}

void CSuperResFrameSrc::setFrame(CMat frame)
{
    frame.copyTo(m_frame);
}

void CSuperResFrameSrc::nextFrame(cv::OutputArray output)
{
    if(m_bCuda == true)
    {
        cv::cuda::GpuMat cuFrame;
        cuFrame.upload(m_frame);
        cuFrame.copyTo(output);
    }
    else
        m_frame.copyTo(output);
}

void CSuperResFrameSrc::reset()
{
    m_frame.release();
}
