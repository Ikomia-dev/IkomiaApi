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

#include "CArrayIO.h"
#include "Data/CDataArrayInfo.h"
#include <QObject>

CArrayIO::CArrayIO() : CProtocolTaskIO(IODataType::ARRAY)
{
    m_description = QObject::tr("Generic multi-dimensional array.\n").toStdString();
}

CArrayIO::CArrayIO(const CMat &array) : CProtocolTaskIO(IODataType::ARRAY)
{
    m_description = QObject::tr("Generic multi-dimensional array.\n").toStdString();
    m_array = array;
    m_bNewDataInfo = true;
}

CArrayIO::CArrayIO(const CArrayIO &io) : CProtocolTaskIO(io)
{
    m_array = io.m_array;
    m_bNewDataInfo = true;
}

CArrayIO::CArrayIO(const CArrayIO &&io) : CProtocolTaskIO(io)
{
    m_array = std::move(io.m_array);
    m_bNewDataInfo = true;
}

CArrayIO &CArrayIO::operator=(const CArrayIO &io)
{
    CProtocolTaskIO::operator=(io);
    m_array = io.m_array;
    m_bNewDataInfo = true;
    return *this;
}

CArrayIO &CArrayIO::operator=(const CArrayIO &&io)
{
    CProtocolTaskIO::operator=(io);
    m_array = std::move(io.m_array);
    m_bNewDataInfo = true;
    return *this;
}

void CArrayIO::setArray(const CMat &array)
{
    m_array = array;
    m_bNewDataInfo = true;
}

CMat CArrayIO::getArray() const
{
    return m_array;
}

CDataInfoPtr CArrayIO::getDataInfo()
{
    if(m_bNewDataInfo)
    {
        m_infoPtr = std::make_shared<CDataArrayInfo>(m_array);
        m_bNewDataInfo = false;
    }
    return m_infoPtr;
}

size_t CArrayIO::getUnitElementCount() const
{
    //Process array as a whole
    return 1;
}

bool CArrayIO::isDataAvailable() const
{
    return m_array.data != nullptr;
}

void CArrayIO::clearData()
{
    m_array.release();
}

std::shared_ptr<CArrayIO> CArrayIO::clone() const
{
    return std::static_pointer_cast<CArrayIO>(cloneImp());
}

std::shared_ptr<CProtocolTaskIO> CArrayIO::cloneImp() const
{
    return std::shared_ptr<CArrayIO>(new CArrayIO(*this));
}
