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

#include "CImageIO.h"
#include "Data/CDataImageInfo.h"
#include "CDataImageIO.h"
#include "CDataIO.hpp"

using CImageDataIO = CDataIO<CDataImageIO, CMat>;

CImageIO::CImageIO() : CWorkflowTaskIO(IODataType::IMAGE, "CImageIO")
{
    m_description = QObject::tr("2D or 3D images.\n"
                                "Can be single frame from video or camera stream.").toStdString();
    m_saveFormat = DataFileFormat::PNG;
}

CImageIO::CImageIO(IODataType data) : CWorkflowTaskIO(data, "CImageIO")
{
    m_description = QObject::tr("2D or 3D images.\n"
                                "Can be single frame from video or camera stream.").toStdString();
    m_saveFormat = DataFileFormat::PNG;
}

CImageIO::CImageIO(IODataType data, const CMat &image) : CWorkflowTaskIO(data, "CImageIO")
{
    m_description = QObject::tr("2D or 3D images.\n"
                                "Can be single frame from video or camera stream.").toStdString();
    m_saveFormat = DataFileFormat::PNG;
    m_image = image;
    m_dimCount = m_image.dims;
    m_channelCount = m_image.channels();
    m_bNewDataInfo = true;
}

CImageIO::CImageIO(IODataType data, const CMat &image, const std::string& name) : CWorkflowTaskIO(data, name)
{
    m_description = QObject::tr("2D or 3D images.\n"
                                "Can be single frame from video or camera stream.").toStdString();
    m_saveFormat = DataFileFormat::PNG;
    m_image = image;
    m_dimCount = m_image.dims;
    m_channelCount = m_image.channels();
    m_bNewDataInfo = true;
}

CImageIO::CImageIO(IODataType data, const std::string &name) : CWorkflowTaskIO(data, name)
{
    m_description = QObject::tr("2D or 3D images.\n"
                                "Can be single frame from video or camera stream.").toStdString();
    m_saveFormat = DataFileFormat::PNG;
}

CImageIO::CImageIO(IODataType data, const std::string& name, const std::string &path) : CWorkflowTaskIO(data, name)
{
    m_description = QObject::tr("2D or 3D images.\n"
                                "Can be single frame from video or camera stream.").toStdString();
    m_saveFormat = DataFileFormat::PNG;
    m_name = Utils::File::getFileNameWithoutExtension(path);

    CImageDataIO io(path);
    m_image = io.read();

    if(m_image.data != nullptr)
    {
        m_dimCount = m_image.dims;
        m_channelCount = m_image.channels();
        m_bNewDataInfo = true;
    }
}

CImageIO::CImageIO(const CImageIO &io) : CWorkflowTaskIO(io)
{
    m_image = io.m_image;
    m_overlayMask = io.m_overlayMask;
    m_channelCount = io.m_channelCount;
    m_currentIndex = io.m_currentIndex;
    m_bNewDataInfo = true;
}

CImageIO::CImageIO(const CImageIO&& io) : CWorkflowTaskIO(io)
{
    m_image = std::move(io.m_image);
    m_overlayMask = std::move(io.m_overlayMask);
    m_channelCount = std::move(io.m_channelCount);
    m_currentIndex = std::move(io.m_currentIndex);
    m_bNewDataInfo = true;
}

CImageIO &CImageIO::operator=(const CImageIO& io)
{
    CWorkflowTaskIO::operator=(io);
    m_image = io.m_image;
    m_overlayMask = io.m_overlayMask;
    m_channelCount = io.m_channelCount;
    m_currentIndex = io.m_currentIndex;
    m_bNewDataInfo = true;
    return *this;
}

CImageIO &CImageIO::operator=(const CImageIO&& io)
{
    CWorkflowTaskIO::operator=(io);
    m_image = std::move(io.m_image);
    m_overlayMask = std::move(io.m_overlayMask);
    m_channelCount = std::move(io.m_channelCount);
    m_currentIndex = std::move(io.m_currentIndex);
    m_bNewDataInfo = true;
    return *this;
}

