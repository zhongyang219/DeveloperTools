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
#define CMD_CURRENT_WALLPAPER_REFRESH "CurWallpaperRefresh"
#define CMD_PreviousWallpaper "PreviousWallpaper"
#define CMD_NextWallpaper "NextWallpaper"
#define CMD_WallpaperToolSettings "WallpaperToolSettings"
#define CMD_WallpaperBack "WallpaperBack"
#define CMD_WallpaperHistory "WallpaperHistory"
