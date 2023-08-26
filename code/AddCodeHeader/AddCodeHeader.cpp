#include "AddCodeHeader.h"
#include <QMessageBox>
#include "RemoveCommentHelper.h"
#include "Test.h"

AddCodeHeader* AddCodeHeader::m_pInstance{};

AddCodeHeader::AddCodeHeader()
{
    m_pInstance = this;
}

void AddCodeHeader::InitInstance()
{
//#ifdef _DEBUG
//	CTest::Test();
//#endif

    m_editor.ConnectWidget(&m_mainWidget);

    connect(&m_removeCommentThread, SIGNAL(finished()), this, SLOT(OnRemoveCommentsComplete()));
    connect(&m_editor, SIGNAL(signalRemoveCommentProgress(double)), &m_mainWidget, SLOT(SetProgress(double)));
}

void AddCodeHeader::UnInitInstance()
{
    m_editor.ExitWidget();
    m_removeCommentThread.Stop();
    m_removeCommentThread.wait(2000);
    m_pInstance = nullptr;
}

void AddCodeHeader::UiInitComplete(IMainFrame* pMainFrame)
{
    m_pMainFrame = pMainFrame;
    m_pMainFrame->SetItemEnable(CMD_ADD_CODE_HEADER_EXCUTE, false);
}

void* AddCodeHeader::GetMainWindow()
{
    return (void*)&m_mainWidget;
}

const char* AddCodeHeader::GetModuleName()
{
    return "AddCodeHeader";
}

void AddCodeHeader::OnCommand(const char* cmd, bool checked)
{
    QString strCmd = QString::fromUtf8(cmd);
    if (strCmd == CMD_ADD_CODE_HEADER_EXCUTE)
    {
        QMessageBox::information(&m_mainWidget, nullptr, u8"你点击了“执行”", QMessageBox::Ok);
    }
    else if (strCmd == CMD_SCAN_FILE)
    {
        m_editor.ScanFiles();
    }
    else if (strCmd == CMD_REMOVE_COMMENT_EXCUTE)
    {
        AddCodeHeader::GetInstance()->EnableControl(false);
        if (!m_removeCommentThread.isRunning())
            m_removeCommentThread.start();
    }
    else if (strCmd == CMD_SHOW_ADD_CODE_HEADER)
    {
        m_mainWidget.ShowAddCodeHeader(checked);
        m_pMainFrame->SetItemEnable(CMD_ADD_CODE_HEADER_EXCUTE, checked);
    }
}

IModule::eMainWindowType AddCodeHeader::GetMainWindowType() const
{
    return IModule::MT_QWIDGET;
}

void AddCodeHeader::OnTabEntered()
{
    m_editor.AdjustColumeWidth();
}

AddCodeHeader* AddCodeHeader::GetInstance()
{
    return m_pInstance;
}


CAddCodeHeaderEditor& AddCodeHeader::GetEditor()
{
    return m_editor;
}


CRemoveCommentThread& AddCodeHeader::GetRemoveCommentThread()
{
    return m_removeCommentThread;
}

IMainFrame* AddCodeHeader::GetMainFrame() const
{
    return m_pMainFrame;
}

void AddCodeHeader::EnableControl(bool enable)
{
    m_mainWidget.GetAddFileBtn()->setEnabled(enable);
    m_mainWidget.GetBrowseBtn()->setEnabled(enable);
    m_mainWidget.GetFileTableView()->setEnabled(enable);
    m_mainWidget.GetFileTypeCombo()->setEnabled(enable);
    m_mainWidget.GetFolderPathEidt()->setEnabled(enable);
    m_mainWidget.GetRemoveFileBtn()->setEnabled(enable);
    m_mainWidget.GetClearFileBtn()->setEnabled(enable);
    m_mainWidget.GetRemoveReturnCheck()->setEnabled(enable);
    m_mainWidget.GetRemoveSpaceCheck()->setEnabled(enable);
    m_mainWidget.GetReturnNumEdit()->setEnabled(enable);
    m_pMainFrame->SetItemEnable(CMD_SCAN_FILE, enable);
    m_pMainFrame->SetItemEnable(CMD_REMOVE_COMMENT_EXCUTE, enable);

}


void AddCodeHeader::OnRemoveCommentsComplete()
{
    EnableControl(true);

    QString info;
    const auto& result{ m_removeCommentThread.m_removeResult };
    info = QString(u8"完成，已处理 %1 个文件。\r\n移除 %2 个单行注释，%3 个多行注释").arg(m_removeCommentThread.m_fileCount)
        .arg(result.single_line_comment_removed).arg(result.multi_line_comment_removed);
    if (m_mainWidget.GetRemoveSpaceCheck()->isChecked())
        info += QString(u8"，%1 个空格").arg(result.space_removed);
    if (m_mainWidget.GetRemoveReturnCheck()->isChecked())
        info += QString(u8"，%1 个空白行").arg(result.return_removed);
    QMessageBox::information(&m_mainWidget, nullptr, info, QMessageBox::Ok);
}

IModule * CreateInstance()
{
    return new AddCodeHeader();
}
