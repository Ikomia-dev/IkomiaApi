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

#include "CImageDataManager.h"
#include "CMemoryInfo.h"

CImageDataManager::CImageDataManager()
{
}

CImageDataManager::~CImageDataManager()
{
}

void CImageDataManager::loadData(CDataset<CMat> &dataset)
{
    SubsetBounds bounds = dataset.whole();
    fillDataset(dataset, bounds, dataset.getDataInfo());
}

void CImageDataManager::loadData(CDataset<CMat> &dataset, const SubsetBounds &subsetBounds)
{
    assert(dataset.getDataInfo().dimensionsCount() == subsetBounds.size());

    if(dataset.subset().bounds() != subsetBounds)
    {
        if(dataset.subset().contains(subsetBounds) && dataset.subset().data().dimensionsCount() == subsetBounds.size())
            cropDataset(dataset, subsetBounds);
        else
        {
            auto subsetDataInfo = dataset.getDataInfo().subArray(subsetBounds);
            if(checkMemory(subsetDataInfo))
                fillDataset(dataset, subsetBounds, subsetDataInfo);
            else
                throw CException(CoreExCode::OUT_OF_MEMORY, "Not enough memory to load dataset", __func__, __FILE__, __LINE__);
        }
    }
}

void CImageDataManager::clearData()
{
    m_mapImageIO.clear();
}

bool CImageDataManager::isInMemory(CDataset<CMat> &dataset, const SubsetBounds &subsetBounds)
{
    if(dataset.subset().contains(subsetBounds))
        return true;

    return false;
}

void CImageDataManager::fillDataset(CDataset<CMat> &dataset, const SubsetBounds &subsetBounds, const CArrayDataInfo &subsetDataInfo)
{
    assert(subsetDataInfo.size() > 0);
    dataset.subset().clear();
    dataset.subset().setBounds(subsetBounds);

    if(subsetDataInfo.hasDimension(DataDimension::VOLUME))
    {
        // 3D management
        dataset.subset().setDimensions(dimensions(subsetBounds, DataDimension::IMAGE));
        size_t sizeZ = subsetDataInfo.size(DataDimension::IMAGE);
        assert(subsetDataInfo[0] != nullptr);
        CImageIO io(subsetDataInfo[0]->getFileName());
        CDataImageInfoPtr pInfo = std::dynamic_pointer_cast<CDataImageInfo>(io.dataInfo());
        assert(pInfo != nullptr);

        //Progress bar steps count
        emitSetProgressSteps(sizeZ * subsetDataInfo.size() / sizeZ);

        for(size_t i=0; i<subsetDataInfo.size(); i+=sizeZ)
        {
            CMat volume = CMat(pInfo->m_height, pInfo->m_width, (int)sizeZ, pInfo->m_cvType);
            for(size_t j=0; j<sizeZ; ++j)
            {
                std::string fileName = subsetDataInfo[i+j]->getFileName();
                auto imageIOPtr = getImageIO(fileName);

                if(imageIOPtr == nullptr)
                    throw CException(CoreExCode::NULL_POINTER, "Image reader unavailable", __func__, __FILE__, __LINE__);

                volume.setPlane(j, imageIOPtr->read(subsetBounds));
                emitDoProgress();
            }
            dataset.subset().addData(volume);
        }
    }
    else
    {
        // Single image management
        dataset.subset().setDimensions(dimensions(subsetBounds));
        //Progress bar steps count
        emitSetProgressSteps(subsetDataInfo.size());

        for(size_t i=0; i<subsetDataInfo.size(); ++i)
        {
            std::string fileName = subsetDataInfo[i]->getFileName();
            auto imageIOPtr = getImageIO(fileName);

            if(imageIOPtr == nullptr)
                throw CException(CoreExCode::NULL_POINTER, "Image reader unavailable", __func__, __FILE__, __LINE__);

            CMat data = imageIOPtr->read(subsetBounds);
            dataset.subset().addData(data);
            emitDoProgress();
        }
    }
}

