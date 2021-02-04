#ifndef DATAPROCESSTOOLS_HPP
#define DATAPROCESSTOOLS_HPP

namespace Ikomia
{
    namespace Utils
    {
        namespace Image
        {
            inline double   getMaxValueFromDepth(int depth)
            {
                double maxValue = 1;
                switch(depth)
                {
                    case CV_8S: maxValue = 127; break;
                    case CV_8U: maxValue = 255; break;
                    case CV_16S: maxValue = 32767; break;
                    case CV_16U: maxValue = 65535; break;
                    case CV_32S: maxValue = 2147483647; break;
                    case CV_32F: maxValue = 1.0; break;
                    case CV_64F: maxValue = 1.0; break;
                }
                return maxValue;
            }
            inline CMat     createOverlayMask(const CMat& image, const CMat& colorMap)
            {
                CMat srcOvrImg, ovrImg;

                if(image.depth() != CV_8U)
                    image.convertTo(srcOvrImg, CV_8U);
                else
                    srcOvrImg = image;

                cv::applyColorMap(srcOvrImg, ovrImg, colorMap);
                cv::cvtColor(ovrImg, ovrImg, cv::COLOR_RGB2RGBA);

                #pragma omp parallel for
                    for(int i=0; i<ovrImg.rows; ++i)
                    {
                        for(int j=0; j<ovrImg.cols; ++j)
                        {
                            if(ovrImg.at<cv::Vec4b>(i, j)[0] == 0 && ovrImg.at<cv::Vec4b>(i, j)[1] == 0 && ovrImg.at<cv::Vec4b>(i, j)[2] == 0)
                                ovrImg.at<cv::Vec4b>(i, j)[3] = 0;
                        }
                    }
                return ovrImg;
            }
        }
    }
}

#endif // DATAPROCESSTOOLS_HPP
