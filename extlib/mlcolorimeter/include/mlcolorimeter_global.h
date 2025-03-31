#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(MLCOLORIMETER_LIB)
#  define MLCOLORIMETER_EXPORT Q_DECL_EXPORT
# else
#  define MLCOLORIMETER_EXPORT Q_DECL_IMPORT
# endif
#else
# define MLCOLORIMETER_EXPORT
#endif