void CImageDataManager::cropDataset(CDataset<CMat> &dataset, const SubsetBounds &subsetBounds)
{
    dataset.subset().setBounds(subsetBounds);
    if(dataset.subset().data().hasDimension(DataDimension::VOLUME))
    {
        //Crop subset
        SubsetBounds localBounds = dataset.subset().datasetToSubset(subsetBounds);
        if(localBounds.size() > 0)
        {
            //Remove image dimension stored into CMat
            localBounds.pop_back();
            dataset.subset().data().crop(localBounds);
        }

        //Crop CMat if necessary
        SubsetBounds imgBounds = dimensionBounds(dataset.subset().bounds(), DataDimension::IMAGE);
        SubsetBounds newImgBounds = dimensionBounds(subsetBounds, DataDimension::IMAGE);

        if(imgBounds != newImgBounds)
        {
            //Have to reshape 3D CMat
            cv::Range ranges[3];
            ranges[0] = cv::Range::all();
            ranges[1] = cv::Range::all();
            ranges[2] = cv::Range((int)newImgBounds[0].second.first, (int)newImgBounds[0].second.second);

            for(size_t i=0; i<dataset.subset().data().size(); ++i)
            {
                CMat subVolume = CMat(dataset.subset()[i], ranges).clone();
                dataset.subset()[i] = subVolume;
            }
        }
    }
    else
    {
        SubsetBounds localBounds = dataset.subset().datasetToSubset(subsetBounds);
        if(localBounds.size() > 0)
            dataset.subset().data().crop(localBounds);
    }
}

Dimensions CImageDataManager::dimensions(const SubsetBounds& bounds)
{
    Dimensions dims;
    for(size_t i=0; i<bounds.size(); ++i)
        dims.push_back(std::make_pair(bounds[i].first, bounds[i].second.second - bounds[i].second.first + 1));

    return dims;
}

Dimensions CImageDataManager::dimensions(const SubsetBounds &bounds, DataDimension groupBy)
{
    Dimensions dims;
    for(size_t i=0; i<bounds.size(); ++i)
    {
        if(bounds[i].first != groupBy)
            dims.push_back(std::make_pair(bounds[i].first, bounds[i].second.second - bounds[i].second.first + 1));
    }
    return dims;
}

SubsetBounds CImageDataManager::dimensionBounds(const SubsetBounds& bounds, DataDimension dimension)
{
    SubsetBounds dimBounds;
    for(size_t i=0; i<bounds.size(); ++i)
    {
        if(bounds[i].first == dimension)
        {
            dimBounds.push_back(std::make_pair(dimension, bounds[i].second));
            break;
        }
    }
    return dimBounds;
}

bool CImageDataManager::checkMemory(const CArrayDataInfo &arrayDataInfo)
{
    unsigned long long totalSize = 0;

    if(arrayDataInfo.hasDimension(DataDimension::VOLUME))
    {
        auto filename = arrayDataInfo[0]->getFileName();

        CImageIO io(filename);
        auto pInfo = std::static_pointer_cast<CDataImageInfo>(io.dataInfo());

        // Single or volume image
        size_t imageSize = pInfo->m_width * pInfo->m_height * pInfo->m_nbBand * pInfo->getElementSize();
        totalSize = arrayDataInfo.size() * imageSize;
    }
    else
    {
        for(size_t i=0; i<arrayDataInfo.size(); ++i)
        {
            assert(arrayDataInfo[i] != nullptr);
            auto filename = arrayDataInfo[i]->getFileName();

            CImageIO io(filename);
            auto pInfo = std::static_pointer_cast<CDataImageInfo>(io.dataInfo());

            // Single or volume image
            size_t imageSize = pInfo->m_width * pInfo->m_height * pInfo->m_nbBand * pInfo->getElementSize();
            totalSize += imageSize;
        }
    }

    //Test naif: à améliorer certainement
    CMemoryInfo memInfo;
    return (memInfo.availableMemory() > totalSize);
}

CImageIOPtr CImageDataManager::getImageIO(const std::string &fileName)
{
    auto it = m_mapImageIO.find(fileName);
    if(it != m_mapImageIO.end())
        return it->second;
    else
    {
        auto itNew = m_mapImageIO.emplace(std::make_pair(fileName, std::make_unique<CImageIO>(fileName)));
        return itNew.first->second;
    }
}
