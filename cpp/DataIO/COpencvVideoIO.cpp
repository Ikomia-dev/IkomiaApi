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

#include "COpencvVideoIO.h"
#include "UtilsTools.hpp"
#include "Data/CDataVideoInfo.h"

COpencvVideoIO::COpencvVideoIO(const std::string &fileName) : CVirtualVideoIO(fileName)
{
    m_pVideoBuffer = std::make_unique<CDataVideoBuffer>(fileName);
    if(m_pVideoBuffer->isReadMode())
    {
        // Default resolution of the frame is obtained.The default resolution is system dependent.
        m_width = m_pVideoBuffer->getWidth();
        m_height = m_pVideoBuffer->getHeight();
        // Default frame per second rate
        m_fps = m_pVideoBuffer->getFPS();
        // Total frames
        if(!m_fileName.empty())
            m_frameCount = m_pVideoBuffer->getFrameCount();
        // Default video compress mode format
        m_fourcc = m_pVideoBuffer->getCodec();
    }
}

COpencvVideoIO::COpencvVideoIO(const std::string &fileName, int frameCount) : CVirtualVideoIO(fileName)
{
    m_pVideoBuffer = std::make_unique<CDataVideoBuffer>(fileName, frameCount);
    if(m_pVideoBuffer->isReadMode())
    {
        // Default resolution of the frame is obtained.The default resolution is system dependent.
        m_width = m_pVideoBuffer->getWidth();
        m_height = m_pVideoBuffer->getHeight();
        // Default frame per second rate
        m_fps = m_pVideoBuffer->getFPS();
        // Total frames
        if(!m_fileName.empty())
            m_frameCount = m_pVideoBuffer->getFrameCount();
        // Default video compress mode format
        m_fourcc = m_pVideoBuffer->getCodec();
    }
}

COpencvVideoIO::~COpencvVideoIO()
{
}

VectorString COpencvVideoIO::fileNames(const SubsetBounds &bounds)
{
    Q_UNUSED(bounds);
    VectorString files = {m_fileName};
    return files;
}

CMat COpencvVideoIO::read()
{
    CMat frame = m_pVideoBuffer->snapshot();

    m_imgCvType = frame.type();
    m_imgWidth = frame.cols;
    m_imgHeight = frame.rows;

    if(frame.channels() > 1)
        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);

    return frame;
}

CMat COpencvVideoIO::read(const SubsetBounds &subset)
{
    if(m_frameCount > 0)
    {
        for(size_t i=0; i<subset.size(); ++i)
        {
            if(subset[i].first == DataDimension::IMAGE)
                m_pVideoBuffer->setPosition((int)subset[i].second.first);
        }
    }

    CMat frame = m_pVideoBuffer->snapshot();
    m_imgCvType = frame.type();
    m_imgWidth = frame.cols;
    m_imgHeight = frame.rows;

    if(frame.channels() > 1)
        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);

    cv::Rect roi;
    for(size_t i=0; i<subset.size(); ++i)
    {
        if(subset[i].first == DataDimension::X)
        {
            roi.x = static_cast<int>(subset[i].second.first);
            roi.width = static_cast<int>(subset[i].second.second - subset[i].second.first);
        }
        else if(subset[i].first == DataDimension::Y)
        {
            roi.y = static_cast<int>(subset[i].second.first);
            roi.height = static_cast<int>(subset[i].second.second - subset[i].second.first);
        }
    }

    if(roi.width == 0 && roi.height == 0)
        return frame;
    else
    {
        if(roi.width == 0)
            roi.width = frame.cols;
        if(roi.height == 0)
            roi.height = frame.rows;

        m_imgWidth = roi.width;
        m_imgHeight = roi.height;

        return frame(roi);
    }
}

CMat COpencvVideoIO::readLive()
{
    if(!m_pVideoBuffer->isReadStarted())
    {
        m_pVideoBuffer->clearRead();
        m_pVideoBuffer->startRead();
    }

    CMat frame = m_pVideoBuffer->read();
    m_imgCvType = frame.type();
    m_imgWidth = frame.cols;
    m_imgHeight = frame.rows;
    return frame;
}

