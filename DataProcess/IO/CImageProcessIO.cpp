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

#include "CImageProcessIO.h"
#include "Data/CDataImageInfo.h"
#include "CDataImageIO.h"

CImageProcessIO::CImageProcessIO() : CProtocolTaskIO(IODataType::IMAGE)
{
    m_description = QObject::tr("2D or 3D images.\n"
                                "Can be single frame from video or camera stream.").toStdString();
    m_saveFormat = DataFileFormat::PNG;
}

CImageProcessIO::CImageProcessIO(const CMat &image) : CProtocolTaskIO(IODataType::IMAGE)
{
    m_description = QObject::tr("2D or 3D images.\n"
                                "Can be single frame from video or camera stream.").toStdString();
    m_saveFormat = DataFileFormat::PNG;
    m_image = image;
    m_dimCount = m_image.dims;
    m_channelCount = m_image.channels();
    m_bNewDataInfo = true;
}

CImageProcessIO::CImageProcessIO(IODataType data) : CProtocolTaskIO(data)
{
    m_description = QObject::tr("2D or 3D images.\n"
                                "Can be single frame from video or camera stream.").toStdString();
    m_saveFormat = DataFileFormat::PNG;
}

CImageProcessIO::CImageProcessIO(IODataType data, const CMat &image) : CProtocolTaskIO(data)
{
    m_description = QObject::tr("2D or 3D images.\n"
                                "Can be single frame from video or camera stream.").toStdString();
    m_saveFormat = DataFileFormat::PNG;
    m_image = image;
    m_dimCount = m_image.dims;
    m_channelCount = m_image.channels();
    m_bNewDataInfo = true;
}

CImageProcessIO::CImageProcessIO(const CImageProcessIO &io) : CProtocolTaskIO(io)
{
    m_image = io.m_image;
    m_overlayMask = io.m_overlayMask;
    m_channelCount = io.m_channelCount;
    m_currentIndex = io.m_currentIndex;
    m_bNewDataInfo = true;
}

CImageProcessIO::CImageProcessIO(const CImageProcessIO&& io) : CProtocolTaskIO(io)
{
    m_image = std::move(io.m_image);
    m_overlayMask = std::move(io.m_overlayMask);
    m_channelCount = std::move(io.m_channelCount);
    m_currentIndex = std::move(io.m_currentIndex);
    m_bNewDataInfo = true;
}

CImageProcessIO &CImageProcessIO::operator=(const CImageProcessIO& io)
{
    CProtocolTaskIO::operator=(io);
    m_image = io.m_image;
    m_overlayMask = io.m_overlayMask;
    m_channelCount = io.m_channelCount;
    m_currentIndex = io.m_currentIndex;
    m_bNewDataInfo = true;
    return *this;
}

CImageProcessIO &CImageProcessIO::operator=(const CImageProcessIO&& io)
{
    CProtocolTaskIO::operator=(io);
    m_image = std::move(io.m_image);
    m_overlayMask = std::move(io.m_overlayMask);
    m_channelCount = std::move(io.m_channelCount);
    m_currentIndex = std::move(io.m_currentIndex);
    m_bNewDataInfo = true;
    return *this;
}

void CImageProcessIO::setImage(const CMat &image)
{
    m_image = image;
    m_dimCount = m_image.dims;
    m_channelCount = m_image.channels();
    m_bNewDataInfo = true;
}

void CImageProcessIO::setChannelCount(size_t nb)
{
    m_channelCount = nb;
}

void CImageProcessIO::setCurrentImage(size_t index)
{
    m_currentIndex = index;
}

void CImageProcessIO::setOverlayMask(const CMat &mask)
{
    m_overlayMask = mask;
}

size_t CImageProcessIO::getChannelCount() const
{
    return m_channelCount;
}

CMat CImageProcessIO::getData() const
{
    return m_image;
}

CDataInfoPtr CImageProcessIO::getDataInfo()
{
    if(m_bNewDataInfo)
    {
        m_infoPtr = std::make_shared<CDataImageInfo>(m_image);
        m_bNewDataInfo = false;
    }
    return m_infoPtr;
}

