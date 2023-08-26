#include "MainFrame.h"
#include <QMessageBox>
#include <QMouseEvent>
#include <QApplication>
#include "define.h"

#define QSTR(str) QString::fromWCharArray(L ## str)

MainFrame::MainFrame(QWidget* parent) : RibbonFrameWindow(parent)
{
    setWindowIcon(QIcon(":/DeveloperTools/res/DeveloperTools.ico"));
    setMinimumSize(DPI(500), DPI(400));
    resize(QSize(DPI(800), DPI(600)));
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
