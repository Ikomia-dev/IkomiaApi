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

#ifndef COCVROTATEEX_HPP
#define COCVROTATEEX_HPP

#include "IO/CImageProcessIO.h"
#include "opencv2/cudawarping.hpp"

//-----------------------------//
//----- COcvRotateExParam -----//
//-----------------------------//
class COcvRotateExParam: public CProtocolTaskParam
{
    public:

        COcvRotateExParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_angle = std::stod(paramMap.at("angle"));
            m_scale = std::stod(paramMap.at("scale"));
            m_centerX = std::stoi(paramMap.at("centerX"));
            m_centerY = std::stoi(paramMap.at("centerY"));
            m_interpolation = std::stoi(paramMap.at("interpolation"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("angle", std::to_string(m_angle)));
            map.insert(std::make_pair("scale", std::to_string(m_scale)));
            map.insert(std::make_pair("centerX", std::to_string(m_centerX)));
            map.insert(std::make_pair("centerY", std::to_string(m_centerY)));
            map.insert(std::make_pair("interpolation", std::to_string(m_interpolation)));
            return map;
        }

    public:

        double  m_angle = -90.0;
        double  m_scale = 1.0;
        int     m_centerX = 0;
        int     m_centerY = 0;
        int     m_interpolation = cv::INTER_LINEAR;
};

//------------------------//
//----- COcvRotateEx -----//
//------------------------//
class COcvRotateEx : public CImageProcess2d
{
    public:

        COcvRotateEx() : CImageProcess2d(false)
        {
        }
        COcvRotateEx(const std::string name, const std::shared_ptr<COcvRotateExParam>& pParam) : CImageProcess2d(name, false)
        {
            m_pParam = std::make_shared<COcvRotateExParam>(*pParam);
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        void run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pParam = std::dynamic_pointer_cast<COcvRotateExParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgSrc, imgDst;
            imgSrc = pInput->getImage();
            emit m_signalHandler->doProgress();

            try
            {
                auto center = cv::Point2f(imgSrc.cols/2, imgSrc.rows/2);
                auto rotationMat = cv::getRotationMatrix2D(center, pParam->m_angle, pParam->m_scale);
                bool bCuda = Utils::Gpu::isCudaAvailable();

                if(bCuda == true && (pParam->m_interpolation == cv::INTER_NEAREST ||
                                     pParam->m_interpolation == cv::INTER_LINEAR ||
                                     pParam->m_interpolation == cv::INTER_CUBIC))
                {
                    cv::cuda::GpuMat cuImgSrc, cuImgDst;
                    cuImgSrc.upload(imgSrc);
                    cv::cuda::warpAffine(cuImgSrc, cuImgDst, rotationMat, cuImgSrc.size(), pParam->m_interpolation);
                    cuImgDst.download(imgDst);
                }
                else
                    cv::warpAffine(imgSrc, imgDst, rotationMat, imgSrc.size(), pParam->m_interpolation);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            endTaskRun();
            emit m_signalHandler->doProgress();

            auto pOutput = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(imgDst);

            emit m_signalHandler->doProgress();
        }
};

class COcvRotateExFactory : public CProcessFactory
{
    public:

        COcvRotateExFactory()
        {
            m_info.m_name = QObject::tr("RotateEx").toStdString();
            m_info.m_description = QObject::tr("This process applies scaled rotation of any angle in degree.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Image processing/Geometric image transformations").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "rotation, scale";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/da/d54/group__imgproc__transform.html#ga0203d9ee5fcd28d40dbc4a1ea4451983";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pRotateParam = std::dynamic_pointer_cast<COcvRotateExParam>(pParam);
            if(pRotateParam != nullptr)
                return std::make_shared<COcvRotateEx>(m_info.m_name, pRotateParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pRotateParam = std::make_shared<COcvRotateExParam>();
            assert(pRotateParam != nullptr);
            return std::make_shared<COcvRotateEx>(m_info.m_name, pRotateParam);
        }
};

#endif // COCVROTATEEX_HPP
