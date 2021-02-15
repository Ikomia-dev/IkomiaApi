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

#include "CGmicImageConverter.h"
#include "CException.h"

void CGmicImageConverter::convert(const cv::Mat &src, gmic_image<float> &dst)
{
    if(dst._data == nullptr)
        dst.assign(src.cols, src.rows, 1, src.channels());

    if(src.isContinuous())
    {
        switch(src.depth())
        {
            // case CV_1U: { // 1-bit int.
            //    IplImage *src1 = cvCreateImage(cvGetSize(src),CV_8U,1);
            //    cvConvert(src,src1);
            //    CImg<ucharT>((unsigned char*)src1->imageData,src1->nChannels,src1.cols,src1.rows,1,true).
            //      get_permute_axes("yzcx").move_to(*this);
            //    cvReleaseImage(&src1);
            //  } break;

            case CV_8U: // 8-bit unsigned int.
                if(src.channels() == 1)
                {
                    const unsigned char* pSrc = reinterpret_cast<const unsigned char*>(src.ptr());
                    float* pDst = reinterpret_cast<float*>(dst._data);

                    for(int i=0; i<src.cols*src.rows; ++i)
                        *pDst++ = static_cast<float>(pSrc[i]);
                }
                else
                {
                    const unsigned char* pSrc = reinterpret_cast<const unsigned char*>(src.ptr());
                    const unsigned char* pEndSrc;
                    const unsigned int w3 = src.cols * 3;
                    const unsigned int iplOffset = src.step1() - w3;
                    unsigned int height = src.rows;

                    float* pDstR = &dst(0, 0, 0, 0);
                    float* pDstG = &dst(0, 0, 0, (dst._spectrum >= 2) ? 1 : 0);
                    float* pDstB = &dst(0, 0, 0, (dst._spectrum >= 3) ? 2 : 0);

                    while(height--)
                    {
                        pEndSrc = pSrc + w3;
                        while (pSrc != pEndSrc)
                        {
                            *pDstR++ = static_cast<float>(pSrc[0]);
                            *pDstG++ = static_cast<float>(pSrc[1]);
                            *pDstB++ = static_cast<float>(pSrc[2]);
                            pSrc += 3;
                        }
                        pSrc += iplOffset;
                    }
                }
                break;

            case CV_8S: // 8-bit signed int.
                if(src.channels() == 1)
                {
                    const char* pSrc = reinterpret_cast<const char*>(src.ptr());
                    float* pDst = reinterpret_cast<float*>(dst._data);

                    for(int i=0; i<src.cols*src.rows; ++i)
                        *pDst++ = static_cast<float>(pSrc[i]);
                }
                else
                {
                    const char* pSrc = reinterpret_cast<const char*>(src.ptr());
                    const char* pEndSrc;
                    const unsigned int w3 = src.cols * 3;
                    const unsigned int iplOffset = src.step1() - w3;
                    unsigned int height = src.rows;

                    float* pDstR = &dst(0, 0, 0, 0);
                    float* pDstG = &dst(0, 0, 0, (dst._spectrum >= 2) ? 1 : 0);
                    float* pDstB = &dst(0, 0, 0, (dst._spectrum >= 3) ? 2 : 0);

                    while(height--)
                    {
                        pEndSrc = pSrc + w3;
                        while (pSrc != pEndSrc)
                        {
                            *pDstR++ = static_cast<float>(pSrc[0]);
                            *pDstG++ = static_cast<float>(pSrc[1]);
                            *pDstB++ = static_cast<float>(pSrc[2]);
                            pSrc += 3;
                        }
                        pSrc += iplOffset;
                    }
                }
                break;

            case CV_16U: // 16-bit unsigned int.
                if(src.channels() == 1)
                {
                    const unsigned short* pSrc = reinterpret_cast<const unsigned short*>(src.ptr());
                    float* pDst = reinterpret_cast<float*>(dst._data);

                    for(int i=0; i<src.cols*src.rows; ++i)
                        *pDst++ = static_cast<float>(pSrc[i]);
                }
                else
                {
                    const unsigned short* pSrc = reinterpret_cast<const unsigned short*>(src.ptr());
                    const unsigned short* pEndSrc;
                    const unsigned int w3 = src.cols * 3;
                    const unsigned int iplOffset = src.step1() - w3;
                    unsigned int height = src.rows;

                    float* pDstR = &dst(0, 0, 0, 0);
                    float* pDstG = &dst(0, 0, 0, (dst._spectrum >= 2) ? 1 : 0);
                    float* pDstB = &dst(0, 0, 0, (dst._spectrum >= 3) ? 2 : 0);

                    while(height--)
                    {
                        pEndSrc = pSrc + w3;
                        while (pSrc != pEndSrc)
                        {
                            *pDstR++ = static_cast<float>(pSrc[0]);
                            *pDstG++ = static_cast<float>(pSrc[1]);
                            *pDstB++ = static_cast<float>(pSrc[2]);
                            pSrc += 3;
                        }
                        pSrc += iplOffset;
                    }
                }
                break;

            case CV_16S: // 16-bit signed int.
                if(src.channels() == 1)
                {
                    const short* pSrc = reinterpret_cast<const short*>(src.ptr());
                    float* pDst = reinterpret_cast<float*>(dst._data);

                    for(int i=0; i<src.cols*src.rows; ++i)
                        *pDst++ = static_cast<float>(pSrc[i]);
                }
                else
                {
                    const short* pSrc = reinterpret_cast<const short*>(src.ptr());
                    const short* pEndSrc;
                    const unsigned int w3 = src.cols * 3;
                    const unsigned int iplOffset = src.step1() - w3;
                    unsigned int height = src.rows;

                    float* pDstR = &dst(0, 0, 0, 0);
                    float* pDstG = &dst(0, 0, 0, (dst._spectrum >= 2) ? 1 : 0);
                    float* pDstB = &dst(0, 0, 0, (dst._spectrum >= 3) ? 2 : 0);

                    while(height--)
                    {
                        pEndSrc = pSrc + w3;
                        while (pSrc != pEndSrc)
                        {
                            *pDstR++ = static_cast<float>(pSrc[0]);
                            *pDstG++ = static_cast<float>(pSrc[1]);
                            *pDstB++ = static_cast<float>(pSrc[2]);
                            pSrc += 3;
                        }
                        pSrc += iplOffset;
                    }
                }
                break;

            case CV_32S: // 32-bit signed int.
                if(src.channels() == 1)
                {
                    const int* pSrc = reinterpret_cast<const int*>(src.ptr());
                    float* pDst = reinterpret_cast<float*>(dst._data);

                    for(int i=0; i<src.cols*src.rows; ++i)
                        *pDst++ = static_cast<float>(pSrc[i]);
                }
                else
                {
                    const int* pSrc = reinterpret_cast<const int*>(src.ptr());
                    const int* pEndSrc;
                    const unsigned int w3 = src.cols * 3;
                    const unsigned int iplOffset = src.step1() - w3;
                    unsigned int height = src.rows;

                    float* pDstR = &dst(0, 0, 0, 0);
                    float* pDstG = &dst(0, 0, 0, (dst._spectrum >= 2) ? 1 : 0);
                    float* pDstB = &dst(0, 0, 0, (dst._spectrum >= 3) ? 2 : 0);

                    while(height--)
                    {
                        pEndSrc = pSrc + w3;
                        while (pSrc != pEndSrc)
                        {
                            *pDstR++ = static_cast<float>(pSrc[0]);
                            *pDstG++ = static_cast<float>(pSrc[1]);
                            *pDstB++ = static_cast<float>(pSrc[2]);
                            pSrc += 3;
                        }
                        pSrc += iplOffset;
                    }
                }
                break;

            case CV_32F: // 32-bit float.
                if(src.channels() == 1)
                {
                    const float* pSrc = reinterpret_cast<const float*>(src.ptr());
                    float* pDst = reinterpret_cast<float*>(dst._data);

                    for(int i=0; i<src.cols*src.rows; ++i)
                        *pDst++ = pSrc[i];
                }
                else
                {
                    const float* pSrc = reinterpret_cast<const float*>(src.ptr());
                    const float* pEndSrc;
                    const unsigned int w3 = src.cols * 3;
                    const unsigned int iplOffset = src.step1() - w3;
                    unsigned int height = src.rows;

                    float* pDstR = &dst(0, 0, 0, 0);
                    float* pDstG = &dst(0, 0, 0, (dst._spectrum >= 2) ? 1 : 0);
                    float* pDstB = &dst(0, 0, 0, (dst._spectrum >= 3) ? 2 : 0);

                    while(height--)
                    {
                        pEndSrc = pSrc + w3;
                        while (pSrc != pEndSrc)
                        {
                            *pDstR++ = pSrc[0];
                            *pDstG++ = pSrc[1];
                            *pDstB++ = pSrc[2];
                            pSrc += 3;
                        }
                        pSrc += iplOffset;
                    }
                }
                break;

            case CV_64F: // 64-bit double.
                if(src.channels() == 1)
                {
                    const double* pSrc = reinterpret_cast<const double*>(src.ptr());
                    float* pDst = reinterpret_cast<float*>(dst._data);

                    for(int i=0; i<src.cols*src.rows; ++i)
                        *pDst++ = static_cast<float>(pSrc[i]);
                }
                else
                {
                    const double* pSrc = reinterpret_cast<const double*>(src.ptr());
                    const double* pEndSrc;
                    const unsigned int w3 = src.cols * 3;
                    const unsigned int iplOffset = src.step1() - w3;
                    unsigned int height = src.rows;

                    float* pDstR = &dst(0, 0, 0, 0);
                    float* pDstG = &dst(0, 0, 0, (dst._spectrum >= 2) ? 1 : 0);
                    float* pDstB = &dst(0, 0, 0, (dst._spectrum >= 3) ? 2 : 0);

                    while(height--)
                    {
                        pEndSrc = pSrc + w3;
                        while (pSrc != pEndSrc)
                        {
                            *pDstR++ = static_cast<float>(pSrc[0]);
                            *pDstG++ = static_cast<float>(pSrc[1]);
                            *pDstB++ = static_cast<float>(pSrc[2]);
                            pSrc += 3;
                        }
                        pSrc += iplOffset;
                    }
                }
                break;

            default:
                throw CException(CoreExCode::NOT_IMPLEMENTED, "Gmic converter: invalid cv::Mat depth");
                break;
        }
    }
    else
    {
        //cv::Size size = src.size();
        switch (src.depth())
        {
            /*case CV_8U: // 8-bit unsigned int.
                if(src.channels() == 1)
                {
                    gmic_image<ucharT> tmp(src.cols, src.rows);
                    for (int i = 0; i<size.height; ++i)
                    {
                        const unsigned char* row_i = src.ptr<unsigned char>(i);
                        unsigned char *row_o = tmp.data(0, i);
                        std::memcpy(row_o, row_i, size.width*sizeof(unsigned char));
                    }
                    tmp.move_to(dst);
                }
                else
                {
                    gmic_image<ucharT> tmp(src.cols, src.rows, 1, src.channels());
                    std::vector<cv::Mat> channels;
                    cv::split(src,channels);

                    for (int c = 0; c<src.channels(); ++c)
                    {
                        gmic_image<ucharT> plane = tmp.get_shared_channel(src.channels() - 1 - c);
                        for (int i = 0; i<size.height; ++i)
                        {
                            const unsigned char* row_i = channels[c].ptr<unsigned char>(i);
                            unsigned char *row_o = plane.data(0, i);
                            std::memcpy(row_o, row_i, size.width*sizeof(unsigned char));
                        }
                    }
                    tmp.move_to(dst);
                }
                break;

            case CV_8S: // 8-bit int.
                if(src.channels() == 1)
                {
                    gmic_image<charT> tmp(src.cols, src.rows);
                    for (int i = 0; i<size.height; ++i)
                    {
                        const char* row_i = src.ptr<char>(i);
                        char* row_o = tmp.data(0, i);
                        std::memcpy(row_o, row_i, size.width*sizeof(charT));
                    }
                    tmp.move_to(dst);
                }
                else
                {
                    gmic_image<charT> tmp(src.cols, src.rows, 1, src.channels());
                    std::vector<cv::Mat> channels;
                    cv::split(src, channels);

                    for (int c = 0; c<src.channels(); ++c)
                    {
                        gmic_image<charT> plane = tmp.get_shared_channel(src.channels() - 1 - c);
                        for (int i = 0; i<size.height; ++i)
                        {
                            const char* row_i = channels[c].ptr<char>(i);
                            char *row_o = plane.data(0, i);
                            std::memcpy(row_o, row_i, size.width*sizeof(char));
                        }
                    }
                    tmp.move_to(dst);
                }
                break;

            case CV_16S: // 16-bit int.
                if(src.channels() == 1)
                {
                    gmic_image<shortT> tmp(src.cols, src.rows);
                    for (int i = 0; i<size.height; ++i)
                    {
                        const short* row_i = src.ptr<short>(i);
                        short *row_o = tmp.data(0, i);
                        std::memcpy(row_o, row_i, size.width*sizeof(short));
                    }
                    tmp.move_to(dst);
                }
                else
                {
                    gmic_image<shortT> tmp(src.cols, src.rows, 1, src.channels());
                    std::vector<cv::Mat> channels;
                    cv::split(src, channels);

                    for (int c = 0; c<src.channels(); ++c)
                    {
                        gmic_image<shortT> plane = tmp.get_shared_channel(src.channels() - 1 - c);
                        for (int i = 0; i<size.height; ++i)
                        {
                            const short* row_i = channels[c].ptr<short>(i);
                            short *row_o = plane.data(0, i);
                            std::memcpy(row_o, row_i, size.width*sizeof(short));
                        }
                    }
                    tmp.move_to(dst);
                }
                break;

            case CV_32F: // 32-bit float.float
                if(src.channels() == 1)
                {
                    gmic_image<floatT> tmp(src.cols, src.rows);
                    for (int i = 0; i<size.height; ++i)
                    {
                        const float* row_i = src.ptr<float>(i);
                        float *row_o = tmp.data(0, i);
                        std::memcpy(row_o, row_i, size.width*sizeof(float));
                    }
                    tmp.move_to(dst);
                }
                else
                {
                    gmic_image<floatT> tmp(src.cols, src.rows, 1, src.channels());
                    std::vector<cv::Mat> channels;
                    cv::split(src, channels);

                    for (int c = 0; c<src.channels(); ++c)
                    {
                        gmic_image<floatT> plane = tmp.get_shared_channel(src.channels() - 1 - c);
                        for (int i = 0; i<size.height; ++i)
                        {
                            const float* row_i = channels[c].ptr<float>(i);
                            float *row_o = plane.data(0, i);
                            std::memcpy(row_o, row_i, size.width*sizeof(float));
                        }
                    }
                    tmp.move_to(dst);
                }
                break;

            case CV_64F: // 64-bit double.
                if(src.channels() == 1)
                {
                    gmic_image<doubleT> tmp(src.cols, src.rows);
                    for (int i = 0; i<size.height; ++i)
                    {
                        const double* row_i = src.ptr<double>(i);
                        double *row_o = tmp.data(0,i);
                        std::memcpy(row_o, row_i, size.width*sizeof(double));
                    }
                    tmp.move_to(dst);
                }
                else
                {
                    gmic_image<doubleT> tmp(src.cols, src.rows, 1, src.channels());
                    std::vector<cv::Mat> channels;
                    cv::split(src, channels);

                    for (int c = 0; c<src.channels(); ++c)
                    {
                        gmic_image<doubleT> plane = tmp.get_shared_channel(src.channels() - 1 - c);
                        for (int i = 0; i<size.height; ++i)
                        {
                            const double* row_i = channels[c].ptr<double>(i);
                            double *row_o = plane.data(0, i);
                            std::memcpy(row_o, row_i, size.width*sizeof(double));
                        }
                    }
                    tmp.move_to(dst);
                }
                break;*/

            default:
                throw CException(CoreExCode::NOT_IMPLEMENTED, "Gmic converter: unsupported bits per pixel");
                break;
        }
    }
}

