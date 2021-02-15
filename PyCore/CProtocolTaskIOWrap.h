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

#ifndef CPROTOCOLTASKIOWRAP_HPP
#define CPROTOCOLTASKIOWRAP_HPP

#include "PyCoreGlobal.h"
#include "Protocol/CProtocolTaskIO.h"

class CProtocolTaskIOWrap : public CProtocolTaskIO, public wrapper<CProtocolTaskIO>
{
    public:

        CProtocolTaskIOWrap();
        CProtocolTaskIOWrap(IODataType dataType);
        CProtocolTaskIOWrap(const CProtocolTaskIO& io);

        virtual size_t  getUnitElementCount() const;
        size_t          default_getUnitElementCount() const;

        virtual bool    isDataAvailable() const;
        bool            default_isDataAvailable() const;

        virtual bool    isAutoInput() const;
        bool            default_isAutoInput() const;

        virtual void    clearData();
        void            default_clearData();

        virtual void    copyStaticData(const std::shared_ptr<CProtocolTaskIO>& ioPtr);
        void            default_copyStaticData(const std::shared_ptr<CProtocolTaskIO>& ioPtr);

};

#endif // CPROTOCOLTASKIOWRAP_HPP
