#pragma once

#include "addcodeheader_global.h"
#include "moduleinterface.h"
#include "MainWidget.h"
#include "AddCodeHeaderEditor.h"
#include "CRemoveCommentThread.h"
#include <QObject>
#include "mainframeinterface.h"
#include "AddFileHeadThread.h"

class ADDCODEHEADER_EXPORT AddCodeHeader
    : public QObject, public IModule
{
    Q_OBJECT
public:
    AddCodeHeader();

    virtual void InitInstance() override;
    virtual void UnInitInstance() override;
    virtual void UiInitComplete(IMainFrame* pMainFrame) override;
    virtual void* GetMainWindow() override;
    virtual const char* GetModuleName() override;
    virtual void OnCommand(const char* strCmd, bool checked) override;
    virtual eMainWindowType GetMainWindowType() const override;
    virtual void OnTabEntered() override;

    static AddCodeHeader* GetInstance();

    CAddCodeHeaderEditor& GetEditor();
    CRemoveCommentThread& GetRemoveCommentThread();
    IMainFrame* GetMainFrame() const;

    void EnableControl(bool enable);

private slots:
    void OnRemoveCommentsComplete();
    void OnAddFileHeadComplete();

private:
    CMainWidget m_mainWidget;
    CAddCodeHeaderEditor m_editor;
    CRemoveCommentThread m_removeCommentThread;
    CAddFileHeadThread m_addFileHeadThread;
    IMainFrame* m_pMainFrame;

    static AddCodeHeader* m_pInstance;
};

#ifdef __cplusplus
extern "C" {
#endif
    //导出一个名为CreateInstance的函数以创建对象
    ADDCODEHEADER_EXPORT IModule* CreateInstance();

#ifdef __cplusplus
}
#endif
