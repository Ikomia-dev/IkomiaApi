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

#ifndef COCVNLMEANS_HPP
#define COCVNLMEANS_HPP

#include "Core/C2dImageTask.h"
#include "IO/CImageIO.h"
#include "UtilsTools.hpp"

#ifdef HAVE_OPENCV_CUDAIMGPROC
#include "opencv2/photo/cuda.hpp"
#endif

//----------------------------//
//----- COcvNlMeansParam -----//
//----------------------------//
class COcvFastNlMeansParam: public CWorkflowTaskParam
{
    public:

        COcvFastNlMeansParam() : CWorkflowTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_h = std::stof(paramMap.at("h"));
            m_blockSize = std::stoi(paramMap.at("blockSize"));
            m_searchSize = std::stoi(paramMap.at("searchSize"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("h", std::to_string(m_h)));
            map.insert(std::make_pair("blockSize", std::to_string(m_blockSize)));
            map.insert(std::make_pair("searchSize", std::to_string(m_searchSize)));
            return map;
        }

    public:

        float   m_h = 3.0;
        int     m_blockSize = 7;
        int     m_searchSize = 21;
};

//-----------------------//
//----- COcvNlMeans -----//
//-----------------------//
class COcvFastNlMeans : public C2dImageTask
{
    public:

        COcvFastNlMeans() : C2dImageTask()
        {
        }
        COcvFastNlMeans(const std::string name, const std::shared_ptr<COcvFastNlMeansParam>& pParam) : C2dImageTask(name)
        {
            m_pParam = std::make_shared<COcvFastNlMeansParam>(*pParam);
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        void run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsInput>(getInput(1));
            auto pParam = std::dynamic_pointer_cast<COcvFastNlMeansParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgDst;
            CMat imgSrc = pInput->getImage();
            createGraphicsMask(imgSrc.getNbCols(), imgSrc.getNbRows(), pGraphicsInput);
            emit m_signalHandler->doProgress();

            try
            {
#ifdef HAVE_OPENCV_CUDAIMGPROC
                bool bCuda = Utils::Gpu::isCudaAvailable();
                bCuda = false;
                if(bCuda == true)
                {
                    cv::cuda::GpuMat cuImgSrc, cuImgDst;
                    cuImgSrc.upload(imgSrc);

                    if(pInput->m_channelCount == 1)
                        cv::cuda::fastNlMeansDenoising(cuImgSrc, cuImgDst, pParam->m_h, pParam->m_blockSize, pParam->m_searchSize);
                    else
                        cv::cuda::fastNlMeansDenoisingColored(cuImgSrc, cuImgDst, pParam->m_h, pParam->m_blockSize, pParam->m_searchSize);

                    cuImgDst.download(imgDst);
                }
                else
                {
                    if(pInput->m_channelCount == 1)
                        cv::fastNlMeansDenoising(imgSrc, imgDst, pParam->m_h, pParam->m_blockSize, pParam->m_searchSize);
                    else
                        cv::fastNlMeansDenoisingColored(imgSrc, imgDst, pParam->m_h, pParam->m_blockSize, pParam->m_searchSize);
                }
#else
                if(pInput->m_channelCount == 1)
                    cv::fastNlMeansDenoising(imgSrc, imgDst, pParam->m_h, pParam->m_blockSize, pParam->m_searchSize);
                else
                    cv::fastNlMeansDenoisingColored(imgSrc, imgDst, pParam->m_h, pParam->m_blockSize, pParam->m_searchSize);
#endif
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            endTaskRun();
            applyGraphicsMask(imgSrc, imgDst, 0);
            emit m_signalHandler->doProgress();

            auto pOutput = std::dynamic_pointer_cast<CImageIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(imgDst);

            emit m_signalHandler->doProgress();
        }
};

class COcvFastNlMeansFactory : public CTaskFactory
{
    public:

        COcvFastNlMeansFactory()
        {
            m_info.m_name = "ocv_non_local_means_filter";
            m_info.m_description = QObject::tr("This process smoothes your image with an edge-preserving filter (blur homogeneous area and keep borders intact).").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Computational photography/Denoising").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "smooth,blur,anisotropic,filter,Gaussian,edge preserving,fast";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d1/d79/group__photo__denoise.html#ga4c6b0031f56ea3f98f768881279ffe93";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvFastNlMeansParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvFastNlMeans>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvFastNlMeansParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvFastNlMeans>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVNLMEANS_HPP
