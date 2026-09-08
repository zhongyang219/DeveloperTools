#include "PasswordGenerator.h"
#include "../CCommonTools/Config.h"

static PasswordGenerator* pIns = nullptr;
PasswordGenerator::PasswordGenerator()
{
}

PasswordGenerator* PasswordGenerator::Instance()
{
    return pIns;
}

IMainFrame* PasswordGenerator::GetMainFrame()
{
    return m_pMainFrame;
}

void PasswordGenerator::InitInstance()
{
    CConfig settings(QString::fromUtf8(GetModuleName()));
    m_main_window.LoadConfig();
}

void PasswordGenerator::UiInitComplete(IMainFrame* pMainFrame)
{
    m_pMainFrame = pMainFrame;
}

void PasswordGenerator::UnInitInstance()
{
    m_main_window.SaveConfig();
}

void* PasswordGenerator::GetMainWindow()
{
    return &m_main_window;
}

IModule::eMainWindowType PasswordGenerator::GetMainWindowType() const
{
    return IModule::MT_QWIDGET;
}

const char* PasswordGenerator::GetModuleName()
{
    return "PasswordGenerator";
}

void PasswordGenerator::OnCommand(const char* strCmd, bool checked)
{
    QString cmd(QString::fromUtf8(strCmd));
    if (cmd == "PasswordGeneratorGenerate")
        m_main_window.onGenerateClicked();
    else if (cmd == "PasswordGeneratorCopy")
        m_main_window.onCopyClicked();
    else if (cmd == "PasswordGenerator8Num")
        m_main_window.SetPasswordLength(8);
    else if (cmd == "PasswordGenerator16Num")
        m_main_window.SetPasswordLength(16);
    else if (cmd == "PasswordGenerator24Num")
        m_main_window.SetPasswordLength(24);
    else if (cmd == "PasswordGenerator32Num")
        m_main_window.SetPasswordLength(32);

}


IModule* CreateInstance()
{
    pIns = new PasswordGenerator();
    return pIns;
}
