#pragma once

#include <QtWidgets/QMainWindow>
#include "QtitanRibbon/QtnRibbonMainWindow.h"
#include <qt_windows.h>
#include "moduleinterface.h"
#include "mainwindowinterface.h"

using namespace Qtitan;

class MainFrame : public RibbonMainWindow, public IMainWindowInterface
{
    Q_OBJECT

public:
    MainFrame(QWidget *parent = Q_NULLPTR);
    ~MainFrame();

protected slots:
    void OnTabIndexChanged(int index);      //响应Ribbon菜单标签切换
    void OnActionTriggerd(bool checked);    //响应任意一个Ribbon菜单中的按钮被点击
    void OnAbout();

private:
    void LoadUIFromXml();           //从xml文件加载Ribbon界面
    QWidget* GetModuleMainWindow(IModuleInterfacePtr pModule);  //获取模块的主窗口

    // 通过 IMainWindowInterface 继承
    virtual QAction * GetAction(const QString & strCmd) const override;
    virtual IModuleInterface * GetModule(const QString & strModuleName) const override;

private:
    struct SModule
    {
        IModuleInterfacePtr pModule;
        HMODULE hModule;
    };

    std::map<int, SModule> m_moduleMap;     //保存Ribbon标签索引和IModuleInterface对象的对应关系
    std::map<QString, QAction*> m_actionMap;    //保存命令Id和Action的对应关系
};
