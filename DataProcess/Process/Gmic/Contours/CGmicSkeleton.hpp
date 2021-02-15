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

#ifndef CGMICSKELETON_HPP
#define CGMICSKELETON_HPP

#include "Core/CImageProcess2d.h"
#include "Process/Gmic/CGmicTaskParam.hpp"
#include "Process/Gmic/CGmicImageConverter.h"

//------------------------------//
//----- CGmicSkeletonParam -----//
//------------------------------//
class CGmicSkeletonParam: public CGmicTaskParam
{
    public:

        enum Method { DISTANCE, THINNING };

        CGmicSkeletonParam() : CGmicTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_method = std::stoi(paramMap.at("method"));
            m_smoothness = std::stod(paramMap.at("smoothness"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("method", std::to_string(m_method)));
            map.insert(std::make_pair("smoothness", std::to_string(m_smoothness)));
            return map;
        }

        std::string getParamString() const override
        {
            //Build string of parameters, take care of order.
            return std::to_string(m_method) + "," + std::to_string(m_smoothness);
        }

    public:

        int     m_method = DISTANCE;
        float   m_smoothness = 0;
};

//-------------------------//
//----- CGmicSkeleton -----//
//-------------------------//
class CGmicSkeleton : public CImageProcess2d
{
    public:

        CGmicSkeleton() : CImageProcess2d()
        {
        }
        CGmicSkeleton(const std::string name, const std::shared_ptr<CGmicSkeletonParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<CGmicSkeletonParam>(*pParam);
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        void run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsProcessInput>(getInput(1));
            auto pParam = std::dynamic_pointer_cast<CGmicSkeletonParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid input", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgSrc = pInput->getImage();
            CMat imgDst(imgSrc.rows, imgSrc.cols, imgSrc.type());
            createGraphicsMask(imgSrc.getNbCols(), imgSrc.getNbRows(), pGraphicsInput);
            emit m_signalHandler->doProgress();

            try
            {
                //Put image inputs into a gmic_list object
                gmic_list<float> imageList;
                gmic_list<char> image_names;

                //Allocate list, parameter is the number of image inputs
                imageList.assign(1);
                //Conversion from CMat(cv::Mat) to gmic_image(CImg)
                CGmicImageConverter::convert(imgSrc, imageList[0]);
                //Build command for gmic interpreter
                std::string command = "fx_skeleton[0] " + pParam->getParamString();
                //Call interpreter
                gmic(command.c_str(), imageList, image_names);
                //Conversion from gmic_image to CMat
                CGmicImageConverter::convert(imageList[0], imgDst);
                //Free memory
                imageList.assign(0);
            }
            catch(gmic_exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e.what(), __func__, __FILE__, __LINE__);
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

class CGmicSkeletonFactory : public CProcessFactory
{
    public:

        CGmicSkeletonFactory()
        {
            m_info.m_name = QObject::tr("Skeleton").toStdString();
            m_info.m_description = QObject::tr("This process generates a skeleton image from input image.").toStdString();
            m_info.m_path = QObject::tr("Gmic/Contours").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/gmic.png").toStdString();
            m_info.m_keywords = "binary,edge";
            m_info.m_authors = "David Tschumperlé";
            m_info.m_year = 2011;
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<CGmicSkeletonParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<CGmicSkeleton>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pParam = std::make_shared<CGmicSkeletonParam>();
            assert(pParam != nullptr);
            return std::make_shared<CGmicSkeleton>(m_info.m_name, pParam);
        }
};

#endif // CGMICSKELETON_HPP
