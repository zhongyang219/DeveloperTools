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

static int UnsignedCharToInt(char ch)
{
    return static_cast<int>(static_cast<unsigned char>(ch));
}

static int GetRegInt(HKEY hKey, const wchar_t* keyName, const wchar_t* valueName)
{
    int value = 0;
    QByteArray data = GetRegBinaryData(hKey, keyName, valueName);
    if (data.size() >= 1)
        value += UnsignedCharToInt(data[0]);
    if (data.size() >= 2)
        value += UnsignedCharToInt(data[1]) * 256;
    if (data.size() >= 3)
        value += UnsignedCharToInt(data[2]) * 256 * 256;
    if (data.size() >= 4)
        value += UnsignedCharToInt(data[3]) * 256 * 256 * 256;
    return value;
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
#endif
}

CWallpaperHelper::~CWallpaperHelper()
{
#ifdef Q_OS_WIN
    if (m_pWallpaper != nullptr)
        m_pWallpaper->Release();
#endif
}

#ifdef Q_OS_WIN
//从“\HKEY_CURRENT_USER\Control Panel\Desktop”下的一个键获取壁纸路径
static QString GetWallpapaerFromReg(const wchar_t* regKey)
{
    QString strPath;
    //使用QSettings读取注册表无法获取原始的二进制数据，QSettings内部会将其转换成QString，因此使用WindowsAPI读取
    QByteArray pathData = GetRegBinaryData(HKEY_CURRENT_USER, L"Control Panel\\Desktop", regKey);
    pathData = pathData.mid(0x18);      //去掉前面0x18个字节
    if (!pathData.isEmpty())
        strPath = QString::fromWCharArray((const wchar_t*)pathData.constData());
    return strPath;
}
#endif

QList<QString> CWallpaperHelper::GetCurrentWallpaperPath(bool fromRegistry)
{
#ifdef Q_OS_WIN
    QList<QString> wallpapaersPath;
    //通过IDesktopWallpaper获取壁纸
    if (m_pWallpaper != nullptr && !fromRegistry)
    {
        HRESULT hr;
        // 获取显示器数量
        UINT monitorCount = 0;
        hr = m_pWallpaper->GetMonitorDevicePathCount(&monitorCount);

        // 遍历每个显示器，获取壁纸路径
        for (UINT i = 0; i < monitorCount; i++)
        {
            // 获取显示器的设备路径
            PWSTR monitorId = nullptr;
            hr = m_pWallpaper->GetMonitorDevicePathAt(i, &monitorId);
            if (FAILED(hr))
                continue;

            // 获取该显示器的壁纸路径
            PWSTR wallpaperPath = nullptr;
            hr = m_pWallpaper->GetWallpaper(monitorId, &wallpaperPath);
            if (SUCCEEDED(hr))
            {
                QString strPath = QString::fromWCharArray(wallpaperPath);
                CoTaskMemFree(wallpaperPath);  // 释放壁纸路径内存
                if (!strPath.isEmpty() && !wallpapaersPath.contains(strPath))
                    wallpapaersPath.push_back(strPath);

            }

            CoTaskMemFree(monitorId);  // 释放显示器设备路径内存
        }

        ShowResultInfo(hr);
        return wallpapaersPath;
    }
    //通过注册表获取壁纸
    else
    {
        QString wallpaperPath = GetWallpapaerFromReg(L"TranscodedImageCache");
        if (!wallpaperPath.isEmpty())
            wallpapaersPath.push_back(wallpaperPath);
        //壁纸数量
        int wallpaperCount = GetRegInt(HKEY_CURRENT_USER, L"Control Panel\\Desktop", L"TranscodedImageCount");
        //获取所有壁纸
        for (int i = 0; i < wallpaperCount; i++)
        {
            wchar_t buff[64];
            swprintf_s(buff, L"TranscodedImageCache_%.3d", i);
            QString wallpaperPath = GetWallpapaerFromReg(buff);
            if (!wallpaperPath.isEmpty() && !wallpapaersPath.contains(wallpaperPath))
                wallpapaersPath.push_back(wallpaperPath);
        }
        return wallpapaersPath;
    }
#else
//    QProcess cmdProcess;
//    //使用命令行获取当前壁纸路径
//    QString strArg = "gsettings get org.gnome.desktop.background picture-uri";
//    cmdProcess.start(strArg);
//    cmdProcess.waitForReadyRead();
//    cmdProcess.waitForFinished();
//    QString strRet = QString::fromUtf8(cmdProcess.readAll()).trimmed();
//    //去掉路径前后的引号和前面的file://
//    if (strRet.startsWith('\''))
//        strRet = strRet.mid(1);
//    if (strRet.startsWith("file://"))
//        strRet = strRet.mid(7);
//    if (strRet.endsWith('\''))
//        strRet.chop(1);
//    return QList<QString>() << strRet;

    QStringList wallpaperPaths;

    // 启动 gsettings 命令
    QProcess process;
    process.start("gsettings", QStringList() << "list-recursively" << "org.gnome.desktop.background");
    process.waitForFinished();

    // 读取命令输出
    QString output = process.readAllStandardOutput();
    QStringList lines = output.split("\n");

    // 正则表达式匹配 file:// 开头的路径
    QRegularExpression regex(R"((file://[^\s']+))");

    for (const QString& line : lines)
    {
        QRegularExpressionMatchIterator matchIterator = regex.globalMatch(line);
        while (matchIterator.hasNext())
        {
            QRegularExpressionMatch match = matchIterator.next();
            QString path = match.captured(0).remove("file://");
            wallpaperPaths.append(path);
        }
    }

    return wallpaperPaths;

#endif

    return QList<QString>();
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

bool CWallpaperHelper::IsWindowsWallpaperAutoSwitch()
{
#ifdef Q_OS_WIN
    int backgroundType = GetRegInt(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Wallpapers", L"BackgroundType");
    return backgroundType == 2;
#else
    return false;
#endif
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
