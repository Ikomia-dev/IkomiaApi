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

#ifndef CIMAGEDATAMANAGER_H
#define CIMAGEDATAMANAGER_H

#include "CDataManager.hpp"
#include "CDataIO.hpp"
#include "DataManagementGlobal.hpp"

//Register log category
Q_DECLARE_LOGGING_CATEGORY(LogImageDataManager)

using CImageIO = CDataIO<CDataImageIO, CMat>;
using CImageIOPtr = std::shared_ptr<CDataIO<CDataImageIO, CMat>>;
using CDataImageInfoPtr = std::shared_ptr<CDataImageInfo>;

class DATAMANAGEMENTSHARED_EXPORT CImageDataManager : public CDataManager<CMat>
{
    public:

        CImageDataManager();

        virtual ~CImageDataManager();

        virtual void        loadData(CDataset<CMat>& dataset);
        virtual void        loadData(CDataset<CMat>& dataset, const SubsetBounds& subsetBounds);
        virtual void        clearData();

        bool                isInMemory(CDataset<CMat>& dataset, const SubsetBounds& subsetBounds);

    private:

        CImageIOPtr         getImageIO(const std::string& fileName);

        void                fillDataset(CDataset<CMat>& dataset, const SubsetBounds &subsetBounds, const CArrayDataInfo& arrayDataInfo);

        void                cropDataset(CDataset<CMat>& dataset, const SubsetBounds &subsetBounds);

        Dimensions          dimensions(const SubsetBounds &bounds);
        Dimensions          dimensions(const SubsetBounds &bounds, DataDimension groupBy);
        SubsetBounds        dimensionBounds(const SubsetBounds& bounds, DataDimension dimension);

        bool                checkMemory(const CArrayDataInfo &arrayDataInfo);

    private:

        using CMapFileImageIO = std::unordered_map<std::string, CImageIOPtr>;

        CMapFileImageIO     m_mapImageIO;
};

#endif // CIMAGEDATAMANAGER_H
