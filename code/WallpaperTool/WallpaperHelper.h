#pragma once
#include <QString>

#ifdef Q_OS_WIN
#include <Windows.h>
#include <Shobjidl.h>
#endif

class CWallpaperHelper
{
public:
    CWallpaperHelper();
    ~CWallpaperHelper();

    //获取当前桌面壁纸的路径
    QList<QString> GetCurrentWallpaperPath(bool fromRegistry);

    bool PreviousWallPaper();
    bool NextWallPaper();

    static bool IsWindowsWallpaperAutoSwitch();

#ifdef Q_OS_WIN
    void ShowResultInfo(HRESULT hr);
#endif

private:
#ifdef Q_OS_WIN
    IDesktopWallpaper* m_pWallpaper{};
#endif

};

