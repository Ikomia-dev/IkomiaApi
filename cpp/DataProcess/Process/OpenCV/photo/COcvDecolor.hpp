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

#ifndef COCVDECOLOR_HPP
#define COCVDECOLOR_HPP

#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"

//----------------------------//
//----- COcvDecolorParam -----//
//----------------------------//
class COcvDecolorParam: public CProtocolTaskParam
{
    public:

        COcvDecolorParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            Q_UNUSED(paramMap);
        }

        UMapString  getParamMap() const override
        {
            return UMapString();
        }
};

//-----------------------//
//----- COcvDecolor -----//
//-----------------------//
class COcvDecolor : public CImageProcess2d
{
    public:

        COcvDecolor() : CImageProcess2d()
        {
            addOutput(std::make_shared<CImageProcessIO>());
        }
        COcvDecolor(const std::string name, const std::shared_ptr<COcvDecolorParam>& pParam) : CImageProcess2d(name)
        {
            addOutput(std::make_shared<CImageProcessIO>());
            m_pParam = std::make_shared<COcvDecolorParam>(*pParam);
        }

        size_t     getProgressSteps() override
        {
            return 3;
        }

        void    run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pParam = std::dynamic_pointer_cast<COcvDecolorParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgDst, imgColorBoost;
            cv::Mat imgSrc = pInput->getImage();

            emit m_signalHandler->doProgress();

            if(imgSrc.channels() != 3)
                throw CException(CoreExCode::INVALID_PARAMETER, "Color image required", __func__, __FILE__, __LINE__);

            try
            {
                if(imgSrc.depth() != CV_8U && imgSrc.depth() != CV_8S)
                    imgSrc.convertTo(imgSrc, CV_8U);

                cv::decolor(imgSrc, imgDst, imgColorBoost);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            endTaskRun();
            emit m_signalHandler->doProgress();

            auto pOutput1 = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(0));
            if(pOutput1)
                pOutput1->setImage(imgDst);

            auto pOutput2 = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(1));
            if(pOutput2)
                pOutput2->setImage(imgColorBoost);

            emit m_signalHandler->doProgress();
        }
};

class COcvDecolorFactory : public CProcessFactory
{
    public:

        COcvDecolorFactory()
        {
            m_info.m_name = QObject::tr("Decolor").toStdString();
            m_info.m_description = QObject::tr("This process transforms a color image to a grayscale image. It is a basic tool in digital printing, stylized black-and-white photograph rendering, and in many single channel image processing applications.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Computational photography").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "conversion,convert,color,gray,contrast,preservation";
            m_info.m_authors = "Cewu Lu, Li Xu, and Jiaya Jia";
            m_info.m_article = "Contrast preserving decolorization";
            m_info.m_journal = "In Computational Photography (ICCP), 2012 IEEE International Conference on, pages 1–7. IEEE";
            m_info.m_year = 2012;
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d1/d0d/group__photo.html#ga4864d4c007bda5dacdc5e9d4ed7e222c";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDecolorParam = std::dynamic_pointer_cast<COcvDecolorParam>(pParam);
            if(pDecolorParam != nullptr)
                return std::make_shared<COcvDecolor>(m_info.m_name, pDecolorParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDecolorParam = std::make_shared<COcvDecolorParam>();
            assert(pDecolorParam != nullptr);
            return std::make_shared<COcvDecolor>(m_info.m_name, pDecolorParam);
        }
};

#endif // COCVDECOLOR_HPP
