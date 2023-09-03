#include "PixelRuler.h"
#include <QScreen>
#include <QGuiApplication>

static PixelRuler* pIns = nullptr;

PixelRuler::PixelRuler()
{
}

PixelRuler* PixelRuler::Instance()
{
    return pIns;
}

int PixelRuler::GetScalePercent()
{
    if (m_pMainFrame != nullptr)
    {
        if (m_pMainFrame->IsItemChecked(CMD_SystemScaleCheck))
        {
            return static_cast<int>(QGuiApplication::primaryScreen()->logicalDotsPerInch() * 100 / 96);
        }
        else
        {
            QString strPercent = QString::fromUtf8(m_pMainFrame->GetItemText(CMD_UserScaleCombo));
            strPercent.chop(1);
            return strPercent.toInt();
        }
    }
    return 100;
}

void PixelRuler::Repaint()
{
    m_mainWidget.update();
    m_horizontalRuler.update();
    m_verticalRuler.update();
}

void PixelRuler::InitInstance()
{
}

void PixelRuler::UnInitInstance()
{
    int a = 0;
}

void PixelRuler::UiInitComplete(IMainFrame* pMainFrame)
{
    m_pMainFrame = pMainFrame;
}

void* PixelRuler::GetMainWindow()
{
    return &m_mainWidget;
}

IModule::eMainWindowType PixelRuler::GetMainWindowType() const
{
    return IModule::MT_QWIDGET;
}

const char* PixelRuler::GetModuleName()
{
    return "PixelRuler";
}

void PixelRuler::OnCommand(const char* strCmd, bool checked)
{
    QString cmd(strCmd);
    if (cmd == CMD_SystemScaleCheck)
    {
        m_pMainFrame->SetItemEnable(CMD_UserScaleCombo, !checked);
        Repaint();
    }
    else if (cmd == CMD_ShowHorizontalRuler)
    {
        m_horizontalRuler.setVisible(checked);
    }
    else if (cmd == CMD_ShowVerticalRuler)
    {
        m_verticalRuler.setVisible(checked);
    }
}

void PixelRuler::OnItemChanged(const char* strId, int index, const char* text)
{
    QString cmd(strId);
    if (cmd == CMD_UserScaleCombo)
    {
        Repaint();
    }
}

void PixelRuler::OnAppExit()
{
    m_horizontalRuler.close();
    m_verticalRuler.close();
}


IModule* CreateInstance()
{
    pIns = new PixelRuler();
    return pIns;
}
