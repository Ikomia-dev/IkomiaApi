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

#ifndef COCVDNNDETECTOR_HPP
#define COCVDNNDETECTOR_HPP

#include "COcvDnnProcess.h"

//--------------------------------//
//----- COcvDnnDetectorParam -----//
//--------------------------------//
class COcvDnnDetectorParam: public COcvDnnProcessParam
{
    public:

        enum NetworkType {FASTER_RCNN, R_FCN, SSD, YOLO, FACE_DETECTOR, EAST};

        COcvDnnDetectorParam() : COcvDnnProcessParam()
        {
        }

        void        setParamMap(const UMapString& paramMap) override
        {
            COcvDnnProcessParam::setParamMap(paramMap);
            m_netType = std::stoi(paramMap.at("networkType"));
            m_confidence = std::stod(paramMap.at("confidence"));
            m_nmsThreshold = std::stod(paramMap.at("nmsThreshold"));
        }

        UMapString  getParamMap() const override
        {
            auto paramMap = COcvDnnProcessParam::getParamMap();
            paramMap.insert(std::make_pair("networkType", std::to_string(m_netType)));
            paramMap.insert(std::make_pair("confidence", std::to_string(m_confidence)));
            paramMap.insert(std::make_pair("nmsThreshold", std::to_string(m_nmsThreshold)));
            return paramMap;
        }

    public:

        int     m_netType = NetworkType::SSD;
        double  m_confidence = 0.2;
        double  m_nmsThreshold = 0.4;
};

//---------------------------//
//----- COcvDnnDetector -----//
//---------------------------//
class COcvDnnDetector: public COcvDnnProcess
{
    public:

        COcvDnnDetector() : COcvDnnProcess()
        {
            addOutput(std::make_shared<CGraphicsOutput>());
            addOutput(std::make_shared<CBlobMeasureIO>());
        }
        COcvDnnDetector(const std::string name, const std::shared_ptr<COcvDnnDetectorParam> &pParam): COcvDnnProcess(name)
        {
            m_pParam = std::make_shared<COcvDnnDetectorParam>(*pParam);
            addOutput(std::make_shared<CGraphicsOutput>());
            addOutput(std::make_shared<CBlobMeasureIO>());
        }

        size_t      getProgressSteps() override
        {
            return 3;
        }
        int         getNetworkInputSize() const override
        {
            int size = 299;
            auto pParam = std::dynamic_pointer_cast<COcvDnnDetectorParam>(m_pParam);

            switch(pParam->m_netType)
            {
                case COcvDnnDetectorParam::FASTER_RCNN: size = 600; break;
                case COcvDnnDetectorParam::R_FCN: size = 600; break;
                case COcvDnnDetectorParam::SSD: size = 300; break;
                //case COcvDnnDetectorParam::FACE_DETECTOR: size = 300; break;
                case COcvDnnDetectorParam::FACE_DETECTOR: size = 600; break;
                //case COcvDnnDetectorParam::YOLO: size = 416; break;
                case COcvDnnDetectorParam::YOLO: size = 608; break; //Mulitple of 32
                case COcvDnnDetectorParam::EAST: size = 576; break; //Mulitple of 32
            }
            return size;
        }
        double      getNetworkInputScaleFactor() const override
        {
            double factor = 1.0;
            auto pParam = std::dynamic_pointer_cast<COcvDnnDetectorParam>(m_pParam);

            switch(pParam->m_netType)
            {
                case COcvDnnDetectorParam::FASTER_RCNN: factor = 1.6; break;
                case COcvDnnDetectorParam::R_FCN: factor = 1.0; break;
                case COcvDnnDetectorParam::SSD: factor = 1.0/127.5; break;
                case COcvDnnDetectorParam::YOLO: factor = 1.0/255.0; break;
                case COcvDnnDetectorParam::FACE_DETECTOR: factor = 1.0; break;
                case COcvDnnDetectorParam::EAST: factor = 1.0; break;
            }
            return factor;
        }
        cv::Scalar  getNetworkInputMean() const
        {
            cv::Scalar mean = cv::Scalar();
            auto pParam = std::dynamic_pointer_cast<COcvDnnDetectorParam>(m_pParam);

            switch(pParam->m_netType)
            {
                case COcvDnnDetectorParam::FASTER_RCNN: mean = cv::Scalar(102.9801, 115.9465, 122.7717); break;
                case COcvDnnDetectorParam::R_FCN: mean = cv::Scalar(102.9801, 115.9465, 122.7717); break;
                case COcvDnnDetectorParam::SSD: mean = 127.5; break;
                case COcvDnnDetectorParam::YOLO: mean = cv::Scalar(); break;
                case COcvDnnDetectorParam::FACE_DETECTOR: mean = cv::Scalar(104.0, 177.0, 123.0); break;
                case COcvDnnDetectorParam::EAST: mean = cv::Scalar(123.68, 116.78, 103.94); break;
            }
            return mean;
        }

