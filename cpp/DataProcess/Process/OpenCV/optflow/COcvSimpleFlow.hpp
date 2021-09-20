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

#ifndef COCVSIMPLEFLOW_HPP
#define COCVSIMPLEFLOW_HPP

#include "Core/CVideoOFTask.h"
#include "IO/CVideoIO.h"
#include "opencv2/optflow.hpp"

//------------------------------//
//----- COcvSimpleFlowParam -----//
//------------------------------//
class COcvSimpleFlowParam: public CWorkflowTaskParam
{
    public:

        COcvSimpleFlowParam() : CWorkflowTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_bUseOCL = std::stoi(paramMap.at("bUseOCL"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("bUseOCL", std::to_string(m_bUseOCL)));

            return map;
        }

    public:

        bool    m_bUseOCL = false;
};

//-------------------------//
//----- COcvSimpleFlow -----//
//-------------------------//
class COcvSimpleFlow : public CVideoOFTask
{
    public:

        COcvSimpleFlow() : CVideoOFTask()
        {
        }
        COcvSimpleFlow(const std::string name, const std::shared_ptr<COcvSimpleFlowParam>& pParam) : CVideoOFTask(name)
        {
            m_pParam = std::make_shared<COcvSimpleFlowParam>(*pParam);
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        virtual void notifyVideoStart(int /*frameCount*/) override
        {
            m_imgPrev.release();
        }

        void run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CVideoIO>(getInput(0));
            auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsInput>(getInput(1));
            auto pParam = std::dynamic_pointer_cast<COcvSimpleFlowParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            m_bUseOpenCL = pParam->m_bUseOCL;

            CMat imgSrc = pInput->getImage();

            // Manage when we reach the end of video by copying the last frame => optical flow doesn't change
            if(imgSrc.empty())
                imgSrc = m_imgPrev.clone();

            createGraphicsMask(imgSrc.getNbCols(), imgSrc.getNbRows(), pGraphicsInput);
            emit m_signalHandler->doProgress();

            cv::ocl::setUseOpenCL(m_bUseOpenCL);
            if(m_bUseOpenCL && !cv::ocl::useOpenCL())
                throw CException(CoreExCode::INVALID_PARAMETER, "OpenCL is not available on this platform.", __func__, __FILE__, __LINE__);

            m_imgFlow = cv::Mat(imgSrc.size[0], imgSrc.size[1], CV_32FC2);
            try
            {
                cv::Mat nextImg;
                if(imgSrc.channels() == 1)
                {
                    cv::cvtColor(imgSrc, nextImg, cv::COLOR_GRAY2RGB);
                }
                else
                    nextImg = imgSrc;

                // For first video image, we apply optical flow on same images
                if(m_imgPrev.empty())
                    m_imgPrev = nextImg;

                m_pOF = cv::optflow::createOptFlow_SimpleFlow();
                makeOF(nextImg);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            endTaskRun();

            emit m_signalHandler->doProgress();

            manageOuputs(imgSrc);

            emit m_signalHandler->doProgress();
        }
};

class COcvSimpleFlowFactory : public CTaskFactory
{
    public:

        COcvSimpleFlowFactory()
        {
            m_info.m_name = "ocv_simple_flow";
            m_info.m_description = QObject::tr("This process computes a dense optical flow using the Simple Flow algorithm.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Optical flow algorithms").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "SF, Dense, Optical, Flow";
            m_info.m_authors = "Michael Tao, Jiamin Bai, Pushmeet Kohli, and Sylvain Paris";
            m_info.m_article = "Simpleflow: A non-iterative, sublinear optical flow algorithm";
            m_info.m_journal = "Computer Graphics Forum, volume 31, pages 345–353. Wiley Online Library";
            m_info.m_year = 2012;
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d2/d84/group__optflow.html#ga370e4f91055a5ae14a0db71850b2f788";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pSimpleFlowParam = std::dynamic_pointer_cast<COcvSimpleFlowParam>(pParam);
            if(pSimpleFlowParam != nullptr)
                return std::make_shared<COcvSimpleFlow>(m_info.m_name, pSimpleFlowParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pSimpleFlowParam = std::make_shared<COcvSimpleFlowParam>();
            assert(pSimpleFlowParam != nullptr);
            return std::make_shared<COcvSimpleFlow>(m_info.m_name, pSimpleFlowParam);
        }
};

#endif // COCVSIMPLEFLOW_HPP
