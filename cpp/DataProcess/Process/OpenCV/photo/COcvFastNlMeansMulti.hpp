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

#ifndef COCVFASTNLMEANSMULTI_HPP
#define COCVFASTNLMEANSMULTI_HPP

#include "Core/CVideoTask.h"
#include "UtilsTools.hpp"

//---------------------------------//
//----- COcvNlMeansMultiParam -----//
//---------------------------------//
class COcvFastNlMeansMultiParam: public CWorkflowTaskParam
{
    public:

        COcvFastNlMeansMultiParam() : CWorkflowTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_h = std::stof(paramMap.at("h"));
            m_blockSize = std::stoi(paramMap.at("blockSize"));
            m_searchSize = std::stoi(paramMap.at("searchSize"));
            m_temporalSize = std::stoi(paramMap.at("temporalSize"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("h", std::to_string(m_h)));
            map.insert(std::make_pair("blockSize", std::to_string(m_blockSize)));
            map.insert(std::make_pair("searchSize", std::to_string(m_searchSize)));
            map.insert(std::make_pair("temporalSize", std::to_string(m_temporalSize)));
            return map;
        }

    public:

        float   m_h = 3.0;
        int     m_blockSize = 7;
        int     m_searchSize = 21;
        int     m_temporalSize = 5;
};

//----------------------------//
//----- COcvNlMeansMulti -----//
//----------------------------//
class COcvFastNlMeansMulti : public CVideoTask
{
    public:

        COcvFastNlMeansMulti() : CVideoTask()
        {
        }
        COcvFastNlMeansMulti(const std::string name, const std::shared_ptr<COcvFastNlMeansMultiParam>& pParam) : CVideoTask(name)
        {
            m_pParam = std::make_shared<COcvFastNlMeansMultiParam>(*pParam);
        }

        size_t  getProgressSteps() override
        {
            return 2;
        }

        void notifyVideoStart(int frameCount) override
        {
            Q_UNUSED(frameCount);
            m_srcImages.clear();
        }

        void run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsInput>(getInput(1));
            auto pParam = std::dynamic_pointer_cast<COcvFastNlMeansMultiParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            if(m_srcImages.size() == (size_t)pParam->m_temporalSize)
                m_srcImages.erase(m_srcImages.begin());

            m_srcImages.push_back(pInput->getImage());

            if(m_srcImages.size() != (size_t)pParam->m_temporalSize)
            {
                forwardInputImage();
                emit m_signalHandler->doProgress();
                endTaskRun();
                emit m_signalHandler->doProgress();
            }
            else
            {
                CMat imgDst;
                CMat imgSrc = pInput->getImage();
                createGraphicsMask(imgSrc.getNbCols(), imgSrc.getNbRows(), pGraphicsInput);
                emit m_signalHandler->doProgress();

                try
                {
                    if(pInput->m_channelCount == 1)
                        cv::fastNlMeansDenoisingMulti(m_srcImages, imgDst, pParam->m_temporalSize/2, pParam->m_temporalSize, pParam->m_h, pParam->m_blockSize, pParam->m_searchSize);
                    else
                        cv::fastNlMeansDenoisingColoredMulti(m_srcImages, imgDst, pParam->m_temporalSize/2, pParam->m_temporalSize, pParam->m_h, pParam->m_blockSize, pParam->m_searchSize);

                    applyGraphicsMask(imgSrc, imgDst, 0);
                }
                catch(cv::Exception& e)
                {
                    throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
                }

                endTaskRun();

                auto pOutput = std::dynamic_pointer_cast<CImageIO>(getOutput(0));
                if(pOutput)
                    pOutput->setImage(imgDst);

                emit m_signalHandler->doProgress();
            }
        }

    private:

        std::vector<cv::Mat>   m_srcImages;
};

class COcvFastNlMeansMultiFactory : public CTaskFactory
{
    public:

        COcvFastNlMeansMultiFactory()
        {
            m_info.m_name = QObject::tr("Non Local Means Multi Filter").toStdString();
            m_info.m_description = QObject::tr("This process smoothes a video sequence with an edge-preserving filter (blur homogeneous area and keep borders intact).").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Computational photography/Denoising").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "smooth,blur,anisotropic,filter,Gaussian,edge preserving,fast,video";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d1/d79/group__photo__denoise.html#gaf4421bf068c4d632ea7f0aa38e0bf172";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvFastNlMeansMultiParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvFastNlMeansMulti>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvFastNlMeansMultiParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvFastNlMeansMulti>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVFASTNLMEANSMULTI_HPP
