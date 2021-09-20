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

#ifndef COCVCANNY_HPP
#define COCVCANNY_HPP

#include "Core/C2dImageTask.h"
#include "IO/CImageIO.h"

//------------------------------//
//----- COcvCannyParam -----//
//------------------------------//
class COcvCannyParam: public CWorkflowTaskParam
{
    public:

        COcvCannyParam() : CWorkflowTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_threshold1 = std::stod(paramMap.at("threshold1"));
            m_threshold2 = std::stod(paramMap.at("threshold2"));
            m_apertureSize = std::stoi(paramMap.at("apertureSize"));
            m_L2gradient = std::stoi(paramMap.at("L2gradient"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("threshold1", std::to_string(m_threshold1)));
            map.insert(std::make_pair("threshold2", std::to_string(m_threshold2)));
            map.insert(std::make_pair("apertureSize", std::to_string(m_apertureSize)));
            map.insert(std::make_pair("L2gradient", std::to_string(m_L2gradient)));
            return map;
        }

    public:

        double  	m_threshold1 = 0.0;
        double  	m_threshold2 = 255.0;
        int         m_apertureSize = 3;
        bool        m_L2gradient = false;
};

//-------------------------//
//----- COcvCanny -----//
//-------------------------//
class COcvCanny : public C2dImageTask
{
    public:

        COcvCanny() : C2dImageTask()
        {
            setOutputDataType(IODataType::IMAGE_BINARY, 0);
        }
        COcvCanny(const std::string name, const std::shared_ptr<COcvCannyParam>& pParam) : C2dImageTask(name)
        {
            setOutputDataType(IODataType::IMAGE_BINARY, 0);
            m_pParam = std::make_shared<COcvCannyParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<COcvCannyParam>(m_pParam);

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
                cv::Canny(imgSrc, imgDst, pParam->m_threshold1, pParam->m_threshold2, pParam->m_apertureSize, pParam->m_L2gradient);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            applyGraphicsMask(imgSrc, imgDst, 0);
            emit m_signalHandler->doProgress();

            auto pOutput = std::dynamic_pointer_cast<CImageIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(imgDst);

            endTaskRun();
            emit m_signalHandler->doProgress();
        }
};

class COcvCannyFactory : public CTaskFactory
{
    public:

        COcvCannyFactory()
        {
            m_info.m_name = "ocv_canny";
            m_info.m_description = QObject::tr("The function finds edges in the input image and marks them in the output map edges using the Canny algorithm. The smallest value between threshold1 and threshold2 is used for edge linking. The largest value is used to find initial segments of strong edges.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Image processing/Feature detection").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Derivative,Edges,Contours,Hysterisis,Gradient";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/dd/d1a/group__imgproc__feature.html#ga04723e007ed888ddf11d9ba04e2232de";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pCannyParam = std::dynamic_pointer_cast<COcvCannyParam>(pParam);
            if(pCannyParam != nullptr)
                return std::make_shared<COcvCanny>(m_info.m_name, pCannyParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pCannyParam = std::make_shared<COcvCannyParam>();
            assert(pCannyParam != nullptr);
            return std::make_shared<COcvCanny>(m_info.m_name, pCannyParam);
        }
};

#endif // COCVCANNY_HPP
