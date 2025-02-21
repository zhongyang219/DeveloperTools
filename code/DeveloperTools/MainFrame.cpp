#include "MainFrame.h"
#include <QMessageBox>
#include <QMouseEvent>
#include <QApplication>
#include "../CCommonTools/Config.h"
#include "define.h"

#define QSTR(str) QString::fromWCharArray(L ## str)

MainFrame::MainFrame(const QStringList& cmdLine, QWidget* parent) : RibbonFrameWindow(parent, QString(), false, cmdLine)
{
    QIcon appIcon(":/DeveloperTools/res/DeveloperTools.ico");
    qApp->setWindowIcon(appIcon);
    SetItemIcon("AppAbout", appIcon);

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
    if (!isMaximized())
    {
        QSize windowSize = size();
        settings.WriteValue("windowWidth", windowSize.width());
        settings.WriteValue("windowHeight", windowSize.height());
    }
}

void MainFrame::closeEvent(QCloseEvent* event)
{
    RibbonFrameWindow::closeEvent(event);
}


bool MainFrame::OnCommand(const QString& strCmd, bool checked)
{
    if (strCmd == "AppAbout")
    {
        QString strDebug;
#ifdef _DEBUG
        strDebug = u8"(Debug 模式)";
#endif // DEBUG

        QString strAboutInfo = QString(u8"%1 V%2 %3<br/>"
            "一个各种小工具合集。<br/>"
            "Copyright(C) 2024 by ZhongYang<br/>"
            "<a href=\"https://github.com/zhongyang219/DeveloperTools\">GitHub</a>")
            .arg(APP_NAME).arg(APP_VERSION).arg(strDebug);

        QMessageBox aboutBox;
        aboutBox.setWindowTitle(QString(u8"关于") + APP_NAME);
        aboutBox.setTextFormat(Qt::RichText);  // 设置文本格式为富文本（HTML）
        aboutBox.setText(strAboutInfo);
        QIcon icon = QApplication::windowIcon();
        if (!icon.isNull())
        {
            QSize size = icon.actualSize(QSize(64, 64));
            aboutBox.setIconPixmap(icon.pixmap(size));
        }
        aboutBox.exec();
    }
    return RibbonFrameWindow::OnCommand(strCmd, checked);
}
