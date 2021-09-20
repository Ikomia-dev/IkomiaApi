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

#ifndef COCVFARNEBACKOPTICALFLOW_HPP
#define COCVFARNEBACKOPTICALFLOW_HPP

#include "Core/CVideoOFTask.h"
#include "IO/CVideoIO.h"
#include "opencv2/tracking.hpp"

//------------------------------//
//----- COcvFarnebackOFParam -----//
//------------------------------//
class COcvFarnebackOFParam: public CWorkflowTaskParam
{
    public:

        COcvFarnebackOFParam() : CWorkflowTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_bUseOCL = std::stoi(paramMap.at("useOCL"));
            m_numLevels = std::stoi(paramMap.at("numLevels"));
            m_pyrScale = std::stod(paramMap.at("pyrScale"));
            m_fastPyramids = std::stoi(paramMap.at("fastPyramids"));
            m_winSize = std::stoi(paramMap.at("winSize"));
            m_numIters = std::stoi(paramMap.at("numIters"));
            m_polyN = std::stoi(paramMap.at("polyN"));
            m_polySigma = std::stod(paramMap.at("polySigma"));
            m_flags = std::stoi(paramMap.at("flags"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("useOCL", std::to_string(m_bUseOCL)));
            map.insert(std::make_pair("numLevels", std::to_string(m_numLevels)));
            map.insert(std::make_pair("pyrScale", std::to_string(m_pyrScale)));
            map.insert(std::make_pair("fastPyramids", std::to_string(m_fastPyramids)));
            map.insert(std::make_pair("winSize", std::to_string(m_winSize)));
            map.insert(std::make_pair("numIters", std::to_string(m_numIters)));
            map.insert(std::make_pair("polyN", std::to_string(m_polyN)));
            map.insert(std::make_pair("polySigma", std::to_string(m_polySigma)));
            map.insert(std::make_pair("flags", std::to_string(m_flags)));

            return map;
        }

    public:
        bool    m_bUseOCL = false;
        int  	m_numLevels = 5;
        double  m_pyrScale = 0.5;
        bool  	m_fastPyramids = false;
        int  	m_winSize = 13;
        int  	m_numIters = 10;
        int  	m_polyN = 5;
        double  m_polySigma = 1.1;
        int  	m_flags = 0;

};

//-------------------------//
//----- COcvFarnebackOF -----//
//-------------------------//
class COcvFarnebackOF : public CVideoOFTask
{
    public:

        COcvFarnebackOF() : CVideoOFTask()
        {
        }
        COcvFarnebackOF(const std::string name, const std::shared_ptr<COcvFarnebackOFParam>& pParam) : CVideoOFTask(name)
        {
            m_pParam = std::make_shared<COcvFarnebackOFParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvFarnebackOFParam>(m_pParam);

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
                m_pOF = cv::FarnebackOpticalFlow::create(pParam->m_numLevels, pParam->m_pyrScale, pParam->m_fastPyramids, pParam->m_winSize, pParam->m_numIters, pParam->m_polyN, pParam->m_polySigma, pParam->m_flags);
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

class COcvFarnebackOFFactory : public CTaskFactory
{
    public:

        COcvFarnebackOFFactory()
        {
            m_info.m_name = "ocv_farneback_flow";
            m_info.m_description = QObject::tr("This process computes a dense optical flow using the Gunnar Farneback algorithm.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Video analysis/Object tracking").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Farneback, Dense, Optical, Flow";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/de/d9e/classcv_1_1FarnebackOpticalFlow.html";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pFarnebackOFParam = std::dynamic_pointer_cast<COcvFarnebackOFParam>(pParam);
            if(pFarnebackOFParam != nullptr)
                return std::make_shared<COcvFarnebackOF>(m_info.m_name, pFarnebackOFParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pFarnebackOFParam = std::make_shared<COcvFarnebackOFParam>();
            assert(pFarnebackOFParam != nullptr);
            return std::make_shared<COcvFarnebackOF>(m_info.m_name, pFarnebackOFParam);
        }
};

#endif // COCVFARNEBACKOPTICALFLOW_HPP
