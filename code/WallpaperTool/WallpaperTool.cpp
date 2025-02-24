#include "WallpaperTool.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include "../CCommonTools/CommonTools.h"
#include "../CCommonTools/Config.h"
#include <QApplication>
#include <QTimerEvent>
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

CHistoryWallpaperMgr& WallpaperTool::GetHistoryWallpaperMgr()
{
    return m_historyWallpapers;
}

HistoryWallpaperSearchThread& WallpaperTool::GetHistoryWallpaperThread()
{
    return m_historyWallpaperThread;
}

void WallpaperTool::WallpaperSaveAs(const QString& path)
{
    QString strDir = QFileDialog::getExistingDirectory(m_mainWidget, QString(), m_strLastSaveDir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!strDir.isEmpty())
    {
        m_strLastSaveDir = strDir;
        QString strNewPath = strDir + "/" + QFileInfo(path).fileName();
        QString strLogInfo;
        if (!QFileInfo(strNewPath).isFile())
        {
            if (QFile::copy(path, strNewPath))
            {
                strLogInfo = QString(u8"成功将壁纸 %1 保存到 %2 目录下。").arg(path).arg(strDir);
                QMessageBox::information(m_mainWidget, QString(), strLogInfo);
            }
            else
            {
                strLogInfo = QString(u8"保存壁纸到 %1 到 %2 目录下失败。").arg(path).arg(strDir);
                QMessageBox::information(m_mainWidget, QString(), strLogInfo);
            }
        }
        else
        {
            QMessageBox::warning(m_mainWidget, QString(), QString(u8"文件 %1 已存在！").arg(strNewPath));
            strLogInfo = QString(u8"保存壁纸 %1 时，目标文件 %2 已存在。").arg(path).arg(strNewPath);
        }
        WriteLog(strLogInfo);
    }

}

void WallpaperTool::InitInstance()
{
    m_mainStackedWidget.addWidget(m_mainWidget = new MainWidget());
    m_mainStackedWidget.addWidget(m_historyWidget = new HistoryWallpaperWidget());

    //载入配置
    CConfig settings(QString::fromUtf8(GetModuleName()));
    m_strLastSaveDir = settings.GetValue("lastSaveDir").toString();
    m_settings.Load();
    m_historyWallpapers.Load();

    connect(m_mainWidget, SIGNAL(widgetLayoutChanged(bool, const QString&)), this, SLOT(OnMainWindowLayoutChanged(bool, const QString&)));
    //线程结束时更新历史壁纸列表
    connect(&m_historyWallpaperThread, SIGNAL(finished()), m_historyWidget, SLOT(Refresh()));

    m_autoRefreshTimerId = startTimer(600000);  //设置10分钟定时器
}

void WallpaperTool::UnInitInstance()
{
    CConfig settings(QString::fromUtf8(GetModuleName()));
    settings.WriteValue("lastSaveDir", m_strLastSaveDir);
    m_settings.Save();
    m_historyWallpapers.Save();
}

void WallpaperTool::UiInitComplete(IMainFrame* pMainFrame)
{
    m_pMainFrame = pMainFrame;

    //显示壁纸
    Refresh();
}

void* WallpaperTool::GetMainWindow()
{
    return (void*)&m_mainStackedWidget;
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
        m_mainWidget->ShowGridLayout();
    }
    //当前壁纸另存为
    else if (cmd == CMD_CURRENT_WALLPAPER_SAVE_AS)
    {
        WallpaperSaveAs(m_strCurWallpaperPath);
    }

    //删除当前壁纸
    else if (cmd == CMD_CURRENT_WALLPAPER_DELETE)
    {
        if (QMessageBox::question(m_mainWidget, QString(), QString(u8"确实要从磁盘中删除文件 %1 吗？").arg(m_strCurWallpaperPath), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            QString strLogInfo;
            if (QFile::remove(m_strCurWallpaperPath))
            {
                strLogInfo = QString(u8"删除壁纸 %1 成功。").arg(m_strCurWallpaperPath);
                QMessageBox::information(m_mainWidget, QString(), u8"删除成功！");
            }
            else
            {
                strLogInfo = QString(u8"删除壁纸 %1 失败。").arg(m_strCurWallpaperPath);
                QMessageBox::information(m_mainWidget, QString(), u8"删除失败！");
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

    else if (cmd == CMD_WallpaperHistory)
    {
        if (checked)
            m_mainStackedWidget.setCurrentIndex(1);
        else
            m_mainStackedWidget.setCurrentIndex(0);
        EnableControls();
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
    if (curWallpapersPath.size() == 1)
        m_strCurWallpaperPath = curWallpapersPath.front();
    m_mainWidget->SetWallpapers(curWallpapersPath);
    m_historyWallpapers.AddWallpapers(curWallpapersPath);
    m_historyWallpaperThread.StartThread();
}

void WallpaperTool::EnableControls()
{
    if (m_pMainFrame != nullptr)
    {
        if (m_mainStackedWidget.currentWidget() == m_mainWidget)
        {
            //只有一张壁纸的情况
            if (m_mainWidget->GetWallpaperNum() == 1)
            {
                m_pMainFrame->SetItemEnable(CMD_WallpaperBack, false);
                m_pMainFrame->SetItemEnable(CMD_CURRENT_WALLPAPER_SAVE_AS, true);
                m_pMainFrame->SetItemEnable(CMD_CURRENT_WALLPAPER_DELETE, true);
            }
            else
            {
                m_pMainFrame->SetItemEnable(CMD_WallpaperBack, !m_mainWidget->IsGridLayout());
                m_pMainFrame->SetItemEnable(CMD_CURRENT_WALLPAPER_SAVE_AS, !m_mainWidget->IsGridLayout());
                m_pMainFrame->SetItemEnable(CMD_CURRENT_WALLPAPER_DELETE, !m_mainWidget->IsGridLayout());
            }

        }
        else
        {
            m_pMainFrame->SetItemEnable(CMD_WallpaperBack, false);
            m_pMainFrame->SetItemEnable(CMD_CURRENT_WALLPAPER_SAVE_AS, false);
            m_pMainFrame->SetItemEnable(CMD_CURRENT_WALLPAPER_DELETE, false);
        }
    }
}

void WallpaperTool::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == m_autoRefreshTimerId)
    {
        //如果桌面背景设置为了幻灯片放映，则每隔10分钟自动刷新
        if (CWallpaperHelper::IsWindowsWallpaperAutoSwitch())
            Refresh();
    }
}

void WallpaperTool::OnMainWindowLayoutChanged(bool bGrid, const QString& curWallpaperPath)
{
    if (m_mainWidget->GetWallpaperNum() > 1)
    {
        m_strCurWallpaperPath = curWallpaperPath;
    }
    EnableControls();
}

IModule* CreateInstance()
{
    pIns = new WallpaperTool();
    return pIns;
}
