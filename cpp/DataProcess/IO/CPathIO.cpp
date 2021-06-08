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

#include <QObject>
#include <boost/filesystem.hpp>
#include "CPathIO.h"

CPathIO::CPathIO() : CWorkflowTaskIO(IODataType::FILE_PATH)
{
    m_description = QObject::tr("File system path (file or folder).").toStdString();
    m_infoPtr = std::make_shared<CDataInfo>(m_dataType, m_path);
}

CPathIO::CPathIO(IODataType data) : CWorkflowTaskIO(data)
{
    m_description = QObject::tr("File system path (file or folder).").toStdString();
    m_infoPtr = std::make_shared<CDataInfo>(m_dataType, m_path);
}

CPathIO::CPathIO(IODataType data, const std::string &path) : CWorkflowTaskIO(data)
{
    m_description = QObject::tr("File system path (file or folder).").toStdString();
    m_path = path;
    m_infoPtr = std::make_shared<CDataInfo>(m_dataType, m_path);
}

CPathIO::CPathIO(const CPathIO &in) : CWorkflowTaskIO(in)
{
    m_path = in.m_path;
    m_infoPtr = std::make_shared<CDataInfo>(m_dataType, m_path);
}

CPathIO::CPathIO(CPathIO &&in)
{
    m_path = std::move(in.m_path);
    m_infoPtr = std::make_shared<CDataInfo>(m_dataType, m_path);
}

CPathIO &CPathIO::operator=(const CPathIO &in)
{
    CWorkflowTaskIO::operator=(in);
    m_path = in.m_path;
    m_infoPtr = std::make_shared<CDataInfo>(m_dataType, m_path);
    return *this;
}

CPathIO &CPathIO::operator=(CPathIO &&in)
{
    CWorkflowTaskIO::operator=(in);
    m_path = std::move(in.m_path);
    m_infoPtr = std::make_shared<CDataInfo>(m_dataType, m_path);
    return *this;
}

void CPathIO::setPath(const std::string &path)
{
    assert(m_infoPtr);
    m_path = path;
    m_infoPtr->setFileName(path);
}

std::string CPathIO::getPath() const
{
    return m_path;
}

bool CPathIO::isDataAvailable() const
{
    boost::filesystem::path boostPath(m_path);
    return boost::filesystem::exists(boostPath);
}

bool CPathIO::isAutoInput() const
{
    return true;
}

void CPathIO::clearData()
{
    m_path.clear();
}

std::shared_ptr<CPathIO> CPathIO::clone() const
{
    return std::static_pointer_cast<CPathIO>(cloneImp());
}

WorkflowTaskIOPtr CPathIO::cloneImp() const
{
    return std::shared_ptr<CPathIO>(new CPathIO(*this));
}
