#pragma once

#include "pixelruler_global.h"
#include "moduleinterface.h"
#include "mainframeinterface.h"
#include "MainWidget.h"
#include "RulerWidget.h"

#define CMD_SystemScaleCheck "SystemScaleCheck"
#define CMD_UserScaleCombo "UserScaleCombo"
#define CMD_ShowHorizontalRuler "ShowHorizontalRuler"
#define CMD_ShowVerticalRuler "ShowVerticalRuler"
#define CMD_ScaleUnit10Pixel "ScaleUnit10Pixel"
#define CMD_ScaleUnit8Pixel "ScaleUnit8Pixel"

class PIXELRULER_EXPORT PixelRuler : public IModule
{
public:
    PixelRuler();
    static PixelRuler* Instance();
    int GetScalePercent();
    int GetScaleUnitSize();
    void Repaint();

    // 通过 IModule 继承
    virtual void InitInstance() override;
    virtual void UnInitInstance() override;
    virtual void UiInitComplete(IMainFrame* pMainFrame) override;
    virtual void* GetMainWindow() override;
    virtual eMainWindowType GetMainWindowType() const override;
    virtual const char* GetModuleName() override;
    virtual void OnCommand(const char* strCmd, bool checked) override;
    void OnItemChanged(const char* strId, int index, const char* text) override;
    virtual void OnAppExit() override;

private:
    CMainWidget m_mainWidget;
    CRulerWidget m_horizontalRuler{ Qt::Horizontal };
    CRulerWidget m_verticalRuler{ Qt::Vertical };
    IMainFrame* m_pMainFrame{};
};

#ifdef __cplusplus
extern "C" {
#endif
    __declspec(dllexport) IModule* CreateInstance();
#ifdef __cplusplus
}
#endif