void CGmicImageConverter::convert(const gmic_image<float> &src, cv::Mat &dst)
{
    if(dst.data == nullptr)
    {
        int cvType = dst.type();
        if(cvType == 0)
            cvType = src._spectrum > 1 ? CV_8UC3 : CV_8UC1;

        dst.create(src._height, src._width, cvType);
    }
    else if(dst.rows != (int)src._height || dst.cols != (int)src._width)
    {
        int cvType = dst.type();
        dst.create(src._height, src._width, cvType);
    }

    if(dst.isContinuous())
    {
        switch(dst.depth())
        {
            case CV_8U: // 8-bit unsigned int.
                if(dst.channels() == 1)
                {
                    unsigned char* pDst = reinterpret_cast<unsigned char*>(dst.ptr());
                    for(uint i=0; i<src._width*src._height; ++i)
                        *pDst++ = static_cast<unsigned char>(src._data[i]);
                }
                else
                {
                    unsigned char* pDst = reinterpret_cast<unsigned char*>(dst.ptr());
                    const float* pSrcR = &src(0, 0, 0, 0);
                    const float* pEndSrcR = pSrcR;
                    const float* pSrcG = &src(0, 0, 0, (src._spectrum >= 2) ? 1 : 0);
                    const float* pSrcB = &src(0, 0, 0, (src._spectrum >= 3) ? 2 : 0);
                    unsigned int height = src._height;

                    while (height--)
                    {
                        pEndSrcR += src._width;
                        while (pSrcR != pEndSrcR)
                        {
                            pDst[0] = static_cast<unsigned char>(*pSrcR++);
                            pDst[1] = static_cast<unsigned char>(*pSrcG++);
                            pDst[2] = static_cast<unsigned char>(*pSrcB++);
                            pDst += 3;
                        }
                    }
                }
                break;

            case CV_8S: // 8-bit signed int.
                if(dst.channels() == 1)
                {
                    char* pDst = reinterpret_cast<char*>(dst.ptr());
                    for(uint i=0; i<src._width*src._height; ++i)
                        *pDst++ = static_cast<char>(src._data[i]);
                }
                else
                {
                    char* pDst = reinterpret_cast<char*>(dst.ptr());
                    const float* pSrcR = &src(0, 0, 0, 0);
                    const float* pEndSrcR = pSrcR;
                    const float* pSrcG = &src(0, 0, 0, (src._spectrum >= 2) ? 1 : 0);
                    const float* pSrcB = &src(0, 0, 0, (src._spectrum >= 3) ? 2 : 0);
                    unsigned int height = src._height;

                    while (height--)
                    {
                        pEndSrcR += src._width;
                        while (pSrcR != pEndSrcR)
                        {
                            pDst[0] = static_cast<char>(*pSrcR++);
                            pDst[1] = static_cast<char>(*pSrcG++);
                            pDst[2] = static_cast<char>(*pSrcB++);
                            pDst += 3;
                        }
                    }
                }
                break;

            case CV_16U: // 16-bit unsigned int.
                if(dst.channels() == 1)
                {
                    unsigned short* pDst = reinterpret_cast<unsigned short*>(dst.ptr());
                    for(uint i=0; i<src._width*src._height; ++i)
                        *pDst++ = static_cast<unsigned short>(src._data[i]);
                }
                else
                {
                    unsigned short* pDst = reinterpret_cast<unsigned short*>(dst.ptr());
                    const float* pSrcR = &src(0, 0, 0, 0);
                    const float* pEndSrcR = pSrcR;
                    const float* pSrcG = &src(0, 0, 0, (src._spectrum >= 2) ? 1 : 0);
                    const float* pSrcB = &src(0, 0, 0, (src._spectrum >= 3) ? 2 : 0);
                    unsigned int height = src._height;

                    while (height--)
                    {
                        pEndSrcR += src._width;
                        while (pSrcR != pEndSrcR)
                        {
                            pDst[0] = static_cast<unsigned short>(*pSrcR++);
                            pDst[1] = static_cast<unsigned short>(*pSrcG++);
                            pDst[2] = static_cast<unsigned short>(*pSrcB++);
                            pDst += 3;
                        }
                    }
                }
                break;

            case CV_16S: // 16-bit signed int.
                if(dst.channels() == 1)
                {
                    short* pDst = reinterpret_cast<short*>(dst.ptr());
                    for(uint i=0; i<src._width*src._height; ++i)
                        *pDst++ = static_cast<short>(src._data[i]);
                }
                else
                {
                    short* pDst = reinterpret_cast<short*>(dst.ptr());
                    const float* pSrcR = &src(0, 0, 0, 0);
                    const float* pEndSrcR = pSrcR;
                    const float* pSrcG = &src(0, 0, 0, (src._spectrum >= 2) ? 1 : 0);
                    const float* pSrcB = &src(0, 0, 0, (src._spectrum >= 3) ? 2 : 0);
                    unsigned int height = src._height;

                    while (height--)
                    {
                        pEndSrcR += src._width;
                        while (pSrcR != pEndSrcR)
                        {
                            pDst[0] = static_cast<short>(*pSrcR++);
                            pDst[1] = static_cast<short>(*pSrcG++);
                            pDst[2] = static_cast<short>(*pSrcB++);
                            pDst += 3;
                        }
                    }
                }
                break;

            case CV_32S: // 32-bit signed int.
                if(dst.channels() == 1)
                {
                    int* pDst = reinterpret_cast<int*>(dst.ptr());
                    for(uint i=0; i<src._width*src._height; ++i)
                        *pDst++ = static_cast<int>(src._data[i]);
                }
                else
                {
                    int* pDst = reinterpret_cast<int*>(dst.ptr());
                    const float* pSrcR = &src(0, 0, 0, 0);
                    const float* pEndSrcR = pSrcR;
                    const float* pSrcG = &src(0, 0, 0, (src._spectrum >= 2) ? 1 : 0);
                    const float* pSrcB = &src(0, 0, 0, (src._spectrum >= 3) ? 2 : 0);
                    unsigned int height = src._height;

                    while (height--)
                    {
                        pEndSrcR += src._width;
                        while (pSrcR != pEndSrcR)
                        {
                            pDst[0] = static_cast<int>(*pSrcR++);
                            pDst[1] = static_cast<int>(*pSrcG++);
                            pDst[2] = static_cast<int>(*pSrcB++);
                            pDst += 3;
                        }
                    }
                }
                break;

            case CV_32F: // 32-bit float
                if(dst.channels() == 1)
                {
                    float* pDst = reinterpret_cast<float*>(dst.ptr());
                    for(uint i=0; i<src._width*src._height; ++i)
                        *pDst++ = src._data[i];
                }
                else
                {
                    float* pDst = reinterpret_cast<float*>(dst.ptr());
                    const float* pSrcR = &src(0, 0, 0, 0);
                    const float* pEndSrcR = pSrcR;
                    const float* pSrcG = &src(0, 0, 0, (src._spectrum >= 2) ? 1 : 0);
                    const float* pSrcB = &src(0, 0, 0, (src._spectrum >= 3) ? 2 : 0);
                    unsigned int height = src._height;

                    while (height--)
                    {
                        pEndSrcR += src._width;
                        while (pSrcR != pEndSrcR)
                        {
                            pDst[0] = *pSrcR++;
                            pDst[1] = *pSrcG++;
                            pDst[2] = *pSrcB++;
                            pDst += 3;
                        }
                    }
                }
                break;

            case CV_64F: // 64-bit float
                if(dst.channels() == 1)
                {
                    double* pDst = reinterpret_cast<double*>(dst.ptr());
                    for(uint i=0; i<src._width*src._height; ++i)
                        *pDst++ = static_cast<double>(src._data[i]);
                }
                else
                {
                    double* pDst = reinterpret_cast<double*>(dst.ptr());
                    const float* pSrcR = &src(0, 0, 0, 0);
                    const float* pEndSrcR = pSrcR;
                    const float* pSrcG = &src(0, 0, 0, (src._spectrum >= 2) ? 1 : 0);
                    const float* pSrcB = &src(0, 0, 0, (src._spectrum >= 3) ? 2 : 0);
                    unsigned int height = src._height;

                    while (height--)
                    {
                        pEndSrcR += src._width;
                        while (pSrcR != pEndSrcR)
                        {
                            pDst[0] = static_cast<double>(*pSrcR++);
                            pDst[1] = static_cast<double>(*pSrcG++);
                            pDst[2] = static_cast<double>(*pSrcB++);
                            pDst += 3;
                        }
                    }
                }
                break;

            default:
                throw CException(CoreExCode::NOT_IMPLEMENTED, "Gmic converter: unsupported bits per pixel");
                break;
        }
    }
    else
    {
        throw CException(CoreExCode::NOT_IMPLEMENTED, "Gmic converter: unsupported image format");
    }
}
