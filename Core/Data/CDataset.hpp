#ifndef CDATASET_HPP
#define CDATASET_HPP

#include <memory>
#include "CndArray.hpp"
#include "CDatasetInfo.h"
#include "CDataImageInfo.h"
#include "CDataVideoInfo.h"
#include "CDataSubset.hpp"

/** @cond INTERNAL */

using CArrayDataInfo = CndArray<CDataInfoPtr>;

template <typename T>
class CDataset
{
    public:

        //Constructors
        CDataset(const IODataType& type)
        {
            m_type = type;
            m_currentIndex = 0;
        }
        CDataset(const CDataset& dataset)
        {
            m_type = dataset.m_type;
            m_info = dataset.m_info;
            m_subset = dataset.m_subset;
            m_dataInfo = dataset.m_dataInfo;
            m_currentIndex = dataset.m_currentIndex;
        }
        CDataset(CDataset&& dataset)
        {
            m_type = std::move(dataset.m_type);
            m_info = std::move(dataset.m_info);
            m_subset = std::move(dataset.m_subset);
            m_dataInfo = std::move(dataset.m_dataInfo);
            m_currentIndex = std::move(dataset.m_currentIndex);
        }

        //Destructorr
        ~CDataset()
        {
        }

        std::shared_ptr<CDataset> clone() const
        {
            return std::make_shared<CDataset<T>>(*this);
        }

        //Operators
        CDataset&               operator=(const CDataset& dataset)
        {
            m_type = dataset.m_type;
            m_info = dataset.m_info;
            m_subset = dataset.m_subset;
            m_dataInfo = dataset.m_dataInfo;
            m_currentIndex = dataset.m_currentIndex;
            return *this;
        }
        CDataset&               operator=(CDataset&& dataset)
        {
            m_type = std::move(dataset.m_type);
            m_info = std::move(dataset.m_info);
            m_subset = std::move(dataset.m_subset);
            m_dataInfo = std::move(dataset.m_dataInfo);
            m_currentIndex = std::move(dataset.m_currentIndex);
            return *this;
        }
        CDataInfoPtr            operator[](size_t idx)
        {
            return m_dataInfo[idx];
        }
        const CDataInfoPtr      operator[](size_t idx) const
        {
            return m_dataInfo[idx];
        }

        //Getters        
        IODataType              getType() const
        {
            return m_type;
        }

        CDatasetInfo            getInfo() const
        {
            return m_info;
        }

        const CArrayDataInfo&   getDataInfo() const
        {
            return m_dataInfo;
        }

        size_t                  size() const
        {
            return m_dataInfo.size();
        }
        size_t                  size(DataDimension dim) const
        {
            return m_dataInfo.size(dim);
        }

        CDataSubset<T>&         subset()
        {
            return m_subset;
        }

        bool                    hasDimension(DataDimension dim) const
        {
            return m_dataInfo.hasDimension(dim);
        }

        //Setters        
        void                    setDimensions(const Dimensions& dims)
        {
            m_dataInfo.setDimensions(dims);
        }
        void                    setDimension(DataDimension dim, size_t count)
        {
            m_dataInfo.setDimension(dim, count);
        }

        void                    setType(IODataType type)
        {
            m_type = type;
        }

        //Accessors
        CDataInfoPtr            at(size_t index)
        {
            return m_dataInfo[index];
        }
        const CDataInfoPtr      at(size_t index) const
        {
            return m_dataInfo[index];
        }
        CDataInfoPtr            at(DimensionIndices indices)
        {
            return m_dataInfo.at(indices);
        }
        const CDataInfoPtr      at(DimensionIndices indices) const
        {
            return m_dataInfo.at(indices);
        }

        T&                      dataAt(size_t index)
        {
            size_t subsetIndex = datasetToSubsetIndex(index);
            if(subsetIndex == SIZE_MAX)
                throw CException(CoreExCode::INDEX_OVERFLOW, "Invalid data index", __func__, __FILE__, __LINE__);
            else
                return m_subset[subsetIndex];
        }
        const T&                dataAt(size_t index) const
        {
            size_t subsetIndex = datasetToSubsetIndex(index);
            if(subsetIndex == SIZE_MAX)
                throw CException(CoreExCode::INDEX_OVERFLOW, "Invalid data index", __func__, __FILE__, __LINE__);
            else
                return m_subset[subsetIndex];
        }
        T&                      dataAt(DimensionIndices indices)
        {
            size_t index = m_dataInfo.index(indices);
            return dataAt(index);
        }
        const T&                dataAt(DimensionIndices indices) const
        {
            size_t index = m_dataInfo.index(indices);
            return dataAt(index);
        }

        void                    insertDimension(DataDimension dim, size_t pos)
        {
            m_dataInfo.insertDimension(dim, pos);
        }
        void                    upDimension(DataDimension dim, size_t nb)
        {
            m_dataInfo.upDimension(dim, nb);
        }
        void                    downDimension(DataDimension dim, size_t nb)
        {
            m_dataInfo.downDimension(dim, nb);
        }
        void                    removeDimension(DataDimension dim, DimensionIndices indices)
        {
            size_t index = m_dataInfo.index(indices);
            if(index != SIZE_MAX)
            {
                if(m_currentIndex > 0 && m_currentIndex >= index)
                    m_currentIndex = index - 1;
            }
            m_dataInfo.removeDimension(dim, indices);
        }

