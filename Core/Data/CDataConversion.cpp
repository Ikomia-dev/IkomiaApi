#include "CDataConversion.h"
#include "UtilsTools.hpp"
#include "Main/CoreTools.hpp"

CDataConversion::CDataConversion()
{
}

QImage CDataConversion::CMatToQImage(CMat imgSrc, bool bCopy)
{
    if(imgSrc.data == nullptr)
        throw CException(CoreExCode::NULL_POINTER, "Invalid image: null data buffer", __func__, __FILE__, __LINE__);

    switch(imgSrc.type())
    {
        case CV_8UC1:
            if(bCopy)
            {
                QImage localImg(imgSrc.data, imgSrc.cols, imgSrc.rows, static_cast<int>(imgSrc.step), QImage::Format_Grayscale8);
                return localImg.copy();
            }
            else
                return QImage(imgSrc.data, imgSrc.cols, imgSrc.rows, static_cast<int>(imgSrc.step), QImage::Format_Grayscale8);

        case CV_8UC3:
            if(bCopy)
            {
                QImage localImg(imgSrc.data, imgSrc.cols, imgSrc.rows, static_cast<int>(imgSrc.step), QImage::Format_RGB888);
                return localImg.copy();
            }
            else
                return QImage(imgSrc.data, imgSrc.cols, imgSrc.rows, static_cast<int>(imgSrc.step), QImage::Format_RGB888);

        case CV_8UC4:
            if(bCopy)
            {
                QImage localImg(imgSrc.data, imgSrc.cols, imgSrc.rows, static_cast<int>(imgSrc.step), QImage::Format_RGBA8888);
                return localImg.copy();
            }
            else
                return QImage(imgSrc.data, imgSrc.cols, imgSrc.rows, static_cast<int>(imgSrc.step), QImage::Format_RGBA8888);

        case CV_16SC1:
        case CV_16SC3:
        case CV_16SC4:
        {
            CMat mat8b;
            to8Bits(imgSrc, mat8b);
            return CMatToQImage(mat8b, true);
        }

        case CV_16UC1:
        case CV_16UC3:
        case CV_16UC4:
        {            
            CMat mat8b;
            to8Bits(imgSrc, mat8b);
            return CMatToQImage(mat8b, true);
        }

        case CV_32SC1:
        case CV_32SC3:
        case CV_32SC4:
        {
            CMat mat8b;
            to8Bits(imgSrc, mat8b);
            return CMatToQImage(mat8b, true);
        }

        case CV_32FC1:
        case CV_32FC3:
        case CV_32FC4:
        case CV_64FC1:
        case CV_64FC3:
        case CV_64FC4:
        {
            CMat mat8b;
            to8Bits(imgSrc, mat8b);
            return CMatToQImage(mat8b, true);
        }

        default:
            return QImage();
    }
    return QImage();
}

void CDataConversion::to8Bits(const CMat& src, CMat& dst)
{
    if(src.channels() == 4)
    {
        std::vector<cv::Mat> channels(4);
        cv::split(src, channels);
        CMat alpha = channels[3];
        channels.pop_back();
        CMat noAlphaImg;
        cv::merge(channels, noAlphaImg);
        cv::normalize(noAlphaImg, noAlphaImg, 0, 255, cv::NORM_MINMAX, CV_8UC(noAlphaImg.channels()));
        cv::normalize(alpha, alpha, 0, 255, cv::NORM_MINMAX, CV_8UC(alpha.channels()));
        cv::split(noAlphaImg, channels);
        channels.push_back(alpha);
        cv::merge(channels, dst);
    }
    else
        cv::normalize(src, dst, 0, 255, cv::NORM_MINMAX, CV_8UC(src.channels()));
}

void CDataConversion::grayscaleToAlpha(const CMat &src, CMat &dst)
{
    if(src.channels() != 1)
        throw CException(CoreExCode::INVALID_DIMENSION, QObject::tr("Grayscale image required").toStdString(), __func__, __FILE__, __LINE__);

    std::vector<cv::Mat> channels(4);
    channels[0] = channels[1] = channels[2] = src;

    //Create alpha channel
    cv::Mat cvSrc = src;
    //Operator > generates an 8 bits unsigned int buffer...
    cv::Mat alpha = cvSrc > 0;
    //So we have to multiply it by a correct factor in case source image has a higher bit depth
    if(cvSrc.depth() != CV_8U)
    {
        double factor = Utils::Image::getMaxValue(src) / 255;
        alpha.convertTo(alpha, cvSrc.type(), factor);
    }
    channels[3] = alpha;

    //Merge source image + alpha channel
    cv::merge(channels, dst);
}
