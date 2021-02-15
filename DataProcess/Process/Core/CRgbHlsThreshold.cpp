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

#include "CRgbHlsThreshold.h"
#include "IO/CImageProcessIO.h"

//---------------------------------//
//----- CRgbHlsThresholdParam -----//
//---------------------------------//
CRgbHlsThresholdParam::CRgbHlsThresholdParam() : CProtocolTaskParam()
{
}

void CRgbHlsThresholdParam::setParamMap(const UMapString &paramMap)
{
    m_minR = std::stoi(paramMap.at("minR"));
    m_maxR = std::stoi(paramMap.at("maxR"));
    m_minG = std::stoi(paramMap.at("minG"));
    m_maxG = std::stoi(paramMap.at("maxG"));
    m_minB = std::stoi(paramMap.at("minB"));
    m_maxB = std::stoi(paramMap.at("maxB"));
    m_minH = std::stoi(paramMap.at("minH"));
    m_maxH = std::stoi(paramMap.at("maxH"));
    m_minL = std::stoi(paramMap.at("minL"));
    m_maxL = std::stoi(paramMap.at("maxL"));
    m_minS = std::stoi(paramMap.at("minS"));
    m_maxS = std::stoi(paramMap.at("maxS"));
}

UMapString CRgbHlsThresholdParam::getParamMap() const
{
    UMapString map;
    map.insert(std::make_pair("minR", std::to_string(m_minR)));
    map.insert(std::make_pair("maxR", std::to_string(m_maxR)));
    map.insert(std::make_pair("minG", std::to_string(m_minG)));
    map.insert(std::make_pair("maxG", std::to_string(m_maxG)));
    map.insert(std::make_pair("minB", std::to_string(m_minB)));
    map.insert(std::make_pair("maxB", std::to_string(m_maxB)));
    map.insert(std::make_pair("minH", std::to_string(m_minH)));
    map.insert(std::make_pair("maxH", std::to_string(m_maxH)));
    map.insert(std::make_pair("minL", std::to_string(m_minL)));
    map.insert(std::make_pair("maxL", std::to_string(m_maxL)));
    map.insert(std::make_pair("minS", std::to_string(m_minS)));
    map.insert(std::make_pair("maxS", std::to_string(m_maxS)));
    return map;
}

//----------------------------//
//----- CRgbHlsThreshold -----//
//----------------------------//
CRgbHlsThreshold::CRgbHlsThreshold() : CInteractiveImageProcess2d()
{
    setOutputDataType(IODataType::IMAGE_BINARY, 0);
    addOutput(std::make_shared<CImageProcessIO>());
    setOutputColorMap(1, 0, {{255,0,0}});
}

CRgbHlsThreshold::CRgbHlsThreshold(const std::string name, const std::shared_ptr<CRgbHlsThresholdParam> &pParam)
    : CInteractiveImageProcess2d(name)
{
    setOutputDataType(IODataType::IMAGE_BINARY, 0);
    addOutput(std::make_shared<CImageProcessIO>());
    setOutputColorMap(1, 0, {{255,0,0}});
    m_pParam = std::make_shared<CRgbHlsThresholdParam>(*pParam);
}

size_t CRgbHlsThreshold::getProgressSteps()
{
    return 4;
}

void CRgbHlsThreshold::graphicsChanged()
{
    if(m_layerPtr)
    {
        updateThresholds();
        run();
        emit m_signalHandler->doOutputChanged();
    }
}

void CRgbHlsThreshold::run()
{
    beginTaskRun();
    auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
    auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsProcessInput>(getInput(1));
    auto pParam = std::dynamic_pointer_cast<CRgbHlsThresholdParam>(m_pParam);

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
        if(imgSrc.channels() == 1)
            cv::inRange(imgSrc, pParam->m_minR, pParam->m_maxR, imgDst);
        else
        {
            CMat imgChannels[3];
            CMat imgBinaryTmp;
            //RGB thresholds
            cv::split(imgSrc, imgChannels);
            cv::inRange(imgChannels[0], pParam->m_minR, pParam->m_maxR, imgDst);
            cv::inRange(imgChannels[1], pParam->m_minG, pParam->m_maxG, imgBinaryTmp);
            cv::bitwise_and(imgDst, imgBinaryTmp, imgDst);
            cv::inRange(imgChannels[2], pParam->m_minB, pParam->m_maxB, imgBinaryTmp);
            cv::bitwise_and(imgDst, imgBinaryTmp, imgDst);
            //HLS thresholds
            CMat imgHls;
            cv::cvtColor(imgSrc, imgHls, cv::COLOR_RGB2HLS);
            cv::split(imgHls, imgChannels);
            cv::inRange(imgChannels[0], pParam->m_minH, pParam->m_maxH, imgBinaryTmp);
            cv::bitwise_and(imgDst, imgBinaryTmp, imgDst);
            cv::inRange(imgChannels[1], pParam->m_minL, pParam->m_maxL, imgBinaryTmp);
            cv::bitwise_and(imgDst, imgBinaryTmp, imgDst);
            cv::inRange(imgChannels[2], pParam->m_minS, pParam->m_maxS, imgBinaryTmp);
            cv::bitwise_and(imgDst, imgBinaryTmp, imgDst);
        }
        emit m_signalHandler->doProgress();
    }
    catch(cv::Exception& e)
    {
        throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
    }

    emit m_signalHandler->doProgress();
    applyGraphicsMaskToBinary(imgDst, imgDst, 0);
    forwardInputImage(0, 1);

    auto pOutput = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(0));
    if(pOutput)
        pOutput->setImage(imgDst);

    endTaskRun();
    emit m_signalHandler->doProgress();
}

