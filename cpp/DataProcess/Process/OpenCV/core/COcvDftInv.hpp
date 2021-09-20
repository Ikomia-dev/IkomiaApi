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

#ifndef COCVDftInv_HPP
#define COCVDftInv_HPP


#include "Core/C2dImageTask.h"
#include "IO/CImageIO.h"

//------------------------------//
//----- COcvDftInvParam -----//
//------------------------------//
class COcvDftInvParam: public CWorkflowTaskParam
{
    public:

        COcvDftInvParam() : CWorkflowTaskParam(){}

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
//----- COcvDftInv -----//
//-------------------------//
class COcvDftInv : public C2dImageTask
{
    public:

        COcvDftInv() : C2dImageTask()
        {
            insertInput(std::make_shared<CImageIO>(), 1);
        }
        COcvDftInv(const std::string name, const std::shared_ptr<COcvDftInvParam>& pParam) : C2dImageTask(name)
        {
            m_pParam = std::make_shared<COcvDftInvParam>(*pParam);
            insertInput(std::make_shared<CImageIO>(), 1);
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
        void run() override
        {
            beginTaskRun();
            auto pInputReal = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            auto pInputComplex = std::dynamic_pointer_cast<CImageIO>(getInput(1));
            auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsInput>(getInput(2));
            auto pParam = std::dynamic_pointer_cast<COcvDftInvParam>(m_pParam);

            if(pInputReal == nullptr || pInputComplex == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInputReal->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            if(pInputComplex->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgDst, imgReal, imgComplex, imgRealTmp, imgComplexTmp;
            imgReal = pInputReal->getImage();
            imgComplex = pInputComplex->getImage();

            createGraphicsMask(imgReal.getNbCols(), imgReal.getNbRows(), pGraphicsInput);
            if(isMaskAvailable(0) == true)
            {
                imgReal = pInputReal->getImage().clone();
                imgComplex = pInputComplex->getImage().clone();
                imgRealTmp = cv::Mat::zeros(imgReal.size(), imgReal.type());
                imgComplexTmp = cv::Mat::zeros(imgComplex.size(), imgComplex.type());

                shift(imgReal);
                shift(imgComplex);

                applyGraphicsMask(imgRealTmp, imgReal, 0);
                applyGraphicsMask(imgComplexTmp, imgComplex, 0);

                shift(imgReal);
                shift(imgComplex);
            }
            else
            {
                imgReal = pInputReal->getImage();
                imgComplex = pInputComplex->getImage();
            }

            emit m_signalHandler->doProgress();

            try
            {
                std::vector<cv::Mat> realcomp = {imgReal, imgComplex};
                CMat complex;
                cv::merge(realcomp, complex);
                cv::idft(complex, imgDst);
                std::vector<cv::Mat> planes = {
                        cv::Mat::zeros(complex.size(), CV_32F),
                        cv::Mat::zeros(complex.size(), CV_32F)
                    };
                cv::split(imgDst, planes); // planes[0] = Re(DftInv(I)), planes[1] = Im(DftInv(I))
                cv::magnitude(planes[0], planes[1], imgDst); // sqrt(Re(DftInv(I))^2 + Im(DftInv(I))^2)
                //cv::normalize(imgDst, imgDst, 0, 1, cv::NORM_MINMAX);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            endTaskRun();
            emit m_signalHandler->doProgress();

            auto pOutput = std::dynamic_pointer_cast<CImageIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(imgDst);

            emit m_signalHandler->doProgress();
        }
};

class COcvDftInvFactory : public CTaskFactory
{
    public:

        COcvDftInvFactory()
        {
            m_info.m_name = "ocv_dft_inverse";
            m_info.m_description = QObject::tr("This process calculates the Inverse Discrete Fourier Transform of a 1D or 2D array.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Core functionality/Operations on arrays").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Fourier, inverse, idft, transform";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d2/de8/group__core__array.html#gaa708aa2d2e57a508f968eb0f69aa5ff1";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDftInvParam = std::dynamic_pointer_cast<COcvDftInvParam>(pParam);
            if(pDftInvParam != nullptr)
                return std::make_shared<COcvDftInv>(m_info.m_name, pDftInvParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDftInvParam = std::make_shared<COcvDftInvParam>();
            assert(pDftInvParam != nullptr);
            return std::make_shared<COcvDftInv>(m_info.m_name, pDftInvParam);
        }
};

#endif // COCVDftInv_HPP
