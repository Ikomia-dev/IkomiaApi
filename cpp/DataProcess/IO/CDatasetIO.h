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

#ifndef CDATASETIO_H
#define CDATASETIO_H

#include <QObject>
#include <QFile>
#include <QJsonDocument>
#include "DataProcessGlobal.hpp"
#include "Main/CoreGlobal.hpp"
#include "Workflow/CWorkflowTaskIO.h"
#include "Graphics/CGraphicsItem.hpp"

class DATAPROCESSSHARED_EXPORT CDatasetIO: public CWorkflowTaskIO
{
    public:

        CDatasetIO();
        CDatasetIO(const std::string& name);
        CDatasetIO(const std::string& name, const std::string& srcFormat);

        virtual ~CDatasetIO() = default;

        std::string                                 getSourceFormat() const;
        virtual std::vector<std::string>            getImagePaths() const;
        virtual MapIntStr                           getCategories() const;
        virtual int                                 getCategoryCount() const;
        virtual std::vector<ProxyGraphicsItemPtr>   getAnnotationGraphics(const std::string &imgPath) const;
        virtual std::string                         getMaskPath(const std::string& imgPath) const;
        QJsonDocument                               getJsonDocument() const;

        bool                                        isDataAvailable() const override;
        bool                                        isAutoInput() const override;

        void                                        clearData();

        void                                        save(const std::string& path) override;

        bool                                        load(const std::string& path);

    private:

        std::string                     m_srcFormat;
        QJsonDocument                   m_json;
        static std::set<std::string>    m_formats;
};

#endif // CDATASETIO_H
