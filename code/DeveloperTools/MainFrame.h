#pragma once

#include <QtWidgets/QMainWindow>
#include "QtitanRibbon/QtnRibbonMainWindow.h"
#include <qt_windows.h>
#include "moduleinterface.h"

using namespace Qtitan;

class MainFrame : public RibbonMainWindow
{
    Q_OBJECT

public:
    MainFrame(QWidget *parent = Q_NULLPTR);
    ~MainFrame();

protected slots:

    void OnTabIndexChanged(int index);      //��ӦRibbon�˵���ǩ�л�
    void OnActionTriggerd(bool checked);    //��Ӧ����һ��Ribbon�˵��еİ�ť�����

private:
    void LoadUIFromXml();           //��xml�ļ�����Ribbon����
    QWidget* GetModuleMainWindow(IModuleInterfacePtr pModule);  //��ȡģ���������

private:
    struct SModule
    {
        IModuleInterfacePtr pModule;
        HMODULE hModule;
    };

    std::map<int, SModule> m_moduleMap;     //����Ribbon��ǩ������IModuleInterface����Ķ�Ӧ��ϵ
};
