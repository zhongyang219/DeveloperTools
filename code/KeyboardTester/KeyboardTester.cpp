#include "KeyboardTester.h"
#include <QUrl>
#include <QGuiApplication>
#include <QScreen>

static KeyboardTester* pIns = nullptr;
KeyboardTester::KeyboardTester()
{
}

KeyboardTester* KeyboardTester::Instance()
{
    return pIns;
}

void KeyboardTester::InitInstance()
{
    m_mainWidget.load(QUrl("qrc:/res/index.html"));
    qreal factor = QGuiApplication::primaryScreen()->logicalDotsPerInch() / 96.0;
    m_mainWidget.setZoomFactor(factor);
}

void KeyboardTester::UiInitComplete(IMainFrame* pMainFrame)
{
    m_pMainFrame = pMainFrame;
}

void KeyboardTester::UnInitInstance()
{
}

void* KeyboardTester::GetMainWindow()
{
    return &m_mainWidget;
}

IModule::eMainWindowType KeyboardTester::GetMainWindowType() const
{
    return IModule::MT_QWIDGET;
}

const char* KeyboardTester::GetModuleName()
{
    return "KeyboardTester";
}

void KeyboardTester::OnCommand(const char* strCmd, bool checked)
{
    QString cmd(QString::fromUtf8(strCmd));
}


IModule* CreateInstance()
{
    pIns = new KeyboardTester();
    return pIns;
}
