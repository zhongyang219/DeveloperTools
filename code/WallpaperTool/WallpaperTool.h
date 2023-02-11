#pragma once

#include "wallpapertool_global.h"
#include "moduleinterface.h"
#include <QObject>
#include "MainWidget.h"

class WALLPAPERTOOL_EXPORT WallpaperTool
    : public QObject, public IModuleInterface
{
    Q_OBJECT
public:
    WallpaperTool();

    // 通过 IModuleInterface 继承
    virtual void InitModule() override;
    virtual void UnInitModule() override;
    virtual unsigned long long GetMainWindow() override;
    virtual eMainWindowType GetMainWindowType() const override;
    virtual const wchar_t* GetModuleName() override;
    virtual void CommandTrigerd(const wchar_t* strCmd, bool checked) override;

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
    __declspec(dllexport) IModuleInterface* CreateInstance();
#ifdef __cplusplus
}
#endif
