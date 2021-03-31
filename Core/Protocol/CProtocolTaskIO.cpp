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

#include "CProtocolTaskIO.h"
#include "Main/CoreTools.hpp"

CProtocolTaskIO::CProtocolTaskIO()
{
}

CProtocolTaskIO::CProtocolTaskIO(IODataType dataType)
{
    m_dataType = dataType;
}

CProtocolTaskIO::CProtocolTaskIO(const CProtocolTaskIO& io)
{
    m_name = io.m_name;
    m_description = io.m_description;
    m_saveFolder = io.m_saveFolder;
    m_saveBaseName = io.m_saveBaseName;
    m_dataType = io.m_dataType;
    m_saveFormat = io.m_saveFormat;
    m_dimCount = io.m_dimCount;
    m_bAutoSave = io.m_bAutoSave;
    m_bDisplayable = io.m_bDisplayable;
}

CProtocolTaskIO::CProtocolTaskIO(const CProtocolTaskIO&& io)
{
    m_name = std::move(io.m_name);
    m_description = std::move(io.m_description);
    m_saveFolder = std::move(io.m_saveFolder);
    m_saveBaseName = std::move(io.m_saveBaseName);
    m_dataType = std::move(io.m_dataType);
    m_saveFormat = std::move(io.m_saveFormat);
    m_dimCount = std::move(io.m_dimCount);
    m_bAutoSave = std::move(io.m_bAutoSave);
    m_bDisplayable = std::move(io.m_bDisplayable);
}

CProtocolTaskIO &CProtocolTaskIO::operator=(const CProtocolTaskIO &io)
{
    m_name = io.m_name;
    m_description = io.m_description;
    m_saveFolder = io.m_saveFolder;
    m_saveBaseName = io.m_saveBaseName;
    m_dataType = io.m_dataType;
    m_saveFormat = io.m_saveFormat;
    m_dimCount = io.m_dimCount;
    m_bAutoSave = io.m_bAutoSave;
    m_bDisplayable = io.m_bDisplayable;
    return *this;
}

CProtocolTaskIO &CProtocolTaskIO::operator=(const CProtocolTaskIO&& io)
{
    m_name = std::move(io.m_name);
    m_description = std::move(io.m_description);
    m_saveFolder = std::move(io.m_saveFolder);
    m_saveBaseName = std::move(io.m_saveBaseName);
    m_dataType = std::move(io.m_dataType);
    m_saveFormat = std::move(io.m_saveFormat);
    m_dimCount = std::move(io.m_dimCount);
    m_bAutoSave = std::move(io.m_bAutoSave);
    m_bDisplayable = std::move(io.m_bDisplayable);
    return *this;
}

CProtocolTaskIO::~CProtocolTaskIO()
{
    deleteTemporaryFiles();
}

std::string CProtocolTaskIO::getName() const
{
    return m_name;
}

std::string CProtocolTaskIO::getDescription() const
{
    return m_description;
}

IODataType CProtocolTaskIO::getDataType() const
{
    return m_dataType;
}

DataFileFormat CProtocolTaskIO::getSaveFormat() const
{
    return m_saveFormat;
}

std::string CProtocolTaskIO::getSavePath() const
{
    return m_saveFolder + m_saveBaseName + Utils::Data::getFileFormatExtension(m_saveFormat);
}

size_t CProtocolTaskIO::getDimensionCount() const
{
    return m_dimCount;
}

size_t CProtocolTaskIO::getUnitElementCount() const
{
    return 1;
}

CDataInfoPtr CProtocolTaskIO::getDataInfo()
{
    return m_infoPtr;
}

bool CProtocolTaskIO::isDataAvailable() const
{
    return false;
}

bool CProtocolTaskIO::isAutoInput() const
{
    return false;
}

bool CProtocolTaskIO::isAutoSave() const
{
    return m_bAutoSave;
}

bool CProtocolTaskIO::isDisplayable() const
{
    return m_bDisplayable;
}

