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

#include "COcvDnnSegmentation.h"
#include "Graphics/CGraphicsLayer.h"

//------------------------------------//
//----- COcvDnnSegmentationParam -----//
//------------------------------------//
COcvDnnSegmentationParam::COcvDnnSegmentationParam() : COcvDnnProcessParam()
{
}

void COcvDnnSegmentationParam::setParamMap(const UMapString &paramMap)
{
    COcvDnnProcessParam::setParamMap(paramMap);
    m_netType = std::stoi(paramMap.at("networkType"));
    m_confidence = std::stod(paramMap.at("confidence"));
    m_maskThreshold = std::stod(paramMap.at("maskThreshold"));
}

UMapString COcvDnnSegmentationParam::getParamMap() const
{
    auto paramMap = COcvDnnProcessParam::getParamMap();
    paramMap.insert(std::make_pair("networkType", std::to_string(m_netType)));
    paramMap.insert(std::make_pair("confidence", std::to_string(m_confidence)));
    paramMap.insert(std::make_pair("maskThreshold", std::to_string(m_maskThreshold)));
    return paramMap;
}

//-------------------------------//
//----- COcvDnnSegmentation -----//
//-------------------------------//
COcvDnnSegmentation::COcvDnnSegmentation() : COcvDnnProcess()
{
    setOutputDataType(IODataType::IMAGE_LABEL, 0);
    addOutput(std::make_shared<CImageProcessIO>());
    addOutput(std::make_shared<CImageProcessIO>());
    addOutput(std::make_shared<CGraphicsProcessOutput>());
}

COcvDnnSegmentation::COcvDnnSegmentation(const std::string name, const std::shared_ptr<COcvDnnSegmentationParam> &pParam): COcvDnnProcess(name)
{
    m_pParam = std::make_shared<COcvDnnSegmentationParam>(*pParam);
    setOutputDataType(IODataType::IMAGE_LABEL, 0);
    addOutput(std::make_shared<CImageProcessIO>());
    addOutput(std::make_shared<CImageProcessIO>());
    addOutput(std::make_shared<CGraphicsProcessOutput>());
}

size_t COcvDnnSegmentation::getProgressSteps()
{
    return 3;
}

int COcvDnnSegmentation::getNetworkInputSize() const
{
    int size = 299;
    auto pParam = std::dynamic_pointer_cast<COcvDnnSegmentationParam>(m_pParam);

    switch(pParam->m_netType)
    {
    case COcvDnnSegmentationParam::ENET: size = 512; break;
    case COcvDnnSegmentationParam::FCN: size = 500; break;
    case COcvDnnSegmentationParam::MASK_RCNN: size = 800; break;
    case COcvDnnSegmentationParam::UNET: size = 572; break;
    }
    return size;
}

double COcvDnnSegmentation::getNetworkInputScaleFactor() const
{
    double factor = 1.0;
    auto pParam = std::dynamic_pointer_cast<COcvDnnSegmentationParam>(m_pParam);

    switch(pParam->m_netType)
    {
    case COcvDnnSegmentationParam::ENET: factor = 1.0/255.0; break;
    case COcvDnnSegmentationParam::FCN: factor = 1.0; break;
    case COcvDnnSegmentationParam::MASK_RCNN: factor = 1.0; break;
    case COcvDnnSegmentationParam::UNET: factor = 1.0; break;
    }
    return factor;
}

cv::Scalar COcvDnnSegmentation::getNetworkInputMean() const
{
    cv::Scalar mean = cv::Scalar();
    auto pParam = std::dynamic_pointer_cast<COcvDnnSegmentationParam>(m_pParam);

    switch(pParam->m_netType)
    {
    case COcvDnnSegmentationParam::ENET:
    case COcvDnnSegmentationParam::FCN:
    case COcvDnnSegmentationParam::MASK_RCNN:
    case COcvDnnSegmentationParam::UNET:
        mean = cv::Scalar();
        break;
    }
    return mean;
}

std::vector<cv::String> COcvDnnSegmentation::getOutputsNames() const
{
    auto outNames = COcvDnnProcess::getOutputsNames();

    auto pParam = std::dynamic_pointer_cast<COcvDnnSegmentationParam>(m_pParam);
    if(pParam->m_netType == COcvDnnSegmentationParam::MASK_RCNN)
        outNames.push_back("detection_out_final");

    return outNames;
}

bool COcvDnnSegmentation::isBgr()
{
    bool bBgr = false;
    auto pParam = std::dynamic_pointer_cast<COcvDnnSegmentationParam>(m_pParam);

    switch(pParam->m_netType)
    {
        case COcvDnnSegmentationParam::ENET: bBgr = false; break;
        case COcvDnnSegmentationParam::FCN: bBgr = false; break;
        case COcvDnnSegmentationParam::MASK_RCNN: bBgr = true; break;
        case COcvDnnSegmentationParam::UNET: bBgr = false; break;
    }
    return bBgr;
}

