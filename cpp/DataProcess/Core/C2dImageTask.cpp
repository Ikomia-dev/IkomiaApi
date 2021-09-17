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

#include "C2dImageTask.h"
#include "DataProcessTools.hpp"
#include "IO/CVideoIO.h"
#include "Graphics/CGraphicsConversion.h"
#include "Graphics/CGraphicsLayer.h"
#include "IO/CConvertIO.h"
#include "Data/CDataImageInfo.h"

C2dImageTask::C2dImageTask() : CWorkflowTask()
{
    m_type = CWorkflowTask::Type::IMAGE_PROCESS_2D;
    addInput(std::make_shared<CImageIO>());
    addInput(std::make_shared<CGraphicsInput>());
    addOutput(std::make_shared<CImageIO>());
}

C2dImageTask::C2dImageTask(bool bEnableGraphics) : CWorkflowTask()
{
    m_type = CWorkflowTask::Type::IMAGE_PROCESS_2D;
    m_bEnableGraphics = bEnableGraphics;
    addInput(std::make_shared<CImageIO>());

    if(m_bEnableGraphics)
        addInput(std::make_shared<CGraphicsInput>());

    addOutput(std::make_shared<CImageIO>());
}

C2dImageTask::C2dImageTask(const std::string &name) : CWorkflowTask(name)
{
    m_type = CWorkflowTask::Type::IMAGE_PROCESS_2D;
    addInput(std::make_shared<CImageIO>());
    addInput(std::make_shared<CGraphicsInput>());
    addOutput(std::make_shared<CImageIO>());
}

C2dImageTask::C2dImageTask(const std::string& name, bool bEnableGraphics) : CWorkflowTask(name)
{
    m_type = CWorkflowTask::Type::IMAGE_PROCESS_2D;
    m_bEnableGraphics = bEnableGraphics;
    addInput(std::make_shared<CImageIO>());
    if(m_bEnableGraphics)
        addInput(std::make_shared<CGraphicsInput>());
    addOutput(std::make_shared<CImageIO>());
}

C2dImageTask::~C2dImageTask()
{
}

void C2dImageTask::setActive(bool bActive)
{
    CWorkflowTask::setActive(bActive);
}

void C2dImageTask::setOutputColorMap(size_t index, size_t maskIndex, const std::vector<cv::Vec3b> &colors)
{
    if(index > getOutputCount() - 1)
        throw CException(CoreExCode::INVALID_SIZE, "Invalid output index", __func__, __FILE__, __LINE__);

    if(index >= m_colorMaps.size())
    {
        m_colorMaps.resize(index + 1);
        m_colorMapMaskIndices.resize(index + 1);
    }

    CMat colormap(256, 1, CV_8UC3);
    if(colors.size() == 0)
    {
        //Random colors
        std::srand(std::time(nullptr));
        for(int i=(int)colors.size(); i<256; ++i)
        {
            for(int j=0; j<3; ++j)
                colormap.at<cv::Vec3b>(i, 0)[j] = (uchar)((double)std::rand() / (double)RAND_MAX * 255.0);
        }
    }
    else if(colors.size() == 1)
    {
        colormap = cv::Mat::zeros(colormap.rows, colormap.cols, CV_8UC3);
        colormap.at<cv::Vec3b>(255, 0) = colors[0];
    }
    else
    {        
        for(int i=0; i<std::min<int>(256, (int)colors.size()); ++i)
            colormap.at<cv::Vec3b>(i, 0) = colors[i];

        for(int i=(int)colors.size(); i<256; ++i)
            colormap.at<cv::Vec3b>(i, 0) = {(uchar)i, (uchar)i, (uchar)i};
    }
    m_colorMaps[index] = colormap;
    m_colorMapMaskIndices[index] = maskIndex;
}

