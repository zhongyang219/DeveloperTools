#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(PASSWORD_GENERATOR_LIB)
#  define PASSWORD_GENERATOR_EXPORT Q_DECL_EXPORT
# else
#  define PASSWORD_GENERATOR_EXPORT Q_DECL_IMPORT
# endif
#else
# define PASSWORD_GENERATOR_EXPORT
#endif
