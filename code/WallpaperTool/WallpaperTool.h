#pragma once

#include "wallpapertool_global.h"
#include "moduleinterface.h"
#include <QObject>
#include "MainWidget.h"
#include "WallpaperHelper.h"
#include "SettingsDlg.h"

class WALLPAPERTOOL_EXPORT WallpaperTool
    : public QObject, public IModule
{
    Q_OBJECT
public:
    WallpaperTool();

    static WallpaperTool* Instance();
    IMainFrame* GetMainFrame();

    // 通过 IModule 继承
    virtual void InitInstance() override;
    virtual void UnInitInstance() override;
    virtual void UiInitComplete(IMainFrame* pMainFrame) override;
    virtual void* GetMainWindow() override;
    virtual eMainWindowType GetMainWindowType() const override;
    virtual const char* GetModuleName() override;
    virtual void OnCommand(const char* strCmd, bool checked) override;

private:
    void WriteLog(const QString& strLogInfo);
    void Refresh();

private:
    MainWidget m_mainWidget;
    IMainFrame* m_pMainFrame{};
    QList<QString> m_curWallpapersPath;
    QString m_strLastSaveDir;       //上次壁纸的保存路径
    CWallpaperHelper m_helper;
    SettingsDlg::Data m_settings;
};

#ifdef __cplusplus
extern "C" {
#endif
    Q_DECL_EXPORT IModule* CreateInstance();
#ifdef __cplusplus
}
#endif