void COcvDnnSegmentation::run()
{
    beginTaskRun();
    auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
    auto pParam = std::dynamic_pointer_cast<COcvDnnSegmentationParam>(m_pParam);

    if(pInput == nullptr || pParam == nullptr)
        throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

    if(pInput->isDataAvailable() == false)
        throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

    CMat imgSrc = pInput->getImage();
    std::vector<cv::Mat> netOutputs;
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
        auto inputBlob = cv::dnn::blobFromImage(imgSrc, scaleFactor, cv::Size(size,size), mean, isBgr(), false);
        m_net.setInput(inputBlob);

        auto netOutNames = getOutputsNames();
        m_net.forward(netOutputs, netOutNames);
    }
    catch(cv::Exception& e)
    {
        throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
    }

    readClassNames();
    manageOutput(netOutputs);
    emit m_signalHandler->doProgress();
    endTaskRun();
    emit m_signalHandler->doProgress();
}

void COcvDnnSegmentation::manageOutput(std::vector<cv::Mat> &netOutputs)
{
    forwardInputImage(0, 1);

    auto pParam = std::dynamic_pointer_cast<COcvDnnSegmentationParam>(m_pParam);
    if(pParam->m_netType == COcvDnnSegmentationParam::MASK_RCNN)
    {
        manageMaskRCNNOutput(netOutputs);
        auto colors = generateColorMap(netOutputs[0], false);
        createLegendImage(netOutputs[0], colors, false);
    }
    else
    {
        createLabelImageOutput(netOutputs[0]);
        auto colors = generateColorMap(netOutputs[0], true);
        createLegendImage(netOutputs[0], colors, true);
    }
}

void COcvDnnSegmentation::manageMaskRCNNOutput(std::vector<cv::Mat> &netOutputs)
{
    auto pParam = std::dynamic_pointer_cast<COcvDnnSegmentationParam>(m_pParam);
    auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));

    //Graphics output
    auto pGraphicsOutput = std::dynamic_pointer_cast<CGraphicsProcessOutput>(getOutput(3));
    pGraphicsOutput->setNewLayer("DnnLayer");
    pGraphicsOutput->setImageIndex(1);

    CMat imgSrc = pInput->getImage();
    cv::Mat labelImg = cv::Mat::zeros(imgSrc.rows, imgSrc.cols, CV_8UC1);
    int nbDetections = netOutputs[1].size[2];

    for(int n=0; n<nbDetections; ++n)
    {
        //Detected class
        int classIndex[4] = { 0, 0, n, 1 };
        size_t classId = (size_t)netOutputs[1].at<float>(classIndex);
        //Confidence
        int confidenceIndex[4] = { 0, 0, n, 2 };
        float confidence = netOutputs[1].at<float>(confidenceIndex);

        if(confidence > pParam->m_confidence)
        {
            //Bounding box
            int leftIndex[4] = { 0, 0, n, 3 };
            int topIndex[4] = { 0, 0, n, 4 };
            int rightIndex[4] = { 0, 0, n, 5 };
            int bottomIndex[4] = { 0, 0, n, 6 };
            float left = netOutputs[1].at<float>(leftIndex) * imgSrc.cols;
            float top = netOutputs[1].at<float>(topIndex) * imgSrc.rows;
            float right = netOutputs[1].at<float>(rightIndex) * imgSrc.cols;
            float bottom = netOutputs[1].at<float>(bottomIndex) * imgSrc.rows;
            float width = right - left + 1;
            float height = bottom - top + 1;

            //Create rectangle graphics of bbox
            pGraphicsOutput->addRectangle(left, top, width, height);

            //Retrieve class label
            std::string className = classId < m_classNames.size() ? m_classNames[classId] : "unknown " + std::to_string(classId);
            std::string label = className + " : " + std::to_string(confidence);
            pGraphicsOutput->addText(label, left + 5, top + 5);

            //Extract mask
            cv::Mat objMask(netOutputs[0].size[2], netOutputs[0].size[3], CV_32F, netOutputs[0].ptr<float>(n, classId));
            //Resize to the size of the box
            cv::resize(objMask, objMask, cv::Size(width, height), cv::INTER_LINEAR);
            //Apply thresholding to get the pixel wise mask
            cv::Mat objMaskBinary = (objMask > pParam->m_maskThreshold);
            objMaskBinary.convertTo(objMaskBinary, CV_8U);
            //Create label mask according to the object class (we do classId + 1 because 0 is the background label)
            cv::Mat classLabelImg(height, width, CV_8UC1, cv::Scalar(classId + 1));
            cv::Mat objClassMask(height, width, CV_8UC1, cv::Scalar(0));
            classLabelImg.copyTo(objClassMask, objMaskBinary);
            //Merge object label mask to final label image
            cv::Mat roi = labelImg(cv::Rect(left, top, width, height));
            cv::bitwise_or(roi, objClassMask, roi);
        }
    }

    auto pOutput = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(0));
    if(pOutput)
        pOutput->setImage(labelImg);
}