        SubsetBounds            whole() const
        {
            SubsetBounds subsetBounds;
            Dimensions dims = m_dataInfo.dimensions();

            for(size_t i=0; i<dims.size(); ++i)
                subsetBounds.emplace_back(std::make_pair(dims[i].first, std::make_pair(0, dims[i].second - 1)));

            return subsetBounds;
        }
        SubsetBounds            subsetBounds(size_t index)
        {
            SubsetBounds bounds;

            auto indices = m_dataInfo.indices(index);
            for(size_t i=0; i<indices.size(); ++i)
                bounds.push_back(std::make_pair(indices[i].first, std::make_pair(indices[i].second, indices[i].second)));

            return bounds;
        }

        CndArray<CDataInfoPtr>  subsetDataInfo(SubsetBounds bounds) const
        {
            return m_dataInfo.subArray(bounds);
        }

        void                    appendFile(const std::string& fileName)
        {
            //Check if dataset is not already full
            if(m_currentIndex < m_dataInfo.size())
            {
                if(m_dataInfo[m_currentIndex] == nullptr)
                    m_dataInfo[m_currentIndex] = createDataInfoPtr(fileName);
                else
                    m_dataInfo[m_currentIndex].reset(createRawDataInfoPtr(fileName));

                m_currentIndex++;
            }
            else
                throw CException(CoreExCode::STRUCTURE_OVERFLOW, "Maximum dataset capacity reached", __func__, __FILE__, __LINE__);
        }

        void                    setCurrentIndex(const size_t index)
        {
            m_currentIndex = index;
        }

        void                    setFile(const DimensionIndices &indices, const std::string& fileName)
        {
            size_t index = m_dataInfo.index(indices);
            if(index < m_dataInfo.size())
            {
                if(m_dataInfo[index] == nullptr)
                    m_dataInfo[index] = createDataInfoPtr(fileName);
                else
                    m_dataInfo[index].reset(createRawDataInfoPtr(fileName));
            }
            else
                throw CException(CoreExCode::STRUCTURE_OVERFLOW, "Maximum dataset capacity reached", __func__, __FILE__, __LINE__);
        }
        void                    setFiles(const DimensionIndices &startIndices, const std::vector<std::string>& files)
        {
            size_t index = m_dataInfo.index(startIndices);
            if(index + files.size() < m_dataInfo.size())
            {
                for(size_t i=0; i<files.size(); ++i)
                {
                    if(m_dataInfo[index+i] == nullptr)
                        m_dataInfo[index+i] = createDataInfoPtr(files[i]);
                    else
                        m_dataInfo[index+i].reset(createRawDataInfoPtr(files[i]));
                }
            }
            else
                throw CException(CoreExCode::STRUCTURE_OVERFLOW, "Maximum dataset capacity reached", __func__, __FILE__, __LINE__);
        }

        void                    clear()
        {
            m_subset.clear();
            m_dataInfo.clear();
            m_currentIndex = 0;
        }

        void                    removeAt(size_t index)
        {
            m_dataInfo.removeAt(index);
            if(m_currentIndex > 0 && m_currentIndex >= index)
                m_currentIndex--;
        }
        void                    removeAt(DimensionIndices& indices)
        {
            removeAt(m_dataInfo.index(indices));
        }

        size_t                  datasetToSubsetIndex(size_t idx)
        {
            const DimensionIndices globalIndices = m_dataInfo.indices(idx);
            DimensionIndices subsetIndices = m_subset.datasetToSubset(globalIndices);

            if(subsetIndices.size() == 0)
                return SIZE_MAX;
            else
                return m_subset.data().index(subsetIndices);
        }
        size_t                  subsetToDatasetIndex(size_t idx)
        {
            Dimensions dims = m_dataInfo.dimensions();
            DimensionIndices subsetIndices = subset().data().indices(idx);
            DimensionIndices datasetIndices = m_subset.subsetToDataset(subsetIndices);

            for(size_t i=0; i<datasetIndices.size(); ++i)
                if(subsetIndices[i].second >= dims[i].second)
                    return SIZE_MAX;

            return m_dataInfo.index(datasetIndices);
        }

    private:

        CDataInfoPtr            createDataInfoPtr(std::string fileName)
        {
            CDataInfoPtr pInfo = nullptr;
            switch(m_type)
            {
                case IODataType::IMAGE:
                    pInfo = std::make_shared<CDataImageInfo>(fileName);
                    break;
                case IODataType::VIDEO:
                    pInfo = std::make_shared<CDataVideoInfo>(fileName);
                    break;
                default:
                    break;
            }
            return pInfo;
        }
        CDataInfo*              createRawDataInfoPtr(std::string fileName)
        {
            CDataInfo* pInfo = nullptr;
            switch(m_type)
            {
                case IODataType::IMAGE:
                    pInfo = new CDataImageInfo(fileName);
                    break;
                case IODataType::VIDEO:
                    pInfo = new CDataVideoInfo(fileName);
                    break;
                default: break;
            }
            return pInfo;
        }

    private:

        IODataType              m_type;
        CDatasetInfo            m_info;
        CDataSubset<T>          m_subset;
        CndArray<CDataInfoPtr>  m_dataInfo;
        size_t                  m_currentIndex;
};

/** @cond INTERNAL */

#endif // CDATASET_HPP
