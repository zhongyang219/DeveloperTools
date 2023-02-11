#include "WallpaperHelper.h"
#include <QSettings>

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

CWallpaperHelper::CWallpaperHelper()
{
}

QString CWallpaperHelper::GetCurrentWallpaperPath()
{
    QString strPath;

    //QSettings reg("HKEY_CURRENT_USER\\Control Panel\\Desktop", QSettings::NativeFormat);
    //QByteArray pathData{ reg.value("TranscodedImageCache").toByteArray() };
    //if (pathData.size() > 27)
    //{
    //    strPath = QString::fromUtf8(pathData.data() + 27);
    //}

    //使用QSettings读取注册表无法获取原始的二进制数据，QSettings内部会将其转换成QString，因此改成使用WindowsAPI读取
#ifdef Q_OS_WIN
    QByteArray pathData = GetRegBinaryData(HKEY_CURRENT_USER, L"Control Panel\\Desktop", L"TranscodedImageCache");
    pathData = pathData.mid(0x18);      //去掉前面0x18个字节
    if (!pathData.isEmpty())
        strPath = QString::fromWCharArray((const wchar_t*)pathData.constData());

#endif

    return strPath;
}
