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
#include "Workflow/CWorkflowTaskIO.h"

class DATAPROCESSSHARED_EXPORT CPathIO : public CWorkflowTaskIO
{
    public:

        CPathIO();
        CPathIO(IODataType data);
        CPathIO(IODataType data, const std::string& path);
        CPathIO(IODataType data, const std::string& path, const std::string& name);
        CPathIO(const CPathIO& in);
        CPathIO(CPathIO&& in);

        virtual ~CPathIO() = default;

        CPathIO&    operator=(const CPathIO& in);
        CPathIO&    operator=(CPathIO&& in);

        void        setPath(const std::string& path);

        std::string getPath() const;

        bool        isDataAvailable() const override;
        bool        isAutoInput() const override;

        void        clearData() override;

        std::shared_ptr<CPathIO>    clone() const;

    private:

        virtual WorkflowTaskIOPtr   cloneImp() const override;

    private:

        std::string m_path;
};

class DATAPROCESSSHARED_EXPORT CPathIOFactory: public CWorkflowTaskIOFactory
{
    public:

        CPathIOFactory()
        {
            m_name = "CPathIO";
        }

        virtual WorkflowTaskIOPtr   create(IODataType dataType)
        {
            return std::make_shared<CPathIO>(dataType, m_name);
        }
};

#endif // CPATHIO_H
