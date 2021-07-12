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

#include "CWorkflowTaskParam.h"
#include <QHash>
#include <ostream>

CWorkflowTaskParam::CWorkflowTaskParam()
{
}

void CWorkflowTaskParam::setParamMap(const UMapString &paramMap)
{
    Q_UNUSED(paramMap);
}

UMapString CWorkflowTaskParam::getParamMap() const
{
    return UMapString();
}

uint CWorkflowTaskParam::getHashValue() const
{
    std::vector<QString> values;
    auto paramMap = getParamMap();

    for(auto it=paramMap.begin(); it!=paramMap.end(); ++it)
        values.push_back(QString::fromStdString(it->second));

    return qHashRange(values.begin(), values.end());
}

std::ostream& operator<<(std::ostream& os, const CWorkflowTaskParam& param)
{
    auto params = param.getParamMap();
    for(auto it=params.begin(); it!=params.end(); ++it)
        os << it->first << ":" << it->second << std::endl;

    return os;
}
