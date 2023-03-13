#pragma once

#include <QMainWindow>
#include "moduleinterface.h"
#include "mainwindowinterface.h"
#include <QTabWidget>
#include <QMenu>
#include <QPushButton>
#include "../CCommonTools/StyleManager.h"

class MainFrame : public QMainWindow, public IMainWindowInterface
{
    Q_OBJECT

public:
    MainFrame(QWidget *parent = Q_NULLPTR);
    ~MainFrame();

protected slots:
    void OnTabIndexChanged(int index);      //响应Tab标签切换
    void OnActionTriggerd(bool checked);    //响应任意一个Ribbon菜单中的按钮被点击
    void OnFileBtnClicked();
    void OnAbout();
    void OnStyleLight();
    void OnStyleDark();
    void OnStyleDefault();

private:
    void LoadUIFromXml();           //从xml文件加载Ribbon界面
    QWidget* GetModuleMainWindow(IModuleInterfacePtr pModule);  //获取模块的主窗口

    void LoadConfig();
    void SaveConfig() const;
    void ApplyStyle();
    void StyleChanged();

    // 通过 IMainWindowInterface 继承
    virtual QAction * GetAction(const QString & strCmd) const override;
    virtual IModuleInterface * GetModule(const QString & strModuleName) const override;

private:
    QTabWidget* m_pTabWidget;
    std::map<int, IModuleInterfacePtr> m_moduleMap;     //保存Ribbon标签索引和IModuleInterface对象的对应关系
    std::map<QString, QAction*> m_actionMap;    //保存命令Id和Action的对应关系
    QPushButton* m_pFileBtn;                    //文件按钮
    QMenu* m_pFileMenu;                         //文件菜单
    CStyleManager::Style m_style;
};
