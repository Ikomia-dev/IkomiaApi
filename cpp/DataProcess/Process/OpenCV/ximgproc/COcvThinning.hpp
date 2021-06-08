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

#ifndef COCVTHINNING_HPP
#define COCVTHINNING_HPP

#include "Core/C2dImageTask.h"
#include "IO/CImageIO.h"
#include "opencv2/ximgproc.hpp"

//-----------------------------//
//----- COcvThinningParam -----//
//-----------------------------//
class COcvThinningParam: public CWorkflowTaskParam
{
    public:

        COcvThinningParam() : CWorkflowTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_type = std::stoi(paramMap.at("type"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("type", std::to_string(m_type)));
            return map;
        }

    public:

        int     m_type = cv::ximgproc::THINNING_ZHANGSUEN;
};

//-------------------------//
//----- COcvThinning -----//
//-------------------------//
class COcvThinning : public C2dImageTask
{
    public:

        COcvThinning() : C2dImageTask()
        {
            getInput(0)->setDataType(IODataType::IMAGE_BINARY);
        }
        COcvThinning(const std::string name, const std::shared_ptr<COcvThinningParam>& pParam) : C2dImageTask(name)
        {
            getInput(0)->setDataType(IODataType::IMAGE_BINARY);
            m_pParam = std::make_shared<COcvThinningParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvThinningParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgDst;
            CMat imgSrc = pInput->getImage();
            createGraphicsMask(imgSrc.getNbCols(), imgSrc.getNbRows(), pGraphicsInput);
            emit m_signalHandler->doProgress();

            try
            {
                cv::ximgproc::thinning(imgSrc, imgDst, pParam->m_type);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            endTaskRun();
            applyGraphicsMask(imgSrc, imgDst, 0);
            emit m_signalHandler->doProgress();

            auto pOutput = std::dynamic_pointer_cast<CImageIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(imgDst);

            emit m_signalHandler->doProgress();
        }
};

class COcvThinningFactory : public CTaskFactory
{
    public:

        COcvThinningFactory()
        {
            m_info.m_name = QObject::tr("Thinning").toStdString();
            m_info.m_description = QObject::tr("The function transforms a binary blob image into a skeletized form using the technique of Zhang-Suen.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Extended Image Processing").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "binary,thinning,skeleton,blob,Zhang-Suen";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/df/d2d/group__ximgproc.html#ga37002c6ca80c978edb6ead5d6b39740c";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvThinningParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvThinning>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvThinningParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvThinning>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVTHINNING_HPP
