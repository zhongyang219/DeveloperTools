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
    QString GetCurrentWallpaperPath();

    bool SetCurrentDeskTop(int index);

    bool PreviousWallPaper();
    bool NextWallPaper();

#ifdef Q_OS_WIN
    void ShowResultInfo(HRESULT hr);
#endif

private:
#ifdef Q_OS_WIN
    IDesktopWallpaper* m_pWallpaper{};
    LPWSTR m_pMonitorIdBuf;
    LPWSTR m_pWallpaperPathBuf;
#endif

};