void C2dImageTask::updateStaticOutputs()
{
    bool bInputVolume = false;
    bool bInputVideo = false;
    bool bInputStream = false;

    for(size_t i=0; i<getInputCount(); ++i)
    {
        if(getInput(i) == nullptr)
            continue;

        if( getInputDataType(i) == IODataType::VOLUME ||
            getInputDataType(i) == IODataType::VOLUME_BINARY ||
            getInputDataType(i) == IODataType::VOLUME_LABEL)
        {
            bInputVolume = true;
            break;
        }
        else if(getInputDataType(i) == IODataType::VIDEO ||
                getInputDataType(i) == IODataType::VIDEO_BINARY ||
                getInputDataType(i) == IODataType::VIDEO_LABEL)
        {
            // Find input video data type
            bInputVideo = true;
            break;
        }
        else if(getInputDataType(i) == IODataType::LIVE_STREAM ||
                getInputDataType(i) == IODataType::LIVE_STREAM_BINARY ||
                getInputDataType(i) == IODataType::LIVE_STREAM_LABEL)
        {
            // Find input video data type
            bInputStream = true;
            break;
        }
    }

    if(bInputVolume == true)
    {
        auto it = m_actionFlags.find(CWorkflowTask::ActionFlag::APPLY_VOLUME);
        if(it == m_actionFlags.end())
            setActionFlag(CWorkflowTask::ActionFlag::APPLY_VOLUME, false);

        bool bOutputVolume = isActionFlagEnable(CWorkflowTask::ActionFlag::APPLY_VOLUME);

        for(size_t i=0; i<getOutputCount(); ++i)
        {
            if(bOutputVolume)
            {
                auto convertedIO = CConvertIO::convertToVolumeIO(getOutput(i));
                if(convertedIO)
                    setOutput(convertedIO, i);
            }
            else
            {
                auto convertedIO = CConvertIO::convertToImageIO(getOutput(i));
                if(convertedIO)
                    setOutput(convertedIO, i);
            }
        }
    }
    else if(bInputVideo == true)
    {
        removeActionFlag(CWorkflowTask::ActionFlag::APPLY_VOLUME);

        for(size_t i=0; i<getOutputCount(); ++i)
        {
            auto convertedIO = CConvertIO::convertToVideoIO(getOutput(i));
            if(convertedIO)
                setOutput(convertedIO, i);
        }
    }
    else if(bInputStream == true)
    {
        // Remove all action flags
        removeActionFlag(CWorkflowTask::ActionFlag::APPLY_VOLUME);

        // If input data type is a video or image sequence
        for(size_t i=0; i<getOutputCount(); ++i)
        {
            auto convertedIO = CConvertIO::convertToStreamIO(getOutput(i));
            if(convertedIO)
                setOutput(convertedIO, i);
        }
    }
    else
    {
        // No input volume
        // No input video
        // No input stream

        // Remove all action flags
        removeActionFlag(CWorkflowTask::ActionFlag::APPLY_VOLUME);

        // Restore correct IMAGE datatype
        for(size_t i=0; i<getOutputCount(); ++i)
        {
            auto convertedIO = CConvertIO::convertToImageIO(getOutput(i));
            if(convertedIO)
                setOutput(convertedIO, i);
        }
    }
}

void C2dImageTask::beginTaskRun()
{
    CWorkflowTask::beginTaskRun();
    m_graphicsMasks.clear();
}

void C2dImageTask::endTaskRun()
{
    CWorkflowTask::endTaskRun();
    createOverlayMasks();

    // Forward input image information
    auto imgInputPtr = getInput(0);
    auto imgOutputPtr = getOutput(0);

    if(imgInputPtr && imgOutputPtr)
    {
        auto inputInfo = std::dynamic_pointer_cast<CDataImageInfo>(imgInputPtr->getDataInfo());
        auto outputInfo = std::dynamic_pointer_cast<CDataImageInfo>(imgOutputPtr->getDataInfo());

        if(inputInfo && outputInfo)
            outputInfo->setFileName(inputInfo->getFileName());
    }
}

void C2dImageTask::graphicsChanged()
{
    CWorkflowTask::graphicsChanged();
}

void C2dImageTask::globalInputChanged(bool bNewSequence)
{
    CWorkflowTask::globalInputChanged(bNewSequence);
}

void C2dImageTask::createGraphicsMask(size_t width, size_t height, const GraphicsInputPtr &pGraphicsInput)
{
    if(pGraphicsInput == nullptr)
        return;

    if(pGraphicsInput->isDataAvailable() == false)
        return;

    //Generate graphics masks
    CGraphicsConversion graphicsConv((int)width, (int)height);

    auto graphicsItems = pGraphicsInput->getItems();
    CMat mask = graphicsConv.graphicsToBinaryMask(graphicsItems);
    m_graphicsMasks.push_back(mask);
}

void C2dImageTask::applyGraphicsMask(const CMat& src, CMat& dst, size_t maskIndex)
{
    if(maskIndex >= m_graphicsMasks.size())
        return;

    CMat srcTmp = src;
    if(src.channels() > dst.channels())
        cv::cvtColor(dst, dst, cv::COLOR_GRAY2RGB);
    else if(src.channels() < dst.channels())
        cv::cvtColor(srcTmp, srcTmp, cv::COLOR_GRAY2RGB);

    CMat invertedMask;
    cv::bitwise_not(m_graphicsMasks[maskIndex], invertedMask);
    srcTmp.copyTo(dst, invertedMask);
}

