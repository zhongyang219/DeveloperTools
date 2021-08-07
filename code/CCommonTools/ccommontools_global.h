#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(CCOMMONTOOLS_LIB)
#  define CCOMMONTOOLS_EXPORT Q_DECL_EXPORT
# else
#  define CCOMMONTOOLS_EXPORT Q_DECL_IMPORT
# endif
#else
# define CCOMMONTOOLS_EXPORT
#endif