void CRgbHlsThreshold::executeActions(int flags)
{
    if(flags & CRgbHlsThresholdParam::ActionFlags::CLEAR_SEGMENTATION)
    {
        clearInteractionLayer();
        graphicsChanged();
    }
}

void CRgbHlsThreshold::updateThresholds()
{
    auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
    auto pParam = std::dynamic_pointer_cast<CRgbHlsThresholdParam>(m_pParam);

    if(pInput == nullptr || pParam == nullptr)
        return;

    CMat colorSeedImg;
    CMat imgSrc = pInput->getImage();
    double minValue, maxValue;

    createInteractionMask((int)imgSrc.getNbCols(), (int)imgSrc.getNbRows());
    imgSrc.copyTo(colorSeedImg, m_interactionMask);

    if(imgSrc.channels() == 1)
    {
        cv::minMaxLoc(colorSeedImg, &minValue, &maxValue);
        pParam->m_minR = minValue;
        pParam->m_maxR = maxValue;
    }
    else
    {
        CMat imgChannels[3];
        //RGB thresholds
        cv::split(colorSeedImg, imgChannels);
        cv::minMaxLoc(imgChannels[0], &minValue, &maxValue, NULL, NULL, m_interactionMask);
        pParam->m_minR = minValue;
        pParam->m_maxR = maxValue;
        cv::minMaxLoc(imgChannels[1], &minValue, &maxValue, NULL, NULL, m_interactionMask);
        pParam->m_minG = minValue;
        pParam->m_maxG = maxValue;
        cv::minMaxLoc(imgChannels[2], &minValue, &maxValue, NULL, NULL, m_interactionMask);
        pParam->m_minB = minValue;
        pParam->m_maxB = maxValue;
        //HLS thresholds
        CMat imgHls;
        cv::cvtColor(colorSeedImg, imgHls, cv::COLOR_RGB2HLS);
        cv::split(imgHls, imgChannels);
        cv::minMaxLoc(imgChannels[0], &minValue, &maxValue, NULL, NULL, m_interactionMask);
        pParam->m_minH = minValue;
        pParam->m_maxH = maxValue;
        cv::minMaxLoc(imgChannels[1], &minValue, &maxValue, NULL, NULL, m_interactionMask);
        pParam->m_minL = minValue;
        pParam->m_maxL = maxValue;
        cv::minMaxLoc(imgChannels[2], &minValue, &maxValue, NULL, NULL, m_interactionMask);
        pParam->m_minS = minValue;
        pParam->m_maxS = maxValue;
    }
}

//-----------------------------------//
//----- CRgbHlsThresholdFactory -----//
//-----------------------------------//
CRgbHlsThresholdFactory::CRgbHlsThresholdFactory()
{
    m_info.m_name = QObject::tr("RGB-HLS Segmentation").toStdString();
    m_info.m_description = QObject::tr("This process computes color segmentation from RGB and HLS color spaces.").toStdString();
    m_info.m_path = QObject::tr("Core/Segmentation/").toStdString();
    m_info.m_iconPath = QObject::tr(":/Images/default-process.png").toStdString();
    m_info.m_keywords = "Color segmentation, RGB, HLS, threshold";
}

ProtocolTaskPtr CRgbHlsThresholdFactory::create(const ProtocolTaskParamPtr &pParam)
{
    auto pDerivedParam = std::dynamic_pointer_cast<CRgbHlsThresholdParam>(pParam);
    if(pDerivedParam != nullptr)
        return std::make_shared<CRgbHlsThreshold>(m_info.m_name, pDerivedParam);
    else
        return create();
}

ProtocolTaskPtr CRgbHlsThresholdFactory::create()
{
    auto pDerivedParam = std::make_shared<CRgbHlsThresholdParam>();
    assert(pDerivedParam != nullptr);
    return std::make_shared<CRgbHlsThreshold>(m_info.m_name, pDerivedParam);
}
