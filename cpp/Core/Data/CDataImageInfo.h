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

#ifndef CDATAIMAGEINFO_H
#define CDATAIMAGEINFO_H

#include "CDataInfo.h"
#include "Main/CoreGlobal.hpp"
#include "Data/CMat.hpp"

/** @cond INTERNAL */

class CORESHARED_EXPORT CDataImageInfo : public CDataInfo
{
    public:

        //Constructors
        CDataImageInfo();
        CDataImageInfo(IODataType type);
        CDataImageInfo(const std::string& fileName);
        CDataImageInfo(IODataType type, const std::string& fileName);
        CDataImageInfo(const CMat& image);
        CDataImageInfo(const CDataImageInfo& info);
        CDataImageInfo(CDataImageInfo&& info);

        //Destructor
        virtual ~CDataImageInfo();

        //Operators
        CDataImageInfo&             operator=(const CDataImageInfo& info);
        CDataImageInfo&             operator=(CDataImageInfo&& info);

        void                        setCvType(int getType);

        virtual VectorPairString    getStringList() const override;

        void                        updateImage(const CMat& image);

    private:

        std::string                 getTypeString() const;

        void                        computeMinMax(const CMat& image);

    public:

        int     m_width = 0;
        int     m_height = 0;
        int     m_depth = 0;
        int     m_nbBand = 0;
        int     m_nbBit = 0;
        int     m_bitMin = 0;
        int     m_bitMax = 0;
        int     m_cvType = CV_8U;
        double  m_minValue = 0;
        double  m_maxValue = 255;
};

/** @endcond */

#endif // CDATAIMAGEINFO_H
