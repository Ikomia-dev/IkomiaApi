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

#ifndef CVIDEOIOWRAP_H
#define CVIDEOIOWRAP_H

#include "PyDataProcessGlobal.h"
#include "IO/CVideoIO.h"

class CVideoIOWrap : public CVideoIO, public wrapper<CVideoIO>
{
    public:

        CVideoIOWrap();
        CVideoIOWrap(const CMat& image);
        CVideoIOWrap(IODataType dataType);
        CVideoIOWrap(IODataType dataType, const CMat& image);
        CVideoIOWrap(const CVideoIO &io);

        virtual size_t  getUnitElementCount() const;
        size_t          default_getUnitElementCount() const;

        virtual CMat    getImage() const;
        CMat            default_getImage() const;

        virtual bool    isDataAvailable() const;
        bool            default_isDataAvailable() const;

        virtual void    clearData();
        void            default_clearData();

        virtual void    copyStaticData(const std::shared_ptr<CWorkflowTaskIO>& ioPtr);
        void            default_copyStaticData(const std::shared_ptr<CWorkflowTaskIO>& ioPtr);
};

#endif // CVIDEOIOWRAP_H