void CImageIO::setImage(const CMat &image)
{
    m_image = image;
    m_dimCount = m_image.dims;
    m_channelCount = m_image.channels();
    m_bNewDataInfo = true;
}

void CImageIO::setChannelCount(size_t nb)
{
    m_channelCount = nb;
}

void CImageIO::setCurrentImage(size_t index)
{
    m_currentIndex = index;
}

void CImageIO::setOverlayMask(const CMat &mask)
{
    m_overlayMask = mask;
}

size_t CImageIO::getChannelCount() const
{
    return m_channelCount;
}

CMat CImageIO::getData() const
{
    return m_image;
}

CDataInfoPtr CImageIO::getDataInfo()
{
    if(m_bNewDataInfo)
    {
        m_infoPtr = std::make_shared<CDataImageInfo>(m_image);
        m_bNewDataInfo = false;
    }
    return m_infoPtr;
}

CMat CImageIO::getImage() const
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

CMat CImageIO::getImageWithGraphics(const GraphicsInputPtr &graphics) const
{
    auto internalImg = getImage();
    if(internalImg.empty())
        return CMat();

    CMat img =internalImg.clone();
    if(graphics)
    {
        CGraphicsConversion graphicsConv((int)img.getNbCols(), (int)img.getNbRows());
        for(auto it : graphics->getItems())
            it->insertToImage(img, graphicsConv, false, false);
    }
    return img;
}

CMat CImageIO::getImageWithGraphics(const GraphicsOutputPtr &graphics) const
{
    auto internalImg = getImage();
    if(internalImg.empty())
        return CMat();

    CMat img =internalImg.clone();
    if(graphics)
    {
        CGraphicsConversion graphicsConv((int)img.getNbCols(), (int)img.getNbRows());
        for(auto it : graphics->getItems())
            it->insertToImage(img, graphicsConv, false, false);
    }
    return img;
}

size_t CImageIO::getUnitElementCount() const
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

CMat CImageIO::getOverlayMask() const
{
    return m_overlayMask;
}

std::vector<DataFileFormat> CImageIO::getPossibleSaveFormats() const
{
    std::vector<DataFileFormat> formats = {
        DataFileFormat::PNG, DataFileFormat::JPG, DataFileFormat::TIF,
        DataFileFormat::BMP, DataFileFormat::WEBP, DataFileFormat::JP2
    };
    return formats;
}

bool CImageIO::isDataAvailable() const
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

bool CImageIO::isOverlayAvailable() const
{
    return m_overlayMask.empty() == false;
}

void CImageIO::clearData()
{
    m_image.release();
}

void CImageIO::copyStaticData(const WorkflowTaskIOPtr &ioPtr)
{
    CWorkflowTaskIO::copyStaticData(ioPtr);

    auto imgIoPtr = std::dynamic_pointer_cast<CImageIO>(ioPtr);
    if(imgIoPtr)
        m_channelCount = imgIoPtr->getChannelCount();
}

void CImageIO::drawGraphics(const GraphicsInputPtr &graphics)
{
    if(m_image.data == nullptr)
        return;

    CGraphicsConversion graphicsConv((int)m_image.getNbCols(), (int)m_image.getNbRows());
    for(auto it : graphics->getItems())
        it->insertToImage(m_image, graphicsConv, false, false);
}

void CImageIO::drawGraphics(const GraphicsOutputPtr &graphics)
{
    if(m_image.data == nullptr)
        return;

    CGraphicsConversion graphicsConv((int)m_image.getNbCols(), (int)m_image.getNbRows());
    for(auto it : graphics->getItems())
        it->insertToImage(m_image, graphicsConv, false, false);
}

void CImageIO::save()
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

void CImageIO::save(const std::string &path)
{
    CDataImageIO io(path);
    io.write(m_image);
}

std::shared_ptr<CImageIO> CImageIO::clone() const
{
    return std::static_pointer_cast<CImageIO>(cloneImp());
}

std::shared_ptr<CWorkflowTaskIO> CImageIO::cloneImp() const
{
    return std::shared_ptr<CImageIO>(new CImageIO(*this));
}

