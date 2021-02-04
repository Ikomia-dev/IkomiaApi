#ifndef __CMAT_HPP__
#define __CMAT_HPP__

#include <opencv2/opencv.hpp>
#include "CException.h"

/** @cond INTERNAL */

/**
 * @brief
 *
 */
class CMat : public cv::Mat
{
     public:

        CMat() : Mat() {}
        CMat(int rows, int cols, int type) : Mat(rows, cols, type) {}
        CMat(cv::Size size, int type) : Mat(size, type) {}
        CMat(int rows, int cols, int type, const cv::Scalar& s) : Mat(rows, cols, type, s) {}
        CMat(cv::Size size, int type, const cv::Scalar& s) : Mat(size, type, s) {}
        CMat(int ndims, const int* sizes, int type) : Mat(ndims, sizes, type) {}
        CMat(int ndims, const int* sizes, int type, const cv::Scalar& s) : Mat(ndims, sizes, type, s) {}
        CMat(const CMat& m) : Mat(m) {}
        CMat(const Mat& m) : Mat(m) {}
        CMat(int rows, int cols, int type, void* data, size_t step=AUTO_STEP) : Mat(rows, cols, type, data, step) {}
        CMat(cv::Size size, int type, void* data, size_t step=AUTO_STEP) : Mat(size, type, data, step) {}
        CMat(int ndims, const int* sizes, int type, void* data, const size_t* steps=0) : Mat(ndims, sizes, type, data, steps) {}
        CMat(const CMat& m, const cv::Range& rowRange, const cv::Range& colRange=cv::Range::all()) : Mat(m, rowRange, colRange) {}
        CMat(const Mat& m, const cv::Range& rowRange, const cv::Range& colRange=cv::Range::all()) : Mat(m, rowRange, colRange) {}
        CMat(const CMat& m, const cv::Rect& roi) : Mat(m, roi) {}
        CMat(const Mat& m, const cv::Rect& roi) : Mat(m, roi) {}
        CMat(const CMat& m, const cv::Range* ranges) : Mat(m, ranges) {}
        CMat(const Mat& m, const cv::Range* ranges) : Mat(m, ranges) {}

        template<typename _Tp>
        explicit CMat(const std::vector<_Tp>& vec, bool copyData=false) : Mat(vec, copyData) {}

        template<typename _Tp, int n>
        explicit CMat(const cv::Vec<_Tp, n>& vec, bool copyData=true) : Mat(vec, copyData) {}

        template<typename _Tp, int m, int n>
        explicit CMat(const cv::Matx<_Tp, m, n>& mtx, bool copyData=true) : Mat(mtx, copyData) {}

        template<typename _Tp>
        explicit CMat(const cv::Point_<_Tp>& pt, bool copyData=true) : Mat(pt,copyData) {}

        template<typename _Tp>
        explicit CMat(const cv::Point3_<_Tp>& pt, bool copyData=true) : Mat(pt, copyData) {}

        template<typename _Tp>
        explicit CMat(const cv::MatCommaInitializer_<_Tp>& commaInitializer) : Mat(commaInitializer) {}

        /**
         * @brief
         *
         * @param rows
         * @param cols
         * @param stacks
         * @param type
         */
        CMat(int rows, int cols, int stacks, int type) : Mat()
        {
            int sz[3] = {(int)rows, (int)cols, (int)stacks};
            create(3, sz, type);
        }
        /**
         * @brief
         *
         * @param rows
         * @param cols
         * @param stacks
         * @param type
         * @param s
         */
        CMat(int rows, int cols, int stacks, int type, const cv::Scalar& s) : Mat()
        {
            int sz[3] = {(int)rows, (int)cols, (int)stacks};
            create(3, sz, type);
            CMat Tmp(3, sz, type, s);
            *this = Tmp;
        }

        CMat& operator = (const CMat& m)
        {
            cv::Mat::operator =(m);
            return *this;
        }
        CMat& operator = (const Mat& m)
        {
            cv::Mat::operator =(m);
            return *this;
        }
        CMat& operator = (const cv::MatExpr& expr)
        {
            cv::Mat::operator =(expr);
            return *this;
        }

