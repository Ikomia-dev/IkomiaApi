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

#ifndef COCVINSERTCHANNEL_HPP
#define COCVINSERTCHANNEL_HPP

#include "Core/C2dImageTask.h"

//----------------------------------//
//----- COcvInsertChannelParam -----//
//----------------------------------//
class COcvInsertChannelParam: public CWorkflowTaskParam
{
    public:

        COcvInsertChannelParam() : CWorkflowTaskParam(){}

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
//----- COcvInsertChannel -----//
//------------------------------//
class COcvInsertChannel : public C2dImageTask
{
    public:

        COcvInsertChannel() : C2dImageTask(false)
        {
            insertInput(std::make_shared<CImageIO>(), 0);
        }
        COcvInsertChannel(const std::string name, const std::shared_ptr<COcvInsertChannelParam>& pParam) : C2dImageTask(name, false)
        {
            insertInput(std::make_shared<CImageIO>(), 0);
            m_pParam = std::make_shared<COcvInsertChannelParam>(*pParam);
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        void run() override
        {
            beginTaskRun();

            if(getInputCount() < 2)
                throw CException(CoreExCode::INVALID_PARAMETER, "Not enough inputs", __func__, __FILE__, __LINE__);

            auto pInput1 = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            auto pInput2 = std::dynamic_pointer_cast<CImageIO>(getInput(1));
            auto pParam = std::dynamic_pointer_cast<COcvInsertChannelParam>(m_pParam);

            if(pInput1 == nullptr || pInput2 == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput1->isDataAvailable() == false || pInput2->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty images", __func__, __FILE__, __LINE__);

            //Source image
            auto imgSrc1 = pInput1->getImage();
            //Channel to insert
            auto imgSrc2 = pInput2->getImage();

            cv::Mat imgDst;
            if(imgSrc1.channels() == 1)
            {
                imgDst.create(imgSrc1.rows, imgSrc1.cols, CV_MAKETYPE(imgSrc1.depth(), 3));
                imgDst = 0;
                cv::insertChannel(imgSrc1, imgDst, 0);
            }
            else
                imgDst = imgSrc1.clone();

            emit m_signalHandler->doProgress();

            try
            {
                cv::insertChannel(imgSrc2, imgDst, pParam->m_index);
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

class COcvInsertChannelFactory : public CTaskFactory
{
    public:

        COcvInsertChannelFactory()
        {
            m_info.m_name = QObject::tr("Insert channel").toStdString();
            m_info.m_description = QObject::tr("The function inserts the given single channel(2nd input) into the source image (1st input). Index parameter is a 0-based index.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Core functionality/Operations on arrays").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "channel,insert,color";
            m_info.m_docLink = "https://docs.opencv.org/4.0.1/d2/de8/group__core__array.html#ga1d4bd886d35b00ec0b764cb4ce6eb515";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvInsertChannelParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvInsertChannel>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvInsertChannelParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvInsertChannel>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVINSERTCHANNEL_HPP
