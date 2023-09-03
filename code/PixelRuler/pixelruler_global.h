#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(PIXELRULER_LIB)
#  define PIXELRULER_EXPORT Q_DECL_EXPORT
# else
#  define PIXELRULER_EXPORT Q_DECL_IMPORT
# endif
#else
# define PIXELRULER_EXPORT
#endif
