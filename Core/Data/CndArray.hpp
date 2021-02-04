#ifndef CNDARRAY_HPP
#define CNDARRAY_HPP

#include <cassert>
#include "Main/CoreTools.hpp"


/** @cond INTERNAL */

/*!
 *Row major nd array
 */
template<typename T>
class CndArray
{
    public:

        //Constructors
        CndArray()
        {
        }
        CndArray(const Dimensions& dims)
        {
            m_dims = dims;
            allocate();
        }
        CndArray(const CndArray& array)
        {
            m_dims = array.m_dims;
            m_data = array.m_data;
            m_steps = array.m_steps;
        }
        CndArray(CndArray&& array)
        {
            m_dims = std::move(array.m_dims);
            m_data = std::move(array.m_data);
            m_steps = std::move(array.m_steps);
        }
        ~CndArray()
        {
        }

        //Operators
        CndArray&           operator=(const CndArray& array)
        {
            m_dims = array.m_dims;
            m_data = array.m_data;
            m_steps = array.m_steps;
            return *this;
        }
        CndArray&           operator=(CndArray&& array)
        {
            m_dims = std::move(array.m_dims);
            m_data = std::move(array.m_data);
            m_steps = std::move(array.m_steps);
            return *this;
        }
        T&                  operator[](size_t idx)
        {
            if(idx >= m_data.size())
                throw CException(CoreExCode::INDEX_OVERFLOW, "Index overflow", __func__, __FILE__, __LINE__);

            return m_data[idx];
        }
        const T&            operator[](size_t idx) const
        {
            if(idx >= m_data.size())
                throw CException(CoreExCode::INDEX_OVERFLOW, "Index overflow", __func__, __FILE__, __LINE__);

            return m_data[idx];
        }

        //Accessors
        T                   at(size_t idx)
        {
            if(idx >= m_data.size())
                throw CException(CoreExCode::INDEX_OVERFLOW, "Index overflow", __func__, __FILE__, __LINE__);

            return m_data[idx];
        }
        const T             at(size_t idx) const
        {
            if(idx >= m_data.size())
                throw CException(CoreExCode::INDEX_OVERFLOW, "Index overflow", __func__, __FILE__, __LINE__);

            return m_data[idx];
        }
        T                   at(DimensionIndices indices)
        {
            return at(index(indices));
        }
        const T             at(DimensionIndices indices) const
        {
            return at(index(indices));
        }

        //Setters
        void                setDimensions(Dimensions dims)
        {
            m_data.clear();
            m_dims = dims;
            allocate();
        }
        void                setDimension(DataDimension dim, size_t count)
        {
            bool bUpdate = false;
            for(size_t i=0; i<m_dims.size() && bUpdate == false; ++i)
            {
                if(m_dims[i].first == dim)
                {
                    long diff = (long)(count - m_dims[i].second);
                    if(diff < 0)
                        downDimension(dim, abs(diff));
                    else
                        upDimension(dim, diff);

                    bUpdate = true;
                }
            }

            if(!bUpdate)
            {
                m_dims.push_back(std::make_pair(dim, count));
                allocate();
            }
        }

        //Getters
        Dimensions          dimensions() const
        {
            return m_dims;
        }
        size_t              dimensionsCount() const
        {
            return m_dims.size();
        }
        size_t              dimensionPosition(DataDimension dim) const
        {
            for(size_t i=0; i<m_dims.size(); ++i)
            {
                if(m_dims[i].first == dim)
                    return i;
            }
            return SIZE_MAX;
        }

        size_t              size() const
        {
            return m_data.size();
        }
        size_t              size(DataDimension dim) const
        {
            return Utils::Data::getDimensionSize(m_dims, dim);
        }

        std::vector<size_t> steps() const
        {
            return m_steps;
        }

        CndArray<T>         subArray(const SubsetBounds &bounds) const
        {
            if(bounds.size() != m_dims.size())
                return CndArray<T>();
            else
            {
                Dimensions dims;
                for(size_t i=0; i<bounds.size(); ++i)
                    dims.push_back(std::make_pair(bounds[i].first, bounds[i].second.second - bounds[i].second.first + 1));

                CndArray<T> subArray(dims);
                size_t dstIndex = 0;
                size_t maxDepth = m_dims.size();
                std::vector<size_t> idx, startIdx, endIdx;

                for(size_t i=0; i<maxDepth; ++i)
                {
                    idx.push_back(bounds[i].second.first);
                    startIdx.push_back(idx[i]);
                    endIdx.push_back(bounds[i].second.second);
                }

                int currentDepth = static_cast<int>(maxDepth - 1);
                while(currentDepth >= 0)
                {
                    if(currentDepth == static_cast<int>(maxDepth - 1))
                    {
                        size_t srcIndex = 0;
                        for(size_t j=0; j<maxDepth; ++j)
                            srcIndex += m_steps[j] * idx[j];

                        for(size_t i=startIdx[currentDepth]; i<=endIdx[currentDepth]; ++i)
                        {
                            subArray[dstIndex] = m_data[srcIndex];
                            dstIndex++;
                            srcIndex++;
                        }
                        idx[currentDepth] = endIdx[currentDepth];
                    }

                    size_t currentIdx = idx[currentDepth];
                    currentIdx++;

                    if(currentIdx <= endIdx[currentDepth])
                    {
                        idx[currentDepth] = currentIdx;
                        //Move into next inner..loop
                        currentDepth++;
                    }
                    else
                    {
                        idx[currentDepth] = startIdx[currentDepth];
                        //Move out to the next outter..loop
                        currentDepth--;
                    }
                }
                return subArray;
            }
        }

