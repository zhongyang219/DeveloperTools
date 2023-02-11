#include "WallpaperTool.h"
#include "WallpaperHelper.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include "../CCommonTools/CommonTools.h"
#include "../CCommonTools/Config.h"

WallpaperTool::WallpaperTool()
{
}

void WallpaperTool::InitModule()
{
    m_strCurWallpaperPath = CWallpaperHelper::GetCurrentWallpaperPath();
    m_mainWidget.SetWallpaper(m_strCurWallpaperPath);

    CConfig settings(QString::fromWCharArray(GetModuleName()));
    m_strLastSaveDir = settings.GetValue("lastSaveDir").toString();
}

void WallpaperTool::UnInitModule()
{
    CConfig settings(QString::fromWCharArray(GetModuleName()));
    settings.WriteValue("lastSaveDir", m_strLastSaveDir);
}

unsigned long long WallpaperTool::GetMainWindow()
{
    return (unsigned long long)&m_mainWidget;
}

IModuleInterface::eMainWindowType WallpaperTool::GetMainWindowType() const
{
    return IModuleInterface::MT_QWIDGET;
}

const wchar_t* WallpaperTool::GetModuleName()
{
    return L"WallpaperTool";
}

void WallpaperTool::CommandTrigerd(const wchar_t* strCmd, bool checked)
{
    QString cmd = QString::fromWCharArray(strCmd);

    //当前壁纸另存为
    if (cmd == CMD_CURRENT_WALLPAPER_SAVE_AS)
    {
        QString strDir = QFileDialog::getExistingDirectory(&m_mainWidget, QString(), m_strLastSaveDir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        if (!strDir.isEmpty())
        {
            QString strNewPath = strDir + "/" + QFileInfo(m_strCurWallpaperPath).fileName();
            QString strLogInfo;
            if (!QFileInfo(strNewPath).isFile())
            {
                if (QFile::copy(m_strCurWallpaperPath, strNewPath))
                {
                    QMessageBox::information(&m_mainWidget, QString(), u8"保存成功！");
                    strLogInfo = QString(u8"成功将壁纸 %1 保存到 %2 目录下。").arg(m_strCurWallpaperPath).arg(strDir);
                    m_strLastSaveDir = strDir;
                }
                else
                {
                    QMessageBox::information(&m_mainWidget, QString(), u8"保存失败！");
                    strLogInfo = QString(u8"保存壁纸到 %1 到 %2 目录下失败。").arg(m_strCurWallpaperPath).arg(strDir);
                }
            }
            else
            {
                QMessageBox::warning(&m_mainWidget, QString(), QString(u8"文件 %1 已存在！").arg(strNewPath));
                strLogInfo = QString(u8"保存壁纸 %1 时，目标文件 %2 已存在。").arg(m_strCurWallpaperPath).arg(strNewPath);
            }
            WriteLog(strLogInfo);
        }
    }

    //删除当前壁纸
    else if (cmd == CMD_CURRENT_WALLPAPER_DELETE)
    {
        if (QMessageBox::question(&m_mainWidget, QString(), QString(u8"确实要从磁盘中删除文件 %1 吗？").arg(m_strCurWallpaperPath), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            QString strLogInfo;
            if (QFile::remove(m_strCurWallpaperPath))
            {
                strLogInfo = QString(u8"删除壁纸 %1 成功。").arg(m_strCurWallpaperPath);
                QMessageBox::information(&m_mainWidget, QString(), u8"删除成功！");
            }
            else
            {
                strLogInfo = QString(u8"删除壁纸 %1 失败。").arg(m_strCurWallpaperPath);
                QMessageBox::information(&m_mainWidget, QString(), u8"删除失败！");
            }
            WriteLog(strLogInfo);
        }
    }

    //刷新
    else if (cmd == CMD_CURRENT_WALLPAPER_REFRESH)
    {
        m_strCurWallpaperPath = CWallpaperHelper::GetCurrentWallpaperPath();
        m_mainWidget.SetWallpaper(m_strCurWallpaperPath);
    }
}

void WallpaperTool::WriteLog(const QString& strLogInfo)
{
    if (!strLogInfo.isEmpty())
    {
        QString strLogFilePath = QString("%1/%2.log").arg(qApp->applicationDirPath()).arg(QString::fromWCharArray(GetModuleName()));
        CCommonTools::WriteLog(strLogInfo, strLogFilePath);
    }
}

IModuleInterface* CreateInstance()
{
    return new WallpaperTool();
}
