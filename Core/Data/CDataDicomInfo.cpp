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

#include "CDataDicomInfo.h"

CDataDicomInfo::CDataDicomInfo() : CDataImageInfo()
{
}

CDataDicomInfo::CDataDicomInfo(const CDataDicomInfo &info) : CDataImageInfo(info)
{
    m_nbStudy = info.m_nbStudy;
    m_nbSerie = info.m_nbSerie;
}

CDataDicomInfo::CDataDicomInfo(const CDataDicomInfo&& info) : CDataImageInfo(info)
{
    m_nbStudy = std::move(info.m_nbStudy);
    m_nbSerie = std::move(info.m_nbSerie);
}

CDataDicomInfo::~CDataDicomInfo()
{
}

CDataDicomInfo &CDataDicomInfo::operator=(const CDataDicomInfo& info)
{
    CDataImageInfo::operator=(info);
    m_nbStudy = info.m_nbStudy;
    m_nbSerie = info.m_nbSerie;
    return *this;
}

CDataDicomInfo &CDataDicomInfo::operator=(const CDataDicomInfo&& info)
{
    CDataImageInfo::operator=(info);
    m_nbStudy = std::move(info.m_nbStudy);
    m_nbSerie = std::move(info.m_nbSerie);
    return *this;
}

VectorPairString CDataDicomInfo::getStringList() const
{
    VectorPairString infoList =  CDataImageInfo::getStringList();

    auto it = std::find_if(infoList.begin(), infoList.end(), [](const PairString& elt){ return elt.first == "Metadata"; });
    if(it != infoList.end())
        it->second = "DICOM metadata";

    return infoList;
}
