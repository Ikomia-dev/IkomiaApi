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

#include "CDataVideoInfo.h"
#include "opencv2/opencv.hpp"
#include <QObject>

CDataVideoInfo::CDataVideoInfo() : CDataImageInfo(IODataType::VIDEO)
{
}

CDataVideoInfo::CDataVideoInfo(IODataType type) : CDataImageInfo(type)
{

}

CDataVideoInfo::CDataVideoInfo(const std::string& fileName) : CDataImageInfo(IODataType::VIDEO, fileName)
{
}

CDataVideoInfo::CDataVideoInfo(IODataType type, const std::string &fileName): CDataImageInfo(type, fileName)
{
}

CDataVideoInfo::CDataVideoInfo(int fps, int frameCount, int currentPos, int fourcc) : CDataImageInfo(IODataType::VIDEO)
{
    m_fps = fps;
    m_frameCount = frameCount;
    m_currentPos = currentPos;
    m_fourcc = fourcc;
}

CDataVideoInfo::CDataVideoInfo(const CDataVideoInfo &info) : CDataImageInfo(info)
{
    m_fps = info.m_fps;
    m_frameCount = info.m_frameCount;
    m_currentPos = info.m_currentPos;
    m_fourcc = info.m_fourcc;
}

CDataVideoInfo::CDataVideoInfo(CDataVideoInfo &&info) : CDataImageInfo(info)
{
    m_fps = std::move(info.m_fps);
    m_frameCount = std::move(info.m_frameCount);
    m_currentPos = std::move(info.m_currentPos);
    m_fourcc = std::move(info.m_fourcc);
}

CDataVideoInfo::~CDataVideoInfo()
{
}

CDataVideoInfo& CDataVideoInfo::operator=(const CDataVideoInfo &info)
{
    CDataImageInfo::operator =(info);
    m_fps = info.m_fps;
    m_frameCount = info.m_frameCount;
    m_currentPos = info.m_currentPos;
    m_fourcc = info.m_fourcc;
    return *this;
}

CDataVideoInfo &CDataVideoInfo::operator=(CDataVideoInfo &&info)
{
    CDataImageInfo::operator =(info);
    m_fps = std::move(info.m_fps);
    m_frameCount = std::move(info.m_frameCount);
    m_currentPos = std::move(info.m_currentPos);
    m_fourcc = std::move(info.m_fourcc);
    return *this;
}

VectorPairString CDataVideoInfo::getStringList() const
{
    VectorPairString infoList = CDataImageInfo::getStringList();
    infoList.push_back(std::make_pair(QObject::tr("Total frame number").toStdString(), std::to_string(m_frameCount)));
    infoList.push_back(std::make_pair(QObject::tr("Current frame").toStdString(), std::to_string(m_currentPos)));
    infoList.push_back(std::make_pair(QObject::tr("Video codec").toStdString(), std::to_string(m_fourcc)));
    return infoList;
}