void COcvDnnSegmentation::createLabelImageOutput(const cv::Mat &netOutput)
{
    const int classes = netOutput.size[1];
    const int rows = netOutput.size[2];
    const int cols = netOutput.size[3];

    cv::Mat labelImg = cv::Mat::zeros(rows, cols, CV_8UC1);
    cv::Mat maxVal(rows, cols, CV_32FC1, netOutput.data);

    for(int c=1; c<classes; ++c)
    {
        for(int i=0; i<rows; ++i)
        {
            const float* pScore = netOutput.ptr<float>(0, c, i);
            uint8_t* pMaxClass = labelImg.ptr<uint8_t>(i);
            float* pMaxVal = maxVal.ptr<float>(i);

            for(int j=0; j<cols; ++j)
            {
                if(pScore[j] > pMaxVal[j])
                {
                    pMaxVal[j] = pScore[j];
                    pMaxClass[j] = (uint8_t)c;
                }
            }
        }
    }

    auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
    cv::resize(labelImg, labelImg, pInput->getImage().size(), 0, 0, cv::INTER_LINEAR);

    auto pOutput = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(0));
    if(pOutput)
        pOutput->setImage(labelImg);
}

std::vector<cv::Vec3b> COcvDnnSegmentation::generateColorMap(const cv::Mat &netOutput, bool bWithBackgroundClass)
{
    int classCount = netOutput.size[1];
    std::vector<cv::Vec3b> colors;

    //The label value 0 is reserved for background pixels
    colors.push_back(cv::Vec3b(0, 0, 0));

    if(bWithBackgroundClass == false)
        classCount++;

    //Random colors then
    for(int i=1; i< classCount; ++i)
    {
        cv::Vec3b color;
        for(int j=0; j<3; ++j)
            color[j] = (uchar)((double)std::rand() / (double)RAND_MAX * 255.0);

        colors.push_back(color);
    }
    setOutputColorMap(1, 0, colors);
    return colors;
}

void COcvDnnSegmentation::createLegendImage(const cv::Mat &netOutput, const std::vector<cv::Vec3b> &colors, bool bWithBackgroundClass)
{
    auto pOutput = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(2));
    if(pOutput == nullptr)
        throw CException(CoreExCode::INVALID_PARAMETER, "Invalid legend output", __func__, __FILE__, __LINE__);

    int classCount = netOutput.size[1];
    if(bWithBackgroundClass == false)
        classCount++;

    const int colorAreaHeight = 20;
    const int margin = 10;
    const int hSpacing = 5;
    const int vSpacing = 3;
    const int width = 250;
    const int height = 2*margin + ((classCount-1) * vSpacing) + (classCount * colorAreaHeight);
    const int fontFace = cv::FONT_HERSHEY_SIMPLEX;
    const int fontThickness = 1;
    const double fontScale = Utils::Font::getQtToOcvFontScaleFactor(12);
    cv::Mat legendImg(height, width, CV_8UC3, cv::Scalar(48,48,48));

    int maxTextWidth = 0;
    int maxTextHeight = 0;
    for(size_t i=0; i<m_classNames.size(); ++i)
    {
        int baseline = 0;
        cv::Size textSize = cv::getTextSize(m_classNames[i], fontFace, fontScale, fontThickness, &baseline);

        if(textSize.width > maxTextWidth)
            maxTextWidth = textSize.width;
        if(textSize.height > maxTextHeight)
            maxTextHeight = textSize.height;
    }

    cv::Point textPos(margin, margin + (colorAreaHeight/2 + maxTextHeight/2));
    const int colorAreaWidth = width - (2*margin + maxTextWidth + hSpacing);
    cv::Rect colorRect(margin + maxTextWidth + hSpacing, margin, colorAreaWidth, colorAreaHeight);

    for(int i=0; i< classCount; ++i)
    {
        std::string name;
        if(i < (int)m_classNames.size())
        {
            if(bWithBackgroundClass == false)
            {
                if(i == 0)
                    name = "Background";
                else
                    name = m_classNames[i-1];
            }
            else
                name = m_classNames[i];
        }
        else
            name = "Unknown";

        cv::putText(legendImg, name, textPos, fontFace, fontScale, cv::Scalar(255,255,255), fontThickness, cv::LINE_AA);
        textPos.y += colorAreaHeight + vSpacing;
        cv::rectangle(legendImg, colorRect, colors[i], cv::FILLED);
        colorRect.y += colorAreaHeight + vSpacing;
    }
    pOutput->setImage(legendImg);
}
