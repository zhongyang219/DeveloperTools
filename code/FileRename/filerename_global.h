#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(FILERENAME_LIB)
#  define FILERENAME_EXPORT Q_DECL_EXPORT
# else
#  define FILERENAME_EXPORT Q_DECL_IMPORT
# endif
#else
# define FILERENAME_EXPORT
#endif
