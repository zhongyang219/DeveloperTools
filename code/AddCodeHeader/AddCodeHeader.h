#pragma once

#include "addcodeheader_global.h"
#include "moduleinterface.h"
#include "MainWidget.h"
#include "AddCodeHeaderEditor.h"
#include "CRemoveCommentThread.h"
#include <QObject>

class ADDCODEHEADER_EXPORT AddCodeHeader
    : public QObject, public IModuleInterface
{
    Q_OBJECT
public:
    AddCodeHeader();

    virtual void InitModule() override;
    virtual void UnInitModule() override;
    virtual unsigned long long GetMainWindow() override;
    virtual const wchar_t* GetModuleName() override;
    virtual void CommandTrigerd(const wchar_t* strCmd, bool checked) override;
    virtual eMainWindowType GetMainWindowType() const override;
    virtual void OnTabEntered() override;

    static AddCodeHeader* GetInstance();

    CAddCodeHeaderEditor& GetEditor();
    CRemoveCommentThread& GetRemoveCommentThread();

    void EnableControl(bool enable);

private slots:
    void OnRemoveCommentsComplete();

private:
    CMainWidget m_mainWidget;
    CAddCodeHeaderEditor m_editor;
    CRemoveCommentThread m_removeCommentThread;

    static AddCodeHeader* m_pInstance;

};

#ifdef __cplusplus
extern "C" {
#endif
    //导出一个名为CreateInstance的函数以创建对象
    ADDCODEHEADER_EXPORT IModuleInterface* CreateInstance();

#ifdef __cplusplus
}
#endif
