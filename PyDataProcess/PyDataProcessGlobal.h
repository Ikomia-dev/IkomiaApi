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
