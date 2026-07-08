#include "ColorPicker.h"

static ColorPicker* pIns = nullptr;
ColorPicker::ColorPicker()
{
}

ColorPicker* ColorPicker::Instance()
{
    return pIns;
}

IMainFrame* ColorPicker::GetMainFrame()
{
    return m_pMainFrame;
}

void ColorPicker::InitInstance()
{
    m_main_window.LoadConfig();
}

void ColorPicker::UiInitComplete(IMainFrame* pMainFrame)
{
    m_pMainFrame = pMainFrame;

    //QWidget* pMainWindow = dynamic_cast<QWidget*>(pMainFrame);
    //if (pMainWindow != nullptr)
    //    pMainWindow->setAcceptDrops(true);
}

void ColorPicker::UnInitInstance()
{
    m_main_window.SaveConfig();
}

void* ColorPicker::GetMainWindow()
{
    return &m_main_window;
}

IModule::eMainWindowType ColorPicker::GetMainWindowType() const
{
    return IModule::MT_QWIDGET;
}

const char* ColorPicker::GetModuleName()
{
    return "ColorPicker";
}

void ColorPicker::OnCommand(const char* strCmd, bool checked)
{
    QString cmd(QString::fromUtf8(strCmd));
    if (cmd == CMD_PickColor)
    {
        m_main_window.StartPicking();
    }
    else if (cmd == CMD_CopyRgbValue)
    {

    }
    else if (cmd == CMD_CopyGexValue)
    {

    }
    else if (cmd == CMD_PasteRgbValue)
    {

    }
    else if (cmd == CMD_PasteHexValue)
    {

    }
    else if (cmd == CMD_UseHex)
    {

    }
    else if (cmd == CMD_HexLowerCase)
    {

    }
    else if (cmd == CMD_SelectThemeColor)
    {

    }
    else if (cmd == CMD_AddGetSysColorTable)
    {

    }
}


IModule* CreateInstance()
{
    pIns = new ColorPicker();
    return pIns;
}
