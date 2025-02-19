#pragma once

#include "KeyboardTester_global.h"
#include "moduleinterface.h"
#include "mainframeinterface.h"
#include <QWebEngineView>


class KEYBOARDTESTER_EXPORT KeyboardTester : public IModule
{
public:
    KeyboardTester();
	static KeyboardTester* Instance();

    // 通过 IModule 继承
    virtual void InitInstance() override;
	virtual void UiInitComplete(IMainFrame* pMainFrame) override;
    virtual void UnInitInstance() override;
    virtual void* GetMainWindow() override;
    virtual eMainWindowType GetMainWindowType() const override;
    virtual const char* GetModuleName() override;
    virtual void OnCommand(const char* strCmd, bool checked) override;

private:
    QWebEngineView m_mainWidget;
	IMainFrame* m_pMainFrame{};
};

#ifdef __cplusplus
extern "C" {
#endif
    Q_DECL_EXPORT IModule* CreateInstance();
#ifdef __cplusplus
}
#endif
