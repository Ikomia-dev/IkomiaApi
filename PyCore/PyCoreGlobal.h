#ifndef PYCORE_GLOBAL_H
#define PYCORE_GLOBAL_H

#ifndef HAVE_SNPRINTF
    #define HAVE_SNPRINTF
#endif

#include <QtCore/qglobal.h>
#include <Python.h>
#include "boost/python.hpp"
#include "PythonThread.hpp"
#include "CException.h"
#include "Main/CoreTools.hpp"

using namespace Ikomia;
using namespace boost::python;

#if defined(PYCORE_LIBRARY)
#  define PYCORESHARED_EXPORT Q_DECL_EXPORT
#else
#  define PYCORESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PYCORE_GLOBAL_H
