#include "PixelRuler.h"
#include <QScreen>
#include <QGuiApplication>
#include "../CCommonTools/Config.h"
#include "../CCommonTools/TextEditWidget.h"
#include <QFile>
#include "define.h"

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

int PixelRuler::GetScaleUnitSize()
{
    if (m_pMainFrame != nullptr)
    {
        if (m_pMainFrame->IsItemChecked(CMD_ScaleUnit8Pixel))
            return 8;
    }
    return 10;
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
    //保存设置
    CConfig settings(QString::fromUtf8(GetModuleName()));
    settings.WriteValue("useSystemScale", m_pMainFrame->IsItemChecked(CMD_SystemScaleCheck));
    settings.WriteValue("userScale", m_pMainFrame->GetItemCurIndex(CMD_UserScaleCombo));
    settings.WriteValue("scaleUnit8Pixel", m_pMainFrame->IsItemChecked(CMD_ScaleUnit8Pixel));
}

void PixelRuler::UiInitComplete(IMainFrame* pMainFrame)
{
    m_pMainFrame = pMainFrame;
    //载入设置
    CConfig settings(QString::fromUtf8(GetModuleName()));
    m_pMainFrame->SetItemChecked(CMD_SystemScaleCheck, settings.GetValue("useSystemScale").toBool());
    m_pMainFrame->SetItemCurIIndex(CMD_UserScaleCombo, settings.GetValue("userScale").toInt());
    bool scaleUnit8Pixel = settings.GetValue("scaleUnit8Pixel").toBool();
    if (scaleUnit8Pixel)
        m_pMainFrame->SetItemChecked(CMD_ScaleUnit8Pixel, true);
    else
        m_pMainFrame->SetItemChecked(CMD_ScaleUnit10Pixel, true);

    EnableControl();
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
        EnableControl();
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
    else if (cmd == CMD_ScaleUnit10Pixel || cmd == CMD_ScaleUnit8Pixel)
    {
        Repaint();
    }
    else if (cmd == CMD_PixelRulerHelp)
    {
        CTextEditDialog dlg(&m_mainWidget);
        dlg.resize(DPI(560), DPI(360));
        dlg.setWindowTitle(u8"像素尺帮助");
        QFile file(":/res/help.md");
        if (file.open(QIODevice::ReadOnly))
        {
            QString helpContents = QString::fromUtf8(file.readAll());
#if (QT_VERSION >= QT_VERSION_CHECK(5,14,0))
            dlg.GetEdit()->setMarkdown(helpContents);
#else
            dlg.GetEdit()->setPlainText(helpContents);
#endif
            dlg.GetEdit()->setReadOnly(true);
        }
        dlg.exec();
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

void PixelRuler::EnableControl()
{
    if (m_pMainFrame != nullptr)
    {
        m_pMainFrame->SetItemEnable(CMD_UserScaleCombo, !m_pMainFrame->IsItemChecked(CMD_SystemScaleCheck));
    }
}


IModule* CreateInstance()
{
    pIns = new PixelRuler();
    return pIns;
}
