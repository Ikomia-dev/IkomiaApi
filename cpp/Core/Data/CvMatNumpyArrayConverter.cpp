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

#include "CvMatNumpyArrayConverter.h"

#define NO_IMPORT_ARRAY
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#define PY_ARRAY_UNIQUE_SYMBOL IKOMIA_ARRAY_API
#include <numpy/ndarrayobject.h>

//------------------//
//----- Macros -----//
//------------------//
#define ERRWRAP2(expr) \
try \
{ \
    CPyAllowThreads allowThreads; \
    expr; \
} \
catch (const cv::Exception &e) \
{ \
    PyErr_SetString(opencv_error, e.what()); \
    return 0; \
}

static size_t REFCOUNT_OFFSET = (size_t)&(((PyObject*)0)->ob_refcnt) +
                                (0x12345678 != *(const size_t*)"\x78\x56\x34\x12\0\0\0\0\0") * sizeof(int);

static inline PyObject* pyObjectFromRefcount(const int* refcount)
{
    return (PyObject*)((size_t)refcount - REFCOUNT_OFFSET);
}

static inline int* refcountFromPyObject(const PyObject* obj)
{
    return (int*)((size_t)obj + REFCOUNT_OFFSET);
}

enum { ARG_NONE = 0, ARG_MAT = 1, ARG_SCALAR = 2 };

//--------------------------//
//----- Error handling -----//
//--------------------------//
static PyObject* opencv_error = nullptr;

static int failmsg(const char *fmt, ...)
{
    char str[1000];

    va_list ap;
    va_start(ap, fmt);
    vsnprintf(str, sizeof(str), fmt, ap);
    va_end(ap);

    PyErr_SetString(PyExc_TypeError, str);
    return 0;
}

//---------------------------//
//----- Numpy allocator -----//
//---------------------------//
class CNumpyAllocator : public cv::MatAllocator
{
    public:

        CNumpyAllocator()
        {
            m_stdAllocator = cv::Mat::getStdAllocator();
        }
        ~CNumpyAllocator()
        {
        }

        cv::UMatData*   allocate(PyObject* o, int dims, const int* sizes, int type, size_t* step) const
        {
            cv::UMatData* u = new cv::UMatData(this);
            u->data = u->origdata = (uchar*)PyArray_DATA((PyArrayObject*)o);
            npy_intp* _strides = PyArray_STRIDES((PyArrayObject*)o);

            for (int i = 0; i < dims - 1; i++)
                step[i] = (size_t)_strides[i];

            step[dims - 1] = CV_ELEM_SIZE(type);
            u->size = sizes[0] * step[0];
            u->userdata = o;
            return u;
        }

        cv::UMatData*   allocate(int dims0, const int* sizes, int type, void* data, size_t* step, cv::AccessFlag flags, cv::UMatUsageFlags usageFlags) const
        {
            if (data != 0)
            {
                // issue #6969: CV_Error(cv::Error::StsAssert, "The data should normally be NULL!");
                // probably this is safe to do in such extreme case
                return m_stdAllocator->allocate(dims0, sizes, type, data, step, flags, usageFlags);
            }

            CPyEnsureGIL gil;
            int depth = CV_MAT_DEPTH(type);
            int cn = CV_MAT_CN(type);
            const int f = (int)(sizeof(size_t) / 8);
            int typenum =   depth == CV_8U ? NPY_UBYTE :
                            depth == CV_8S ? NPY_BYTE :
                            depth == CV_16U ? NPY_USHORT :
                            depth == CV_16S ? NPY_SHORT :
                            depth == CV_32S ? NPY_INT :
                            depth == CV_32F ? NPY_FLOAT :
                            depth == CV_64F ? NPY_DOUBLE : f*NPY_ULONGLONG + (f ^ 1)*NPY_UINT;

            int i, dims = dims0;
            cv::AutoBuffer<npy_intp> _sizes(dims + 1);
            for (i = 0; i < dims; i++)
                _sizes[i] = sizes[i];

            if (cn > 1)
                _sizes[dims++] = cn;

            PyObject* o = PyArray_SimpleNew(dims, _sizes, typenum);
            if (!o)
                CV_Error_(cv::Error::StsError, ("The numpy array of typenum=%d, ndims=%d can not be created", typenum, dims));

            return allocate(o, dims0, sizes, type, step);
        }