CMat CImageProcessIO::getImage() const
{
    if( m_dataType == IODataType::IMAGE ||
        m_dataType == IODataType::IMAGE_BINARY ||
        m_dataType == IODataType::IMAGE_LABEL ||
        m_dataType == IODataType::VIDEO ||
        m_dataType == IODataType::VIDEO_BINARY ||
        m_dataType == IODataType::VIDEO_LABEL ||
        m_dataType == IODataType::DESCRIPTORS ||
        m_dimCount == 2)
    {
        return m_image;
    }
    else if(m_dataType == IODataType::VOLUME || m_dataType == IODataType::VOLUME_BINARY)
    {
        if(m_currentIndex < m_image.getNbStacks())
            return m_image.getPlane(m_currentIndex);
        else
            return m_image.getPlane(0);
    }
    return CMat();
}

size_t CImageProcessIO::getUnitElementCount() const
{
    if( m_dataType == IODataType::IMAGE ||
        m_dataType == IODataType::IMAGE_BINARY ||
        m_dataType == IODataType::IMAGE_LABEL ||
        m_dataType == IODataType::VIDEO ||
        m_dataType == IODataType::VIDEO_BINARY ||
        m_dataType == IODataType::VIDEO_LABEL)
    {
        return 1;
    }
    else if(m_dataType == IODataType::VOLUME ||
            m_dataType == IODataType::VOLUME_BINARY)
    {
        return m_image.getNbStacks();
    }
    return 0;
}

CMat CImageProcessIO::getOverlayMask() const
{
    return m_overlayMask;
}

std::vector<DataFileFormat> CImageProcessIO::getPossibleSaveFormats() const
{
    std::vector<DataFileFormat> formats = {
        DataFileFormat::PNG, DataFileFormat::JPG, DataFileFormat::TIF,
        DataFileFormat::BMP, DataFileFormat::WEBP, DataFileFormat::JP2
    };
    return formats;
}

bool CImageProcessIO::isDataAvailable() const
{
    if( m_dataType == IODataType::IMAGE ||
        m_dataType == IODataType::IMAGE_BINARY ||
        m_dataType == IODataType::IMAGE_LABEL ||
        m_dataType == IODataType::VOLUME ||
        m_dataType == IODataType::VOLUME_BINARY ||
        m_dataType == IODataType::VIDEO ||
        m_dataType == IODataType::VIDEO_BINARY ||
        m_dataType == IODataType::VIDEO_LABEL ||
        m_dataType == IODataType::DESCRIPTORS)
    {
        return m_image.data != nullptr;
    }
    else
        return false;
}

bool CImageProcessIO::isOverlayAvailable() const
{
    return m_overlayMask.empty() == false;
}

void CImageProcessIO::clearData()
{
    m_image.release();
}

void CImageProcessIO::copyStaticData(const ProtocolTaskIOPtr &ioPtr)
{
    CProtocolTaskIO::copyStaticData(ioPtr);

    auto imgIoPtr = std::dynamic_pointer_cast<CImageProcessIO>(ioPtr);
    if(imgIoPtr)
        m_channelCount = imgIoPtr->getChannelCount();
}

void CImageProcessIO::save()
{
    if( m_dataType == IODataType::VIDEO ||
        m_dataType == IODataType::VIDEO_BINARY ||
        m_dataType == IODataType::VIDEO_LABEL)
    {
        // For videos, output files are generated while processing.
        // We juste have to remove file from temporary list to avoid deletion.
        m_tempFiles.clear();
    }
    else
        save(getSavePath());
}

void CImageProcessIO::save(const std::string &path)
{
    CDataImageIO io(path);
    io.write(m_image);
}

std::shared_ptr<CImageProcessIO> CImageProcessIO::clone() const
{
    return std::static_pointer_cast<CImageProcessIO>(cloneImp());
}

std::shared_ptr<CProtocolTaskIO> CImageProcessIO::cloneImp() const
{
    return std::shared_ptr<CImageProcessIO>(new CImageProcessIO(*this));
}

