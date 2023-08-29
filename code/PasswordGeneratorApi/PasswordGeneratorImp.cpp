// 这是主 DLL 文件。

#include "stdafx.h"

#include "PasswordGeneratorImp.h"
#include "mainframeinterface.h"

namespace PasswordGeneratorApi
{
    IMainFrame* mainFrame = nullptr;

    static std::wstring ClrStringToStdWstring(System::String^ str)
    {
        const wchar_t* chars = reinterpret_cast<const wchar_t*>((System::Runtime::InteropServices::Marshal::StringToHGlobalUni(str)).ToPointer());
        return std::wstring(chars);
    }

    static System::String^ utf8ToClrString(const char* str)
    {
        System::Text::Encoding^ u8enc = System::Text::Encoding::UTF8;
        return u8enc->GetString((unsigned char*)str, static_cast<int>(strlen(str)));
    }

    static std::string ClrStringToUtf8(System::String^ str)
    {
        cli::array<unsigned char>^ arr = System::Text::Encoding::UTF8->GetBytes(str);
        std::string rtnStr;
        for (int i = 0; i < arr->Length; i++)
            rtnStr[i] = arr[i];
    }

    static void SetStatusBarText(System::String^ str, int timeOut)
    {
        if (mainFrame != nullptr)
        {
            mainFrame->SetStatusBarText(ClrStringToUtf8(str).c_str(), timeOut);
        }
    }

    void CPasswordGenerator::InitInstance()
    {
    }

    void CPasswordGenerator::UnInitInstance()
    {
    }

    void CPasswordGenerator::UiInitComplete(IMainFrame* pMainFrame)
    {
        mainFrame = pMainFrame;
    }

    void* CPasswordGenerator::GetMainWindow()
    {
        return Program::GetForm(true)->GetHandle().ToPointer();
    }

    IModule::eMainWindowType CPasswordGenerator::GetMainWindowType() const
    {
        return IModule::MT_HWND;
    }

    const char* CPasswordGenerator::GetModuleName()
    {
        return "PasswordGenerator";
    }

    void CPasswordGenerator::OnCommand(const char* strCmd, bool checked)
    {
        std::string cmd = strCmd;
        if (cmd == "PasswordGeneratorAbout")
            Program::GetForm(true)->aboutButton_Click(nullptr, nullptr);
        else if (cmd == "PasswordGeneratorGenerate")
            Program::GetForm(true)->GenerateButton_Click(nullptr, nullptr);
        else if (cmd == "PasswordGeneratorCopy")
            Program::GetForm(true)->CopyButton_Click(nullptr, nullptr);
    }

}


IModule* CreateInstance()
{
    return new PasswordGeneratorApi::CPasswordGenerator();
}
