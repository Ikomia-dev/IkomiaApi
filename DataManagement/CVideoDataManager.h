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

#ifndef CVIDEODATAMANAGER_H
#define CVIDEODATAMANAGER_H

#include "CDataManager.hpp"
#include "DataManagementGlobal.hpp"
#include "CDataIO.hpp"

using CVideoIO = CDataIO<CDataVideoIO, CMat>;
using CVideoIOPtr = std::shared_ptr<CDataIO<CDataVideoIO, CMat>>;
using CDataVideoInfoPtr = std::shared_ptr<CDataVideoInfo>;

class DATAMANAGEMENTSHARED_EXPORT CVideoDataManager : public CDataManager<CMat>
{
    public:

        CVideoDataManager();

        virtual ~CVideoDataManager();

        virtual void        loadData(CDataset<CMat>& dataset);
        virtual void        loadData(CDataset<CMat>& dataset, const SubsetBounds& subsetBounds);
        virtual void        clearData();

        bool                isInMemory(CDataset<CMat>& dataset, const SubsetBounds& subsetBounds);

        void                setLiveMode(bool bLive);
        void                setCurrentVideoIO(CDataset<CMat>& dataset);

        CDataVideoInfoPtr   getDataVideoInfoPtr();
        CDataVideoBuffer::Type  getSourceType() const;

        CMat                playVideo(CDataset<CMat> &dataset);
        CMat                playVideo(CDataset<CMat> &dataset, const SubsetBounds& subsetBounds);

        void                writeVideo(const CMat& image, const std::string& path);

        void                stopWriteVideo();
        void                stopReadVideo();

        void                waitWriteFinished();

        void                closeCamera();

    private:

        CVideoIOPtr         getVideoIO(const std::string& fileName);

        void                fillDataset(CDataset<CMat>& dataset, const SubsetBounds &subsetBounds, const CArrayDataInfo& arrayDataInfo);

        void                cropDataset(CDataset<CMat>& dataset, const SubsetBounds &subsetBounds);

        Dimensions          dimensions(const SubsetBounds &bounds);
        Dimensions          dimensions(const SubsetBounds &bounds, DataDimension groupBy);
        SubsetBounds        dimensionBounds(const SubsetBounds& bounds, DataDimension dimension);

        bool                checkMemory(const CArrayDataInfo &arrayDataInfo);
        bool                isCamOrIPStreamLinux(const std::string& filename);

    private:

        using CMapFileVideoIO = std::unordered_map<std::string, CVideoIOPtr>;

        CMapFileVideoIO     m_mapVideoIO;
        CVideoIOPtr         m_pCurrentVideoIO = nullptr;
        bool                m_bLive = false;
};

#endif // CVIDEODATAMANAGER_H
