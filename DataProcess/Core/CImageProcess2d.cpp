#include "CImageProcess2d.h"
#include "DataProcessTools.hpp"
#include "IO/CVideoProcessIO.h"
#include "Graphics/CGraphicsConversion.h"
#include "Graphics/CGraphicsLayer.h"

CImageProcess2d::CImageProcess2d() : CProtocolTask()
{
    m_type = CProtocolTask::Type::IMAGE_PROCESS_2D;
    addInput(std::make_shared<CImageProcessIO>());
    addInput(std::make_shared<CGraphicsProcessInput>());
    addOutput(std::make_shared<CImageProcessIO>());
}

CImageProcess2d::CImageProcess2d(bool bEnableGraphics) : CProtocolTask()
{
    m_type = CProtocolTask::Type::IMAGE_PROCESS_2D;
    m_bEnableGraphics = bEnableGraphics;
    addInput(std::make_shared<CImageProcessIO>());

    if(m_bEnableGraphics)
        addInput(std::make_shared<CGraphicsProcessInput>());

    addOutput(std::make_shared<CImageProcessIO>());
}

CImageProcess2d::CImageProcess2d(const std::string &name) : CProtocolTask(name)
{
    m_type = CProtocolTask::Type::IMAGE_PROCESS_2D;
    addInput(std::make_shared<CImageProcessIO>());
    addInput(std::make_shared<CGraphicsProcessInput>());
    addOutput(std::make_shared<CImageProcessIO>());
}

CImageProcess2d::CImageProcess2d(const std::string& name, bool bEnableGraphics) : CProtocolTask(name)
{
    m_type = CProtocolTask::Type::IMAGE_PROCESS_2D;
    m_bEnableGraphics = bEnableGraphics;
    addInput(std::make_shared<CImageProcessIO>());
    if(m_bEnableGraphics)
        addInput(std::make_shared<CGraphicsProcessInput>());
    addOutput(std::make_shared<CImageProcessIO>());
}

CImageProcess2d::~CImageProcess2d()
{
}

void CImageProcess2d::setActive(bool bActive)
{
    CProtocolTask::setActive(bActive);
}

void CImageProcess2d::setOutputColorMap(size_t index, size_t maskIndex, const std::vector<cv::Vec3b> &colors)
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

