#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(KEYBOARDTESTER_LIB)
#  define KEYBOARDTESTER_EXPORT Q_DECL_EXPORT
# else
#  define KEYBOARDTESTER_EXPORT Q_DECL_IMPORT
# endif
#else
# define KEYBOARDTESTER_EXPORT
#endif
