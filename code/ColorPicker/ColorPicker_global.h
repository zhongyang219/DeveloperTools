#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(COLOR_PICKER_LIB)
#  define COLOR_PICKER_EXPORT Q_DECL_EXPORT
# else
#  define COLOR_PICKER_EXPORT Q_DECL_IMPORT
# endif
#else
# define COLOR_PICKER_EXPORT
#endif
