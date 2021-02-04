#ifndef CDATASUBSET_HPP
#define CDATASUBSET_HPP

#include <cassert>
#include "CndArray.hpp"

/** @cond INTERNAL */

/**
 * @brief CDataSubset template class: generic container for a subset of a CDataset
 * @details  This template class is used to store a subset of an original dataset.
 * It is composed of a CndArray to store data and the bounds information of each dimension relative to the associated dataset.
 */

template <typename T>
class CDataSubset
{
    public:

        //Constructors
        /**
         * @brief Default constructor: create empty subset.
         */
        CDataSubset()
        {
            m_currentIndex = 0;
        }
        /**
         * @brief Create an empty subset with dimensions and bounds information from associated dataset.
         * @param dims : list of dimensions to describe subset structure. See CoreDefine.hpp for details.
         * @param bounds : lower and upper limits for each dimension relative to the associated dataset. See CoreDefine.hpp for details.
         */
        CDataSubset(const Dimensions& dims, const SubsetBounds& bounds)
        {
            m_currentIndex = 0;
            m_subsetBounds = bounds;
            m_data.setDimensions(dims);
        }
        /**
         * @brief Copy constructor.
         */
        CDataSubset(const CDataSubset<T>& subset)
        {
            m_currentIndex = subset.m_currentIndex;
            m_subsetBounds = subset.m_subsetBounds;
            m_data = subset.m_data;
        }
        /**
         * @brief Universal reference copy constructor.
         */
        CDataSubset(CDataSubset<T>&& subset)
        {
            m_currentIndex = std::move(subset.m_currentIndex);
            m_subsetBounds = std::move(subset.m_subsetBounds);
            m_data = std::move(subset.m_data);
        }
        ~CDataSubset()
        {
        }

        //Operators
        /**
         * @brief Assignement operator.
         */
        CDataSubset&        operator=(const CDataSubset<T>& subset)
        {
            m_currentIndex = subset.m_currentIndex;
            m_subsetBounds = subset.m_subsetBounds;
            m_data = subset.m_data;
            return *this;
        }
        /**
         * @brief Universal reference assignement operator.
         */
        CDataSubset&        operator=(CDataSubset<T>&& subset)
        {
            m_currentIndex = std::move(subset.m_currentIndex);
            m_subsetBounds = std::move(subset.m_subsetBounds);
            m_data = std::move(subset.m_data);
            return *this;
        }

        /**
         * @brief Element-wise access operator.
         * @details Access to the element of the data array at index idx.
         * @param idx : index of the element in the data array. Important, idx is not the index in the associated dataset.
         * @return Reference of element.
         */
        T&                  operator[](size_t idx)
        {
            return m_data[idx];
        }
        /**
         * @brief Element-wise access operator.
         * @details Access to the element of the data array at index idx.
         * @param idx : index of the element in the data array. Important, idx is not the index in the associated dataset.
         * @return Const reference of element.
         */
        const T&            operator[](size_t idx) const
        {
            return m_data[idx];
        }

        //Setters        
        /**
         * @brief Set the dimensions of the subset.
         * @param dims : list of dimensions to describe subset structure. See CoreDefine.hpp for details.
         */
        void                setDimensions(const Dimensions& dims)
        {
            m_data.setDimensions(dims);
        }
        /**
         * @brief Set the bounds for all dimensions.
         * @param bounds: lower and upper limits for each dimension relative to the associated dataset. See CoreDefine.hpp for details.
         */
        void                setBounds(const SubsetBounds& bounds)
        {
            m_subsetBounds = bounds;
        }

        //Getters
        /**
         * @brief Get the dimension count of the subset
         * @return dimension count
         */
        size_t              dimensionCount() const
        {
            return m_subsetBounds.size();
        }
        /**
         * @brief Get the bounds for all dimensions of the subset.
         * @details This method returns a list of bounds according to the dimension count. Bounds are pair values, the first value is the lower limit and the second one is the upper limit.
         * Lower and upper limits are valid indexes from the associated dataset.
         * @return Reference of SubsetBounds. See CoreDefine.hpp for details.
         */
        SubsetBounds&       bounds()
        {
            return m_subsetBounds;
        }
        /**
         * @brief Get bounds for a specific dimension.
         * @details This method returns bounds of the given dimension. Bounds are pair values, the first value is the lower limit and the second one is the upper limit.
         * Lower and upper limits are valid indexes from the associated dataset.
         * @param dim : DataDimension. See CoreDefine.hpp for details.
         * @return DimensionBound. See CoreDefine.hpp for details.
         */
        DimensionBounds     bounds(DataDimension dim) const
        {
            for(size_t i=0; i<m_subsetBounds.size(); ++i)
            {
                if(m_subsetBounds[i].first == dim)
                    return m_subsetBounds[i];
            }
            return std::make_pair(dim, std::make_pair(-1.0, -1.0));
        }
        /**
         * @brief Get the number of elements in the subset.
         * @return Element count.
         */
        size_t              size() const
        {
            return m_data.size();
        }
        /**
         * @brief Get the data array
         * @return Reference of CndArray<T>
         */
        CndArray<T>&        data()
        {
            return m_data;
        }

