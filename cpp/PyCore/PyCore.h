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

#ifndef PYCORE_H
#define PYCORE_H

#include "PyCoreGlobal.h"

using namespace boost::python;


//Special call policy that increases the reference count of the object being constructed
//Needed for our CWorkflowTaskWidget based objects that can be extended in Python
//Solve the issue of reference count for std::shared_ptr return from extended python class
//and lead to segmentation fault caused by double delete
template <class Base = default_call_policies>
struct incref_return_value_policy : Base
{
    static PyObject *postcall(PyObject *args, PyObject *result)
    {
        PyObject *self = PyTuple_GET_ITEM(args, 0);
        Py_INCREF(self);
        return result;
    }
};

//Template class for bi-directionnal conversion:
//std::vector<T> <-> python list
template <typename T>
struct vector_to_python_list
{
    static PyObject* convert(const std::vector<T>& vec)
    {
        list pylist;
        for(size_t i=0; i<vec.size(); i++)
            pylist.append(vec[i]);

        return incref(pylist.ptr());
    }
};

template <typename T>
struct vector_from_python_list
{
    static void* convertible(PyObject *obj_ptr)
    {
        if(!PyList_Check(obj_ptr))
            return 0;

        return obj_ptr;
    }

    static void construct(PyObject *obj_ptr, converter::rvalue_from_python_stage1_data* data)
    {
        std::vector<T> vec;
        for(Py_ssize_t i=0; i<PyList_Size(obj_ptr); i++)
        {
            PyObject *pyvalue = PyList_GetItem(obj_ptr, i);
            T value = typename extract<T>::extract(pyvalue);
            vec.push_back(value);
        }

        void* storage = ((converter::rvalue_from_python_storage<std::vector<T> >*)data)->storage.bytes;
        new (storage) std::vector<T>(vec);
        data->convertible = storage;
    }

    vector_from_python_list()
    {
        converter::registry::push_back(&convertible, &construct, type_id<std::vector<T> >());
    }
};

//Register std::vector<T> <-> python list converters
template<typename T>
void registerStdVector()
{
    to_python_converter<std::vector<T>, vector_to_python_list<T>>();
    vector_from_python_list<T>();
}

// Type that allows for registration of conversions from python iterable types.
struct iterable_converter
{
    // Registers converter from a python interable type to the provided type.
    template <typename Container>
    iterable_converter& from_python()
    {
        boost::python::converter::registry::push_back(
                    &iterable_converter::convertible,
                    &iterable_converter::construct<Container>,
                    boost::python::type_id<Container>());

        // Support chaining.
        return *this;
    }

    // Check if PyObject is iterable.
    static void* convertible(PyObject* object)
    {
        return PyObject_GetIter(object) ? object : NULL;
    }

    // Convert iterable PyObject to C++ container type.
    // Container Concept requirements:
    //  * Container::value_type is CopyConstructable.
    //  * Container can be constructed and populated with two iterators. I.e. Container(begin, end)
    template <typename Container>
    static void construct(PyObject* object, boost::python::converter::rvalue_from_python_stage1_data* data)
    {
        namespace python = boost::python;
        // Object is a borrowed reference, so create a handle indicting it is
        // borrowed for proper reference counting.
        python::handle<> handle(python::borrowed(object));

        // Obtain a handle to the memory block that the converter has allocated for the C++ type.
        typedef python::converter::rvalue_from_python_storage<Container> storage_type;
        void* storage = reinterpret_cast<storage_type*>(data)->storage.bytes;

        typedef python::stl_input_iterator<typename Container::value_type> iterator;

        // Allocate the C++ type into the converter's memory block, and assign
        // its handle to the converter's convertible variable.  The C++
        // container is populated by passing the begin and end iterators of
        // the python object to the container's constructor.
        new (storage) Container(
                    iterator(python::object(handle)), // begin
                    iterator());                      // end
        data->convertible = storage;
    }
};

// Generic override of Python __copy__ and __deepcopy__
template<class T>
inline PyObject * managingPyObject(T *p)
{
    return typename manage_new_object::apply<T *>::type()(p);
}

template<class Copyable>
object generic_copy(object copyable)
{
    Copyable *newCopyable(new Copyable(extract<const Copyable&>(copyable)));
    object result(detail::new_reference(managingPyObject(newCopyable)));
    extract<dict>(result.attr("__dict__"))().update(copyable.attr("__dict__"));
    return result;
}

template<class Copyable>
object generic_deepcopy(object copyable, dict memo)
{
    object copyMod = import("copy");
    object deepcopy = copyMod.attr("deepcopy");
    Copyable *newCopyable(new Copyable(extract<const Copyable&>(copyable)));
    object result(detail::new_reference(managingPyObject(newCopyable)));

    // HACK: copyableId shall be the same as the result of id(copyable) in Python -
    // please tell me that there is a better way! (and which ;-p)
    int copyableId = (int)((long long)copyable.ptr());
    memo[copyableId] = result;

    extract<dict>(result.attr("__dict__"))().update(deepcopy(extract<dict>(copyable.attr("__dict__"))(), memo));
    return result;
}

#endif // PYCORE_H
