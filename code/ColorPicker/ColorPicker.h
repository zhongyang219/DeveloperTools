#pragma once

#include "ColorPicker_global.h"
#include "moduleinterface.h"
#include "mainframeinterface.h"
#include "widgets/colorpickerwindow.h"

#define CMD_ColorPickerAddFiles "ColorPickerAddFiles"
#define CMD_ColorPickerAddFolder "ColorPickerAddFolder"
#define CMD_ColorPickerDelete "ColorPickerDelete"
#define CMD_ColorPickerClear "ColorPickerClear"
#define CMD_RenameWithModifiedTime "RenameWithModifiedTime"

class COLOR_PICKER_EXPORT ColorPicker : public IModule
{
public:
    ColorPicker();
	static ColorPicker* Instance();
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
	ColorPickerWindow m_main_window;
};

#ifdef __cplusplus
extern "C" {
#endif
    Q_DECL_EXPORT IModule* CreateInstance();
#ifdef __cplusplus
}
#endif
