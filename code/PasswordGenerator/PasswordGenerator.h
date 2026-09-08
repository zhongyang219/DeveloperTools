#pragma once

#include "PasswordGenerator_global.h"
#include "moduleinterface.h"
#include "mainframeinterface.h"
#include "widgets/passwordgeneratorwidget.h"

#define CMD_PickColor "PickColor"
#define CMD_CopyRgbValue "CopyRgbValue"
#define CMD_CopyGexValue "CopyGexValue"
#define CMD_PasteRgbValue "PasteRgbValue"
#define CMD_PasteHexValue "PasteHexValue"
#define CMD_UseHex "UseHex"
#define CMD_HexLowerCase "HexLowerCase"
#define CMD_SelectThemeColor "SelectThemeColor"
#define CMD_AddGetSysColorTable "AddGetSysColorTable"
#define CMD_ImportColorTable "ImportColorTable"
#define CMD_ExportColorTable "ExportColorTable"

class PASSWORD_GENERATOR_EXPORT PasswordGenerator : public IModule
{
public:
    PasswordGenerator();
    static PasswordGenerator* Instance();
    IMainFrame* GetMainFrame();

    // 通过 IModule 继承
    virtual void InitInstance() override;
    virtual void UiInitComplete(IMainFrame* pMainFrame) override;
    virtual void UnInitInstance() override;
    virtual void* GetMainWindow() override;
    virtual eMainWindowType GetMainWindowType() const override;
    virtual const char* GetModuleName() override;
    virtual void OnCommand(const char* strCmd, bool checked) override;

private:
    IMainFrame* m_pMainFrame{};
    PasswordGeneratorWidget m_main_window;
};

#ifdef __cplusplus
extern "C" {
#endif
    Q_DECL_EXPORT IModule* CreateInstance();
#ifdef __cplusplus
}
#endif
