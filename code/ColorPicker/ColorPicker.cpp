#include "ColorPicker.h"
#include "../CCommonTools/Config.h"
#include "common/Common.h"

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
    CConfig settings(QString::fromUtf8(GetModuleName()));
    m_main_window.SetUseHex(settings.GetValue("use_hex", false).toBool());
    m_main_window.SetHexLowerCase(settings.GetValue("hex_lower_case", true).toBool());
    m_main_window.SetColor(settings.GetValue("color").toString());

    m_main_window.LoadConfig();
}

void ColorPicker::UiInitComplete(IMainFrame* pMainFrame)
{
    m_pMainFrame = pMainFrame;
    pMainFrame->SetItemChecked(CMD_UseHex, m_main_window.GetUseHex());
    pMainFrame->SetItemChecked(CMD_HexLowerCase, m_main_window.GetHexLowerCase());
#ifndef Q_OS_WIN
    pMainFrame->SetItemEnable(CMD_AddGetSysColorTable, false);
#endif
}

void ColorPicker::UnInitInstance()
{
    m_main_window.SaveConfig();

    CConfig settings(QString::fromUtf8(GetModuleName()));
    settings.WriteValue("use_hex", m_main_window.GetUseHex());
    settings.WriteValue("hex_lower_case", m_main_window.GetHexLowerCase());
    settings.WriteValue("color", m_main_window.GetColor().name());
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
        m_main_window.CopyRgbValue();
    }
    else if (cmd == CMD_CopyGexValue)
    {
        m_main_window.CopyHexValue();
    }
    else if (cmd == CMD_PasteRgbValue)
    {
        m_main_window.PasteRgbValue();
    }
    else if (cmd == CMD_PasteHexValue)
    {
        m_main_window.PasteHexValue();
    }
    else if (cmd == CMD_UseHex)
    {
        m_main_window.SetUseHex(checked);
    }
    else if (cmd == CMD_HexLowerCase)
    {
        m_main_window.SetHexLowerCase(checked);
    }
    else if (cmd == CMD_SelectThemeColor)
    {
        QColor theme_color = Common::GetSystemThemeColor();
        m_main_window.SetColor(theme_color);
    }
    else if (cmd == CMD_AddGetSysColorTable)
    {
        m_main_window.AddGetSysColorTable();
    }
}


IModule* CreateInstance()
{
    pIns = new ColorPicker();
    return pIns;
}