        /**
         * @brief
         *
         * @param rows
         * @param cols
         * @param stacks
         * @param type
         * @return cv::MatExpr
         */
        static cv::MatExpr zeros(int rows, int cols, int stacks, int type)
        {
            int sz[3] = {rows, cols, stacks};
            return Mat::zeros(3, sz, type);
        }

        /**
         * @brief
         *
         * @param rows
         * @param cols
         * @param stacks
         * @param type
         * @return cv::MatExpr
         */
        static cv::MatExpr ones(int rows, int cols, int stacks, int type)
        {
            int sz[3] = {rows, cols, stacks};
            return Mat::ones(3, sz, type);
        }

        /**
         * @brief
         *
         * @param pRows
         * @param pCols
         * @param pStacks
         */
        void	getSize(size_t* pRows, size_t* pCols, size_t* pStacks) const
        {
            *pRows = this->size[0];
            *pCols = this->size[1];
            *pStacks = this->size[2];
        }

        /**
         * @brief
         *
         * @return size_t
         */
        size_t	getNbRows() const
        {
            return this->size[0];
        }
        /**
         * @brief
         *
         * @return size_t
         */
        size_t	getNbCols() const
        {
            return this->size[1];
        }
        /**
         * @brief
         *
         * @return size_t
         */
        size_t	getNbStacks() const
        {
            if(this->dims < 3)
                return 1;
            else
                return this->size[2];
        }
        /**
         * @brief
         *
         * @param id
         * @return CMat
         */
        CMat	getPlane(size_t index) const
        {
            if(this->data == nullptr)
                throw CException(CoreExCode::NULL_POINTER, "Invalid image plane", __func__, __FILE__, __LINE__);

            if(index >= getNbStacks())
                throw CException(CoreExCode::INVALID_SIZE, "Invalid z index", __func__, __FILE__, __LINE__);

            int nRows = this->size[0];
            int nCols = this->size[1];
            void* pData = this->data + index*nRows*nCols*this->elemSize();
            return CMat(nRows, nCols, type(), pData);
        }

        /**
         * @brief
         *
         * @param index
         * @param mat2D
         */
        void    setPlane(size_t index, const CMat& mat2D)
        {
            assert(this->data != nullptr);
            assert((int)index < this->size[2]);

            if(mat2D.data == nullptr)
                throw CException(CoreExCode::NULL_POINTER, "Invalid 2D CMat", __func__, __FILE__, __LINE__);

            int srcRows = mat2D.size[0];
            int srcCols = mat2D.size[1];
            int dstRows = this->size[0];
            int dstCols = this->size[1];

            if(dstRows == srcRows && dstCols == srcCols && elemSize() == mat2D.elemSize())
            {
                void* pDst = this->data + index * dstRows * dstCols * elemSize();
                memcpy(pDst, mat2D.data, srcRows * srcCols * elemSize());
            }
            else
                throw CException(CoreExCode::INVALID_SIZE, "Invalid 2D CMat sizes", __func__, __FILE__, __LINE__);
        }
};

/**
 * @brief
 *
 */
template<typename _Tp>
class CMat_ : public cv::Mat_<_Tp>
{
    public:

        typedef _Tp value_type;
        typedef typename cv::DataType<_Tp>::channel_type channel_type;
        typedef cv::MatIterator_<_Tp> iterator;
        typedef cv::MatConstIterator_<_Tp> const_iterator;

