#include "MainFrame.h"
#include <QMessageBox>
#include <QMouseEvent>
#include <QApplication>
#include "../CCommonTools/Config.h"
#include "define.h"

#define QSTR(str) QString::fromWCharArray(L ## str)

MainFrame::MainFrame(QWidget* parent) : RibbonFrameWindow(parent)
{
    setWindowIcon(QIcon(":/DeveloperTools/res/DeveloperTools.ico"));
    setMinimumSize(DPI(500), DPI(400));

    //载入配置
    CConfig settings;
    int tabIndex = settings.GetValue("tabIndex").toInt();
    SetTabIndex(tabIndex);
    int windowWidth = settings.GetValue("windowWidth", DPI(800)).toInt();
    int windowHeight = settings.GetValue("windowHeight", DPI(600)).toInt();
    resize(QSize(windowWidth, windowHeight));
}

MainFrame::~MainFrame()
{
    //保存配置
    CConfig settings;
    settings.WriteValue("tabIndex", GetTabIndex());
    QSize windowSize = size();
    settings.WriteValue("windowWidth", windowSize.width());
    settings.WriteValue("windowHeight", windowSize.height());
}


bool MainFrame::OnCommand(const QString& strCmd, bool checked)
{
    if (strCmd == "AppAbout")
    {
        QString strAboutInfo = QString(u8"%1 %2\r\n一个各种小工具合集。\r\nCopyright(C) 2023 by ZhongYang").arg(APP_NAME).arg(APP_VERSION);
        QMessageBox::about(this, QString(u8"关于") + APP_NAME, strAboutInfo);
    }
    return RibbonFrameWindow::OnCommand(strCmd, checked);
}
