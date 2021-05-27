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

#include "CConvertIO.h"

CConvertIO::CConvertIO()
{
}

ImageProcessIOPtr CConvertIO::convertToImageIO(const ProtocolTaskIOPtr &ioFrom)
{
    auto dataType = CConvertIO::getTargetImageType(ioFrom->getDataType());
    if(dataType != IODataType::NONE)
    {
        auto imgIOPtr = std::make_shared<CImageProcessIO>();
        imgIOPtr->CProtocolTaskIO::operator=(*ioFrom);
        imgIOPtr->setDataType(dataType);
        imgIOPtr->setSaveFormat(DataFileFormat::PNG);
        return imgIOPtr;
    }
    return nullptr;
}

VideoProcessIOPtr CConvertIO::convertToVideoIO(const ProtocolTaskIOPtr &ioFrom)
{
    auto dataType = CConvertIO::getTargetVideoType(ioFrom->getDataType());
    if(dataType != IODataType::NONE)
    {
        auto videoIOPtr = std::make_shared<CVideoProcessIO>();
        videoIOPtr->CProtocolTaskIO::operator=(*ioFrom);
        videoIOPtr->setDataType(dataType);
        videoIOPtr->setSaveFormat(DataFileFormat::AVI);
        return videoIOPtr;
    }
    return nullptr;
}

VideoProcessIOPtr CConvertIO::convertToStreamIO(const ProtocolTaskIOPtr &ioFrom)
{
    auto dataType = CConvertIO::getTargetStreamType(ioFrom->getDataType());
    if(dataType != IODataType::NONE)
    {
        auto videoIOPtr = std::make_shared<CVideoProcessIO>();
        videoIOPtr->CProtocolTaskIO::operator=(*ioFrom);
        videoIOPtr->setDataType(dataType);
        videoIOPtr->setSaveFormat(DataFileFormat::AVI);
        return videoIOPtr;
    }
    return nullptr;
}

ImageProcessIOPtr CConvertIO::convertToVolumeIO(const ProtocolTaskIOPtr &ioFrom)
{
    auto dataType = CConvertIO::getTargetVolumeType(ioFrom->getDataType());
    if(dataType != IODataType::NONE)
    {
        auto volumeIOPtr = std::make_shared<CImageProcessIO>();
        volumeIOPtr->CProtocolTaskIO::operator=(*ioFrom);
        volumeIOPtr->setDataType(dataType);
        volumeIOPtr->setSaveFormat(DataFileFormat::PNG);
        return volumeIOPtr;
    }
    return nullptr;
}

IODataType CConvertIO::getTargetImageType(IODataType srcType)
{
    IODataType type = IODataType::NONE;

    if(srcType == IODataType::VOLUME ||
        srcType == IODataType::VIDEO ||
        srcType == IODataType::LIVE_STREAM)
    {
        type = IODataType::IMAGE;
    }
    else if(srcType == IODataType::VOLUME_BINARY ||
            srcType == IODataType::VIDEO_BINARY ||
            srcType == IODataType::LIVE_STREAM_BINARY)
    {
        type = IODataType::IMAGE_BINARY;
    }
    else if(srcType == IODataType::VOLUME_LABEL ||
            srcType == IODataType::VIDEO_LABEL ||
            srcType == IODataType::LIVE_STREAM_LABEL)
    {
        type = IODataType::IMAGE_LABEL;
    }
    return type;
}

IODataType CConvertIO::getTargetVideoType(IODataType srcType)
{
    IODataType type = IODataType::NONE;

    if(srcType == IODataType::IMAGE ||
        srcType == IODataType::LIVE_STREAM ||
        srcType == IODataType::VOLUME)
    {
        type = IODataType::VIDEO;
    }
    else if(srcType == IODataType::IMAGE_BINARY ||
             srcType == IODataType::LIVE_STREAM_BINARY ||
             srcType == IODataType::VOLUME_BINARY)
    {
        type = IODataType::VIDEO_BINARY;
    }
    else if(srcType == IODataType::IMAGE_LABEL ||
             srcType == IODataType::LIVE_STREAM_LABEL ||
             srcType == IODataType::VOLUME_LABEL)
    {
        type = IODataType::VIDEO_LABEL;
    }
    return type;
}

IODataType CConvertIO::getTargetStreamType(IODataType srcType)
{
    IODataType type = IODataType::NONE;

    if(srcType == IODataType::IMAGE ||
       srcType == IODataType::VIDEO ||
       srcType == IODataType::VOLUME)
    {
        type = IODataType::LIVE_STREAM;
    }
    else if(srcType == IODataType::IMAGE_BINARY ||
            srcType == IODataType::VIDEO_BINARY ||
            srcType == IODataType::VOLUME_BINARY)
    {
        type = IODataType::LIVE_STREAM_BINARY;
    }
    else if(srcType == IODataType::IMAGE_LABEL ||
            srcType == IODataType::VIDEO_LABEL ||
            srcType == IODataType::VOLUME_LABEL)
    {
        type = IODataType::LIVE_STREAM_LABEL;
    }
    return type;
}

IODataType CConvertIO::getTargetVolumeType(IODataType srcType)
{
    IODataType type = IODataType::NONE;

    if(srcType == IODataType::IMAGE ||
       srcType == IODataType::VIDEO ||
       srcType == IODataType::LIVE_STREAM)
    {
        type = IODataType::VOLUME;
    }
    else if(srcType == IODataType::IMAGE_BINARY ||
             srcType == IODataType::VIDEO_BINARY ||
             srcType == IODataType::LIVE_STREAM_BINARY)
    {
        type = IODataType::VOLUME_BINARY;
    }
    else if(srcType == IODataType::IMAGE_LABEL ||
             srcType == IODataType::VIDEO_LABEL ||
             srcType == IODataType::LIVE_STREAM_LABEL)
    {
        type = IODataType::VOLUME_LABEL;
    }
    return type;
}
