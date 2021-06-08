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

#ifndef COCVSALIENCYFINEGRAINED_HPP
#define COCVSALIENCYFINEGRAINED_HPP

#include "Core/C2dImageTask.h"
#include "IO/CImageIO.h"
#include <opencv2/saliency.hpp>

//------------------------------//
//----- COcvSaliencyFineGrainedParam -----//
//------------------------------//
class COcvSaliencyFineGrainedParam: public CWorkflowTaskParam
{
    public:

        COcvSaliencyFineGrainedParam() : CWorkflowTaskParam(){}

        void setParamMap(const UMapString& paramMap) override
        {
            Q_UNUSED(paramMap)
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            return map;
        }
};

//-------------------------//
//----- COcvSaliencyFineGrained -----//
//-------------------------//
class COcvSaliencyFineGrained : public C2dImageTask
{
    public:

        COcvSaliencyFineGrained() : C2dImageTask()
        {
            addOutput(std::make_shared<CImageIO>(IODataType::IMAGE_BINARY));
        }
        COcvSaliencyFineGrained(const std::string name, const std::shared_ptr<COcvSaliencyFineGrainedParam>& pParam) : C2dImageTask(name)
        {
            addOutput(std::make_shared<CImageIO>(IODataType::IMAGE_BINARY));
            m_pParam = std::make_shared<COcvSaliencyFineGrainedParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvSaliencyFineGrainedParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat saliencyMap, binaryMap;
            CMat imgSrc = pInput->getImage();
            createGraphicsMask(imgSrc.getNbCols(), imgSrc.getNbRows(), pGraphicsInput);
            emit m_signalHandler->doProgress();

            try
            {
                if(m_pSaliency.get() == nullptr)
                    m_pSaliency = cv::saliency::StaticSaliencyFineGrained::create();

                if(m_pSaliency->computeSaliency(imgSrc, saliencyMap))
                {
                    if(!m_pSaliency->computeBinaryMap( saliencyMap, binaryMap ))
                        throw CException(CoreExCode::INVALID_PARAMETER, "Compute binary map has failed", __func__, __FILE__, __LINE__);
                }
                else
                {
                    throw CException(CoreExCode::INVALID_PARAMETER, "Compute saliency has failed", __func__, __FILE__, __LINE__);
                }

            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            endTaskRun();
            applyGraphicsMask(imgSrc, saliencyMap, 0);
            applyGraphicsMask(imgSrc, binaryMap, 0);
            emit m_signalHandler->doProgress();

            auto pOutput1 = std::dynamic_pointer_cast<CImageIO>(getOutput(0));
            if(pOutput1)
                pOutput1->setImage(saliencyMap);

            auto pOutput2 = std::dynamic_pointer_cast<CImageIO>(getOutput(1));
            if(pOutput2)
                pOutput2->setImage(binaryMap);

            emit m_signalHandler->doProgress();
        }

    private:
        cv::Ptr<cv::saliency::StaticSaliencyFineGrained> m_pSaliency;
};

class COcvSaliencyFineGrainedFactory : public CTaskFactory
{
    public:

        COcvSaliencyFineGrainedFactory()
        {
            m_info.m_name = QObject::tr("StaticSaliencyFineGrained").toStdString();
            m_info.m_description = QObject::tr("Starting from the principle of natural image statistics, this method simulate the behavior of pre-attentive visual search. The algorithm analyze the log spectrum of each image and obtain the spectral residual. Then transform the spectral residual to spatial domain to obtain the saliency map, which suggests the positions of proto-objects.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Saliency API").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Saliency,Features,Detection";
            m_info.m_authors = "Sebastian Montabone and Alvaro Soto";
            m_info.m_article = "Human detection using a mobile platform and novel features derived from a visual saliency mechanism";
            m_info.m_journal = "Image and Vision Computing, Vol. 28 Issue 3, pages 391–402. Elsevier";
            m_info.m_year = 2010;
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/da/dd0/classcv_1_1saliency_1_1StaticSaliencyFineGrained.html";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pSaliencyFineGrainedParam = std::dynamic_pointer_cast<COcvSaliencyFineGrainedParam>(pParam);
            if(pSaliencyFineGrainedParam != nullptr)
                return std::make_shared<COcvSaliencyFineGrained>(m_info.m_name, pSaliencyFineGrainedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pSaliencyFineGrainedParam = std::make_shared<COcvSaliencyFineGrainedParam>();
            assert(pSaliencyFineGrainedParam != nullptr);
            return std::make_shared<COcvSaliencyFineGrained>(m_info.m_name, pSaliencyFineGrainedParam);
        }
};

#endif // COCVSALIENCYFINEGRAINED_HPP
