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

#include "CDataArrayInfo.h"
#include <QObject>

CDataArrayInfo::CDataArrayInfo() : CDataInfo(IODataType::ARRAY)
{
}

CDataArrayInfo::CDataArrayInfo(const CMat &array) : CDataInfo(IODataType::ARRAY)
{
    updateArray(array);
}

void CDataArrayInfo::updateArray(const CMat &array)
{
    if(array.empty())
        return;

    m_cvType = array.type();

    m_sizes.resize(array.dims);
    for(size_t i=0; i<m_sizes.size(); ++i)
        m_sizes[i] = array.size[i];

    switch(m_cvType)
    {
        case CV_8UC1:
        case CV_8UC3:
        case CV_8UC4:
            setElementSize(1);
            break;

        case CV_16UC1:
        case CV_16UC3:
        case CV_16UC4:
        case CV_16SC1:
        case CV_16SC3:
        case CV_16SC4:
            setElementSize(2);
            break;

        case CV_32SC1:
        case CV_32SC3:
        case CV_32SC4:
        case CV_32FC1:
        case CV_32FC3:
        case CV_32FC4:
            setElementSize(4);
            break;

        case CV_64FC1:
        case CV_64FC3:
        case CV_64FC4:
            setElementSize(8);
        break;
    }
    computeMinMax(array);
}

VectorPairString CDataArrayInfo::getStringList() const
{
    VectorPairString infoList = CDataInfo::getStringList();

    VectorPairString newList;
    newList.push_back(std::make_pair(QObject::tr("Type").toStdString(), getTypeString()));
    newList.push_back(std::make_pair(QObject::tr("Size").toStdString(), getSizeString()));
    newList.push_back(std::make_pair(QObject::tr("Min value").toStdString(), std::to_string(m_minValue)));
    newList.push_back(std::make_pair(QObject::tr("Max value").toStdString(), std::to_string(m_maxValue)));

    infoList.insert(infoList.begin() + 1, newList.begin(), newList.end());
    return infoList;
}

std::string CDataArrayInfo::getTypeString() const
{
    std::string type;
    switch(m_cvType)
    {
        case CV_8UC1:
        case CV_8UC3:
        case CV_8UC4:
            type = "unsigned 8 bits integer";
            break;

        case CV_16UC1:
        case CV_16UC3:
        case CV_16UC4:
            type = "unsigned 16 bits integer";
            break;

        case CV_16SC1:
        case CV_16SC3:
        case CV_16SC4:
            type = "signed 16 bits integer";
            break;

        case CV_32SC1:
        case CV_32SC3:
        case CV_32SC4:
            type = "signed 32 bits integer";
            break;

        case CV_32FC1:
        case CV_32FC3:
        case CV_32FC4:
            type = "float";
            break;

        case CV_64FC1:
        case CV_64FC3:
        case CV_64FC4:
            type = "double";
            break;
    }
    return type;
}

std::string CDataArrayInfo::getSizeString() const
{
    if(m_sizes.size() == 0)
        return "";

    std::string strSize = std::to_string(m_sizes[0]);
    for(size_t i=1; i<m_sizes.size(); ++i)
        strSize += " x " + std::to_string(m_sizes[i]);

    return strSize;
}

void CDataArrayInfo::computeMinMax(const CMat &array)
{
    //cv::minMaxIdx does not support multi-channels input
    if(array.channels() > 1)
    {
        auto tmpImg = array.reshape(1, 0);
        cv::minMaxIdx(tmpImg, &m_minValue, &m_maxValue);
    }
    else
        cv::minMaxIdx(array, &m_minValue, &m_maxValue);
}
