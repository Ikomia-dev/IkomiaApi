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

#ifndef CDATAINFO_H
#define CDATAINFO_H

#include <string>
#include <memory>
#include "Main/CoreGlobal.hpp"
#include "Main/CoreDefine.hpp"

/** @cond INTERNAL */

class CORESHARED_EXPORT CDataInfo
{
    public:

        //Constructors
        CDataInfo(IODataType type);
        CDataInfo(IODataType type, const std::string& fileName);
        CDataInfo(const CDataInfo& data);
        CDataInfo(CDataInfo&& data);

        //Destructor
        virtual ~CDataInfo();

        //Operators
        CDataInfo&                  operator=(const CDataInfo& data);
        CDataInfo&                  operator=(CDataInfo&& data);

        //Setters
        void                        setFileName(const std::string& fileName);
        void                        setElementSize(int size);

        //Getters
        std::string                 getFileName() const;
        IODataType                  getType() const;
        int                         getElementSize() const;
        UMapString                  getMetadata() const;
        UMapString&                 metadata();
        virtual VectorPairString    getStringList() const;

    private:

        IODataType      m_type;
        std::string     m_fileName;
        UMapString      m_metadata;
        int             m_elementSize = 1;
};

using CDataInfoPtr = std::shared_ptr<CDataInfo>;

/** @endcond */

#endif // CDATAINFO_H