        //! default constructor
        /**
         * @brief
         *
         */
        CMat_() : cv::Mat_<_Tp>() {}
        //! equivalent to Mat(_rows, _cols, DataType<_Tp>::type)
        /**
         * @brief
         *
         * @param _rows
         * @param _cols
         */
        CMat_(int _rows, int _cols) : cv::Mat_<_Tp>(_rows, _cols) {}
        //! constructor that sets each matrix element to specified value
        /**
         * @brief
         *
         * @param _rows
         * @param _cols
         * @param value
         */
        CMat_(int _rows, int _cols, const _Tp& value) : cv::Mat_<_Tp>(_rows, _cols, value) {}
        //! equivalent to Mat(_size, DataType<_Tp>::type)
        /**
         * @brief
         *
         * @param _size
         */
        explicit CMat_(cv::Size _size) : cv::Mat_<_Tp>(_size) {}
        //! constructor that sets each matrix element to specified value
        /**
         * @brief
         *
         * @param _size
         * @param value
         */
        CMat_(cv::Size _size, const _Tp& value) : cv::Mat_<_Tp>(_size, value) {}
        //! n-dim array constructor
        /**
         * @brief
         *
         * @param _ndims
         * @param _sizes
         */
        CMat_(int _ndims, const int* _sizes) : cv::Mat_<_Tp>(_ndims, _sizes) {}
        //! n-dim array constructor that sets each matrix element to specified value
        /**
         * @brief
         *
         * @param _ndims
         * @param _sizes
         * @param value
         */
        CMat_(int _ndims, const int* _sizes, const _Tp& value) : cv::Mat_<_Tp>(_ndims, _sizes, value) {}
        //! copy/conversion contructor. If m is of different type, it's converted
        /**
         * @brief
         *
         * @param m
         */
        CMat_(const CMat& m) : cv::Mat_<_Tp>(m) {}
        //! copy constructor
        /**
         * @brief
         *
         * @param m
         */
        CMat_(const CMat_<_Tp>& m) : cv::Mat_<_Tp>(m) {}
        //! constructs a matrix on top of user-allocated data. step is in bytes(!!!), regardless of the type
        /**
         * @brief
         *
         * @param _rows
         * @param _cols
         * @param _data
         * @param _step
         */
        CMat_(int _rows, int _cols, _Tp* _data, size_t _step=CMat::AUTO_STEP) : cv::Mat_<_Tp>(_rows, _cols, _data, _step) {}
        //! constructs n-dim matrix on top of user-allocated data. steps are in bytes(!!!), regardless of the type
        /**
         * @brief
         *
         * @param _ndims
         * @param _sizes
         * @param _data
         * @param _steps
         */
        CMat_(int _ndims, const int* _sizes, _Tp* _data, const size_t* _steps=0) : cv::Mat_<_Tp>(_ndims, _sizes, _data, _steps) {}
        //! selects a submatrix
        /**
         * @brief
         *
         * @param m
         * @param rowRange
         * @param colRange
         */
        CMat_(const CMat_<_Tp>& m, const cv::Range& rowRange, const cv::Range& colRange=cv::Range::all()) : cv::Mat_<_Tp>(m, rowRange, colRange) {}
        //! selects a submatrix
        /**
         * @brief
         *
         * @param m
         * @param roi
         */
        CMat_(const CMat_<_Tp>& m, const cv::Rect& roi) : cv::Mat_<_Tp>(m, roi) {}
        //! selects a submatrix, n-dim version
        /**
         * @brief
         *
         * @param m
         * @param ranges
         */
        CMat_(const CMat_<_Tp>& m, const cv::Range* ranges) : cv::Mat_<_Tp>(m, ranges) {}

        //! from a matrix expression
        /**
         * @brief
         *
         * @param e
         */
        explicit CMat_(const cv::MatExpr& e) : cv::Mat_<_Tp>(e) {}
        //! makes a matrix out of Vec, std::vector, Point_ or Point3_. The matrix will have a single column
        /**
         * @brief
         *
         * @param vec
         * @param copyData
         */
        explicit CMat_(const std::vector<_Tp>& vec, bool copyData=false) : cv::Mat_<_Tp>(vec, copyData) {}
        /**
         * @brief
         *
         * @param cv::Vec<cv::DataType<_Tp>::channel_type
         * @param vec
         * @param copyData
         */
        template<int n> explicit CMat_(const cv::Vec<typename cv::DataType<_Tp>::channel_type, n>& vec, bool copyData=true) : cv::Mat_<_Tp>(vec, copyData) {}
        /**
         * @brief
         *
         * @param cv::Matx<cv::DataType<_Tp>::channel_type
         * @param m
         * @param mtx
         * @param copyData
         */
        template<int m, int n> explicit CMat_(const cv::Matx<typename cv::DataType<_Tp>::channel_type, m, n>& mtx, bool copyData=true) : cv::Mat_<_Tp>(mtx, copyData) {}
        /**
         * @brief
         *
         * @param pt
         * @param copyData
         */
        explicit CMat_(const cv::Point_<typename cv::DataType<_Tp>::channel_type>& pt, bool copyData=true) : cv::Mat_<_Tp>(pt, copyData) {}
        /**
         * @brief
         *
         * @param pt
         * @param copyData
         */
        explicit CMat_(const cv::Point3_<typename cv::DataType<_Tp>::channel_type>& pt, bool copyData=true) : cv::Mat_<_Tp>(pt, copyData) {}
        /**
         * @brief
         *
         * @param commaInitializer
         */
        explicit CMat_(const cv::MatCommaInitializer_<_Tp>& commaInitializer) : cv::Mat_<_Tp>(commaInitializer) {}

