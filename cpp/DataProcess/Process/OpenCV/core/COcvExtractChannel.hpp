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

#ifndef COCVEXTRACTCHANNEL_HPP
#define COCVEXTRACTCHANNEL_HPP

#include "Core/C2dImageTask.h"

//-----------------------------------//
//----- COcvExtractChannelParam -----//
//-----------------------------------//
class COcvExtractChannelParam: public CWorkflowTaskParam
{
    public:

        COcvExtractChannelParam() : CWorkflowTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_index = std::stoi(paramMap.at("index"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("index", std::to_string(m_index)));
            return map;
        }

    public:

        int m_index = 0;
};

//------------------------------//
//----- COcvExtractChannel -----//
//------------------------------//
class COcvExtractChannel : public C2dImageTask
{
    public:

        COcvExtractChannel() : C2dImageTask(false)
        {
        }
        COcvExtractChannel(const std::string name, const std::shared_ptr<COcvExtractChannelParam>& pParam) : C2dImageTask(name, false)
        {
            m_pParam = std::make_shared<COcvExtractChannelParam>(*pParam);
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        void    run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            auto pParam = std::dynamic_pointer_cast<COcvExtractChannelParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty images", __func__, __FILE__, __LINE__);

            CMat imgDst;
            auto imgSrc = pInput->getImage();

            if(pParam->m_index >= imgSrc.channels())
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid channel index", __func__, __FILE__, __LINE__);

            emit m_signalHandler->doProgress();

            try
            {
                cv::extractChannel(imgSrc, imgDst, pParam->m_index);
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

class COcvExtractChannelFactory : public CTaskFactory
{
    public:

        COcvExtractChannelFactory()
        {
            m_info.m_name = "ocv_extract_channel";
            m_info.m_description = QObject::tr("The function extracts a single channel from source image (Index parameter is a 0-based index).").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Core functionality/Operations on arrays").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "channel,extract,color";
            m_info.m_docLink = "https://docs.opencv.org/4.0.1/d2/de8/group__core__array.html#gacc6158574aa1f0281878c955bcf35642";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvExtractChannelParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvExtractChannel>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvExtractChannelParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvExtractChannel>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVEXTRACTCHANNEL_HPP
