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

#include "CDatasetInfo.h"
#include <algorithm>
#include <cassert>

CDatasetInfo::CDatasetInfo()
{
}

CDatasetInfo::CDatasetInfo(const CDatasetInfo &info)
{
    m_name = info.m_name;
}

CDatasetInfo::CDatasetInfo(CDatasetInfo&& info)
{
    m_name = std::move(info.m_name);
}

CDatasetInfo &CDatasetInfo::operator=(const CDatasetInfo &info)
{
    m_name = info.m_name;
    return *this;
}

CDatasetInfo &CDatasetInfo::operator=(CDatasetInfo &&info)
{
    m_name = std::move(info.m_name);
    return *this;
}

bool CDatasetInfo::operator==(const CDatasetInfo &other) const
{
    return m_name == other.m_name;
}

std::string CDatasetInfo::getName() const
{
    return m_name;
}

void CDatasetInfo::setName(const std::string &name)
{
    m_name = name;
}

