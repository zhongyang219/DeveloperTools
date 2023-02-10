#pragma once
#include <QString>
class CWallpaperHelper
{
public:
    CWallpaperHelper();

    //从注册表解析当前桌面壁纸的路径
    static QString GetCurrentWallpaperPath();

};

