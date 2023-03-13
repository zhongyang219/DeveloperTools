#include "MainFrame.h"
#include <QApplication>
#include <QFile>
#include "define.h"
#include <QWindow>
#include <QFile>
#include <QDomDocument>
#include <QIcon>
#include "../CCommonTools/CommonTools.h"
#include <QMessageBox>
#include <QLibrary>
#include <QToolBar>
#include <QDebug>
#include <QVBoxLayout>
#include <QLabel>
#include <QTabBar>
#include "../CCommonTools/Config.h"

#define ICON_SIZE DPI(32)

MainFrame::MainFrame(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowIcon(QIcon(":/DeveloperTools/res/DeveloperTools.ico"));
    setMinimumSize(DPI(500), DPI(400));
    resize(DPI(800), DPI(600));

    setCentralWidget(m_pTabWidget = new QTabWidget(this));
    m_pTabWidget->setObjectName("MainFrameTab");
    m_pTabWidget->tabBar()->setObjectName("MainFrameTabBar");

    //QIcon iconAbout(CCommonTools::CreateIcon(":/DeveloperTools/res/DeveloperTools.ico", DPI(16)));
    //QAction* aboutAction = bar->addAction(iconAbout, u8"关于...", Qt::ToolButtonIconOnly);
    //connect(aboutAction, SIGNAL(triggered()), this, SLOT(OnAbout()));

    //在第一个标签左边添加文件按钮
    m_pTabWidget->addTab(new QWidget(), QString());
    m_pTabWidget->setTabEnabled(0, false);
    m_pFileBtn = new QPushButton(u8"文件");
    m_pFileBtn->setObjectName("MainFrameFileBtn");
    m_pTabWidget->tabBar()->setTabButton(0, QTabBar::LeftSide, m_pFileBtn);
    connect(m_pFileBtn, SIGNAL(clicked()), this, SLOT(OnFileBtnClicked()));

    //加载插件
    LoadUIFromXml();

    //初始化文件菜单
    m_pFileMenu = new QMenu(this);
    m_pFileMenu->setObjectName("FileMenu");
    QMenu* pStyleMenu = m_pFileMenu->addMenu(u8"主题");
    pStyleMenu->setObjectName("FileMenu");
    pStyleMenu->addAction(u8"浅色", this, SLOT(OnStyleLight()));
    pStyleMenu->addAction(u8"深色", this, SLOT(OnStyleDark()));
    pStyleMenu->addAction(u8"默认", this, SLOT(OnStyleDefault()));
    m_pFileMenu->addAction(CCommonTools::CreateIcon(":/DeveloperTools/res/DeveloperTools.ico", DPI(16)), u8"关于...", this, SLOT(OnAbout()));
    m_pFileMenu->addSeparator();
    m_pFileMenu->addAction(CCommonTools::CreateIcon("Images/delete.png", DPI(16)), u8"退出", this, SLOT(close()));

    //初始化插件
    for (auto iter = m_moduleMap.begin(); iter != m_moduleMap.end(); ++iter)
    {
        if (iter->second != nullptr)
            iter->second->InitModule();
    }

    //标签切换消息
    connect(m_pTabWidget, SIGNAL(currentChanged(int)), this, SLOT(OnTabIndexChanged(int)));

    LoadConfig();

    //设置样式
    ApplyStyle();
}

MainFrame::~MainFrame()
{
    SaveConfig();
    //反初始化插件
    for (auto iter = m_moduleMap.begin(); iter != m_moduleMap.end(); ++iter)
    {
        if (iter->second != nullptr)
            iter->second->UnInitModule();
    }
}

void MainFrame::OnTabIndexChanged(int index)
{
    IModuleInterfacePtr pModule = m_moduleMap[index];
    if (pModule != nullptr)
    {
        pModule->OnTabEntered();
    }
}

void MainFrame::OnActionTriggerd(bool checked)
{
    QAction* pSender = qobject_cast<QAction*>(QObject::sender());
    if (pSender != nullptr)
    {
        QString strCmdId = pSender->data().toString();
        if (!strCmdId.isEmpty())
        {
            //RibbonBar* bar = dynamic_cast<RibbonBar*>(menuBar());
            int index = m_pTabWidget->currentIndex();
            IModuleInterfacePtr pModule = m_moduleMap[index];
            if (pModule != nullptr)
            {
                pModule->CommandTrigerd(strCmdId.toStdWString().c_str(), checked);
            }
        }
    }

}

void MainFrame::OnFileBtnClicked()
{
    QPoint btnPos = mapToGlobal(m_pFileBtn->rect().bottomLeft());
    btnPos.setY(btnPos.y() + DPI(4));
    m_pFileMenu->exec(btnPos);
}

void MainFrame::OnAbout()
{
    QString strAboutInfo = QString(u8"%1 %2\r\n一个各种小工具合集。\r\nCopyright(C) 2023 by ZhongYang").arg(APP_NAME).arg(APP_VERSION);
    QMessageBox::about(this, QString(u8"关于") + APP_NAME, strAboutInfo);
}

void MainFrame::OnStyleLight()
{
    m_style = CStyleManager::S_LIGHT;
    //ApplyStyle();
    StyleChanged();
}

void MainFrame::OnStyleDark()
{
    m_style = CStyleManager::S_DARK;
    //ApplyStyle();
    StyleChanged();
}

void MainFrame::OnStyleDefault()
{
    m_style = CStyleManager::S_DEFAULT;
    //ApplyStyle();
    StyleChanged();
}

