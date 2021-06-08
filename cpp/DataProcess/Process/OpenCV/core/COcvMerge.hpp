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

#ifndef COCVMERGE_HPP
#define COCVMERGE_HPP


#include "Core/C2dImageTask.h"
#include "IO/CImageIO.h"

//------------------------------//
//----- COcvMergeParam -----//
//------------------------------//
class COcvMergeParam: public CWorkflowTaskParam
{
    public:

        COcvMergeParam() : CWorkflowTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_inputCount = std::stoi(paramMap.at("inputCount"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("inputCount", std::to_string(m_inputCount)));
            return map;
        }

    public:

        int     m_inputCount = 1;
};

//---------------------//
//----- COcvMerge -----//
//---------------------//
class COcvMerge : public C2dImageTask
{
    public:

        COcvMerge() : C2dImageTask(false)
        {
        }
        COcvMerge(const std::string name, const std::shared_ptr<COcvMergeParam>& pParam) : C2dImageTask(name, false)
        {
            m_pParam = std::make_shared<COcvMergeParam>(*pParam);
            clearInputs();
            for(int i=0; i<pParam->m_inputCount; ++i)
                addInput(std::make_shared<CImageIO>());
        }

        void    setParam(const WorkflowTaskParamPtr &pParam) override
        {
            m_pParam = pParam;
            parametersModified();
        }

        void    parametersModified() override
        {
            auto pParam = std::dynamic_pointer_cast<COcvMergeParam>(m_pParam);
            int inOldCount = (int)getInputCount();

            if(pParam->m_inputCount > inOldCount)
            {
                assert(getInput(0) != nullptr);
                for(int i=0; i<pParam->m_inputCount - inOldCount; ++i)
                    addInput(std::make_shared<CImageIO>(getInput(0)->getDataType()));
            }
            else
            {
                for(int i=0; i<inOldCount - pParam->m_inputCount; ++i)
                {
                    removeInput(getInputCount() - 1);
                    emit m_signalHandler->doInputRemoved(getInputCount());
                }
            }
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        void    run() override
        {
            beginTaskRun();

            auto pParam = std::dynamic_pointer_cast<COcvMergeParam>(m_pParam);
            if((int)getInputCount() != pParam->m_inputCount)
                throw CException(CoreExCode::INVALID_PARAMETER, "Inputs count mismatch", __func__, __FILE__, __LINE__);

            std::vector<std::shared_ptr<CImageIO>> inputs;
            for(int i=0; i<pParam->m_inputCount; ++i)
            {
                auto pInput = std::dynamic_pointer_cast<CImageIO>(getInput(i));
                inputs.push_back(pInput);
            }

            CMat imgDst;
            emit m_signalHandler->doProgress();

            try
            {
                std::vector<cv::Mat> images;
                cv::Size size;
                int type;

                for(int i=0; i<pParam->m_inputCount; ++i)
                {
                    if(inputs[i] && inputs[i]->isDataAvailable() == true)
                    {
                        CMat imgGray;
                        CMat img = inputs[i]->getImage();

                        if(img.channels() > 1)
                            cv::cvtColor(img, imgGray, cv::COLOR_RGB2GRAY);
                        else
                            imgGray = img;

                        size = imgGray.size();
                        type = imgGray.type();
                        images.push_back(imgGray);
                    }
                    else
                        images.push_back(CMat());
                }

                for(size_t i=0; i<images.size(); ++i)
                    if(images.empty())
                        images[i] = cv::Mat::zeros(size, type);

                if(images.size() < 3)
                {
                    for(size_t i=images.size(); i<3; ++i)
                        images.push_back(cv::Mat::zeros(size, type));
                }
                cv::merge(images, imgDst);
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

class COcvMergeFactory : public CTaskFactory
{
    public:

        COcvMergeFactory()
        {
            m_info.m_name = QObject::tr("Merge Operator").toStdString();
            m_info.m_description = QObject::tr("Creates one multi-channel array out of several single-channel ones. ").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Core functionality/Operations on arrays").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Merge,Color,Fusion,Channel,RGB";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d2/de8/group__core__array.html#ga61f2f2bde4a0a0154b2333ea504fab1d";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvMergeParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvMerge>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvMergeParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvMerge>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVMERGE_HPP