        /**
         * @brief
         *
         * @param rows
         * @param cols
         * @param stacks
         */
        CMat_(int rows, int cols, int stacks) : cv::Mat_<_Tp>()
        {
            int sz[3] = {rows, cols, stacks};
            cv::Mat_<_Tp>::create(3, sz);
        }
        /**
         * @brief
         *
         * @param rows
         * @param cols
         * @param stacks
         * @param value
         */
        CMat_(int rows, int cols, int stacks, const _Tp& value) : cv::Mat_<_Tp>()
        {
            int sz[3] = {rows, cols, stacks};
            cv::Mat_<_Tp>::create(3, sz);
            CMat_<_Tp> Tmp(3, sz, value);
            *this = Tmp;
        }

        /**
         * @brief
         *
         * @param rows
         * @param cols
         * @param stacks
         * @return CMat_
         */
        static CMat_ zeros(int rows, int cols, int stacks)
        {
            int sz[3] = {rows, cols, stacks};
            return static_cast<CMat_<_Tp>>(cv::Mat::zeros(3, sz, cv::DataType<_Tp>::type));
        }

        /**
         * @brief
         *
         * @param rows
         * @param cols
         * @param stacks
         * @return CMat_
         */
        static CMat_ ones(int rows, int cols, int stacks)
        {
            int sz[3] = {rows, cols, stacks};
            return static_cast<CMat_<_Tp>>(cv::Mat::ones(3, sz, cv::DataType<_Tp>::type));
        }

        /**
         * @brief
         *
         * @param pRows
         * @param pCols
         * @param pStacks
         */
        void	getSize(size_t* pRows, size_t* pCols, size_t* pStacks) const
        {
            *pRows = this->size[0];
            *pCols = this->size[1];
            *pStacks = this->size[2];
        }
        /**
         * @brief
         *
         * @return size_t
         */
        size_t	getNbRows() const
        {
            return this->size[0];
        }
        /**
         * @brief
         *
         * @return size_t
         */
        size_t	getNbCols() const
        {
            return this->size[1];
        }
        /**
         * @brief
         *
         * @return size_t
         */
        size_t	getNbStacks() const
        {
            return this->size[2];
        }

        /**
         * @brief
         *
         * @param id
         * @return CMat_<_Tp>
         */
        CMat_<_Tp>  getPlane(size_t id) const
        {
            if(id >= getNbStacks())
                throw CException(CoreExCode::INVALID_SIZE, "Invalid z index", __func__, __FILE__, __LINE__);

            int nRows = this->size[0];
            int nCols = this->size[1];
            _Tp* pData = (_Tp*)(this->data + id*nRows*nCols*sizeof(_Tp));
            return CMat_<_Tp>(nRows,nCols,pData);
        }

        /**
         * @brief
         *
         * @param index
         * @param mat2D
         */
        void        setPlane(size_t index, const CMat_<_Tp> &mat2D)
        {
            assert(mat2D.data != nullptr);
            int srcRows = mat2D.size[0];
            int srcCols = mat2D.size[1];
            int dstRows = this->size[0];
            int dstCols = this->size[1];

            if(dstRows == srcRows && dstCols == srcCols)
            {
                void* pDst = this->data + index * dstRows * dstCols * sizeof(_Tp);
                memcpy(pDst, mat2D.data, srcRows * srcCols * sizeof(_Tp));
            }
            else
                throw CException(CoreExCode::INVALID_SIZE, "Invalid 2D CMat sizes", __func__, __FILE__, __LINE__);
        }
};

/** @endcond */

#endif
