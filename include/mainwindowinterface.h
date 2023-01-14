#ifndef MAINWINDOWINTERFACE
#define MAINWINDOWINTERFACE

#include <map>
#include <QString>
#include <QAction>
#include "ModuleInterface.h"

class IMainWindowInterface
{
public:

    //获取主窗口中的action
    //输入参数：
    //  strCmd: 命令ID
    //返回值：命令ID对应的QAction对象
    virtual QAction* GetAction(const QString& strCmd) const = 0;

    //获取主窗口中的module
    //输入参数：
    //  strModuleName: 模块名称
    //返回值：模块接口的指针
    virtual IModuleInterface* GetModule(const QString& strModuleName) const = 0;
};

#endif // MAINWINDOWINTERFACE

