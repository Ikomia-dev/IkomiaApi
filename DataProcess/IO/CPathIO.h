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

#ifndef CPATHIO_H
#define CPATHIO_H

#include "DataProcessGlobal.hpp"
#include "Protocol/CProtocolTaskIO.h"

class CFolderInput;

class DATAPROCESSSHARED_EXPORT CPathIO : public CProtocolTaskIO
{
    public:

        CPathIO();
        CPathIO(IODataType data);
        CPathIO(IODataType data, const std::string& path);
        CPathIO(const CFolderInput& in);

        virtual ~CPathIO() = default;

        void        setPath(const std::string& path);

        std::string getPath() const;

        bool        isDataAvailable() const override;
        bool        isAutoInput() const override;

        void        clearData() override;

        void        copy(const std::shared_ptr<CProtocolTaskIO> &ioPtr) override;

        std::shared_ptr<CProtocolTaskIO>            clone() const;

    private:

        virtual std::shared_ptr<CProtocolTaskIO>    cloneImp() const override;

    private:

        std::string m_path;
};

class DATAPROCESSSHARED_EXPORT CPathIOFactory: public CProtocolTaskIOFactory
{
    public:

        CPathIOFactory()
        {
            m_name = "CPathIO";
        }

        virtual ProtocolTaskIOPtr   create(IODataType dataType)
        {
            return std::make_shared<CPathIO>(dataType);
        }
};

#endif // CPATHIO_H