        bool            allocate(cv::UMatData* u, cv::AccessFlag accessFlags, cv::UMatUsageFlags usageFlags) const
        {
            return m_stdAllocator->allocate(u, accessFlags, usageFlags);
        }

        void            deallocate(cv::UMatData* u) const
        {
            if (!u)
                return;

            CPyEnsureGIL gil;
            if(u->refcount == 0)
            {
                PyObject* o = (PyObject*)u->userdata;
                Py_XDECREF(o);
                delete u;
            }
        }

    public:

        const MatAllocator* m_stdAllocator;
};

CNumpyAllocator g_numpyAllocator;

//-----------------------------------------------//
//----- Numpy NdArray <-> cv::Mat converter -----//
//-----------------------------------------------//
cv::Mat CvMatNumpyArrayConverter::toMat(PyObject *pyObj)
{
    cv::Mat cvMat;
    bool bAllowND = true;

    if (!PyArray_Check(pyObj))
    {
        failmsg("argument is not a numpy array");
        if (!cvMat.data)
            cvMat.allocator = &g_numpyAllocator;
    }
    else
    {
        PyArrayObject* pyArray = (PyArrayObject*)pyObj;

        bool bNeedcopy = false, bNeedcast = false;
        int typenum = PyArray_TYPE(pyArray), new_typenum = typenum;
        int type =	typenum == NPY_UBYTE ? CV_8U :
                    typenum == NPY_BYTE ? CV_8S :
                    typenum == NPY_USHORT ? CV_16U :
                    typenum == NPY_SHORT ? CV_16S :
                    typenum == NPY_INT ? CV_32S :
                    typenum == NPY_INT32 ? CV_32S :
                    typenum == NPY_FLOAT ? CV_32F :
                    typenum == NPY_DOUBLE ? CV_64F : -1;

        if (type < 0)
        {
            if (typenum == NPY_INT64 || typenum == NPY_UINT64 || type == NPY_LONG)
            {
                bNeedcopy = bNeedcast = true;
                new_typenum = NPY_INT;
                type = CV_32S;
            }
            else
            {
                failmsg("Argument data type is not supported");
                cvMat.allocator = &g_numpyAllocator;
                return cvMat;
            }
        }

#ifndef CV_MAX_DIM
		const int CV_MAX_DIM = 32;
#endif

		int ndims = PyArray_NDIM(pyArray);
		if (ndims >= CV_MAX_DIM) 
		{
			failmsg("Dimensionality of argument is too high");
			if (!cvMat.data)
				cvMat.allocator = &g_numpyAllocator;
			return cvMat;
		}

		int size[CV_MAX_DIM + 1];
		size_t step[CV_MAX_DIM + 1];
		size_t elemsize = CV_ELEM_SIZE1(type);
		const npy_intp* _sizes = PyArray_DIMS(pyArray);
		const npy_intp* _strides = PyArray_STRIDES(pyArray);
		bool ismultichannel = ndims == 3 && _sizes[2] <= CV_CN_MAX;

		for (int i = ndims - 1; i >= 0 && !bNeedcopy; i--) 
		{
			// these checks handle cases of
			//  a) multi-dimensional (ndims > 2) arrays, as well as simpler 1- and 2-dimensional cases
			//  b) transposed arrays, where _strides[] elements go in non-descending order
			//  c) flipped arrays, where some of _strides[] elements are negative
			if ((i == ndims - 1 && (size_t)_strides[i] != elemsize) || (i < ndims - 1 && _strides[i] < _strides[i + 1]))
				bNeedcopy = true;
		}

		if (ismultichannel && _strides[1] != (npy_intp)elemsize * _sizes[2])
			bNeedcopy = true;

		if (bNeedcopy) 
		{
			if (bNeedcast) 
			{
				pyObj = PyArray_Cast(pyArray, new_typenum);
				pyArray = (PyArrayObject*)pyObj;
			}
			else 
			{
				pyArray = PyArray_GETCONTIGUOUS(pyArray);
				pyObj = (PyObject*)pyArray;
			}
			_strides = PyArray_STRIDES(pyArray);
		}

		for (int i = 0; i < ndims; i++) 
		{
			size[i] = (int)_sizes[i];
			step[i] = (size_t)_strides[i];
		}

		// handle degenerate case
		if (ndims == 0) 
		{
			size[ndims] = 1;
			step[ndims] = elemsize;
			ndims++;
		}

		if (ismultichannel) 
		{
			ndims--;
			type |= CV_MAKETYPE(0, size[2]);
		}

		if (ndims > 2 && !bAllowND) 
			failmsg("%s has more than 2 dimensions");
		else 
		{
			cvMat = cv::Mat(ndims, size, type, PyArray_DATA(pyArray), step);
			cvMat.u = g_numpyAllocator.allocate(pyObj, ndims, size, type, step);
			cvMat.addref();

			if (!bNeedcopy)
				Py_INCREF(pyObj);
		}
		cvMat.allocator = &g_numpyAllocator;
	}
	return cvMat;
}