CMat COpencvVideoIO::readLive(const SubsetBounds& subset)
{
    if(!m_pVideoBuffer->isReadStarted())
        m_pVideoBuffer->startRead();

    cv::Rect roi;
    CMat frame = m_pVideoBuffer->read();

    m_imgCvType = frame.type();
    m_imgWidth = frame.cols;
    m_imgHeight = frame.rows;

    for(size_t i=0; i<subset.size(); ++i)
    {
        if(subset[i].first == DataDimension::X)
        {
            roi.x = static_cast<int>(subset[i].second.first);
            roi.width = static_cast<int>(subset[i].second.second - subset[i].second.first);
        }
        else if(subset[i].first == DataDimension::Y)
        {
            roi.y = static_cast<int>(subset[i].second.first);
            roi.height = static_cast<int>(subset[i].second.second - subset[i].second.first);
        }
    }

    if(roi.width == 0 && roi.height == 0)
        return frame;
    else
    {
        if(roi.width == 0)
            roi.width = frame.cols;
        if(roi.height == 0)
            roi.height = frame.rows;

        m_imgWidth = roi.width;
        m_imgHeight = roi.height;

        return frame(roi);
    }
}

void COpencvVideoIO::write(const CMat &image)
{
    assert(!image.empty());
    assert(m_pVideoBuffer);

    if(!m_pVideoBuffer->isWriteStarted())        
        m_pVideoBuffer->startStreamWrite(image.getNbCols(), image.getNbRows(), 25);

    CMat frame;
    if(image.channels() > 1)
        cv::cvtColor(image, frame, cv::COLOR_BGR2RGB);

    m_pVideoBuffer->write(frame);
}

void COpencvVideoIO::write(const CMat &image, const std::string &path)
{
    assert(!image.empty());
    assert(m_pVideoBuffer);

    if(!m_pVideoBuffer->isWriteStarted())
    {
        m_pVideoBuffer->setVideoPath(path);
        m_pVideoBuffer->startStreamWrite(image.getNbCols(), image.getNbRows(), 25);
    }

    CMat frame;
    if(image.channels() > 1)
        cv::cvtColor(image, frame, cv::COLOR_BGR2RGB);

    m_pVideoBuffer->write(frame);
}

void COpencvVideoIO::stopWrite()
{
    assert(m_pVideoBuffer);
    m_pVideoBuffer->stopWrite();
}

void COpencvVideoIO::stopRead()
{
    assert(m_pVideoBuffer);
    m_pVideoBuffer->stopRead();
}

void COpencvVideoIO::waitWriteFinished()
{
    assert(m_pVideoBuffer);
    m_pVideoBuffer->waitWriteFinished();
}

void COpencvVideoIO::closeCamera()
{
    assert(m_pVideoBuffer);
    m_pVideoBuffer->closeCamera();
}

Dimensions COpencvVideoIO::dimensions()
{
    Dimensions dims = {{DataDimension::TIME, 1}};
    return dims;
}

Dimensions COpencvVideoIO::dimensions(const SubsetBounds &bounds)
{
    Q_UNUSED(bounds);
    return dimensions();
}

CDataInfoPtr COpencvVideoIO::dataInfo()
{
    assert(m_pVideoBuffer);

    // Lire image à faire
    std::shared_ptr<CDataVideoInfo> pInfo = std::make_shared<CDataVideoInfo>();
    pInfo->setFileName(m_fileName);
    pInfo->setCvType(m_imgCvType);
    pInfo->m_sourceType = m_pVideoBuffer->getSourceType();
    pInfo->m_width = m_imgWidth;
    pInfo->m_height = m_imgHeight;
    pInfo->m_fps = m_fps;
    pInfo->m_frameCount = m_frameCount;
    pInfo->m_currentPos = m_pVideoBuffer->getCurrentPos();
    pInfo->m_fourcc = m_fourcc;
    return pInfo;
}

CDataInfoPtr COpencvVideoIO::dataInfo(const SubsetBounds &/*subset*/)
{
    return dataInfo();
}

