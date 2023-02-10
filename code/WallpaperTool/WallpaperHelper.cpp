#include "WallpaperHelper.h"
#include <QSettings>

CWallpaperHelper::CWallpaperHelper()
{
}

QString CWallpaperHelper::GetCurrentWallpaperPath()
{
    QSettings reg("HKEY_CURRENT_USER\\Control Panel\\Desktop", QSettings::NativeFormat);
    QByteArray pathData{ reg.value("TranscodedImageCache").toByteArray() };
    QString strPath;
    if (pathData.size() > 27)
    {
        strPath = QString::fromUtf8(pathData.data() + 27);
    }
    return strPath;
}
