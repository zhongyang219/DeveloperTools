#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(WALLPAPERTOOL_LIB)
#  define WALLPAPERTOOL_EXPORT Q_DECL_EXPORT
# else
#  define WALLPAPERTOOL_EXPORT Q_DECL_IMPORT
# endif
#else
# define WALLPAPERTOOL_EXPORT
#endif

#define CMD_CURRENT_WALLPAPER_SAVE_AS "CurWallpaperSaveAs"
#define CMD_CURRENT_WALLPAPER_DELETE "CurWallpaperDelete"