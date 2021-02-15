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

#ifndef CFOLDERINPUT_H
#define CFOLDERINPUT_H

#include "DataProcessGlobal.hpp"
#include "Protocol/CProtocolTaskIO.h"

class DATAPROCESSSHARED_EXPORT CFolderInput : public CProtocolTaskIO
{
    public:

        CFolderInput();
        CFolderInput(const std::string& path);

        void        setPath(const std::string& path);

        std::string getPath() const;

        bool        isDataAvailable() const override;

        void        clearData() override;

        std::shared_ptr<CProtocolTaskIO>    clone() const;

    private:

         std::shared_ptr<CProtocolTaskIO>   cloneImp() const override;

    private:

         std::string m_path = "";
};

class DATAPROCESSSHARED_EXPORT CFolderInputFactory: public CProtocolTaskIOFactory
{
    public:

        CFolderInputFactory()
        {
            m_name = "CFolderInput";
        }

        virtual ProtocolTaskIOPtr   create(IODataType dataType)
        {
            Q_UNUSED(dataType);
            return std::make_shared<CFolderInput>();
        }
};

#endif // CFOLDERINPUT_H
