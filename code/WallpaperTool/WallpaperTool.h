#pragma once

#include "wallpapertool_global.h"
#include "moduleinterface.h"
#include <QObject>
#include "MainWidget.h"
#include "WallpaperHelper.h"

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
    QString m_strCurWallpaperPath;
    QString m_strLastSaveDir;       //上次壁纸的保存路径
    CWallpaperHelper m_helper;
};

#ifdef __cplusplus
extern "C" {
#endif
    Q_DECL_EXPORT IModule* CreateInstance();
#ifdef __cplusplus
}
#endif
