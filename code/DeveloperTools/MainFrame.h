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
    void OnTabIndexChanged(int index);      //��ӦRibbon�˵���ǩ�л�
    void OnActionTriggerd(bool checked);    //��Ӧ����һ��Ribbon�˵��еİ�ť�����
    void OnAbout();

private:
    void LoadUIFromXml();           //��xml�ļ�����Ribbon����
    QWidget* GetModuleMainWindow(IModuleInterfacePtr pModule);  //��ȡģ���������

    // ͨ�� IMainWindowInterface �̳�
    virtual QAction * GetAction(const QString & strCmd) const override;
    virtual IModuleInterface * GetModule(const QString & strModuleName) const override;

private:
    struct SModule
    {
        IModuleInterfacePtr pModule;
        HMODULE hModule;
    };

    std::map<int, SModule> m_moduleMap;     //����Ribbon��ǩ������IModuleInterface����Ķ�Ӧ��ϵ
    std::map<QString, QAction*> m_actionMap;    //��������Id��Action�Ķ�Ӧ��ϵ
};
