#pragma once

#include "filerename_global.h"
#include "moduleinterface.h"
#include "mainframeinterface.h"
#include "MainWidget.h"

#define CMD_FileRenameAddFiles "FileRenameAddFiles"
#define CMD_FileRenameAddFolder "FileRenameAddFolder"
#define CMD_FileRenameDelete "FileRenameDelete"
#define CMD_FileRenameClear "FileRenameClear"
#define CMD_RenameWithModifiedTime "RenameWithModifiedTime"

class FILERENAME_EXPORT FileRename : public IModule
{
public:
    FileRename();

    // 通过 IModule 继承
    virtual void InitInstance() override;
    virtual void UnInitInstance() override;
    virtual void* GetMainWindow() override;
    virtual eMainWindowType GetMainWindowType() const override;
    virtual const char* GetModuleName() override;
    virtual void OnCommand(const char* strCmd, bool checked) override;

private:
    MainWidget m_mainWidget;
};

#ifdef __cplusplus
extern "C" {
#endif
    Q_DECL_EXPORT IModule* CreateInstance();
#ifdef __cplusplus
}
#endif
