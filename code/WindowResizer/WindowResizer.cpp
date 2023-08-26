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

// CWindowResizerApp 初始化

BOOL CWindowResizerApp::InitInstance()
{
	CWinApp::InitInstance();

    // 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
    CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

    CWindowResizerDlg* pDlg = new CWindowResizerDlg();
    m_pMainWnd = pDlg;
    pDlg->Create(IDD_WINDOWRESIZER_DIALOG);

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

void WindowResizer::InitInstance()
{
    int nReturnCode = -1;
    CWinThread* pThread = AfxGetThread();
    CWinApp* pApp = AfxGetApp();

    // AFX internal initialization
    if (!AfxWinInit(GetModuleHandle(_T("WindowResizer")), nullptr, _T(""), SW_SHOW))
        return;

    // App global initializations (rare)
    if (pApp != NULL && !pApp->InitApplication())
        return;

    // Perform specific initializations
    if (pThread != nullptr && !pThread->InitInstance())
    {
        if (pThread->m_pMainWnd != NULL)
        {
            TRACE(traceAppMsg, 0, "Warning: Destroying non-NULL m_pMainWnd\n");
            pThread->m_pMainWnd->DestroyWindow();
        }
    }
    //nReturnCode = pThread->Run();
}

void WindowResizer::UnInitInstance()
{
    int nReturnCode = -1;
    CWinThread* pThread = AfxGetThread();
    if (pThread != nullptr)
        nReturnCode = pThread->ExitInstance();
#ifdef _DEBUG
    // Check for missing AfxLockTempMap calls
    if (AfxGetModuleThreadState()->m_nTempMapLock != 0)
    {
        TRACE(traceAppMsg, 0, "Warning: Temp map lock count non-zero (%ld).\n",
            AfxGetModuleThreadState()->m_nTempMapLock);
    }
    AfxLockTempMaps();
    AfxUnlockTempMaps(-1);
#endif

    AfxWinTerm();
}

void* WindowResizer::GetMainWindow()
{
    HWND hWnd = theApp.m_pMainWnd->GetSafeHwnd();
    return hWnd;
}

IModule::eMainWindowType WindowResizer::GetMainWindowType() const
{
    return IModule::MT_HWND;
}

const char* WindowResizer::GetModuleName()
{
    return "WindowResizer";
}

void WindowResizer::OnCommand(const char* strCmd, bool checked)
{
    std::string cmd = strCmd;
    if (cmd == "FindWindow")
    {
        CWindowResizerDlg* pDlg = dynamic_cast<CWindowResizerDlg*>(theApp.m_pMainWnd);
        pDlg->OnBnClickedFindWindowButton();
    }
    else if (cmd == "UserGuid")
    {
        CWindowResizerDlg* pDlg = dynamic_cast<CWindowResizerDlg*>(theApp.m_pMainWnd);
        pDlg->OnBnClickedAboutButton();
    }

}

IModule * CreateInstance()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return new WindowResizer();
}
