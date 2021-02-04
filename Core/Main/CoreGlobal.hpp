#ifndef CORE_GLOBAL_HPP
#define CORE_GLOBAL_HPP

#include <QtCore/qglobal.h>

#ifndef HAVE_SNPRINTF
    #define HAVE_SNPRINTF
#endif

#if defined(CORE_LIBRARY)
#  define CORESHARED_EXPORT Q_DECL_EXPORT
#else
#  define CORESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // CORE_GLOBAL_HPP
