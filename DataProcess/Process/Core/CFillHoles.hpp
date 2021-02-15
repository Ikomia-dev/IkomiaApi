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

#ifndef CFILLHOLES_HPP
#define CFILLHOLES_HPP

#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"

//----------------------//
//----- CFillHoles -----//
//----------------------//
class CFillHoles : public CImageProcess2d
{
    public:

        CFillHoles() : CImageProcess2d()
        {
            getInput(0)->setDataType(IODataType::IMAGE_BINARY);
            setOutputDataType(IODataType::IMAGE_BINARY, 0);
        }
        CFillHoles(const std::string name, const std::shared_ptr<CProtocolTaskParam>& pParam) : CImageProcess2d(name)
        {
            getInput(0)->setDataType(IODataType::IMAGE_BINARY);
            setOutputDataType(IODataType::IMAGE_BINARY, 0);
            m_pParam = std::make_shared<CProtocolTaskParam>(*pParam);
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

            if(pInput == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgSrc = pInput->getImage();
            CMat imgDst = imgSrc.clone();
            createGraphicsMask(imgSrc.getNbCols(), imgSrc.getNbRows(), pGraphicsInput);
            emit m_signalHandler->doProgress();

            try
            {
                std::vector<std::vector<cv::Point>> contours;
                cv::findContours(imgDst, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
                cv::drawContours(imgDst, contours, -1, cv::Scalar(255), cv::FILLED);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            endTaskRun();
            emit m_signalHandler->doProgress();
            applyGraphicsMask(imgSrc, imgDst, 0);

            auto pOutput = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(imgDst);

            emit m_signalHandler->doProgress();
        }
};

class CFillHolesFactory : public CProcessFactory
{
    public:

        CFillHolesFactory()
        {
            m_info.m_name = QObject::tr("Fill Holes").toStdString();
            m_info.m_description = QObject::tr("This process fill holes on binary images.").toStdString();
            m_info.m_path = QObject::tr("Core/Utils").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/default-process.png").toStdString();
            m_info.m_keywords = "fill,holes,binary";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            if(pParam != nullptr)
                return std::make_shared<CFillHoles>(m_info.m_name, pParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<CProtocolTaskParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<CFillHoles>(m_info.m_name, pDerivedParam);
        }
};

#endif // CFILLHOLES_HPP