PyObject* CvMatNumpyArrayConverter::toNDArray(const cv::Mat& cvMat)
{
	if (!cvMat.data)
		Py_RETURN_NONE;

	cv::Mat cvTemp;
	cv::Mat* pCvMat = (cv::Mat*)&cvMat;

	if (!pCvMat->u || pCvMat->allocator != &g_numpyAllocator)
	{
		cvTemp.allocator = &g_numpyAllocator;
		cvMat.copyTo(cvTemp);
		pCvMat = &cvTemp;
	}
	PyObject* pyObj = (PyObject*)pCvMat->u->userdata;
	Py_INCREF(pyObj);
	return pyObj;
}

//----------------------------//
//----- Boost converters -----//
//----------------------------//
PyObject *BoostCvMatToNumpyArrayConverter::convert(const CMat &m)
{
    if(m.data == nullptr)
        Py_RETURN_NONE;

    cv::Mat temp;
    cv::Mat* p = (cv::Mat*)&m;

    if (!p->u || p->allocator != &g_numpyAllocator)
    {
        temp.allocator = &g_numpyAllocator;
        ERRWRAP2(m.copyTo(temp));
        p = &temp;
    }
    PyObject* o = (PyObject*)p->u->userdata;
    Py_INCREF(o);
    return o;
}

BoostNumpyArrayToCvMatConverter::BoostNumpyArrayToCvMatConverter()
{
    boost::python::converter::registry::push_back(convertible, construct, boost::python::type_id<CMat>());
}

void *BoostNumpyArrayToCvMatConverter::convertible(PyObject *object)
{
    if (!PyArray_Check(object))
        return NULL;

#ifndef CV_MAX_DIM
    const int CV_MAX_DIM = 32;
#endif
    PyArrayObject* oarr = (PyArrayObject*) object;

    int typenum = PyArray_TYPE(oarr);
    if( typenum != NPY_INT64 && typenum != NPY_UINT64 && typenum != NPY_LONG && typenum != NPY_UBYTE &&
        typenum != NPY_BYTE && typenum != NPY_USHORT && typenum != NPY_SHORT && typenum != NPY_INT &&
        typenum != NPY_INT32 && typenum != NPY_FLOAT && typenum != NPY_DOUBLE)
    {
        return NULL;
    }

    //Data type not supported
    int ndims = PyArray_NDIM(oarr);
    if (ndims >= CV_MAX_DIM)
        return NULL; //too many dimensions

    return object;
}

