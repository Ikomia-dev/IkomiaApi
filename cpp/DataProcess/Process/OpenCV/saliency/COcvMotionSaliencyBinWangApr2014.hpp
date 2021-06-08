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

#ifndef COCVMOTIONSALIENCYBINWANGAPR2014_HPP
#define COCVMOTIONSALIENCYBINWANGAPR2014_HPP

#include "Core/C2dImageTask.h"
#include "IO/CImageIO.h"
#include <opencv2/saliency.hpp>

//------------------------------//
//----- COcvMotionSaliencyBinWangApr2014Param -----//
//------------------------------//
class COcvMotionSaliencyBinWangApr2014Param: public CWorkflowTaskParam
{
    public:

        COcvMotionSaliencyBinWangApr2014Param() : CWorkflowTaskParam(){}

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

//--------------------------------------------//
//----- COcvMotionSaliencyBinWangApr2014 -----//
//--------------------------------------------//
class COcvMotionSaliencyBinWangApr2014 : public C2dImageTask
{
    public:

        COcvMotionSaliencyBinWangApr2014() : C2dImageTask()
        {
            setOutputDataType(IODataType::IMAGE_BINARY, 0);
        }
        COcvMotionSaliencyBinWangApr2014(const std::string name, const std::shared_ptr<COcvMotionSaliencyBinWangApr2014Param>& pParam) : C2dImageTask(name)
        {
            setOutputDataType(IODataType::IMAGE_BINARY, 0);
            m_pParam = std::make_shared<COcvMotionSaliencyBinWangApr2014Param>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvMotionSaliencyBinWangApr2014Param>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat saliencyMap;
            CMat imgSrc = pInput->getImage();
            createGraphicsMask(imgSrc.getNbCols(), imgSrc.getNbRows(), pGraphicsInput);
            emit m_signalHandler->doProgress();

            try
            {
                if(m_pSaliency.get() == nullptr)
                {
                    m_pSaliency = cv::saliency::MotionSaliencyBinWangApr2014::create();
                    m_width = imgSrc.cols;
                    m_height = imgSrc.rows;

                    m_pSaliency->setImagesize(m_width, m_height);
                    if(!m_pSaliency->init())
                        throw CException(CoreExCode::INVALID_PARAMETER, "MotionSaliencyBinWangApr2014 Init failed", __func__, __FILE__, __LINE__);
                }
                else
                {
                    if(m_width != imgSrc.cols || m_height != imgSrc.rows)
                    {
                        m_width = imgSrc.cols;
                        m_height = imgSrc.rows;

                        m_pSaliency->setImagesize(m_width, m_height);
                        if(!m_pSaliency->init())
                            throw CException(CoreExCode::INVALID_PARAMETER, "MotionSaliencyBinWangApr2014 Init failed", __func__, __FILE__, __LINE__);
                    }
                }

                CMat img;
                if(imgSrc.channels() == 3)
                {
                    CMat imgLab, Lab[3];
                    cv::cvtColor(imgSrc, imgLab, cv::COLOR_RGB2Lab);
                    cv::split(imgLab,Lab);
                    img = Lab[0];
                }
                else if(imgSrc.channels() == 1)
                    img = imgSrc;
                else
                    throw CException(CoreExCode::INVALID_PARAMETER, "Images with more than 3 channels are not managed", __func__, __FILE__, __LINE__);

                if(!m_pSaliency->computeSaliency(img, saliencyMap))
                    throw CException(CoreExCode::INVALID_PARAMETER, "Compute saliency has failed", __func__, __FILE__, __LINE__);

                saliencyMap *= 255;

            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            endTaskRun();
            applyGraphicsMask(imgSrc, saliencyMap, 0);
            emit m_signalHandler->doProgress();

            auto pOutput = std::dynamic_pointer_cast<CImageIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(saliencyMap);

            emit m_signalHandler->doProgress();
        }

    private:
        cv::Ptr<cv::saliency::MotionSaliencyBinWangApr2014> m_pSaliency;
        int m_width = 0;
        int m_height = 0;
};

class COcvMotionSaliencyBinWangApr2014Factory : public CTaskFactory
{
    public:

        COcvMotionSaliencyBinWangApr2014Factory()
        {
            m_info.m_name = QObject::tr("MotionSaliencyBinWangApr2014").toStdString();
            m_info.m_description = QObject::tr("This process highlights the moving objects or areas of change in the scene using the Fast Self-tuning Background Subtraction Algorithm.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Saliency API").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Saliency,Motion,Features,Detection";
            m_info.m_authors = "Bin Wang and Piotr Dudek";
            m_info.m_article = "A fast self-tuning background subtraction algorithm";
            m_info.m_journal = "CVPR Workshops, pages 401–404. IEEE";
            m_info.m_year = 2014;
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/dc/d9b/classcv_1_1saliency_1_1MotionSaliencyBinWangApr2014.html";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pMotionSaliencyBinWangApr2014Param = std::dynamic_pointer_cast<COcvMotionSaliencyBinWangApr2014Param>(pParam);
            if(pMotionSaliencyBinWangApr2014Param != nullptr)
                return std::make_shared<COcvMotionSaliencyBinWangApr2014>(m_info.m_name, pMotionSaliencyBinWangApr2014Param);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pMotionSaliencyBinWangApr2014Param = std::make_shared<COcvMotionSaliencyBinWangApr2014Param>();
            assert(pMotionSaliencyBinWangApr2014Param != nullptr);
            return std::make_shared<COcvMotionSaliencyBinWangApr2014>(m_info.m_name, pMotionSaliencyBinWangApr2014Param);
        }
};

#endif // COCVMOTIONSALIENCYBINWANGAPR2014_HPP
