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

#ifndef COCVDFT_HPP
#define COCVDFT_HPP


#include "Core/C2dImageTask.h"
#include "IO/CImageIO.h"

//------------------------------//
//----- COcvDftParam -----//
//------------------------------//
class COcvDftParam: public CWorkflowTaskParam
{
    public:

        COcvDftParam() : CWorkflowTaskParam(){}

        void setParamMap(const UMapString& paramMap) override
        {
            m_flags = std::stoi(paramMap.at("flags"));
            m_nonZeroRows = std::stoi(paramMap.at("nonZeroRows"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("flags", std::to_string(m_flags)));
            map.insert(std::make_pair("nonZeroRows", std::to_string(m_nonZeroRows)));
            return map;
        }

    public:

        int         m_flags = 0;
        int         m_nonZeroRows = 0;
};

//-------------------------//
//----- COcvDft -----//
//-------------------------//
class COcvDft : public C2dImageTask
{
    public:

        COcvDft() : C2dImageTask(false)
        {
            addOutput(std::make_shared<CImageIO>());
            addOutput(std::make_shared<CImageIO>());
        }
        COcvDft(const std::string name, const std::shared_ptr<COcvDftParam>& pParam) : C2dImageTask(name, false)
        {
            m_pParam = std::make_shared<COcvDftParam>(*pParam);
            addOutput(std::make_shared<CImageIO>());
            addOutput(std::make_shared<CImageIO>());
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        void shift(CMat img)
        {
            using namespace cv;
            // crop the spectrum, if it has an odd number of rows or columns
            img = img(Rect(0, 0, img.cols & -2, img.rows & -2));

            // rearrange the quadrants of Fourier image  so that the origin is at the image center
            int cx = img.cols/2;
            int cy = img.rows/2;

            Mat q0(img, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
            Mat q1(img, Rect(cx, 0, cx, cy));  // Top-Right
            Mat q2(img, Rect(0, cy, cx, cy));  // Bottom-Left
            Mat q3(img, Rect(cx, cy, cx, cy)); // Bottom-Right

            Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
            q0.copyTo(tmp);
            q3.copyTo(q0);
            tmp.copyTo(q3);

            q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
            q2.copyTo(q1);
            tmp.copyTo(q2);
        }

        CMat updateMag(CMat complex)
        {
            CMat magI;
            cv::Mat planes[] = {
                cv::Mat::zeros(complex.size(), CV_32F),
                cv::Mat::zeros(complex.size(), CV_32F)
            };
            cv::split(complex, planes); // planes[0] = Re(DFT(I)), planes[1] = Im(DFT(I))
            cv::magnitude(planes[0], planes[1], magI); // sqrt(Re(DFT(I))^2 + Im(DFT(I))^2)
            // switch to logarithmic scale: log(1 + magnitude)
            magI += cv::Scalar::all(1);
            cv::log(magI, magI);
            shift(magI); // rearrage quadrants
            // Transform the magnitude matrix into a viewable image (float values 0-1)
            cv::normalize(magI, magI, 1, 0, cv::NORM_INF);

            return magI;
        }

        void run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            auto pParam = std::dynamic_pointer_cast<COcvDftParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgDst;
            std::vector<cv::Mat> realcomp = { CMat(), CMat() };
            CMat imgSrc = pInput->getImage();
            emit m_signalHandler->doProgress();

            try
            {
                cv::Mat padded;
                int m = cv::getOptimalDFTSize(imgSrc.rows);
                int n = cv::getOptimalDFTSize(imgSrc.cols);
                // create output image of optimal size
                cv::copyMakeBorder(imgSrc, padded, 0, m - imgSrc.rows, 0, n - imgSrc.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));
                // copy the source image, on the border add zero values
                cv::Mat matFloat;
                padded.convertTo(matFloat, CV_32F);
                auto zeros = cv::Mat::zeros(padded.size(), CV_32F);
                std::vector<cv::Mat> planes = { matFloat, zeros };
                // create a complex matrix
                CMat complex;
                cv::merge(planes, complex);
                cv::dft(complex, complex, cv::DFT_COMPLEX_OUTPUT);  // fourier transform
                cv::split(complex, realcomp);
                imgDst = updateMag(complex);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            endTaskRun();
            emit m_signalHandler->doProgress();

            // For conserving the real result transform for processing and inverse transform
            auto pOutputReal = std::dynamic_pointer_cast<CImageIO>(getOutput(0));
            if(pOutputReal)
                pOutputReal->setImage(realcomp[0]);
            // For conserving the real result transform for processing and inverse transform
            auto pOutputComplex = std::dynamic_pointer_cast<CImageIO>(getOutput(1));
            if(pOutputComplex)
                pOutputComplex->setImage(realcomp[1]);

            // For displaying spectrum
            auto pOutput = std::dynamic_pointer_cast<CImageIO>(getOutput(2));
            if(pOutput)
                pOutput->setImage(imgDst);

            emit m_signalHandler->doProgress();
        }
};

class COcvDftFactory : public CTaskFactory
{
    public:

        COcvDftFactory()
        {
            m_info.m_name = QObject::tr("Discrete Fourier Transform").toStdString();
            m_info.m_description = QObject::tr("This process performs a forward or inverse Discrete Fourier transform of a 1D or 2D floating-point array.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Core functionality/Operations on arrays").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Fourier, dft, idft, transform";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d2/de8/group__core__array.html#gadd6cf9baf2b8b704a11b5f04aaf4f39d";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDftParam = std::dynamic_pointer_cast<COcvDftParam>(pParam);
            if(pDftParam != nullptr)
                return std::make_shared<COcvDft>(m_info.m_name, pDftParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDftParam = std::make_shared<COcvDftParam>();
            assert(pDftParam != nullptr);
            return std::make_shared<COcvDft>(m_info.m_name, pDftParam);
        }
};

#endif // COCVDFT_HPP