void BoostNumpyArrayToCvMatConverter::construct(PyObject *object, boost::python::converter::rvalue_from_python_stage1_data *data)
{
    namespace python = boost::python;

    // Object is a borrowed reference, so create a handle indicting it is
    // borrowed for proper reference counting.
    python::handle<> handle(python::borrowed(object));

    // Obtain a handle to the memory block that the converter has allocated
    // for the C++ type.
    typedef python::converter::rvalue_from_python_storage<CMat> storage_type;
    void* storage = reinterpret_cast<storage_type*>(data)->storage.bytes;

    // Allocate the C++ type into the converter's memory block, and assign
    // its handle to the converter's convertible variable.  The C++
    // container is populated by passing the begin and end iterators of
    // the python object to the container's constructor.
    PyArrayObject* oarr = (PyArrayObject*) object;

    bool needcopy = false, needcast = false;
    int typenum = PyArray_TYPE(oarr), new_typenum = typenum;
    int type = typenum == NPY_UBYTE ? CV_8U : typenum == NPY_BYTE ? CV_8S :
                typenum == NPY_USHORT ? CV_16U :
                typenum == NPY_SHORT ? CV_16S :
                typenum == NPY_INT ? CV_32S :
                typenum == NPY_INT32 ? CV_32S :
                typenum == NPY_FLOAT ? CV_32F :
                typenum == NPY_DOUBLE ? CV_64F : -1;

    if (type < 0)
    {
        needcopy = needcast = true;
        new_typenum = NPY_INT;
        type = CV_32S;
    }

#ifndef CV_MAX_DIM
    const int CV_MAX_DIM = 32;
#endif

    int ndims = PyArray_NDIM(oarr);
    int size[CV_MAX_DIM + 1];
    size_t step[CV_MAX_DIM + 1];
    size_t elemsize = CV_ELEM_SIZE1(type);
    const npy_intp* _sizes = PyArray_DIMS(oarr);
    const npy_intp* _strides = PyArray_STRIDES(oarr);
    bool ismultichannel = ndims == 3 && _sizes[2] <= CV_CN_MAX;

    for (int i = ndims - 1; i >= 0 && !needcopy; i--)
    {
        // these checks handle cases of
        //  a) multi-dimensional (ndims > 2) arrays, as well as simpler 1- and 2-dimensional cases
        //  b) transposed arrays, where _strides[] elements go in non-descending order
        //  c) flipped arrays, where some of _strides[] elements are negative
        if ((i == ndims - 1 && (size_t) _strides[i] != elemsize)
                || (i < ndims - 1 && _strides[i] < _strides[i + 1]))
            needcopy = true;
    }

    if (ismultichannel && _strides[1] != (npy_intp) elemsize * _sizes[2])
        needcopy = true;

    if (needcopy)
    {
        if (needcast)
        {
            object = PyArray_Cast(oarr, new_typenum);
            oarr = (PyArrayObject*) object;
        }
        else
        {
            oarr = PyArray_GETCONTIGUOUS(oarr);
            object = (PyObject*) oarr;
        }
        _strides = PyArray_STRIDES(oarr);
    }

    for (int i = 0; i < ndims; i++)
    {
        size[i] = (int) _sizes[i];
        step[i] = (size_t) _strides[i];
    }

    // handle degenerate case
    if (ndims == 0)
    {
        size[ndims] = 1;
        step[ndims] = elemsize;
        ndims++;
    }

    if (ismultichannel)
    {
        ndims--;
        type |= CV_MAKETYPE(0, size[2]);
    }

    if (!needcopy)
        Py_INCREF(object);

    CMat* m = new (storage) CMat(ndims, size, type, PyArray_DATA(oarr), step);
    m->u = g_numpyAllocator.allocate(object, ndims, size, type, step);
    m->allocator = &g_numpyAllocator;
    m->addref();
    data->convertible = storage;
}


