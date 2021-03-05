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

#include "CVideoProcessIO.h"
#include "Data/CDataVideoInfo.h"
#include "CDataVideoIO.h"
#include "UtilsTools.hpp"

CVideoProcessIO::CVideoProcessIO() : CImageProcessIO(IODataType::VIDEO)
{
    m_description = QObject::tr("Video with read/write capabilities.").toStdString();
    m_saveFormat = DataFileFormat::AVI;
}

CVideoProcessIO::CVideoProcessIO(CMat image) : CImageProcessIO(IODataType::VIDEO, image)
{
    m_description = QObject::tr("Video with read/write capabilities.").toStdString();
    m_saveFormat = DataFileFormat::AVI;
}

CVideoProcessIO::CVideoProcessIO(IODataType data) : CImageProcessIO(data)
{
    m_description = QObject::tr("Video with read/write capabilities.").toStdString();
    m_saveFormat = DataFileFormat::AVI;
}

CVideoProcessIO::CVideoProcessIO(IODataType data, const CMat& image) : CImageProcessIO(data, image)
{
    m_description = QObject::tr("Video with read/write capabilities.").toStdString();
    m_saveFormat = DataFileFormat::AVI;
}

CVideoProcessIO::CVideoProcessIO(const CVideoProcessIO &io) : CImageProcessIO(io)
{
}

CVideoProcessIO::CVideoProcessIO(const CVideoProcessIO&& io) : CImageProcessIO(io)
{
}

CVideoProcessIO &CVideoProcessIO::operator=(const CVideoProcessIO& io)
{
    CImageProcessIO::operator=(io);
    return *this;
}

CVideoProcessIO &CVideoProcessIO::operator=(const CVideoProcessIO&& io)
{
    CImageProcessIO::operator=(io);
    return *this;
}

void CVideoProcessIO::setVideoPath(const std::string& path)
{
    std::string extension = Utils::File::extension(path);
    if(CDataVideoIO::isVideoFormat(extension, true))
    {
        // Videos
        m_pVideoBuffer = std::make_unique<CDataVideoBuffer>(path);
    }
    else
    {
        // Image sequence
        auto ret = CDataVideoIO::getImageSequenceInfo(path);
        m_pVideoBuffer = std::make_unique<CDataVideoBuffer>(ret.first, ret.second);
    }
}

void CVideoProcessIO::setVideoPos(int pos)
{
    if(m_pVideoBuffer)
        m_pVideoBuffer->setPosition(pos);
    else
        throw CException(CoreExCode::NULL_POINTER, QObject::tr("Video buffer pointer is null.").toStdString(), __func__, __FILE__, __LINE__);
}

void CVideoProcessIO::startVideo()
{
    if(m_pVideoBuffer)
        m_pVideoBuffer->startRead();
    else
        throw CException(CoreExCode::NULL_POINTER, QObject::tr("Video buffer pointer is null.").toStdString(), __func__, __FILE__, __LINE__);
}

void CVideoProcessIO::stopVideo()
{
    if(m_pVideoBuffer)
        m_pVideoBuffer->stopRead();
    else
        throw CException(CoreExCode::NULL_POINTER, QObject::tr("Video buffer pointer is null.").toStdString(), __func__, __FILE__, __LINE__);
}

void CVideoProcessIO::startVideoWrite(int width, int height, int frames, int fps, int fourcc)
{
    if(m_pVideoBuffer)
        m_pVideoBuffer->startWrite(width, height, frames, fps, fourcc);
    else
        throw CException(CoreExCode::NULL_POINTER, QObject::tr("Video buffer pointer is null.").toStdString(), __func__, __FILE__, __LINE__);
}

void CVideoProcessIO::stopVideoWrite()
{
    if(m_pVideoBuffer)
        m_pVideoBuffer->stopWrite();
    else
        throw CException(CoreExCode::NULL_POINTER, QObject::tr("Video buffer pointer is null.").toStdString(), __func__, __FILE__, __LINE__);
}