        bool                hasDimension(DataDimension dim) const
        {
            for(size_t i=0; i<m_dims.size(); ++i)
            {
                if(m_dims[i].first == dim)
                    return true;
            }
            return false;
        }

        //Methods
        void                insertDimension(DataDimension dim, size_t pos=SIZE_MAX, size_t nb=1)
        {
            size_t dimIndex;
            bool bNewDimension = !hasDimension(dim);

            if(bNewDimension)
            {
                if(pos < m_dims.size())
                    m_dims.insert(m_dims.begin() + pos, std::make_pair(dim, 1));
                else
                    m_dims.push_back(std::make_pair(dim, 1));

                //First dimension added
                if(m_data.size() == 0)
                    m_data.push_back(T());

                computeSteps();
                dimIndex = m_dims.size() - 1;
                nb--;
            }
            else
                dimIndex = dimensionPosition(dim);

            if(nb > 0)
            {
                size_t nbInsertion = nb;
                for(size_t i=0; i<m_dims.size(); ++i)
                {
                    if(i != dimIndex)
                        nbInsertion *= m_dims[i].second;
                }

                size_t nbPerStep = m_steps[dimIndex] * nb;
                size_t step = m_steps[dimIndex] * m_dims[dimIndex].second;
                m_dims[dimIndex].second += nb;

                auto it = m_data.begin();
                if(pos >= size(dim))
                    step < m_data.size() ? it += step : it = m_data.end();
                else
                {
                    auto indices = Utils::Data::allocateDimensionIndices(m_dims);
                    indices[dimIndex].second = pos;
                    auto startIndex = index(indices);
                    startIndex < m_data.size() ? it += startIndex : it = m_data.end();
                }

                for(size_t i=0; i<nbInsertion; i+=nbPerStep)
                {
                    it = m_data.insert(it, nbPerStep, T());
                    size_t advanceCount = nbPerStep + step;

                    if(std::distance(m_data.begin(), it) + advanceCount < m_data.size())
                        it += advanceCount;
                    else
                        it = m_data.end();
                }
                computeSteps();
            }
        }

        void                upDimension(DataDimension dim, size_t nb)
        {
            insertDimension(dim, SIZE_MAX, nb);
        }

        void                downDimension(DataDimension dim, size_t nb)
        {
            size_t dimIndex = dimensionPosition(dim);
            if(dimIndex != SIZE_MAX && nb < m_dims[dimIndex].second)
            {
                size_t nbDeletion = nb;
                for(size_t i=0; i<m_dims.size(); ++i)
                {
                    if(i != dimIndex)
                        nbDeletion *= m_dims[i].second;
                }

                m_dims[dimIndex].second -= nb;
                size_t nbPerStep = m_steps[dimIndex] * nb;
                size_t step = m_steps[dimIndex] * m_dims[dimIndex].second;
                auto it = m_data.end();

                for(size_t i=0; i<nbDeletion; i+=nbPerStep)
                {
                    it = m_data.erase(it - nbPerStep, it);
                    it -= step;
                }
                computeSteps();
            }
        }

        size_t              index(DimensionIndices indices) const
        {
            size_t index = 0;
            size_t size = m_dims.size();
            assert(indices.size() == size);

            for(size_t i=0; i<size; ++i)
                index += m_steps[i] * indices[i].second;

            if(index >= m_data.size())
                return SIZE_MAX;
            else
                return index;
        }

        DimensionIndices    indices(size_t index)
        {
            if(index >= m_data.size())
                throw CException(CoreExCode::INDEX_OVERFLOW, "Index overflow", __func__, __FILE__, __LINE__);

            DimensionIndices indices(m_dims.size());
            for(size_t i=0; i<m_dims.size(); ++i)
            {
                indices[i].first = m_dims[i].first;
                indices[i].second = index / m_steps[i];
                index = index % m_steps[i];
            }
            return indices;
        }

        void                emplace_back(T& elt)
        {
            m_data.emplace_back(elt);
        }

