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

#ifndef CDATASETIOWRAP_H
#define CDATASETIOWRAP_H

#include "PyDataProcessGlobal.h"
#include "IO/CDatasetIO.h"


class CDatasetIOWrap : public CDatasetIO, public wrapper<CDatasetIO>
{
    public:

        CDatasetIOWrap();
        CDatasetIOWrap(const std::string& name);
        CDatasetIOWrap(const std::string& name, const std::string& sourceFormat);

        std::vector<std::string>            getImagePaths() const override;
        std::vector<std::string>            default_getImagePaths() const;
        MapIntStr                           getCategories() const override;
        MapIntStr                           default_getCategories() const;
        int                                 getCategoryCount() const override;
        int                                 default_getCategoryCount() const;
        std::vector<ProxyGraphicsItemPtr>   getGraphicsAnnotations(const std::string &imgPath) const override;
        std::vector<ProxyGraphicsItemPtr>   default_getGraphicsAnnotations(const std::string &imgPath) const;
        std::string                         getMaskPath(const std::string& imgPath) const override;
        std::string                         default_getMaskPath(const std::string& imgPath) const;

        bool                                isDataAvailable() const override;
        bool                                default_isDataAvailable() const;

        void                                clearData() override;
        void                                default_clearData();

        void                                save(const std::string& path) override;
        void                                default_save(const std::string& path);
};

#endif // CDATASETIOWRAP_H