        //Operations
        /**
         * @brief Clear data array
         */
        void                clear()
        {
            m_currentIndex = 0;
            m_data.clear();
        }
        /**
         * @brief Add element to subset
         * @details This method adds an element at the end of the subset. The size of the subset is determined by his bounds for each dimension, so it is fixed.
         * An exception (CoreExCode::STRUCTURE_OVERFLOW) is thrown when the maximum capacity of the subset is reached.
         * @param data : new element.
         */
        void                addData(T& data)
        {
            //Check if dataset is not already full
            if(m_currentIndex < m_data.size())
            {
                m_data[m_currentIndex] = data;
                m_currentIndex++;
            }
            else
                throw CException(CoreExCode::STRUCTURE_OVERFLOW, "Maximum dataset capacity reached", __func__, __FILE__, __LINE__);
        }
        /**
         * @brief This method tells whether the subset contains the data into the given bounds from the associated dataset.
         * @param subsetBounds: list of bounds for all dimension. See CoreDefine.hpp for details.
         * @return true if corresponding data is in the subset, false otherwise.
         */
        bool                contains(const SubsetBounds& subsetBounds) const
        {
            //Both subsets must have the same dimensions declaration order
            size_t size = std::min(m_subsetBounds.size(), subsetBounds.size());
            if(size == 0)
                return false;

            for(size_t i=0; i<size; ++i)
            {
                if( subsetBounds[i].second.first < m_subsetBounds[i].second.first ||
                        subsetBounds[i].second.first > m_subsetBounds[i].second.second ||
                        subsetBounds[i].second.second < m_subsetBounds[i].second.first ||
                        subsetBounds[i].second.second > m_subsetBounds[i].second.second)
                {
                    return false;
                }
            }
            return true;
        }
        /**
         * @brief Maps dimension bounds from associated dataset reference to subset reference.
         * @param datasetBounds: associated dataset dimension bounds.
         * @return SubsetBounds: subset dimension bounds.
         */
        SubsetBounds        datasetToSubset(const SubsetBounds& datasetBounds) const
        {
            //Maps datasetBounds from dataset reference to memory subset reference
            SubsetBounds localBounds;
            size_t size = std::min(m_subsetBounds.size(), datasetBounds.size());

            for(size_t i=0; i<size; ++i)
            {
                auto first = datasetBounds[i].second.first - m_subsetBounds[i].second.first;
                auto last = datasetBounds[i].second.second - m_subsetBounds[i].second.first;

                if(first > last)
                    return SubsetBounds();
                else
                    localBounds.push_back(std::make_pair(m_subsetBounds[i].first, std::make_pair(first, last)));
            }
            return localBounds;
        }
        /**
         * @brief Maps dimension indexes from associated dataset reference to subset reference.
         * @param datasetIndices: associated dataset dimension indexes.
         * @return CDimensionIndices: subset dimension indexes.
         */
        DimensionIndices    datasetToSubset(const DimensionIndices &datasetIndices) const
        {
            //Maps datasetIndices from dataset reference to memory subset reference
            DimensionIndices localIndices = Utils::Data::allocateDimensionIndices(m_data.dimensions());
            size_t size = std::min(localIndices.size(), datasetIndices.size());

            for(size_t i=0; i<size; ++i)
            {
                int idx = static_cast<int>(datasetIndices[i].second - m_subsetBounds[i].second.first);
                if(idx < 0 || idx >= static_cast<int>(m_data.size(datasetIndices[i].first)))
                    return DimensionIndices();
                else
                    localIndices[i] = std::make_pair(datasetIndices[i].first, idx);
            }
            return localIndices;
        }
        /**
         * @brief Maps dimension indexes from subset reference to associated dataset reference.
         * @param subsetIndices: subset dimension indexes.
         * @return CDimensionIndices: associated dataset dimension indexes.
         */
        DimensionIndices    subsetToDataset(const DimensionIndices& subsetIndices) const
        {
            //Maps subsetIndices from subset reference to dataset reference
            DimensionIndices datasetIndices = Utils::Data::allocateDimensionIndices(subsetIndices);
            size_t size = std::min(m_subsetBounds.size(), subsetIndices.size());

            for(size_t i=0; i<size; ++i)
                datasetIndices[i].second = subsetIndices[i].second + m_subsetBounds[i].second.first;

            return datasetIndices;
        }

    private:

        SubsetBounds        m_subsetBounds;
        CndArray<T>         m_data;
        size_t              m_currentIndex;
};

/** @endcond */

#endif // CDATASUBSET_HPP
