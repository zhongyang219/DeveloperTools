#include "WallpaperHelper.h"
#include <QSettings>
#include "WallpaperTool.h"
#include "mainframeinterface.h"
#ifndef Q_OS_WIN
#include <QProcess>
#endif

CWallpaperHelper::CWallpaperHelper()
{
#ifdef Q_OS_WIN
    CoInitialize(NULL);
    HRESULT hr = CoCreateInstance(__uuidof(DesktopWallpaper), 0, CLSCTX_LOCAL_SERVER, __uuidof(IDesktopWallpaper), (void**)&m_pWallpaper);
    if (!SUCCEEDED(hr))
    {
        m_pWallpaper = nullptr;
    }
    m_pMonitorIdBuf = (LPWSTR)GlobalAlloc(GPTR, 64);
    m_pWallpaperPathBuf = (LPWSTR)GlobalAlloc(GPTR, 256);
#endif
}

CWallpaperHelper::~CWallpaperHelper()
{
#ifdef Q_OS_WIN
    GlobalFree(m_pMonitorIdBuf);
    GlobalFree(m_pWallpaperPathBuf);
    if (m_pWallpaper != nullptr)
        m_pWallpaper->Release();
#endif
}

QString CWallpaperHelper::GetCurrentWallpaperPath()
{
#ifdef Q_OS_WIN
    if (m_pWallpaper != nullptr)
    {
        HRESULT hr = m_pWallpaper->GetWallpaper(m_pMonitorIdBuf, &m_pWallpaperPathBuf);
        ShowResultInfo(hr);
        return QString::fromWCharArray(m_pWallpaperPathBuf);
    }
#else

    QProcess cmdProcess;
    //使用命令行获取当前壁纸路径
    QString strArg = "gsettings get org.gnome.desktop.background picture-uri";
    cmdProcess.start(strArg);
    cmdProcess.waitForReadyRead();
    cmdProcess.waitForFinished();
    QString strRet = QString::fromUtf8(cmdProcess.readAll()).trimmed();
    //去掉路径前后的引号和前面的file://
    if (strRet.startsWith('\''))
        strRet = strRet.mid(1);
    if (strRet.startsWith("file://"))
        strRet = strRet.mid(7);
    if (strRet.endsWith('\''))
        strRet.chop(1);
    return strRet;

//    return "/usr/share/backgrounds/1-warty-final-ubuntukylin.jpg";
#endif

    return QString();
}

bool CWallpaperHelper::SetCurrentDeskTop(int index)
{
#ifdef Q_OS_WIN
    if (m_pWallpaper != nullptr)
    {
        UINT desktCount = 0;
        m_pWallpaper->GetMonitorDevicePathCount(&desktCount);
        if (index >= 0 && index < desktCount)
        {
            m_pWallpaper->GetMonitorDevicePathAt(index, &m_pMonitorIdBuf);
            return true;
        }
    }
#endif
    return false;
}

bool CWallpaperHelper::PreviousWallPaper()
{
#ifdef Q_OS_WIN
    if (m_pWallpaper != nullptr)
    {
        HRESULT hr = m_pWallpaper->AdvanceSlideshow(NULL, DSD_BACKWARD);
        ShowResultInfo(hr);
        return SUCCEEDED(hr);
    }
#endif
    return false;
}

bool CWallpaperHelper::NextWallPaper()
{
#ifdef Q_OS_WIN
    if (m_pWallpaper != nullptr)
    {
        HRESULT hr = m_pWallpaper->AdvanceSlideshow(NULL, DSD_FORWARD);
        ShowResultInfo(hr);
        return SUCCEEDED(hr);
    }
#endif
    return false;
}

#ifdef Q_OS_WIN
void CWallpaperHelper::ShowResultInfo(HRESULT hr)
{
    if (!SUCCEEDED(hr))
    {
        DWORD errorCode = ERROR_CAN_NOT_COMPLETE;
        if ((hr & 0xFFFF0000) == MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, 0))
        {
            errorCode = HRESULT_CODE(hr);
        }
        LPVOID lpMsgBuf;
        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            errorCode,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR)&lpMsgBuf,
            0, NULL);

        QString strMsg = QString::fromWCharArray((LPCTSTR)lpMsgBuf);
        WallpaperTool::Instance()->GetMainFrame()->SetStatusBarText(strMsg.toUtf8().constData(), 1500);

        LocalFree(lpMsgBuf);
    }
}
#endif
