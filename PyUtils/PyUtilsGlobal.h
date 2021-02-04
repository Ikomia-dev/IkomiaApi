#ifndef PYUTILS_GLOBAL_H
#define PYUTILS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PYUTILS_LIBRARY)
#  define PYUTILSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PYUTILSSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PYUTILS_GLOBAL_H
