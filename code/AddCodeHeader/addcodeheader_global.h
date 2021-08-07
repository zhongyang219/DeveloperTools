#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(ADDCODEHEADER_LIB)
#  define ADDCODEHEADER_EXPORT Q_DECL_EXPORT
# else
#  define ADDCODEHEADER_EXPORT Q_DECL_IMPORT
# endif
#else
# define ADDCODEHEADER_EXPORT
#endif
