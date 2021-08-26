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

#ifndef COCVDNNCLASSIFIER_HPP
#define COCVDNNCLASSIFIER_HPP

#include "COcvDnnProcess.h"
#include "Graphics/CGraphicsLayer.h"

//----------------------------------//
//----- COcvDnnClassifierParam -----//
//----------------------------------//
class COcvDnnClassifierParam: public COcvDnnProcessParam
{
    public:

        enum NetworkType {ALEXNET, GOOGLENET, INCEPTION, VGG, RESNET};

        COcvDnnClassifierParam() : COcvDnnProcessParam()
        {
        }

        void        setParamMap(const UMapString& paramMap) override
        {
            COcvDnnProcessParam::setParamMap(paramMap);
            m_netType = std::stoi(paramMap.at("networkType"));
        }

        UMapString  getParamMap() const override
        {
            auto paramMap = COcvDnnProcessParam::getParamMap();
            paramMap.insert(std::make_pair("networkType", std::to_string(m_netType)));
            return paramMap;
        }

    public:

        int m_netType = NetworkType::VGG;
};

//-----------------------------//
//----- COcvDnnClassifier -----//
//-----------------------------//
class COcvDnnClassifier: public COcvDnnProcess
{
    public:

        COcvDnnClassifier() : COcvDnnProcess()
        {
            addOutput(std::make_shared<CGraphicsOutput>());
            addOutput(std::make_shared<CBlobMeasureIO>());
        }
        COcvDnnClassifier(const std::string& name, const std::shared_ptr<COcvDnnClassifierParam>& pParam) : COcvDnnProcess(name)
        {
            m_pParam = std::make_shared<COcvDnnClassifierParam>(*pParam);
            addOutput(std::make_shared<CGraphicsOutput>());
            addOutput(std::make_shared<CBlobMeasureIO>());
        }

        size_t      getProgressSteps() override
        {
            return 3;
        }
        int         getNetworkInputSize() const override
        {
            int size = 224;
            auto pParam = std::dynamic_pointer_cast<COcvDnnClassifierParam>(m_pParam);

            switch(pParam->m_netType)
            {
                case COcvDnnClassifierParam::ALEXNET: size = 224; break;
                case COcvDnnClassifierParam::GOOGLENET: size = 224; break;
                case COcvDnnClassifierParam::INCEPTION: size = 224; break;
                case COcvDnnClassifierParam::VGG: size = 300; break;
                case COcvDnnClassifierParam::RESNET: size = 224; break;
            }
            return size;
        }

        void        run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            //auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsInput>(getInput(1));
            auto pParam = std::dynamic_pointer_cast<COcvDnnClassifierParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgOrigin = pInput->getImage();
            cv::Mat dnnOutput;
            CMat imgSrc;

            //Need color image as input
            if(imgOrigin.channels() < 3)
                cv::cvtColor(imgOrigin, imgSrc, cv::COLOR_GRAY2RGB);
            else
                imgSrc = imgOrigin;

            //createGraphicsMask(imgSrc.getNbCols(), imgSrc.getNbRows(), pGraphicsInput);
            emit m_signalHandler->doProgress();

            try
            {
                if(m_net.empty() || pParam->m_bUpdate)
                {
                    m_net = readDnn();
                    if(m_net.empty())
                        throw CException(CoreExCode::INVALID_PARAMETER, "Failed to load network", __func__, __FILE__, __LINE__);

                    pParam->m_bUpdate = false;
                }
                int size = getNetworkInputSize();
                double scaleFactor = getNetworkInputScaleFactor();
                cv::Scalar mean = getNetworkInputMean();
                auto inputBlob = cv::dnn::blobFromImage(imgSrc, scaleFactor, cv::Size(size,size), mean, false, false);
                m_net.setInput(inputBlob);
                dnnOutput = m_net.forward(m_outputLayerName);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            readClassNames();
            endTaskRun();
            emit m_signalHandler->doProgress();
            manageOutput(dnnOutput);
            emit m_signalHandler->doProgress();
        }

        void        manageOutput(cv::Mat &dnnOutput)
        {
            forwardInputImage();

            //Sort the 1 x n matrix of probabilities
            cv::Mat sortedIdx;
            cv::sortIdx(dnnOutput, sortedIdx, cv::SORT_EVERY_ROW | cv::SORT_DESCENDING);

            auto classId = sortedIdx.at<int>(0, 0);
            double confidence = dnnOutput.at<float>(classId);

            //Graphics output
            auto pGraphicsOutput = std::dynamic_pointer_cast<CGraphicsOutput>(getOutput(1));
            assert(pGraphicsOutput);
            pGraphicsOutput->setNewLayer("DnnLayer");
            pGraphicsOutput->setImageIndex(0);

            //Measures output
            auto pMeasureOutput = std::dynamic_pointer_cast<CBlobMeasureIO>(getOutput(2));
            pMeasureOutput->clearData();

            //We don't create the final CGraphicsText instance here for thread-safety reason
            //So we saved necessary information into the output and the final object is
            //created when the output is managed by the App
            std::string className = classId < (int)m_classNames.size() ? m_classNames[classId] : "unknown " + std::to_string(classId);
            std::string label = className + " : " + std::to_string(confidence);
            pGraphicsOutput->addText(label, 30, 30);

            //Store values to be shown in results table
            for(int i=0; i<sortedIdx.cols; ++i)
            {
                classId = sortedIdx.at<int>(i);
                className = classId < (int)m_classNames.size() ? m_classNames[classId] : "unknown " + std::to_string(classId);
                pMeasureOutput->addObjectMeasure(CObjectMeasure(CMeasure(CMeasure::CUSTOM, QObject::tr("Confidence").toStdString()), dnnOutput.at<float>(classId), i, className));
            }
        }
};

//------------------------------------//
//----- COcvDnnClassifierFactory -----//
//------------------------------------//
class COcvDnnClassifierFactory : public CTaskFactory
{
    public:

        COcvDnnClassifierFactory()
        {
            m_info.m_name = QObject::tr("DNN classification").toStdString();
            m_info.m_description = QObject::tr("This process gives the possibility to launch inference from already trained networks for classification purpose (CAFFE, TENSORFLOW, DARKNET and TORCH)).").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Deep neural network").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "cnn,dnn,deep,neural,network,classification";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d6/d0f/group__dnn.html";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvDnnClassifierParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvDnnClassifier>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvDnnClassifierParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvDnnClassifier>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVDNNCLASSIFIER_HPP