void C2dImageTask::applyGraphicsMaskToBinary(const CMat& src, CMat& dst, size_t maskIndex)
{
    if(maskIndex >= m_graphicsMasks.size())
        return;

    cv::bitwise_and(src, m_graphicsMasks[maskIndex], dst);
}

void C2dImageTask::applyInputGraphicsMask(int graphicsIndex, int inputImgIndex, int outputImgIndex, MaskMode mode)
{
    auto imgInputPtr = std::dynamic_pointer_cast<CImageIO>(getInput(inputImgIndex));
    if(imgInputPtr == nullptr)
        throw CException(CoreExCode::INVALID_PARAMETER, "Invalid image input index", __func__, __FILE__, __LINE__);

    auto imgOutputPtr = std::dynamic_pointer_cast<CImageIO>(getOutput(outputImgIndex));
    if(imgInputPtr == nullptr && imgOutputPtr == nullptr)
        throw CException(CoreExCode::INVALID_PARAMETER, "Invalid image output index", __func__, __FILE__, __LINE__);

    CMat src = imgInputPtr->getImage();
    CMat dst = imgOutputPtr->getImage();

    auto mask = createInputGraphicsMask(graphicsIndex, src.getNbCols(), src.getNbRows());
    if(mask.empty())
        return;

    if(mode == MaskMode::MERGE_SOURCE)
    {
        CMat srcTmp = src;
        if(src.channels() > dst.channels())
            cv::cvtColor(dst, dst, cv::COLOR_GRAY2RGB);
        else if(src.channels() < dst.channels())
            cv::cvtColor(srcTmp, srcTmp, cv::COLOR_GRAY2RGB);

        CMat invertedMask;
        cv::bitwise_not(mask, invertedMask);
        srcTmp.copyTo(dst, invertedMask);
    }
    else if(mode == MaskMode::MASK_ONLY)
    {
        if(dst.channels() > mask.channels())
            cv::cvtColor(mask, mask, cv::COLOR_GRAY2RGB);

        if(dst.depth() != mask.depth())
            mask.convertTo(mask, dst.depth());

        cv::bitwise_and(dst, mask, dst);
    }
}

CMat C2dImageTask::getGraphicsMask(size_t index) const
{
    if(index < m_graphicsMasks.size())
        return m_graphicsMasks[index];
    else
        return CMat();
}

bool C2dImageTask::isMaskAvailable(size_t index) const
{
    if(index >= m_graphicsMasks.size())
        return false;
    else
        return m_graphicsMasks[index].data != nullptr;
}

void C2dImageTask::forwardInputImage(int inputIndex, int outputIndex)
{
    if(getOutputCount() == 0)
        throw CException(CoreExCode::INVALID_SIZE, "Wrong outputs count", __func__, __FILE__, __LINE__);

    auto pInput = std::dynamic_pointer_cast<CImageIO>(getInput(inputIndex));
    auto pOutput = std::dynamic_pointer_cast<CImageIO>(getOutput(outputIndex));

    if(pInput && pOutput)
        pOutput->setImage(pInput->getImage());
}

CMat C2dImageTask::createInputGraphicsMask(int index, int width, int height)
{
    auto graphicsInputPtr = std::dynamic_pointer_cast<CGraphicsInput>(getInput(index));
    if(graphicsInputPtr == nullptr)
        return CMat();

    if(graphicsInputPtr->isDataAvailable() == false)
        return CMat();

    //Generate graphics masks
    CGraphicsConversion graphicsConv(width, height);
    auto graphicsItems = graphicsInputPtr->getItems();
    return graphicsConv.graphicsToBinaryMask(graphicsItems);
}

void C2dImageTask::createOverlayMasks()
{
    for(size_t i=0; i<getOutputCount(); ++i)
    {
        if(i < m_colorMaps.size() && m_colorMaps[i].empty() == false)
        {
            auto pOutput = std::dynamic_pointer_cast<CImageIO>(getOutput(i));
            auto pOutputMask = std::dynamic_pointer_cast<CImageIO>(getOutput(m_colorMapMaskIndices[i]));

            if(pOutput && pOutputMask)
            {
                auto mask = Utils::Image::createOverlayMask(pOutputMask->getImage(), m_colorMaps[i]);
                pOutput->setOverlayMask(mask);
            }
        }
    }
}
