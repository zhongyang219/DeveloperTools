#ifndef MODULEINTERFACE
#define MODULEINTERFACE
#include <memory>

class IModuleInterface
{
public:
    IModuleInterface()
    {}

    virtual ~IModuleInterface()
    {}

    //定义返回的窗口的类型
    enum eMainWindowType
    {
        MT_QWIDGET,     //QWidget类型的指针
        MT_HWND,        //HWND
    };

    //模块被加载后由框架调用
    virtual void InitModule() = 0;

    //模块被析构前由框架调用
    virtual void UnInitModule() = 0;

    //返回一个窗口，当切换到该模块的标签时，此窗口会被显示到MainFrame中。可以返回空
    virtual unsigned long long GetMainWindow() = 0;

    //获取返回的主窗口的类型
    virtual eMainWindowType GetMainWindowType() const = 0;

    virtual const wchar_t* GetModuleName() = 0;

    //当主窗口触发了一个命令时由框架调用。
    //strCmd：命令的ID
    //checked：命令按钮是否被选中
    virtual void CommandTrigerd(const wchar_t* strCmd, bool checked) = 0;

    virtual void OnTabEntered() {}

};

typedef std::shared_ptr<IModuleInterface> IModuleInterfacePtr;

//一个形如IModuleInterface* func()的函数的指针
//模块实现IModuleInterface接口后，还应该导出一个名为“CreateInstance”的函数以返回对象
typedef IModuleInterface* (__stdcall *pfCreateModuleInstance)();


#endif // MODULEINTERFACE

