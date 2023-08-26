// WindowResizer.h: WindowResizer DLL 的主标头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// 主符号
#include "moduleinterface.h"
#include "Macros.h"

// CWindowResizerApp
// 有关此类实现的信息，请参阅 WindowResizer.cpp
//

class CWindowResizerApp : public CWinApp
{
public:
	CWindowResizerApp();

    // 重写
public:
	virtual BOOL InitInstance();


	DECLARE_MESSAGE_MAP()
    virtual int ExitInstance();
};

extern CWindowResizerApp theApp;


/// //////////////////////////////////////////////////////////////////////////
class WindowResizer : public IModule
{
public:
    WindowResizer();

public:
    // 通过 IModule 继承
    virtual void InitInstance() override;
    virtual void UnInitInstance() override;
    virtual void* GetMainWindow() override;
    virtual eMainWindowType GetMainWindowType() const override;
    virtual const char* GetModuleName() override;
    virtual void OnCommand(const char* strCmd, bool checked) override;

};

#ifdef __cplusplus
extern "C" {
#endif
    //导出一个名为CreateInstance的函数以创建对象
    WINDOWRISZER_API IModule* CreateInstance();

#ifdef __cplusplus
}
#endif
