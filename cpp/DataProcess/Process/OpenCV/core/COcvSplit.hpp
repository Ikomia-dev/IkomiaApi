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

#ifndef COCVSPLIT_HPP
#define COCVSPLIT_HPP

#include "Core/C2dImageTask.h"
#include "IO/CImageIO.h"

//------------------------------//
//----- COcvSplitParam -----//
//------------------------------//
class COcvSplitParam: public CWorkflowTaskParam
{
    public:

        COcvSplitParam() : CWorkflowTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_outputCount = std::stoi(paramMap.at("outputCount"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("outputCount", std::to_string(m_outputCount)));
            return map;
        }

    public:

        int     m_outputCount = 1;
};

//---------------------//
//----- COcvSplit -----//
//---------------------//
class COcvSplit : public C2dImageTask
{
    public:

        COcvSplit() : C2dImageTask(false)
        {
        }
        COcvSplit(const std::string name, const std::shared_ptr<COcvSplitParam>& pParam) : C2dImageTask(name, false)
        {
            m_pParam = std::make_shared<COcvSplitParam>(*pParam);
            parametersModified();            
        }

        void    parametersModified() override
        {
            auto pParam = std::dynamic_pointer_cast<COcvSplitParam>(m_pParam);
            int outOldCount = (int)getOutputCount();

            if(pParam->m_outputCount > outOldCount)
            {
                for(int i=0; i<pParam->m_outputCount - outOldCount; ++i)
                    addOutput(std::make_shared<CImageIO>());
            }
            else
            {
                for(int i=0; i<outOldCount - pParam->m_outputCount; ++i)
                {
                    removeOutput(getOutputCount() - 1);
                    emit m_signalHandler->doOutputRemoved(getOutputCount());
                }
            }
            // Update outputs color (videos...)
            updateStaticOutputs();
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        void    run() override
        {
            beginTaskRun();

            auto pInput = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            if(pInput == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat* pImgDst = new CMat[pInput->m_channelCount];
            emit m_signalHandler->doProgress();

            try
            {
                if(pInput->m_channelCount > 1)
                    cv::split(pInput->getImage(), pImgDst);
                else
                    pImgDst[0] = pInput->getImage().clone();
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            endTaskRun();
            emit m_signalHandler->doProgress();

            for(size_t i=0; i<getOutputCount(); ++i)
            {
                auto pOutput = std::dynamic_pointer_cast<CImageIO>(getOutput(i));
                if(pOutput)
                {
                    if(i < pInput->m_channelCount)
                        pOutput->setImage(pImgDst[i]);
                    else
                        pOutput->setImage(CMat());
                }
            }
            delete[] pImgDst;
            emit m_signalHandler->doProgress();
        }
};

class COcvSplitFactory : public CTaskFactory
{
    public:

        COcvSplitFactory()
        {
            m_info.m_name = QObject::tr("Split Operator").toStdString();
            m_info.m_description = QObject::tr("Divide a multi-channel image into several single-channel image.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Core functionality/Operations on arrays").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Split,Color,Divide,Channel,RGB";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d2/de8/group__core__array.html#ga0547c7fed86152d7e9d0096029c8518a";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvSplitParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvSplit>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pParam = std::make_shared<COcvSplitParam>();
            assert(pParam != nullptr);
            return std::make_shared<COcvSplit>(m_info.m_name, pParam);
        }
};

#endif // COCVSPLIT_HPP
