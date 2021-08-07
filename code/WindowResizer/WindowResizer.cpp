// WindowResizer.cpp: 定义 DLL 的初始化例程。
//

#include "pch.h"
#include "framework.h"
#include "WindowResizer.h"
#include <string>
#include "WindowResizerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO:  如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。  这意味着
//		它必须作为以下项中的第一个语句:
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CWindowResizerApp

BEGIN_MESSAGE_MAP(CWindowResizerApp, CWinApp)
END_MESSAGE_MAP()


// CWindowResizerApp 构造

CWindowResizerApp::CWindowResizerApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CWindowResizerApp 对象

CWindowResizerApp theApp;

HWND CWindowResizerApp::m_hMainWnd;

// CWindowResizerApp 初始化

BOOL CWindowResizerApp::InitInstance()
{
	CWinApp::InitInstance();

    CWindowResizerDlg dlg;
    dlg.Create(IDD_WINDOWRESIZER_DIALOG);
    //dlg.ShowWindow(SW_SHOW);
    //INT_PTR nResponse = dlg.DoModal();
    m_pMainWnd = &dlg;
    m_hMainWnd = dlg.GetSafeHwnd();

	return TRUE;
}


int CWindowResizerApp::ExitInstance()
{
    // TODO: 在此添加专用代码和/或调用基类

    return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////

WindowResizer::WindowResizer()
{
}


void WindowResizer::InitModule()
{
}

void WindowResizer::UnInitModule()
{
    //DWORD dwExitCode;
    //GetExitCodeThread(m_hThread, &dwExitCode);
    //FreeLibraryAndExitThread((HMODULE)hModule, dwExitCode);

    //AfxEndThread(dwExitCode, FALSE);
    //::TerminateProcess(::GetCurrentProcess, 0);
    //ExitInstance();

}

unsigned long long WindowResizer::GetMainWindow()
{
    return (unsigned long long)CWindowResizerApp::m_hMainWnd;
}

IModuleInterface::eMainWindowType WindowResizer::GetMainWindowType() const
{
    return IModuleInterface::MT_HWND;
}


const wchar_t* WindowResizer::GetModuleName()
{
    return L"WindowResizer";
}

void WindowResizer::CommandTrigerd(const wchar_t* strCmd, bool checked)
{
    std::wstring wcsCmd = strCmd;
    if (wcsCmd == L"FindWindow")
    {
        AfxMessageBox(_T("点击了查找窗口"), MB_ICONINFORMATION | MB_OK);
    }
    else if (wcsCmd == L"UserGuid")
    {
        AfxMessageBox(_T("点击了使用说明"), MB_ICONINFORMATION | MB_OK);
    }
}


IModuleInterface * CreateInstance()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return new WindowResizer();
}
