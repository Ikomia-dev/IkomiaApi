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

#include "CBinaryToGraphics.h"
#include "Main/CoreTools.hpp"

//----------------------------------//
//----- CBinaryToGraphicsParam -----//
//----------------------------------//
CBinaryToGraphicsParam::CBinaryToGraphicsParam() : CProtocolTaskParam()
{
}

void CBinaryToGraphicsParam::setParamMap(const UMapString &paramMap)
{
    Q_UNUSED(paramMap);
}

UMapString CBinaryToGraphicsParam::getParamMap() const
{
    return UMapString();
}

//-----------------------------//
//----- CBinaryToGraphics -----//
//-----------------------------//
CBinaryToGraphics::CBinaryToGraphics() : CImageProcess2d(false)
{
    addOutput(std::make_shared<CGraphicsProcessOutput>());
}

CBinaryToGraphics::CBinaryToGraphics(const std::string name, const std::shared_ptr<CBinaryToGraphicsParam> &pParam) : CImageProcess2d(name, false)
{
    m_pParam = std::make_shared<CBinaryToGraphicsParam>(*pParam);
    addOutput(std::make_shared<CGraphicsProcessOutput>());
}

size_t CBinaryToGraphics::getProgressSteps()
{
    return 3;
}

void CBinaryToGraphics::run()
{
    beginTaskRun();

    auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));

    if(pInput == nullptr)
        throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

    if(pInput->isDataAvailable() == false)
        throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

    try
    {
        CMat img;
        CMat imgSrc = pInput->getImage();

        //Source image must be 1 single channel
        if(imgSrc.channels() > 1)
            cv::cvtColor(imgSrc, img, cv::COLOR_BGR2GRAY);
        else
            img = imgSrc;

        auto pGraphicsOutput = std::dynamic_pointer_cast<CGraphicsProcessOutput>(getOutput(1));
        assert(pGraphicsOutput);
        pGraphicsOutput->setNewLayer(getName());
        emit m_signalHandler->doProgress();

        if( pInput->getDataType() == IODataType::IMAGE_LABEL ||
            pInput->getDataType() == IODataType::VIDEO_LABEL ||
            pInput->getDataType() == IODataType::LIVE_STREAM_LABEL)
        {
            computeLabel(img);
        }
        else
            computeBinary(img);

        emit m_signalHandler->doProgress();
    }
    catch(std::exception& e)
    {
        throw CException(CoreExCode::INVALID_PARAMETER, e.what(), __func__, __FILE__, __LINE__);
    }

    forwardInputImage(0, 0);
    endTaskRun();
    emit m_signalHandler->doProgress();
}

void CBinaryToGraphics::computeBinary(const CMat &src)
{
    CMat tmp;
    if(src.depth() != CV_8U)
    {
        //Convert to 32 bits integer
        src.convertTo(tmp, CV_32SC1);
        assert(tmp.type() == CV_32SC1);
    }
    else
        tmp = src;

    //Find blobs
    m_blobs.clear();
    m_hierarchy.clear();
    cv::findContours(tmp, m_blobs, m_hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_TC89_L1);

    //Create associated graphics
    GraphicsPolygonProperty prop;
    if(m_graphicsContextPtr)
        prop = m_graphicsContextPtr->getPolygonProperty();

    computeGraphics(prop.m_penColor, prop.m_brushColor, prop.m_lineSize);
}

void CBinaryToGraphics::computeLabel(const CMat &src)
{
    double minVal, maxVal;
    cv::minMaxLoc(src, &minVal, &maxVal);

    //Get graphics property : polygon
    GraphicsPolygonProperty prop;
    if(m_graphicsContextPtr)
        prop = m_graphicsContextPtr->getPolygonProperty();

    int labelIndex = 1;
    std::string label = prop.m_category;

    //For each label
    for(size_t i=1; i<=maxVal; ++i)
    {
        //Extract current label image as binary
        CMat imgTmp;
        imgTmp = (src == i);

        //Find blobs
        cv::findContours(imgTmp, m_blobs, m_hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_TC89_L1);
        if(m_blobs.size() > 0)
        {
            //Create associated graphics
            prop.m_category = label + std::to_string(labelIndex);
            CColor penColor = Utils::Graphics::getRandomCColor();
            CColor brushColor = penColor;
            computeGraphics(penColor, brushColor, prop.m_lineSize);
            labelIndex++;
            m_blobs.clear();
            m_hierarchy.clear();
        }
    }
}

void CBinaryToGraphics::computeGraphics(const CColor& penColor, const CColor& brushColor, const int lineSize)
{
    auto pOutput = std::dynamic_pointer_cast<CGraphicsProcessOutput>(getOutput(1));
    assert(pOutput);
    CGraphicsConversion convert;
    auto graphics = convert.blobsToProxyGraphics(m_blobs, m_hierarchy, penColor, brushColor, lineSize);

    for(size_t i=0; i<graphics.size(); ++i)
        pOutput->addItem(graphics[i]);

    auto pInputImgOverlay = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
    assert(pInputImgOverlay);

    if(pInputImgOverlay->isDataAvailable())
        pOutput->setImageIndex(0);
}
