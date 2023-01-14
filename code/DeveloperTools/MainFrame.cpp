#include "MainFrame.h"
#include "QtitanRibbon/QtnRibbonBar.h"
#include "QtitanRibbon/QtnRibbonStyle.h"
#include <QApplication>
#include <QFile>
#include "QtitanRibbon/QtnRibbonPage.h"
#include "QtitanRibbon/QtnRibbonGroup.h"
#include "define.h"
#include <QWindow>
#include <QFile>
#include <QDomDocument>
#include <QIcon>

MainFrame::MainFrame(QWidget *parent)
    : RibbonMainWindow(parent)
{
    setWindowIcon(QIcon(":/DeveloperTools/res/DeveloperTools.ico"));
    setMinimumSize(DPI(500), DPI(400));
    resize(DPI(800), DPI(600));

    //初始化Ribbon菜单
    RibbonBar* bar = new RibbonBar(this);
    setMenuBar(bar);
    bar->setTitleBarVisible(false);
    RibbonStyle* ribbonStyle = new RibbonStyle;
    ribbonStyle->setTheme(OfficeStyle::Office2013Gray);
    qApp->setStyle(ribbonStyle);

    ////初始化第一个Ribbon页面
    //RibbonPage* page = bar->addPage(u8"开始");

    LoadUIFromXml();

    //加载插件

    //初始化插件
    for (auto iter = m_moduleMap.begin(); iter != m_moduleMap.end(); ++iter)
    {
        if (iter->second.pModule != nullptr)
            iter->second.pModule->InitModule();
    }

    //响应Ribbon标签切换消息
    connect(bar, SIGNAL(currentPageIndexChanged(int)), this, SLOT(OnTabIndexChanged(int)));

    OnTabIndexChanged(0);

}

MainFrame::~MainFrame()
{
    //反初始化插件
    for (auto iter = m_moduleMap.begin(); iter != m_moduleMap.end(); ++iter)
    {
        if (iter->second.pModule != nullptr)
            iter->second.pModule->UnInitModule();

        if (iter->second.hModule != NULL)
        {
            //if (iter->second.pModule->GetMainWindowType() == IModuleInterface::MT_HWND)
            //    FreeLibraryAndExitThread(iter->second.hModule, 0);
            //else
            FreeLibrary(iter->second.hModule);
        }
    }
}

void MainFrame::OnTabIndexChanged(int index)
{
    IModuleInterfacePtr pModule = m_moduleMap[index].pModule;
    if (pModule != nullptr)
    {
        QWidget* pWidget = GetModuleMainWindow(pModule);
        if (pWidget != nullptr)
        {
            //动态切换centralWidget前，需要将上次的centralWidget的parent置空
            if (centralWidget() != nullptr)
                centralWidget()->setParent(nullptr);
            setCentralWidget(pWidget);
            pWidget->show();
            pModule->OnTabEntered();
            return;
        }
    }
    if (centralWidget() != nullptr)
        centralWidget()->hide();
}

void MainFrame::OnActionTriggerd(bool checked)
{
    QAction* pSender = qobject_cast<QAction*>(QObject::sender());
    if (pSender != nullptr)
    {
        QString strCmdId = pSender->data().toString();
        if (!strCmdId.isEmpty())
        {
            RibbonBar* bar = dynamic_cast<RibbonBar*>(menuBar());
            int index = bar->currentPageIndex();
            IModuleInterfacePtr pModule = m_moduleMap[index].pModule;
            if (pModule != nullptr)
            {
                pModule->CommandTrigerd(strCmdId.toStdWString().c_str(), checked);
            }
        }
    }

}

void MainFrame::LoadUIFromXml()
{
    QString dir = qApp->applicationDirPath();
    QFile file(dir + "/MainFrame.xml");
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        //CLog::GetInstance().WriteLog(L"打开MainFrame.xml文件失败！");
        return;
    }

    QDomDocument doc;
    if (!doc.setContent(&file, false))
        return;

    QDomElement root = doc.documentElement();
    if (root.isNull())
    {
        //CLog::GetInstance().WriteLog(L"MainFrame.xml文件找不到节点！");
        return;
    }

    RibbonBar* bar = dynamic_cast<RibbonBar*>(menuBar());
    if (bar == nullptr)
        return;

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
            bool bModuleLoaded = false;
            IModuleInterfacePtr pModule;
            RibbonPage* pCurPage = nullptr;
            HMODULE hModule = ::LoadLibraryEx(strModulePath.toStdWString().c_str(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
            if (hModule != NULL)
            {
                pfCreateModuleInstance fun = (pfCreateModuleInstance)::GetProcAddress(hModule, "CreateInstance");
                if (fun != nullptr)
                {
                    pModule = IModuleInterfacePtr(fun());
                    if (pModule != nullptr)
                    {
                        int index = bar->getPageCount();
                        SModule module;
                        module.hModule;
                        module.pModule = pModule;
                        m_moduleMap[index] = module;
                        pCurPage = bar->addPage(strTabName);
                        bModuleLoaded = true;
                    }
                }
                else
                {
                    //wchar_t buff[256];
                    //swprintf_s(buff, L"从模块“%s”中获取“CreateInstance”的入口地址失败！", strModulePath.toStdWString());
                    //CLog::GetInstance().WriteLog(buff);
                }
            }
            else
            {
                //wchar_t buff[256];
                //swprintf_s(buff, L"加载模块“%s”失败！", strModulePath.toStdWString());
                //CLog::GetInstance().WriteLog(buff);
            }

            //载入命令组
            if (bModuleLoaded)
            {
                QDomNodeList groupList = nodeInfo.childNodes();
                for (int j = 0; j < groupList.count(); j++)
                {
                    QDomElement groupNodeInfo = groupList.at(j).toElement();
                    QString strTagName = groupNodeInfo.tagName();
                    if (strTagName == "ActionGroup")
                    {
                        QString strGroupName = groupNodeInfo.attribute("name");
                        RibbonGroup* pRibbonGroup = pCurPage->addGroup(strGroupName);

                        //载入命令
                        QDomNodeList actionList = groupNodeInfo.childNodes();
                        for (int k = 0; k < actionList.count(); k++)
                        {
                            QDomElement actionNodeInfo = actionList.at(k).toElement();
                            QString strTagName = actionNodeInfo.tagName();
                            if (strTagName == "Action")
                            {
                                QString strCmdName = actionNodeInfo.attribute("name");
                                QString strIconPath = actionNodeInfo.attribute("icon");
                                QString strCmdId = actionNodeInfo.attribute("commandID");
                                QString strCheckable = actionNodeInfo.attribute("checkable");
                                bool bCheckable = (strCheckable == "true" || strCheckable == "TRUE" || strCheckable == "True");

                                QAction* pAction = pRibbonGroup->addAction(QIcon(qApp->applicationDirPath() + "/" + strIconPath), strCmdName, Qt::ToolButtonTextUnderIcon);
                                pAction->setData(strCmdId);     //将命令的ID作为用户数据保存到QAction对象中
                                pAction->setCheckable(bCheckable);

                                connect(pAction, SIGNAL(triggered(bool)), this, SLOT(OnActionTriggerd(bool)));

                                m_actionMap[strCmdId] = pAction;
                            }
                        }
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
        if (moduleItem.second.pModule != nullptr && QString::fromWCharArray(moduleItem.second.pModule->GetModuleName()) == strModuleName)
            return moduleItem.second.pModule.get();
    }
    return nullptr;
}
