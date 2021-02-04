#ifndef CSUPERRESFRAMESRC_HPP
#define CSUPERRESFRAMESRC_HPP

#include "opencv2/opencv.hpp"
#include "opencv2/superres.hpp"
#include "Data/CMat.hpp"

class CSuperResFrameSrc: public cv::superres::FrameSource
{
    public:

        CSuperResFrameSrc(bool bCuda);
        virtual ~CSuperResFrameSrc();

        void            setFrame(CMat frame);

        virtual void    nextFrame(cv::OutputArray output) override;

        virtual void    reset() override;

    protected:

        CMat    m_frame;
        bool    m_bCuda = false;
};

#endif // CSUPERRESFRAMESRC_HPP
