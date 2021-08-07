﻿// WindowResizer.h: WindowResizer DLL 的主标头文件
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

    static HWND m_hMainWnd;

    // 重写
public:
	virtual BOOL InitInstance();


	DECLARE_MESSAGE_MAP()
    virtual int ExitInstance();
};

class WindowResizer : public IModuleInterface
{
public:
    WindowResizer();

    //重写IModuleInterface
public:
    virtual void InitModule() override;
    virtual void UnInitModule() override;
    virtual unsigned long long GetMainWindow() override;
    virtual const wchar_t* GetModuleName() override;
    virtual void CommandTrigerd(const wchar_t* strCmd, bool checked) override;
    virtual eMainWindowType GetMainWindowType() const override;

};

#ifdef __cplusplus
extern "C" {
#endif
    //导出一个名为CreateInstance的函数以创建对象
    WINDOWRISZER_API IModuleInterface* CreateInstance();

#ifdef __cplusplus
}
#endif