        void        run()
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            //auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsInput>(getInput(1));
            auto pParam = std::dynamic_pointer_cast<COcvDnnDetectorParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgSrc;
            CMat imgOrigin = pInput->getImage();
            std::vector<cv::Mat> netOutputs;

            //Detection networks need color image as input
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

                prepareNetInputs();
                int size = getNetworkInputSize();
                double scaleFactor = getNetworkInputScaleFactor();
                cv::Scalar mean = getNetworkInputMean();
                auto inputBlob = cv::dnn::blobFromImage(imgSrc, scaleFactor, cv::Size(size,size), mean, false, false);
                m_net.setInput(inputBlob);

                auto netOutNames = getOutputsNames();
                m_net.forward(netOutputs, netOutNames);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            readClassNames();
            endTaskRun();
            emit m_signalHandler->doProgress();
            manageOutput(netOutputs);
            emit m_signalHandler->doProgress();
        }

        void        prepareNetInputs()
        {
            auto pParam = std::dynamic_pointer_cast<COcvDnnDetectorParam>(m_pParam);
            if(pParam->m_netType == COcvDnnDetectorParam::FASTER_RCNN || pParam->m_netType == COcvDnnDetectorParam::R_FCN)
            {
                int size = getNetworkInputSize();
                double scaleFactor = getNetworkInputScaleFactor();
                cv::Mat imInfo = (cv::Mat_<float>(1, 3) << size, size, scaleFactor);
                m_net.setInput(imInfo, "im_info");
            }
        }

        void        manageOutput(std::vector<cv::Mat> &netOutputs)
        {
            forwardInputImage();

            auto pParam = std::dynamic_pointer_cast<COcvDnnDetectorParam>(m_pParam);
            switch(pParam->m_netType)
            {
                case COcvDnnDetectorParam::SSD:
                case COcvDnnDetectorParam::FACE_DETECTOR:
                    manageSSDOutput(netOutputs[0]);
                    break;
                case COcvDnnDetectorParam::FASTER_RCNN:
                case COcvDnnDetectorParam::R_FCN:
                    manageRCNNOutput(netOutputs[0]);
                    break;
                case COcvDnnDetectorParam::YOLO:
                    manageYoloOutput(netOutputs);
                    break;
                case COcvDnnDetectorParam::EAST:
                    manageEASTOutput(netOutputs);
                    break;
            }
        }
        void        manageSSDOutput(const cv::Mat &dnnOutput)
        {
            // Network produces output blob with a shape 1x1xNx7 where N is a number of
            // detections and an every detection is a vector of values
            // [batchId, classId, confidence, left, top, right, bottom]
            auto pParam = std::dynamic_pointer_cast<COcvDnnDetectorParam>(m_pParam);
            auto pInput = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            CMat imgSrc = pInput->getImage();

            //Graphics output
            auto pGraphicsOutput = std::dynamic_pointer_cast<CGraphicsOutput>(getOutput(1));
            pGraphicsOutput->setNewLayer("DnnLayer");
            pGraphicsOutput->setImageIndex(0);

            //Measures output
            auto pMeasureOutput = std::dynamic_pointer_cast<CBlobMeasureIO>(getOutput(2));
            pMeasureOutput->clearData();

            for(int i=0; i<dnnOutput.size[2]; i++)
            {
                //Detected class
                int classIndex[4] = { 0, 0, i, 1 };
                size_t classId = (size_t)dnnOutput.at<float>(classIndex);
                //Confidence
                int confidenceIndex[4] = { 0, 0, i, 2 };
                float confidence = dnnOutput.at<float>(confidenceIndex);

                if(confidence > pParam->m_confidence)
                {
                    //Bounding box
                    int leftIndex[4] = { 0, 0, i, 3 };
                    int topIndex[4] = { 0, 0, i, 4 };
                    int rightIndex[4] = { 0, 0, i, 5 };
                    int bottomIndex[4] = { 0, 0, i, 6 };
                    float left = dnnOutput.at<float>(leftIndex) * imgSrc.cols;
                    float top = dnnOutput.at<float>(topIndex) * imgSrc.rows;
                    float right = dnnOutput.at<float>(rightIndex) * imgSrc.cols;
                    float bottom = dnnOutput.at<float>(bottomIndex) * imgSrc.rows;
                    float width = right - left + 1;
                    float height = bottom - top + 1;

                    //Create rectangle graphics of bbox
                    auto graphicsBox = pGraphicsOutput->addRectangle(left, top, width, height);

                    //Retrieve class label
                    std::string className = classId < m_classNames.size() ? m_classNames[classId] : "unknown " + std::to_string(classId);
                    std::string label = className + " : " + std::to_string(confidence);
                    pGraphicsOutput->addText(label, left + 5, top + 5);

                    //Store values to be shown in results table
                    std::vector<CObjectMeasure> results;
                    results.emplace_back(CObjectMeasure(CMeasure(CMeasure::CUSTOM, QObject::tr("Confidence").toStdString()), confidence, graphicsBox->getId(), className));
                    results.emplace_back(CObjectMeasure(CMeasure::Id::BBOX, {left, top, width, height}, graphicsBox->getId(), className));
                    pMeasureOutput->addObjectMeasures(results);
                }
            }
        }
        void        manageRCNNOutput(const cv::Mat &dnnOutput)
        {
            auto pParam = std::dynamic_pointer_cast<COcvDnnDetectorParam>(m_pParam);
            auto pInput = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            CMat imgSrc = pInput->getImage();

            //Graphics output
            auto pGraphicsOutput = std::dynamic_pointer_cast<CGraphicsOutput>(getOutput(1));
            pGraphicsOutput->setNewLayer("DnnLayer");
            pGraphicsOutput->setImageIndex(0);

            //Measures output
            auto pMeasureOutput = std::dynamic_pointer_cast<CBlobMeasureIO>(getOutput(2));
            pMeasureOutput->clearData();

            int size = getNetworkInputSize();
            float xFactor = (float)imgSrc.cols / (float)size;
            float yFactor = (float)imgSrc.rows / (float)size;

            for(int i=0; i<dnnOutput.size[2]; i++)
            {
                //Detected class
                int classIndex[4] = { 0, 0, i, 1 };
                size_t classId = (size_t)dnnOutput.at<float>(classIndex);
                //Confidence
                int confidenceIndex[4] = { 0, 0, i, 2 };
                float confidence = dnnOutput.at<float>(confidenceIndex);

                if(confidence > pParam->m_confidence)
                {
                    //Bounding box
                    int leftIndex[4] = { 0, 0, i, 3 };
                    int topIndex[4] = { 0, 0, i, 4 };
                    int rightIndex[4] = { 0, 0, i, 5 };
                    int bottomIndex[4] = { 0, 0, i, 6 };
                    float left = dnnOutput.at<float>(leftIndex) * xFactor;
                    float top = dnnOutput.at<float>(topIndex) * yFactor;
                    float right = dnnOutput.at<float>(rightIndex) * xFactor;
                    float bottom = dnnOutput.at<float>(bottomIndex) * yFactor;
                    float width = right - left + 1;
                    float height = bottom - top + 1;

                    //Create rectangle graphics of bbox
                    auto graphicsBox = pGraphicsOutput->addRectangle(left, top, width, height);

                    //Retrieve class label
                    std::string className = classId < m_classNames.size() ? m_classNames[classId] : "unknown " + std::to_string(classId);
                    std::string label = className + " : " + std::to_string(confidence);
                    pGraphicsOutput->addText(label, left + 5, top + 5);

                    //Store values to be shown in results table
                    std::vector<CObjectMeasure> results;
                    results.emplace_back(CObjectMeasure(CMeasure(CMeasure::CUSTOM, QObject::tr("Confidence").toStdString()), confidence, graphicsBox->getId(), className));
                    results.emplace_back(CObjectMeasure(CMeasure::Id::BBOX, {left, top, width, height}, graphicsBox->getId(), className));
                    pMeasureOutput->addObjectMeasures(results);
                }
            }
        }
        void        manageYoloOutput(const std::vector<cv::Mat>& netOutputs)
        {
            auto pParam = std::dynamic_pointer_cast<COcvDnnDetectorParam>(m_pParam);
            auto pInput = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            CMat imgSrc = pInput->getImage();
            std::vector<cv::Rect2d> detections;
            std::vector<float> confidences;
            std::vector<size_t> classIds;

            for(size_t i=0; i<netOutputs.size(); ++i)
            {
                for(int j=0; j<netOutputs[i].rows; ++j)
                {
                    const int probabilityIndex = 5;
                    const int probabilitySize = netOutputs[i].cols - probabilityIndex;
                    const float* pProbArray = &netOutputs[i].at<float>(j, probabilityIndex);
                    size_t classId = std::max_element(pProbArray, pProbArray + probabilitySize) - pProbArray;
                    float confidence = netOutputs[i].at<float>(j, (int)classId + probabilityIndex);

                    if (confidence > pParam->m_confidence)
                    {
                        float xCenter = netOutputs[i].at<float>(j, 0) * imgSrc.cols;
                        float yCenter = netOutputs[i].at<float>(j, 1) * imgSrc.rows;
                        float width = netOutputs[i].at<float>(j, 2) * imgSrc.cols;
                        float height = netOutputs[i].at<float>(j, 3) * imgSrc.rows;
                        float left = xCenter - width/2;
                        float top = yCenter - height/2;
                        cv::Rect2d r(left, top, width, height);
                        detections.push_back(r);
                        confidences.push_back(confidence);
                        classIds.push_back(classId);
                    }
                }
            }

            // Apply non-maximum suppression procedure.
            std::vector<int> indices;
            cv::dnn::NMSBoxes(detections, confidences, pParam->m_confidence, pParam->m_nmsThreshold, indices);

            //Graphics output
            auto pGraphicsOutput = std::dynamic_pointer_cast<CGraphicsOutput>(getOutput(1));
            pGraphicsOutput->setNewLayer("DnnLayer");
            pGraphicsOutput->setImageIndex(0);

            //Measures output
            auto pMeasureOutput = std::dynamic_pointer_cast<CBlobMeasureIO>(getOutput(2));
            pMeasureOutput->clearData();

            for(size_t i=0; i<indices.size(); ++i)
            {
                //Create rectangle graphics of bbox
                cv::Rect2d box = detections[indices[i]];
                auto graphicsObj = pGraphicsOutput->addRectangle(box.x, box.y, box.width, box.height);

                //Retrieve class label
                float confidence = confidences[indices[i]];
                size_t classId = classIds[indices[i]];
                std::string className = classId < m_classNames.size() ? m_classNames[classId] : "unknown " + std::to_string(classId);
                std::string label = className + " : " + std::to_string(confidence);
                pGraphicsOutput->addText(label, box.x + 5, box.y + 5);

                //Store values to be shown in results table
                std::vector<CObjectMeasure> results;
                results.emplace_back(CObjectMeasure(CMeasure(CMeasure::CUSTOM, QObject::tr("Confidence").toStdString()), confidence, graphicsObj->getId(), className));
                results.emplace_back(CObjectMeasure(CMeasure::Id::BBOX, {box.x, box.y, box.width, box.height}, graphicsObj->getId(), className));
                pMeasureOutput->addObjectMeasures(results);
            }
        }
        void        manageEASTOutput(const std::vector<cv::Mat>& netOutputs)
        {
            if(netOutputs.size() < 2)
                throw CException(CoreExCode::INVALID_PARAMETER, "Wrong number of EAST Detector outputs", __func__, __FILE__, __LINE__);

            cv::Mat scores = netOutputs[1];
            cv::Mat geometry = netOutputs[0];
            CV_Assert(scores.dims == 4); CV_Assert(geometry.dims == 4); CV_Assert(scores.size[0] == 1);
            CV_Assert(geometry.size[0] == 1); CV_Assert(scores.size[1] == 1); CV_Assert(geometry.size[1] == 5);
            CV_Assert(scores.size[2] == geometry.size[2]); CV_Assert(scores.size[3] == geometry.size[3]);

            auto pParam = std::dynamic_pointer_cast<COcvDnnDetectorParam>(m_pParam);
            auto pInput = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            CMat imgSrc = pInput->getImage();

            std::vector<cv::RotatedRect> detections;
            std::vector<float> confidences;
            const int height = scores.size[2];
            const int width = scores.size[3];

            for (int y = 0; y < height; ++y)
            {
                const float* scoresData = scores.ptr<float>(0, 0, y);
                const float* x0_data = geometry.ptr<float>(0, 0, y);
                const float* x1_data = geometry.ptr<float>(0, 1, y);
                const float* x2_data = geometry.ptr<float>(0, 2, y);
                const float* x3_data = geometry.ptr<float>(0, 3, y);
                const float* anglesData = geometry.ptr<float>(0, 4, y);

                for (int x = 0; x < width; ++x)
                {
                    float score = scoresData[x];
                    if (score < pParam->m_confidence)
                        continue;

                    // Decode a prediction.
                    // Multiple by 4 because feature maps are 4 time less than input image.
                    float offsetX = x * 4.0f, offsetY = y * 4.0f;
                    float angle = anglesData[x];
                    float cosA = std::cos(angle);
                    float sinA = std::sin(angle);
                    float h = x0_data[x] + x2_data[x];
                    float w = x1_data[x] + x3_data[x];

                    cv::Point2f offset(offsetX + cosA * x1_data[x] + sinA * x2_data[x], offsetY - sinA * x1_data[x] + cosA * x2_data[x]);
                    cv::Point2f p1 = cv::Point2f(-sinA * h, -cosA * h) + offset;
                    cv::Point2f p3 = cv::Point2f(-cosA * w, sinA * w) + offset;
                    cv::RotatedRect r(0.5f * (p1 + p3), cv::Size2f(w, h), -angle * 180.0f / (float)CV_PI);
                    detections.push_back(r);
                    confidences.push_back(score);
                }
            }

            // Apply non-maximum suppression procedure.
            std::vector<int> indices;
            cv::dnn::NMSBoxes(detections, confidences, pParam->m_confidence, pParam->m_nmsThreshold, indices);

            //Graphics output
            auto pGraphicsOutput = std::dynamic_pointer_cast<CGraphicsOutput>(getOutput(1));
            pGraphicsOutput->setNewLayer("DnnLayer");
            pGraphicsOutput->setImageIndex(0);

            //Measures output
            auto pMeasureOutput = std::dynamic_pointer_cast<CBlobMeasureIO>(getOutput(2));
            pMeasureOutput->clearData();

            int size = getNetworkInputSize();
            float xFactor = (float)imgSrc.cols / (float)size;
            float yFactor = (float)imgSrc.rows / (float)size;

            for(size_t i=0; i<indices.size(); ++i)
            {
                //Create polygon graphics of rotated box
                cv::RotatedRect& box = detections[indices[i]];
                cv::Point2f vertices[4];
                box.points(vertices);

                PolygonF poly;
                for(int j=0; j<4; ++j)
                    poly.push_back(CPointF(vertices[j].x * xFactor, vertices[j].y * yFactor));

                auto graphicsObj = pGraphicsOutput->addPolygon(poly);

                //Store values to be shown in results table
                std::vector<CObjectMeasure> results;
                results.emplace_back(CObjectMeasure(CMeasure(CMeasure::CUSTOM, QObject::tr("Confidence").toStdString()), confidences[indices[i]], graphicsObj->getId(), "Text"));
                results.emplace_back(CObjectMeasure(CMeasure::Id::ORIENTED_BBOX, {box.center.x * xFactor, box.center.y * yFactor, box.size.width, box.size.height, box.angle}, graphicsObj->getId(), "Text"));
                pMeasureOutput->addObjectMeasures(results);
            }
        }
};

//----------------------------------//
//----- COcvDnnDetectorFactory -----//
//----------------------------------//
class COcvDnnDetectorFactory : public CTaskFactory
{
    public:

        COcvDnnDetectorFactory()
        {
            m_info.m_name = "ocv_dnn detection";
            m_info.m_description = QObject::tr("This process gives the possibility to launch inference from already trained networks for detection purpose (CAFFE, TENSORFLOW, DARKNET and TORCH)).").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Deep neural network").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "cnn,dnn,deep,neural,network,detection,object,face";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d6/d0f/group__dnn.html";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvDnnDetectorParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvDnnDetector>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvDnnDetectorParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvDnnDetector>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVDNNDETECTOR_HPP
