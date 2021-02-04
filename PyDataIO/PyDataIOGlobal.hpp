#ifndef PYDATAIOGLOBAL_HPP
#define PYDATAIOGLOBAL_HPP

#ifndef HAVE_SNPRINTF
    #define HAVE_SNPRINTF
#endif

#include <QtCore/qglobal.h>
#include "boost/python.hpp"
#include "PythonThread.hpp"

using namespace boost::python;

#if defined(PYDATAIO_LIBRARY)
#  define PYDATAIOSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PYDATAIOSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PYDATAIOGLOBAL_HPP
