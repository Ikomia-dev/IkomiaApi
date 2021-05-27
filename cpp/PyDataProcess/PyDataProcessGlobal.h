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

#ifndef PYDATAPROCESS_GLOBAL_H
#define PYDATAPROCESS_GLOBAL_H

#ifndef HAVE_SNPRINTF
    #define HAVE_SNPRINTF
#endif

#include <QtCore/qglobal.h>
#include "boost/python.hpp"
#include "PythonThread.hpp"
#include "PyDataProcessTools.hpp"

using namespace Ikomia;
using namespace boost::python;

#if defined(PYDATAPROCESS_LIBRARY)
#  define PYDATAPROCESSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PYDATAPROCESSSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PYDATAPROCESS_GLOBAL_H
