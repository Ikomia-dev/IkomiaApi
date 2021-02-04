#ifndef DATAIO_GLOBAL_HPP
#define DATAIO_GLOBAL_HPP

#include <QtCore/qglobal.h>

#if defined(DATAIO_LIBRARY)
#  define DATAIOSHARED_EXPORT Q_DECL_EXPORT
#else
#  define DATAIOSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // DATAIO_GLOBAL_HPP
