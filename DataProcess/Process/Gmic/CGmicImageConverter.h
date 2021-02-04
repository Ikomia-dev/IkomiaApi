#ifndef CGMICIMAGECONVERTER_H
#define CGMICIMAGECONVERTER_H

#include "gmic.h"
#include "opencv2/opencv.hpp"

class CGmicImageConverter
{
    public:

        static void convert(const cv::Mat& src, gmic_image<float>& dst);
        static void convert(const gmic_image<float>& src, cv::Mat& dst);
};

#endif // CGMICIMAGECONVERTER_H
