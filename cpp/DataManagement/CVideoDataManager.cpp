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

#include "CVideoDataManager.h"
#include "CMemoryInfo.h"

CVideoDataManager::CVideoDataManager()
{
}

CVideoDataManager::~CVideoDataManager()
{
}

void CVideoDataManager::loadData(CDataset<CMat> &dataset)
{
    SubsetBounds bounds = dataset.whole();
    fillDataset(dataset, bounds, dataset.getDataInfo());
}

void CVideoDataManager::loadData(CDataset<CMat> &dataset, const SubsetBounds &subsetBounds)
{
    assert(dataset.getDataInfo().dimensionsCount() == subsetBounds.size());

    if(dataset.subset().bounds() != subsetBounds)
    {
        if(dataset.subset().contains(subsetBounds))
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

void CVideoDataManager::clearData()
{
    m_mapVideoIO.clear();
}

bool CVideoDataManager::isInMemory(CDataset<CMat> &dataset, const SubsetBounds &subsetBounds)
{
    if(dataset.subset().contains(subsetBounds))
        return true;

    return false;
}

void CVideoDataManager::fillDataset(CDataset<CMat> &dataset, const SubsetBounds &subsetBounds, const CArrayDataInfo &subsetDataInfo)
{
    assert(subsetDataInfo.size() > 0);
    dataset.subset().clear();
    dataset.subset().setBounds(subsetBounds);

    if(subsetDataInfo.hasDimension(DataDimension::TIME))
    {
        // Video management (image sequence and real video)
        dataset.subset().setDimensions(dimensions(subsetBounds));
        //Progress bar steps count
        emitSetProgressSteps(subsetDataInfo.size());

        for(size_t i=0; i<subsetDataInfo.size(); ++i)
        {
            std::string fileName = subsetDataInfo[i]->getFileName();
            m_pCurrentVideoIn = getVideoIO(fileName);

            if(m_pCurrentVideoIn == nullptr)
                throw CException(CoreExCode::NULL_POINTER, "Video reader unavailable", __func__, __FILE__, __LINE__);

            CMat data = m_pCurrentVideoIn->read(subsetBounds);
            dataset.subset().addData(data);
            emitDoProgress();
        }
    }
}

void CVideoDataManager::cropDataset(CDataset<CMat> &dataset, const SubsetBounds &subsetBounds)
{
    dataset.subset().setBounds(subsetBounds);
    SubsetBounds localBounds = dataset.subset().datasetToSubset(subsetBounds);
    if(localBounds.size() > 0)
        dataset.subset().data().crop(localBounds);
}

Dimensions CVideoDataManager::dimensions(const SubsetBounds& bounds)
{
    Dimensions dims;
    for(size_t i=0; i<bounds.size(); ++i)
        dims.push_back(std::make_pair(bounds[i].first, bounds[i].second.second - bounds[i].second.first + 1));

    return dims;
}

Dimensions CVideoDataManager::dimensions(const SubsetBounds &bounds, DataDimension groupBy)
{
    Dimensions dims;
    for(size_t i=0; i<bounds.size(); ++i)
    {
        if(bounds[i].first != groupBy)
            dims.push_back(std::make_pair(bounds[i].first, bounds[i].second.second - bounds[i].second.first + 1));
    }
    return dims;
}

SubsetBounds CVideoDataManager::dimensionBounds(const SubsetBounds& bounds, DataDimension dimension)
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

bool CVideoDataManager::checkMemory(const CArrayDataInfo &arrayDataInfo)
{
    unsigned long long totalSize = 0;

    if(arrayDataInfo.hasDimension(DataDimension::TIME))
    {
        auto filename = arrayDataInfo[0]->getFileName();

        // Check if cam or ip stream on linux platform
        if(isCamOrIPStreamLinux(filename))
            return true;

        // if not single or volume image, try video
        CVideoDataIO videoio(filename);
        auto pVideoInfo = std::static_pointer_cast<CDataVideoInfo>(videoio.dataInfo());
        assert(pVideoInfo != nullptr);
        size_t videoSize = pVideoInfo->m_width * pVideoInfo->m_height * pVideoInfo->m_nbBand * pVideoInfo->getElementSize() * pVideoInfo->m_frameCount;
        totalSize = arrayDataInfo.size() * videoSize;
    }

    //Test naif: à améliorer certainement
    CMemoryInfo memInfo;
    return (memInfo.availableMemory() > totalSize);
}

bool CVideoDataManager::isCamOrIPStreamLinux(const std::string& filename)
{
    bool bRet = filename.find("http") != std::string::npos;
    bool bRet1 = filename.find("/dev/video") != std::string::npos;

    return bRet | bRet1;
}

void CVideoDataManager::removeVideoIO(const std::string &filename)
{
    auto it = m_mapVideoIO.find(filename);
    if(it != m_mapVideoIO.end())
        m_mapVideoIO.erase(it);
}

void CVideoDataManager::setLiveMode(bool bLive)
{
    m_bLive = bLive;
}

void CVideoDataManager::setCurrentVideoIO(CDataset<CMat> &dataset)
{
    m_pCurrentVideoIn = getVideoIO(dataset.getDataInfo().at(0)->getFileName());
    if(m_pCurrentVideoIn == nullptr)
        return;
}

CVideoDataIOPtr CVideoDataManager::getVideoIO(const std::string &fileName)
{
    std::string videoPath = fileName;
    std::string extension = Utils::File::extension(fileName);

    if(!extension.empty() && !CDataVideoIO::isVideoFormat(extension, true))
        videoPath = CDataVideoIO::getImageSequenceInfo(fileName).first;

    auto it = m_mapVideoIO.find(videoPath);
    if(it != m_mapVideoIO.end())
        return it->second;
    else
    {
        if(m_bLive)
        {
            auto itNew = m_mapVideoIO.emplace(std::make_pair(videoPath, std::make_shared<CVideoDataIO>(true, fileName)));
            return itNew.first->second;
        }
        else
        {
            auto itNew = m_mapVideoIO.emplace(std::make_pair(videoPath, std::make_shared<CVideoDataIO>(fileName)));
            return itNew.first->second;
        }
    }
}

CDataVideoInfoPtr CVideoDataManager::getDataVideoInfoPtr()
{
    if(m_pCurrentVideoIn == nullptr)
        return nullptr;

    return std::dynamic_pointer_cast<CDataVideoInfo>(m_pCurrentVideoIn->dataInfo());
}

CDataVideoBuffer::Type CVideoDataManager::getSourceType() const
{
    if(!m_pCurrentVideoIn)
        return CDataVideoBuffer::NONE;

    auto infoPtr = std::static_pointer_cast<CDataVideoInfo>(m_pCurrentVideoIn->dataInfo());
    if(!infoPtr)
        return CDataVideoBuffer::NONE;

    return static_cast<CDataVideoBuffer::Type>(infoPtr->m_sourceType);
}

CMat CVideoDataManager::playVideo(CDataset<CMat> &dataset)
{
    auto datasetInfo = dataset.getDataInfo();
    m_pCurrentVideoIn = getVideoIO(datasetInfo[0]->getFileName());

    if(m_pCurrentVideoIn == nullptr)
        return CMat();

    auto bounds = dataset.subsetBounds(0);
    dataset.subset().clear();
    dataset.subset().setBounds(bounds);
    dataset.subset().setDimensions(dimensions(bounds));

    CMat data;
    if(m_bLive)
        data = m_pCurrentVideoIn->readLive();
    else
        data = m_pCurrentVideoIn->read();

    dataset.subset().addData(data);
    return data;
}

CMat CVideoDataManager::playVideo(CDataset<CMat>& dataset, const SubsetBounds& subsetBounds)
{
    m_pCurrentVideoIn = getVideoIO(dataset.getDataInfo().at(0)->getFileName());
    if(m_pCurrentVideoIn == nullptr)
        return CMat();

    auto bounds = dataset.subsetBounds(0);
    dataset.subset().clear();
    dataset.subset().setBounds(bounds);
    dataset.subset().setDimensions(dimensions(bounds));

    CMat data;
    if(m_bLive)
        data = m_pCurrentVideoIn->readLive(subsetBounds);
    else
        data = m_pCurrentVideoIn->read(subsetBounds);

    dataset.subset().addData(data);
    return data;
}

void CVideoDataManager::writeVideo(const CMat& image, const std::string& path)
{
    m_pCurrentVideoOut = getVideoIO(path);
    if(m_pCurrentVideoOut)
        m_pCurrentVideoOut->write(image);
}

void CVideoDataManager::stopWriteVideo()
{
    if(m_pCurrentVideoOut)
    {
        m_pCurrentVideoOut->stopWrite();
        auto filenames = m_pCurrentVideoOut->getFileNames(SubsetBounds());

        if(filenames.size() > 0)
            removeVideoIO(filenames[0]);
    }
}

void CVideoDataManager::stopReadVideo()
{
    if(m_pCurrentVideoIn)
        m_pCurrentVideoIn->stopRead();
}

void CVideoDataManager::waitWriteFinished()
{
    if(m_pCurrentVideoOut)
    {
        m_pCurrentVideoOut->waitWriteFinished();
        auto filenames = m_pCurrentVideoOut->getFileNames(SubsetBounds());

        if(filenames.size() > 0)
            removeVideoIO(filenames[0]);
    }
}

void CVideoDataManager::closeCamera()
{
    if(m_pCurrentVideoIn)
        m_pCurrentVideoIn->closeCamera();
}