void CImageProcess2d::updateStaticOutputs()
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
        auto it = m_actionFlags.find(CProtocolTask::ActionFlag::APPLY_VOLUME);
        if(it == m_actionFlags.end())
            setActionFlag(CProtocolTask::ActionFlag::APPLY_VOLUME, false);

        bool bOutputVolume = isActionFlagEnable(CProtocolTask::ActionFlag::APPLY_VOLUME);

        for(size_t i=0; i<getOutputCount(); ++i)
        {
            auto dataType = getOutputDataType(i);
            if((dataType == IODataType::IMAGE ||
               dataType == IODataType::VIDEO ||
               dataType == IODataType::LIVE_STREAM) && bOutputVolume == true)
            {
                setOutput(std::make_shared<CImageProcessIO>(IODataType::VOLUME), i);
            }
            else if((dataType == IODataType::IMAGE_BINARY ||
                     dataType == IODataType::VIDEO_BINARY ||
                     dataType == IODataType::LIVE_STREAM_BINARY) && bOutputVolume == true)
            {
                setOutput(std::make_shared<CImageProcessIO>(IODataType::VOLUME_BINARY), i);
            }
            else if((dataType == IODataType::IMAGE_LABEL ||
                     dataType == IODataType::VIDEO_LABEL ||
                     dataType == IODataType::LIVE_STREAM_LABEL) && bOutputVolume == true)
            {
                setOutput(std::make_shared<CImageProcessIO>(IODataType::VOLUME_LABEL), i);
            }
            else if((dataType == IODataType::VOLUME ||
                     dataType == IODataType::VIDEO ||
                     dataType == IODataType::LIVE_STREAM) && bOutputVolume == false)
            {
                setOutput(std::make_shared<CImageProcessIO>(), i);
            }
            else if((dataType == IODataType::VOLUME_BINARY ||
                     dataType == IODataType::VIDEO_BINARY ||
                     dataType == IODataType::LIVE_STREAM_BINARY) && bOutputVolume == false)
            {
                setOutput(std::make_shared<CImageProcessIO>(IODataType::IMAGE_BINARY), i);
            }
            else if((dataType == IODataType::VOLUME_LABEL ||
                    dataType == IODataType::VIDEO_LABEL ||
                    dataType == IODataType::LIVE_STREAM_LABEL) && bOutputVolume == false)
            {
                setOutput(std::make_shared<CImageProcessIO>(IODataType::IMAGE_LABEL), i);
            }
        }
    }
    else if(bInputVideo == true)
    {
        removeActionFlag(CProtocolTask::ActionFlag::APPLY_VOLUME);

        for(size_t i=0; i<getOutputCount(); ++i)
        {
            auto dataType = getOutputDataType(i);
            if(dataType == IODataType::IMAGE ||
                dataType == IODataType::LIVE_STREAM ||
                dataType == IODataType::VOLUME)
            {
                setOutput(std::make_shared<CVideoProcessIO>(), i);
            }
            else if(dataType == IODataType::IMAGE_BINARY ||
                     dataType == IODataType::LIVE_STREAM_BINARY ||
                     dataType == IODataType::VOLUME_BINARY)
            {
                setOutput(std::make_shared<CVideoProcessIO>(IODataType::VIDEO_BINARY), i);
            }
            else if(dataType == IODataType::IMAGE_LABEL ||
                     dataType == IODataType::LIVE_STREAM_LABEL ||
                     dataType == IODataType::VOLUME_LABEL)
            {
                setOutput(std::make_shared<CVideoProcessIO>(IODataType::VIDEO_LABEL), i);
            }
        }
    }
    else if(bInputStream == true)
    {
        // Remove all action flags
        removeActionFlag(CProtocolTask::ActionFlag::APPLY_VOLUME);

        // If input data type is a video or image sequence
        for(size_t i=0; i<getOutputCount(); ++i)
        {
            auto dataType = getOutputDataType(i);
            if(dataType == IODataType::IMAGE ||
               dataType == IODataType::VIDEO ||
               dataType == IODataType::VOLUME)
            {
                setOutput(std::make_shared<CVideoProcessIO>(IODataType::LIVE_STREAM), i);
            }
            else if(dataType == IODataType::IMAGE_BINARY ||
                    dataType == IODataType::VIDEO_BINARY ||
                    dataType == IODataType::VOLUME_BINARY)
            {
                setOutput(std::make_shared<CVideoProcessIO>(IODataType::LIVE_STREAM_BINARY), i);
            }
            else if(dataType == IODataType::IMAGE_LABEL ||
                    dataType == IODataType::VIDEO_LABEL ||
                    dataType == IODataType::VOLUME_LABEL)
            {
                setOutput(std::make_shared<CVideoProcessIO>(IODataType::LIVE_STREAM_LABEL), i);
            }
        }
    }
    else
    {
        // No input volume
        // No input video
        // No input stream

        // Remove all action flags
        removeActionFlag(CProtocolTask::ActionFlag::APPLY_VOLUME);

        // Restore correct IMAGE datatype
        for(size_t i=0; i<getOutputCount(); ++i)
        {
            if(getOutputDataType(i) == IODataType::VOLUME ||
               getOutputDataType(i) == IODataType::VIDEO ||
               getOutputDataType(i) == IODataType::LIVE_STREAM)
            {
                setOutput(std::make_shared<CImageProcessIO>(), i);
            }
            else if(getOutputDataType(i) == IODataType::VOLUME_BINARY ||
                    getOutputDataType(i) == IODataType::VIDEO_BINARY ||
                    getOutputDataType(i) == IODataType::LIVE_STREAM_BINARY)
            {
                setOutput(std::make_shared<CImageProcessIO>(IODataType::IMAGE_BINARY), i);
            }
            else if(getOutputDataType(i) == IODataType::VOLUME_LABEL ||
                    getOutputDataType(i) == IODataType::VIDEO_LABEL ||
                    getOutputDataType(i) == IODataType::LIVE_STREAM_LABEL)
            {
                setOutput(std::make_shared<CImageProcessIO>(IODataType::IMAGE_LABEL), i);
            }
        }
    }
}