void MainFrame::LoadUIFromXml()
{
    QString dir = qApp->applicationDirPath();
    QFile file(dir + "/MainFrame.xml");
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        CCommonTools::WriteLog(u8"打开MainFrame.xml文件失败！");
        return;
    }

    QDomDocument doc;
    if (!doc.setContent(&file, false))
        return;

    QDomElement root = doc.documentElement();
    if (root.isNull())
    {
        CCommonTools::WriteLog(u8"MainFrame.xml文件找不到节点！");
        return;
    }

    //加载页面
    QDomNodeList childNode = root.childNodes();
    for (int i = 0; i < childNode.count(); i++)
    {
        QDomElement nodeInfo = childNode.at(i).toElement();
        QString tagName = nodeInfo.tagName();
        if (tagName == "Page")
        {
            QString strTabName = nodeInfo.attribute("name");
            QString strModulePath = nodeInfo.attribute("modulePath");

            //载入模块
            QToolBar* pToolbar;
            bool bModuleLoaded = false;
            IModuleInterfacePtr pModule;
            QLibrary libMgr(strModulePath);
            if (libMgr.load())
            {
                pfCreateModuleInstance fun = (pfCreateModuleInstance)libMgr.resolve("CreateInstance");
                if (fun != nullptr)
                {
                    pModule = IModuleInterfacePtr(fun());
                    if (pModule != nullptr)
                    {
                        qDebug() << QString(u8"载入插件 %1 成功").arg(strModulePath);

                        int index = m_pTabWidget->count();

                        //添加标签页
                        QWidget* pWidget = new QWidget();
                        m_pTabWidget->addTab(pWidget, strTabName);
                        QVBoxLayout* pLayout = new QVBoxLayout();
                        pLayout->setMargin(0);
                        pWidget->setLayout(pLayout);
                        pLayout->addWidget(pToolbar = new QToolBar());
                        pToolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
                        pToolbar->setIconSize(QSize(ICON_SIZE, ICON_SIZE));
                        QWidget* pModuleMainWindow = GetModuleMainWindow(pModule);
                        if (pModuleMainWindow != nullptr)
                            pLayout->addWidget(pModuleMainWindow);
                        else
                            pLayout->addWidget(new QLabel(u8"主窗口加载失败"));

                        m_moduleMap[index] = pModule;
                        bModuleLoaded = true;
                    }
                }
                else
                {
                    CCommonTools::WriteLog(QString(u8"从模块“%1”中获取“CreateInstance”的入口地址失败！").arg(strModulePath));
                }
            }
            else
            {
                CCommonTools::WriteLog(QString(u8"加载模块“%1”失败！").arg(strModulePath));
            }

            //载入命令组
            if (bModuleLoaded)
            {
                QDomNodeList actionList = nodeInfo.childNodes();
                for (int j = 0; j < actionList.count(); j++)
                {
                    QDomElement actionNodeInfo = actionList.at(j).toElement();
                    QString strTagName = actionNodeInfo.tagName();
                    if (strTagName == "Action")
                    {
                        QString strCmdName = actionNodeInfo.attribute("name");
                        QString strIconPath = actionNodeInfo.attribute("icon");
                        QString strCmdId = actionNodeInfo.attribute("commandID");
                        QString strCheckable = actionNodeInfo.attribute("checkable");
                        bool bCheckable = (strCheckable == "true" || strCheckable == "TRUE" || strCheckable == "True");
                        QString strTip = actionNodeInfo.attribute("tip");

                        QAction* pAction = pToolbar->addAction(CCommonTools::CreateIcon(qApp->applicationDirPath() + "/" + strIconPath, ICON_SIZE), strCmdName);
                        pAction->setData(strCmdId);     //将命令的ID作为用户数据保存到QAction对象中
                        pAction->setCheckable(bCheckable);
                        pAction->setToolTip(strTip);

                        connect(pAction, SIGNAL(triggered(bool)), this, SLOT(OnActionTriggerd(bool)));

                        m_actionMap[strCmdId] = pAction;
                    }
                    else if (strTagName == "Separator")
                    {
                        pToolbar->addSeparator();
                    }
                }
            }
        }
    }
}

QWidget* MainFrame::GetModuleMainWindow(IModuleInterfacePtr pModule)
{
    QWidget* pWidget{};
    IModuleInterface::eMainWindowType windowType = pModule->GetMainWindowType();
    if (windowType == IModuleInterface::MT_HWND)
    {
        WId wid = (WId)(pModule->GetMainWindow());
        pWidget = QWidget::createWindowContainer(QWindow::fromWinId(wid));
    }
    else
    {
        pWidget = (QWidget*)pModule->GetMainWindow();
    }
    return pWidget;
}

void MainFrame::LoadConfig()
{
    CConfig settings;
    int tabIndex = settings.GetValue("tabIndex").toInt();
    m_pTabWidget->setCurrentIndex(tabIndex);
    m_style = static_cast<CStyleManager::Style>(settings.GetValue("style", 1).toInt());
}

void MainFrame::SaveConfig() const
{
    CConfig settings;
    settings.WriteValue("tabIndex", m_pTabWidget->currentIndex());
    settings.WriteValue("style", static_cast<int>(m_style));
}

void MainFrame::ApplyStyle()
{
    CStyleManager::Instance()->SetStyle(m_style, this);
}

void MainFrame::StyleChanged()
{
    QMessageBox::question(this, QString(), u8"更改主题后请重新启动程序以生效。");
}

QAction * MainFrame::GetAction(const QString & strCmd) const
{
    auto iter = m_actionMap.find(strCmd);
    if (iter != m_actionMap.end())
        return iter->second;
    else
        return nullptr;
}

IModuleInterface * MainFrame::GetModule(const QString & strModuleName) const
{
    for (const auto& moduleItem : m_moduleMap)
    {
        if (moduleItem.second != nullptr && QString::fromWCharArray(moduleItem.second->GetModuleName()) == strModuleName)
            return moduleItem.second.get();
    }
    return nullptr;
}
