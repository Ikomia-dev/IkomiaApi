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

#include "CDataInfo.h"
#include <QObject>

CDataInfo::CDataInfo(IODataType type)
{
    m_type = type;
    m_fileName.empty();
}

CDataInfo::CDataInfo(IODataType type, const std::string& fileName)
{
    m_type = type;
    m_fileName = fileName;
}

CDataInfo::CDataInfo(const CDataInfo &data)
{
    m_type = data.m_type;
    m_fileName = data.m_fileName;
    m_elementSize = data.m_elementSize;
}

CDataInfo::CDataInfo(CDataInfo &&data)
{
    m_type = std::move(data.m_type);
    m_fileName = std::move(data.m_fileName);
    m_elementSize = std::move(data.m_elementSize);
}

CDataInfo::~CDataInfo()
{
}

CDataInfo& CDataInfo::operator=(const CDataInfo &data)
{
    m_type = data.m_type;
    m_fileName = data.m_fileName;
    m_elementSize = data.m_elementSize;
    return *this;
}

CDataInfo &CDataInfo::operator=(CDataInfo &&data)
{
    m_type = std::move(data.m_type);
    m_fileName = std::move(data.m_fileName);
    m_elementSize = std::move(data.m_elementSize);
    return *this;
}

void CDataInfo::setFileName(const std::string &fileName)
{
    m_fileName = fileName;
}

void CDataInfo::setElementSize(int size)
{
    m_elementSize = size;
}

std::string CDataInfo::getFileName() const
{
    return m_fileName;
}

IODataType CDataInfo::getType() const
{
    return m_type;
}

int CDataInfo::getElementSize() const
{
    return m_elementSize;
}

UMapString CDataInfo::getMetadata() const
{
    return m_metadata;
}

UMapString &CDataInfo::metadata()
{
    return m_metadata;
}

VectorPairString CDataInfo::getStringList() const
{
    VectorPairString infoList;
    infoList.push_back(std::make_pair(QObject::tr("File path").toStdString(), m_fileName));

    if(getMetadata().size() > 0)
        infoList.push_back(std::make_pair("Metadata", QObject::tr("General metadata").toStdString()));

    for(auto&& it : getMetadata())
        infoList.push_back(std::make_pair(it.first, it.second));

    return infoList;
}

