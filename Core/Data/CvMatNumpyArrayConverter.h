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

#include <opencv2/core/core.hpp>
#include <boost/python.hpp>
#include "Main/CoreGlobal.hpp"
#include "Data/CMat.hpp"
#include "PythonThread.hpp"

//-----------------------------------------------//
//----- Numpy NdArray <-> cv::Mat converter -----//
//-----------------------------------------------//
struct CvMatNumpyArrayConverter
{
    static cv::Mat		toMat(PyObject* pyObj);
    static PyObject*	toNDArray(const cv::Mat& cvMat);
};

//----------------------------//
//----- Boost converters -----//
//----------------------------//
struct BoostCvMatToNumpyArrayConverter
{
    static PyObject* convert(const CMat &m);
};

struct BoostNumpyArrayToCvMatConverter
{
    BoostNumpyArrayToCvMatConverter();

    /// @brief Check if PyObject is an array and can be converted to OpenCV matrix.
    static void* convertible(PyObject* object);

    /// @brief Construct a Mat from an NDArray object.
    static void  construct(PyObject* object, boost::python::converter::rvalue_from_python_stage1_data* data);
};
