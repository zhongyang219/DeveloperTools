#pragma once

#include "wallpapertool_global.h"
#include "moduleinterface.h"
#include <QObject>
#include "MainWidget.h"

class WALLPAPERTOOL_EXPORT WallpaperTool
    : public QObject, public IModule
{
    Q_OBJECT
public:
    WallpaperTool();

    // 通过 IModule 继承
    virtual void InitInstance() override;
    virtual void UnInitInstance() override;
    virtual void* GetMainWindow() override;
    virtual eMainWindowType GetMainWindowType() const override;
    virtual const char* GetModuleName() override;
    virtual void OnCommand(const char* strCmd, bool checked) override;

private:
    void WriteLog(const QString& strLogInfo);

private:
    MainWidget m_mainWidget;
    QString m_strCurWallpaperPath;
    QString m_strLastSaveDir;       //上次壁纸的保存路径
};

#ifdef __cplusplus
extern "C" {
#endif
    __declspec(dllexport) IModule* CreateInstance();
#ifdef __cplusplus
}
#endif