void CVideoProcessIO::addVideoImage(const CMat& image)
{
    m_videoImgList.push_back(image);
}

void CVideoProcessIO::writeImage(CMat image)
{
    assert(m_pVideoBuffer);
    CMat tmp;
    if(image.channels() == 1)
        cv::cvtColor(image, tmp, cv::COLOR_GRAY2BGR);
    else
        cv::cvtColor(image, tmp, cv::COLOR_RGB2BGR);

    m_pVideoBuffer->write(tmp);
}

bool CVideoProcessIO::hasVideo()
{
    return m_pVideoBuffer != nullptr;
}

CMat CVideoProcessIO::getImage() const
{
    if(m_pVideoBuffer && m_pVideoBuffer->isReadStarted())
    {
        CMat image = m_pVideoBuffer->read();
        return image;
    }
    else
        return m_image;
}

size_t CVideoProcessIO::getVideoFrameCount() const
{
    if(m_pVideoBuffer)
        return m_pVideoBuffer->getFrameCount();
    else
        return 0;
}

std::vector<CMat> CVideoProcessIO::getVideoImages() const
{
    return m_videoImgList;
}

std::string CVideoProcessIO::getVideoPath() const
{
    if(!m_pVideoBuffer)
        return "";
    else
        return m_pVideoBuffer->getCurrentPath();
}

CMat CVideoProcessIO::getSnapshot(int pos)
{
    assert(m_pVideoBuffer);
    return m_pVideoBuffer->snapshot(pos);
}

int CVideoProcessIO::getCurrentPos() const
{
    assert(m_pVideoBuffer);
    return m_pVideoBuffer->getCurrentPos();
}

CMat CVideoProcessIO::getStaticImage() const
{
    return m_image;
}

size_t CVideoProcessIO::getUnitElementCount() const
{
    if(m_pVideoBuffer)
        return m_pVideoBuffer->getFrameCount();
    else
        return 0;
}

CDataInfoPtr CVideoProcessIO::getDataInfo()
{
    if(m_pVideoBuffer)
    {
        m_infoPtr = std::make_shared<CDataVideoInfo>(m_pVideoBuffer->getFPS(),
                                                     m_pVideoBuffer->getFrameCount(),
                                                     m_pVideoBuffer->getCurrentPos(),
                                                     m_pVideoBuffer->getCodec());
        auto infoPtr = std::static_pointer_cast<CDataVideoInfo>(m_infoPtr);
        infoPtr->setFileName(m_pVideoBuffer->getCurrentPath());
        infoPtr->m_width = m_pVideoBuffer->getWidth();
        infoPtr->m_height = m_pVideoBuffer->getHeight();
        infoPtr->m_sourceType = m_pVideoBuffer->getSourceType();
    }
    return m_infoPtr;
}

bool CVideoProcessIO::isDataAvailable() const
{
    bool bRet =  m_image.data != nullptr || m_videoImgList.size() > 0 ;
    if(m_pVideoBuffer)
        bRet = bRet || m_pVideoBuffer->isReadOpened();

    return bRet;
}

void CVideoProcessIO::clearData()
{
    m_image.release();
}

void CVideoProcessIO::waitWriteFinished()
{
    m_pVideoBuffer->waitWriteFinished();
}

void CVideoProcessIO::waitReadImage() const
{
    if(m_pVideoBuffer)
        while(!m_pVideoBuffer->hasReadImage());
}

std::shared_ptr<CVideoProcessIO> CVideoProcessIO::clone() const
{
    return std::static_pointer_cast<CVideoProcessIO>(cloneImp());
}

void CVideoProcessIO::save()
{
    m_tempFiles.clear();
}

void CVideoProcessIO::save(const std::string &path)
{
    Q_UNUSED(path);
}

std::shared_ptr<CProtocolTaskIO> CVideoProcessIO::cloneImp() const
{
    return std::shared_ptr<CVideoProcessIO>(new CVideoProcessIO(*this));
}
