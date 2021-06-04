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

#ifndef COCVDEEPFLOW_HPP
#define COCVDEEPFLOW_HPP

#include "Core/CVideoOFTask.h"
#include "IO/CVideoIO.h"
#include "opencv2/optflow.hpp"

//------------------------------//
//----- COcvDeepFlowParam -----//
//------------------------------//
class COcvDeepFlowParam: public CWorkflowTaskParam
{
    public:

        COcvDeepFlowParam() : CWorkflowTaskParam(){}

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
//----- COcvDeepFlow -----//
//-------------------------//
class COcvDeepFlow : public CVideoOFTask
{
    public:

        COcvDeepFlow() : CVideoOFTask()
        {
        }
        COcvDeepFlow(const std::string name, const std::shared_ptr<COcvDeepFlowParam>& pParam) : CVideoOFTask(name)
        {
            m_pParam = std::make_shared<COcvDeepFlowParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvDeepFlowParam>(m_pParam);

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

            m_imgFlow = cv::Mat(imgSrc.size[0], imgSrc.size[1], CV_32FC2);
            try
            {
                cv::Mat nextGray = manageInputs(imgSrc);
                m_pOF = cv::optflow::createOptFlow_DeepFlow();
                makeOF(nextGray);
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

class COcvDeepFlowFactory : public CTaskFactory
{
    public:

        COcvDeepFlowFactory()
        {
            m_info.m_name = QObject::tr("DeepFlow").toStdString();
            m_info.m_description = QObject::tr("This process computes a dense optical flow using the DeepFlow algorithm.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Optical flow algorithms").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Deep, Dense, Optical, Flow";
            m_info.m_authors = "Philippe Weinzaepfel, Jerome Revaud, Zaid Harchaoui, and Cordelia Schmid";
            m_info.m_article = "Deepflow: Large displacement optical flow with deep matching";
            m_info.m_journal = "ICCV 2013 pages 1385–1392";
            m_info.m_year = 2013;
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d2/d84/group__optflow.html#ga5e735547610172695cb0b39495af9d6d";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDeepFlowParam = std::dynamic_pointer_cast<COcvDeepFlowParam>(pParam);
            if(pDeepFlowParam != nullptr)
                return std::make_shared<COcvDeepFlow>(m_info.m_name, pDeepFlowParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDeepFlowParam = std::make_shared<COcvDeepFlowParam>();
            assert(pDeepFlowParam != nullptr);
            return std::make_shared<COcvDeepFlow>(m_info.m_name, pDeepFlowParam);
        }
};

#endif // COCVDEEPFLOW_HPP