void CImageProcess2d::beginTaskRun()
{
    CProtocolTask::beginTaskRun();
    m_graphicsMasks.clear();
}

void CImageProcess2d::endTaskRun()
{
    CProtocolTask::endTaskRun();
    createOverlayMasks();
}

void CImageProcess2d::graphicsChanged()
{
    CProtocolTask::graphicsChanged();
}

void CImageProcess2d::globalInputChanged(bool bNewSequence)
{
    CProtocolTask::globalInputChanged(bNewSequence);
}

void CImageProcess2d::createGraphicsMask(size_t width, size_t height, const GraphicsProcessInputPtr &pGraphicsInput)
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

void CImageProcess2d::applyGraphicsMask(const CMat& src, CMat& dst, size_t maskIndex)
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

void CImageProcess2d::applyGraphicsMaskToBinary(const CMat& src, CMat& dst, size_t maskIndex)
{
    if(maskIndex >= m_graphicsMasks.size())
        return;

    cv::bitwise_and(src, m_graphicsMasks[maskIndex], dst);
}

void CImageProcess2d::applyInputGraphicsMask(int graphicsIndex, int inputImgIndex, int outputImgIndex, MaskMode mode)
{
    auto imgInputPtr = std::dynamic_pointer_cast<CImageProcessIO>(getInput(inputImgIndex));
    if(imgInputPtr == nullptr)
        throw CException(CoreExCode::INVALID_PARAMETER, "Invalid image input index", __func__, __FILE__, __LINE__);

    auto imgOutputPtr = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(outputImgIndex));
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

CMat CImageProcess2d::getGraphicsMask(size_t index) const
{
    if(index < m_graphicsMasks.size())
        return m_graphicsMasks[index];
    else
        return CMat();
}

bool CImageProcess2d::isMaskAvailable(size_t index) const
{
    if(index >= m_graphicsMasks.size())
        return false;
    else
        return m_graphicsMasks[index].data != nullptr;
}

void CImageProcess2d::forwardInputImage(int inputIndex, int outputIndex)
{
    if(getOutputCount() == 0)
        throw CException(CoreExCode::INVALID_SIZE, "Wrong outputs count", __func__, __FILE__, __LINE__);

    auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(inputIndex));
    auto pOutput = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(outputIndex));

    if(pInput && pOutput)
        pOutput->setImage(pInput->getImage());
}

CMat CImageProcess2d::createInputGraphicsMask(int index, int width, int height)
{
    auto graphicsInputPtr = std::dynamic_pointer_cast<CGraphicsProcessInput>(getInput(index));
    if(graphicsInputPtr == nullptr)
        return CMat();

    if(graphicsInputPtr->isDataAvailable() == false)
        return CMat();

    //Generate graphics masks
    CGraphicsConversion graphicsConv(width, height);
    auto graphicsItems = graphicsInputPtr->getItems();
    return graphicsConv.graphicsToBinaryMask(graphicsItems);
}

void CImageProcess2d::createOverlayMasks()
{
    for(size_t i=0; i<getOutputCount(); ++i)
    {
        if(i < m_colorMaps.size() && m_colorMaps[i].empty() == false)
        {
            auto pOutput = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(i));
            auto pOutputMask = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(m_colorMapMaskIndices[i]));

            if(pOutput && pOutputMask)
            {
                auto mask = Utils::Image::createOverlayMask(pOutputMask->getImage(), m_colorMaps[i]);
                pOutput->setOverlayMask(mask);
            }
        }
    }
}
