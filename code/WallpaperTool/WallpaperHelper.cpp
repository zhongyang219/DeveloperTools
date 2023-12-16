#include "WallpaperHelper.h"
#include <QSettings>
#include "WallpaperTool.h"
#include "mainframeinterface.h"
#ifndef Q_OS_WIN
#include <QProcess>
#endif

#ifdef Q_OS_WIN
#include <qt_windows.h>
#define TOTALBYTES    8192
#define BYTEINCREMENT 4096

//使用Windows API从注册表读取一个二进制数据
//https://learn.microsoft.com/zh-cn/windows/win32/api/winreg/nf-winreg-regqueryvalueexw
static QByteArray GetRegBinaryData(HKEY hKey, const wchar_t* keyName, const wchar_t* valueName)
{
    QByteArray binData;
    LONG lRes = RegOpenKeyExW(hKey, keyName, 0, KEY_READ, &hKey);
    if (lRes == ERROR_SUCCESS)
    {
        DWORD BufferSize = TOTALBYTES;
        DWORD cbData;
        DWORD dwRet;

        PPERF_DATA_BLOCK PerfData = (PPERF_DATA_BLOCK)malloc(BufferSize);

        cbData = BufferSize;

        dwRet = RegQueryValueExW(hKey, valueName, NULL, NULL, (LPBYTE)PerfData, &cbData);
        while (dwRet == ERROR_MORE_DATA)
        {
            // Get a buffer that is big enough.
            BufferSize += BYTEINCREMENT;
            PerfData = (PPERF_DATA_BLOCK)realloc(PerfData, BufferSize);
            cbData = BufferSize;

            dwRet = RegQueryValueExW(hKey, valueName, NULL, NULL, (LPBYTE)PerfData, &cbData);
        }
        if (dwRet == ERROR_SUCCESS)
        {
            binData = QByteArray((const char*)PerfData, BufferSize);
        }
        free(PerfData);
    }
    RegCloseKey(hKey);

    return binData;
}
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////
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

QString CWallpaperHelper::GetCurrentWallpaperPath(bool fromRegistry)
{
#ifdef Q_OS_WIN
    if (m_pWallpaper != nullptr && !fromRegistry)
    {
        HRESULT hr = m_pWallpaper->GetWallpaper(m_pMonitorIdBuf, &m_pWallpaperPathBuf);
        ShowResultInfo(hr);
        return QString::fromWCharArray(m_pWallpaperPathBuf);
    }
    else
    {
        QString strPath;
        //使用QSettings读取注册表无法获取原始的二进制数据，QSettings内部会将其转换成QString，因此使用WindowsAPI读取
        QByteArray pathData = GetRegBinaryData(HKEY_CURRENT_USER, L"Control Panel\\Desktop", L"TranscodedImageCache");
        pathData = pathData.mid(0x18);      //去掉前面0x18个字节
        if (!pathData.isEmpty())
            strPath = QString::fromWCharArray((const wchar_t*)pathData.constData());
        return strPath;
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
