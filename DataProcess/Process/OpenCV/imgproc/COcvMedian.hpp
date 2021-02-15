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

#ifndef COCVMEDIAN_HPP
#define COCVMEDIAN_HPP

#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"

//---------------------------//
//----- COcvMedianParam -----//
//---------------------------//
class COcvMedianParam: public CProtocolTaskParam
{
    public:

        COcvMedianParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_ksize = std::stoi(paramMap.at("kSize"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("kSize", std::to_string(m_ksize)));
            return map;
        }

    public:

        int m_ksize = 3;
};

//----------------------//
//----- COcvMedian -----//
//----------------------//
class COcvMedian : public CImageProcess2d
{
    public:

        COcvMedian() : CImageProcess2d()
        {
        }
        COcvMedian(const std::string name, const std::shared_ptr<COcvMedianParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<COcvMedianParam>(*pParam);
        }

        size_t getProgressSteps() override
        {
            return 3;
        }

        void run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsProcessInput>(getInput(1));
            auto pParam = std::dynamic_pointer_cast<COcvMedianParam>(m_pParam);

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
                cv::medianBlur(imgSrc, imgDst, pParam->m_ksize);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            endTaskRun();
            applyGraphicsMask(imgSrc, imgDst, 0);
            emit m_signalHandler->doProgress();

            auto pOutput = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(imgDst);

            emit m_signalHandler->doProgress();
        }
};

class COcvMedianFactory : public CProcessFactory
{
    public:

        COcvMedianFactory()
        {
            m_info.m_name = QObject::tr("Median Filter").toStdString();
            m_info.m_description = QObject::tr("This process smoothes your image with a median filter.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Image processing/Image filtering").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "smooth,blur,isotropic,filter";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d4/d86/group__imgproc__filter.html#ga564869aa33e58769b4469101aac458f9";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pMedianParam = std::dynamic_pointer_cast<COcvMedianParam>(pParam);
            if(pMedianParam != nullptr)
                return std::make_shared<COcvMedian>(m_info.m_name, pMedianParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pMedianParam = std::make_shared<COcvMedianParam>();
            assert(pMedianParam != nullptr);
            return std::make_shared<COcvMedian>(m_info.m_name, pMedianParam);
        }
};

#endif // COCVMEDIAN_HPP