void CProtocolTaskIO::setDataType(IODataType type)
{
    m_dataType = type;
}

void CProtocolTaskIO::setName(const std::string &name)
{
    m_name = name;
}

void CProtocolTaskIO::setDimensionCount(size_t nb)
{
    m_dimCount = nb;
}

void CProtocolTaskIO::setSaveInfo(const std::string &folder, const std::string &baseName)
{
    m_saveFolder = folder;
    m_saveBaseName = baseName;
}

void CProtocolTaskIO::setDescription(const std::string &description)
{
    m_description = description;
}

void CProtocolTaskIO::setAutoSave(bool bEnable)
{
    m_bAutoSave = bEnable;
}

void CProtocolTaskIO::setSaveFormat(DataFileFormat format)
{
    m_saveFormat = format;
}

void CProtocolTaskIO::setDisplayable(bool bEnable)
{
    m_bDisplayable = bEnable;
}

void CProtocolTaskIO::clearData()
{
}

std::shared_ptr<CProtocolTaskIO> CProtocolTaskIO::clone() const
{
    return cloneImp();
}

void CProtocolTaskIO::copy(const std::shared_ptr<CProtocolTaskIO> &ioPtr)
{
    *this = *ioPtr;
}

void CProtocolTaskIO::copyStaticData(const std::shared_ptr<CProtocolTaskIO> &ioPtr)
{
    if(ioPtr)
        m_dimCount = ioPtr->getDimensionCount();
}

void CProtocolTaskIO::addTemporaryFile(const std::string &path)
{
    m_tempFiles.insert(path);
}

void CProtocolTaskIO::save()
{
}

void CProtocolTaskIO::save(const std::string &path)
{
    Q_UNUSED(path);
}

std::string CProtocolTaskIO::getClassName(IODataType ioDataType)
{
    switch(ioDataType)
    {
        case IODataType::NONE:
            return "";

        case IODataType::IMAGE:
        case IODataType::IMAGE_BINARY:
        case IODataType::IMAGE_LABEL:
        case IODataType::VOLUME:
        case IODataType::VOLUME_BINARY:
        case IODataType::VOLUME_LABEL:
        case IODataType::DESCRIPTORS:
            return "CImageProcessIO";

        case IODataType::VIDEO:
        case IODataType::VIDEO_BINARY:
        case IODataType::VIDEO_LABEL:
        case IODataType::LIVE_STREAM:
        case IODataType::LIVE_STREAM_BINARY:
        case IODataType::LIVE_STREAM_LABEL:
            return "CVideoProcessIO";

        case IODataType::INPUT_GRAPHICS:
            return "CGraphicsProcessInput";

        case IODataType::OUTPUT_GRAPHICS:
            return "CGraphicsProcessOutput";

        case IODataType::BLOB_VALUES:
            return "CMeasureProcessIO";

        case IODataType::NUMERIC_VALUES:
            return "CFeatureProcessIO";

        case IODataType::WIDGET:
            return "CWidgetOutput";

        case IODataType::PROJECT_FOLDER:
        case IODataType::FOLDER_PATH:
        case IODataType::FILE_PATH:
            return "CPathIO";

        case IODataType::ARRAY:
            return "CArrayIO";

        default: return "";
    }
}

std::shared_ptr<CProtocolTaskIO> CProtocolTaskIO::cloneImp() const
{
    return std::shared_ptr<CProtocolTaskIO>(new CProtocolTaskIO(*this));
}

void CProtocolTaskIO::deleteTemporaryFiles()
{
    for(auto it=m_tempFiles.begin(); it!=m_tempFiles.end(); ++it)
    {
        try
        {
            boost::filesystem::path path(*it);
            boost::filesystem::remove(path);
        }
        catch (const boost::filesystem::filesystem_error& e)
        {
            qWarning() << QString::fromStdString(e.code().message());
        }
    }
}
