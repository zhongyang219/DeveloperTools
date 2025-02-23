#pragma once

#include "wallpapertool_global.h"
#include "moduleinterface.h"
#include <QObject>
#include "MainWidget.h"
#include "WallpaperHelper.h"
#include "SettingsDlg.h"
#include "HistoryWallpaperMgr.h"
#include "HistoryWallpaperWidget.h"
#include <QStackedWidget>

class WALLPAPERTOOL_EXPORT WallpaperTool
    : public QObject, public IModule
{
    Q_OBJECT
public:
    WallpaperTool();

    static WallpaperTool* Instance();
    IMainFrame* GetMainFrame();
    CHistoryWallpaperMgr& GetHistoryWallpaperMgr();
    HistoryWallpaperSearchThread& GetHistoryWallpaperThread();
    void WallpaperSaveAs(const QString& path);

    // 通过 IModule 继承
    virtual void InitInstance() override;
    virtual void UnInitInstance() override;
    virtual void UiInitComplete(IMainFrame* pMainFrame) override;
    virtual void* GetMainWindow() override;
    virtual eMainWindowType GetMainWindowType() const override;
    virtual const char* GetModuleName() override;
    virtual void OnCommand(const char* strCmd, bool checked) override;

    void WriteLog(const QString& strLogInfo);
private:
    void Refresh();
    void EnableControls();

private slots:
    void OnMainWindowLayoutChanged(bool bGrid, const QString& curWallpaperPath);     //响应窗口布局改变（在单席位查看和宫格布局中切换）

private:
    QStackedWidget m_mainStackedWidget;
    MainWidget* m_mainWidget;
    HistoryWallpaperWidget* m_historyWidget;
    IMainFrame* m_pMainFrame{};
    QString m_strCurWallpaperPath;
    QString m_strLastSaveDir;       //上次壁纸的保存路径
    CWallpaperHelper m_helper;
    SettingsDlg::Data m_settings;
    CHistoryWallpaperMgr m_historyWallpapers;
    HistoryWallpaperSearchThread m_historyWallpaperThread{ m_historyWallpapers };
};

#ifdef __cplusplus
extern "C" {
#endif
    Q_DECL_EXPORT IModule* CreateInstance();
#ifdef __cplusplus
}
#endif
