#include "WallpaperTool.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include "../CCommonTools/CommonTools.h"
#include "../CCommonTools/Config.h"
#include <QApplication>
#include "mainframeinterface.h"

static WallpaperTool* pIns = nullptr;

WallpaperTool::WallpaperTool()
{
}

WallpaperTool* WallpaperTool::Instance()
{
    return pIns;
}

IMainFrame* WallpaperTool::GetMainFrame()
{
    return m_pMainFrame;
}

void WallpaperTool::InitInstance()
{
    //载入配置
    CConfig settings(QString::fromUtf8(GetModuleName()));
    m_strLastSaveDir = settings.GetValue("lastSaveDir").toString();
    m_settings.Load();

    connect(&m_mainWidget, SIGNAL(widgetLayoutChanged(bool, const QString&)), this, SLOT(OnMainWindowLayoutChanged(bool, const QString&)));
}

void WallpaperTool::UnInitInstance()
{
    CConfig settings(QString::fromUtf8(GetModuleName()));
    settings.WriteValue("lastSaveDir", m_strLastSaveDir);
    m_settings.Save();
}

void WallpaperTool::UiInitComplete(IMainFrame* pMainFrame)
{
    m_pMainFrame = pMainFrame;

    //显示壁纸
    auto curWallpapersPath = m_helper.GetCurrentWallpaperPath(m_settings.wallpaperAcquireMethod == SettingsDlg::Data::Registry);
    m_mainWidget.SetWallpapers(curWallpapersPath);
}

void* WallpaperTool::GetMainWindow()
{
    return (void*)&m_mainWidget;
}

IModule::eMainWindowType WallpaperTool::GetMainWindowType() const
{
    return IModule::MT_QWIDGET;
}

const char* WallpaperTool::GetModuleName()
{
    return "WallpaperTool";
}

void WallpaperTool::OnCommand(const char* strCmd, bool checked)
{
    QString cmd = QString::fromUtf8(strCmd);

    //返回
    if (cmd == CMD_WallpaperBack)
    {
        m_mainWidget.ShowGridLayout();
    }
    //当前壁纸另存为
    else if (cmd == CMD_CURRENT_WALLPAPER_SAVE_AS)
    {
        QString strDir = QFileDialog::getExistingDirectory(&m_mainWidget, QString(), m_strLastSaveDir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        if (!strDir.isEmpty())
        {
            m_strLastSaveDir = strDir;
            QString strNewPath = strDir + "/" + QFileInfo(m_strCurWallpaperPath).fileName();
            QString strLogInfo;
            if (!QFileInfo(strNewPath).isFile())
            {
                if (QFile::copy(m_strCurWallpaperPath, strNewPath))
                {
                    strLogInfo = QString(u8"成功将壁纸 %1 保存到 %2 目录下。").arg(m_strCurWallpaperPath).arg(strDir);
                    QMessageBox::information(&m_mainWidget, QString(), strLogInfo);
                }
                else
                {
                    strLogInfo = QString(u8"保存壁纸到 %1 到 %2 目录下失败。").arg(m_strCurWallpaperPath).arg(strDir);
                    QMessageBox::information(&m_mainWidget, QString(), strLogInfo);
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
        Refresh();
    }

    else if (cmd == CMD_PreviousWallpaper)
    {
        m_helper.PreviousWallPaper();
        CCommonTools::DelayNotBlocked(200);
        Refresh();
    }

    else if (cmd == CMD_NextWallpaper)
    {
        m_helper.NextWallPaper();
        CCommonTools::DelayNotBlocked(200);
        Refresh();
    }

    else if (cmd == CMD_WallpaperToolSettings)
    {
        SettingsDlg dlg;
        dlg.SetData(m_settings);
        if (dlg.exec() == QDialog::Accepted)
            m_settings = dlg.GetData();
    }
}

void WallpaperTool::WriteLog(const QString& strLogInfo)
{
    if (!strLogInfo.isEmpty())
    {
        QString strLogFilePath = QString("%1/%2.log").arg(qApp->applicationDirPath()).arg(QString::fromUtf8(GetModuleName()));
        CCommonTools::WriteLog(strLogInfo, strLogFilePath);
    }
}

void WallpaperTool::Refresh()
{
    auto curWallpapersPath = m_helper.GetCurrentWallpaperPath(m_settings.wallpaperAcquireMethod == SettingsDlg::Data::Registry);
    m_mainWidget.SetWallpapers(curWallpapersPath);
}

void WallpaperTool::OnMainWindowLayoutChanged(bool bGrid, const QString& curWallpaperPath)
{
    m_strCurWallpaperPath = curWallpaperPath;
    m_pMainFrame->SetItemEnable(CMD_WallpaperBack, !bGrid);
    m_pMainFrame->SetItemEnable(CMD_CURRENT_WALLPAPER_SAVE_AS, !bGrid);
    m_pMainFrame->SetItemEnable(CMD_CURRENT_WALLPAPER_DELETE, !bGrid);
}

IModule* CreateInstance()
{
    pIns = new WallpaperTool();
    return pIns;
}
