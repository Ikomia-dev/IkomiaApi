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

#ifndef COCVCASCADECLASSIFIER_H
#define COCVCASCADECLASSIFIER_H

#include "Core/C2dImageTask.h"
#include "IO/CImageIO.h"
#include "IO/CGraphicsOutput.h"
#include "Graphics/CGraphicsLayer.h"
#include "Graphics/CGraphicsRectangle.h"

//--------------------------------------//
//----- COcvCascadeClassifierParam -----//
//--------------------------------------//
class COcvCascadeClassifierParam: public CWorkflowTaskParam
{
    public:

        COcvCascadeClassifierParam(): CWorkflowTaskParam()
        {
#ifdef _WIN32
            //m_modelFiles.push_back("C:/msys64/mingw64/share/OpenCV/haarcascades/haarcascade_eye.xml");
            m_modelFiles.push_back("haarcascade_frontalcatface.xml");
#else
            m_modelFiles.push_back("haarcascade_eye.xml");
            //std::string modelFile = "/usr/share/opencv/haarcascades/haarcascade_frontalface_default.xml";
            //std::string modelFile = "/usr/share/opencv/haarcascades/haarcascade_frontalface_alt2.xml";
#endif
        }
        COcvCascadeClassifierParam(std::string modelFile) : CWorkflowTaskParam()
        {
            m_modelFiles.push_back(modelFile);
        }
        COcvCascadeClassifierParam(std::vector<std::string> modelFiles) : CWorkflowTaskParam()
        {
            m_modelFiles = modelFiles;
        }

        void        setParamMap(const UMapString& paramMap) override
        {
            for(auto it=paramMap.begin(); it!=paramMap.end(); ++it)
                m_modelFiles.push_back(it->second);
        }

        UMapString  getParamMap() const override
        {
            UMapString map;

            for(size_t i=0; i<m_modelFiles.size(); ++i)
            {
                std::string name = "ModelFile" + std::to_string(i+1);
                map.insert(std::make_pair(name, m_modelFiles[i]));
            }
            return map;
        }

    public:

        std::vector<std::string>    m_modelFiles;
};

//---------------------------------//
//----- COcvCascadeClassifier -----//
//---------------------------------//
class COcvCascadeClassifier : public C2dImageTask
{
    public:

        COcvCascadeClassifier() : C2dImageTask()
        {
            addOutput(std::make_shared<CGraphicsOutput>());
        }
        COcvCascadeClassifier(const std::string name, const std::shared_ptr<COcvCascadeClassifierParam>& pParam) : C2dImageTask(name)
        {
            m_pParam = std::make_shared<COcvCascadeClassifierParam>(*pParam);
            addOutput(std::make_shared<CGraphicsOutput>());
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        void run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            auto pParam = std::dynamic_pointer_cast<COcvCascadeClassifierParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            cv::CascadeClassifier cascade;
            CMat imgDst = pInput->getImage().clone();
            CMat grayImg;
            CMat boostImg;
            emit m_signalHandler->doProgress();

            if(imgDst.channels() > 1)
                cv::decolor(imgDst, grayImg, boostImg);
            else
                grayImg = imgDst;

            if(getOutputCount() < 2)
                throw CException(CoreExCode::INVALID_SIZE, "Wrong outputs count", __func__, __FILE__, __LINE__);

            auto pOutImage = std::dynamic_pointer_cast<CImageIO>(getOutput(0));
            pOutImage->setImage(imgDst);
            emit m_signalHandler->doProgress();

            try
            {
                auto pOutGraphics = std::dynamic_pointer_cast<CGraphicsOutput>(getOutput(1));
                pOutGraphics->setNewLayer(getName());
                pOutGraphics->setImageIndex(0);

                for(auto&& it : pParam->m_modelFiles)
                {
                    if(cascade.load(it))
                    {
                        std::vector<cv::Rect> detectedObjects;
                        cascade.detectMultiScale(grayImg, detectedObjects);

                        for(size_t i=0 ; i<detectedObjects.size(); ++i)
                            pOutGraphics->addRectangle(detectedObjects[i].x, detectedObjects[i].y, detectedObjects[i].width, detectedObjects[i].height);
                    }
                }
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            endTaskRun();
            emit m_signalHandler->doProgress();
        }
};

class COcvCascadeClassifierFactory : public CTaskFactory
{
    public:

        COcvCascadeClassifierFactory()
        {
            m_info.m_name = QObject::tr("Cascade Classifier Filter").toStdString();
            m_info.m_description = QObject::tr("").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Object detection").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Detection, Haar";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d1/de5/classcv_1_1CascadeClassifier.html";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pCascadeParam = std::dynamic_pointer_cast<COcvCascadeClassifierParam>(pParam);
            if(pCascadeParam != nullptr)
                return std::make_shared<COcvCascadeClassifier>(m_info.m_name, pCascadeParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pCascadeParam = std::make_shared<COcvCascadeClassifierParam>();
            assert(pCascadeParam != nullptr);
            return std::make_shared<COcvCascadeClassifier>(m_info.m_name, pCascadeParam);
        }
};

#endif // COCVCASCADECLASSIFIER_H