        size_t              find(const T& elt)
        {
            auto it = std::find(m_data.begin(), m_data.end(), elt);
            if(it == m_data.end())
                return SIZE_MAX;
            else
                return std::distance(m_data.begin(), it);
        }

        void                clear()
        {
            m_data.clear();
            m_steps.clear();
            m_dims.clear();
        }

        void                removeAt(size_t idx)
        {
            if(idx >= m_data.size())
                throw CException(CoreExCode::INDEX_OVERFLOW, "Index overflow", __func__, __FILE__, __LINE__);

            if(m_dims.size() >= 2 && m_dims[m_dims.size() - 2].second == 1)
            {
                m_data.erase(m_data.begin() + idx);
                m_dims.back().second--;
                m_steps[m_steps.size() - 2]--;
            }
            else
            {
                DimensionIndices firstIndices = indices(idx);
                firstIndices.back().second = 0;
                size_t firstIdx = index(firstIndices);
                size_t lastIdx = firstIdx + size(m_dims.back().first);
                std::rotate(m_data.begin()+idx, m_data.begin()+idx+1, m_data.begin()+lastIdx);
                m_data[lastIdx - 1] = T();
            }
        }
        void                removeAt(const DimensionIndices &indices)
        {
            assert(indices.size() == m_dims.size());

            if(m_dims.size() >= 2 && m_dims[m_dims.size() - 2].second == 1)
            {
                size_t idx = index(indices);
                m_data.erase(m_data.begin() + idx);
                m_dims.back().second--;
                m_steps[m_steps.size() - 2]--;
            }
            else
            {
                DimensionIndices firstIndices = indices;
                firstIndices.back().second = 0;
                size_t firstIdx = index(firstIndices);
                size_t idx = index(indices);
                size_t lastIdx = firstIdx + size(m_dims.back().first);
                std::rotate(m_data.begin()+idx, m_data.begin()+idx+1, m_data.begin()+lastIdx);
                m_data[lastIdx - 1] = T();
            }
        }
        void                removeDimension(DataDimension dim, DimensionIndices indices)
        {
            assert(indices.size() == m_dims.size());
            indices.back().second = 0;
            size_t dimIndex = dimensionPosition(dim);

            if(dimIndex == SIZE_MAX)
                throw CException(CoreExCode::INVALID_DIMENSION, "Dimension does not exist", __func__, __FILE__, __LINE__);

            if(dimIndex == 0 && m_dims[dimIndex].second == 1)
            {
                //Clear all
                m_dims.clear();
                m_data.clear();
                m_steps.clear();
            }
            else if(dimIndex == 0 || (dimIndex > 0 && m_dims[dimIndex - 1].second == 1))
            {
                //Delete data and update structure
                size_t firstIndex = index(indices);
                if(firstIndex != SIZE_MAX)
                {
                    size_t count = 1;
                    for(size_t i=dimIndex+1; i<m_dims.size(); ++i)
                        count *= m_dims[i].second;

                    m_data.erase(m_data.begin() + firstIndex, m_data.begin() + firstIndex + count);

                    if(m_dims[dimIndex].second == 1)
                        m_dims.erase(m_dims.begin() + dimIndex);
                    else
                        m_dims[dimIndex].second--;

                    computeSteps();
                }
            }
            else
            {
                //Clear data, structure don't change
                size_t firstIndex = index(indices);
                if(firstIndex != SIZE_MAX)
                {
                    size_t count = 1;
                    for(size_t i=dimIndex+1; i<m_dims.size(); ++i)
                        count *= m_dims[i].second;

                    for(size_t i=firstIndex; i<firstIndex+count; ++i)
                        m_data[i] = T();
                }
            }
        }

        void                crop(const SubsetBounds &bounds)
        {
            if(bounds.size() != m_dims.size())
                throw CException(CoreExCode::INVALID_SIZE, "Dimensions number must be the same", __func__, __FILE__, __LINE__);

            CndArray<T> subArray = this->subArray(bounds);
            this->m_dims = subArray.m_dims;
            this->m_steps = subArray.m_steps;
            this->m_data = subArray.m_data;
        }

    private:

        void                allocate()
        {
            size_t size = 1;
            for(size_t i=0; i<m_dims.size(); ++i)
                size *= m_dims[i].second;

            if(m_data.size() != size)
                m_data.resize(size, T());

            computeSteps();
        }
        void                computeSteps()
        {
            m_steps.clear();
            for(size_t i=0; i<m_dims.size(); ++i)
            {
                size_t step = 1;
                for(size_t j=i+1; j<m_dims.size(); ++j)
                    step *= m_dims[j].second;

                m_steps.push_back(step);
            }
        }

    private:

        Dimensions          m_dims;
        std::vector<T>      m_data;
        std::vector<size_t> m_steps;
};

/** @cond INTERNAL */

#endif // CNDARRAY_HPP
