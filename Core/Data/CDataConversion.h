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

#ifndef CDATACONVERSION_H
#define CDATACONVERSION_H

#include "opencv2/opencv.hpp"
#include <QImage>
#include "CMat.hpp"
#include "Main/CoreGlobal.hpp"

/** @cond INTERNAL */

class CORESHARED_EXPORT CDataConversion
{
    public:

        CDataConversion();

        static QImage   CMatToQImage(CMat imgSrc, bool bCopy = false);
        static void     grayscaleToAlpha(const CMat& src, CMat& dst);

    private:

        static void     to8Bits(const CMat& src, CMat& dst);
};

/** @endcond */

#endif // CDATACONVERSION_H
