/*
 * Copyright (C) 2021 Ikomia SAS
 * Contact: https://www.ikomia.com
 *
 * This file is part of the Ikomia API libraries.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef CDATAARRAYINFO_H
#define CDATAARRAYINFO_H

#include "CDataInfo.h"
#include "Main/CoreGlobal.hpp"
#include "Data/CMat.hpp"

/** @cond INTERNAL */

class CORESHARED_EXPORT CDataArrayInfo : public CDataInfo
{
    public:

        CDataArrayInfo();
        CDataArrayInfo(const CMat& array);

        virtual ~CDataArrayInfo() = default;

        void                updateArray(const CMat& array);

        VectorPairString    getStringList() const override;

    private:

        std::string         getTypeString() const;
        std::string         getSizeString() const;

        void                computeMinMax(const CMat& array);

    public:

        std::vector<size_t> m_sizes;
        int                 m_cvType = CV_32FC1;
        double              m_minValue = 0.0;
        double              m_maxValue = 1.0;
};

/** @endcond */

#endif // CDATAARRAYINFO_H
