#ifndef UTILS_GLOBAL_H
#define UTILS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(UTILS_LIBRARY)
#  define UTILSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define UTILSSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // UTILS_GLOBAL_H
