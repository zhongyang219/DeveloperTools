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

    connect(&m_removeCommentThread, SIGNAL(finished()), this, SLOT(OnRemoveCommentsComplete()));
    connect(&m_addFileHeadThread, SIGNAL(finished()), this, SLOT(OnAddFileHeadComplete()));
    connect(&m_editor, SIGNAL(signalRemoveCommentProgress(double)), &m_mainWidget, SLOT(SetProgress(double)));
}

void AddCodeHeader::UnInitInstance()
{
    m_editor.ExitWidget();
    m_removeCommentThread.Stop();
    m_addFileHeadThread.Stop();
    m_removeCommentThread.wait(2000);
    m_addFileHeadThread.wait(2000);
    m_pInstance = nullptr;
}

void AddCodeHeader::UiInitComplete(IMainFrame* pMainFrame)
{
    m_editor.ConnectWidget(&m_mainWidget, pMainFrame);
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
        if (QMessageBox::question(&m_mainWidget, nullptr, u8"注意：所有文件的文件头将会被覆盖，且无法恢复，如果有必要的话请事先做好备份。\r\n要继续吗？", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            if (!m_addFileHeadThread.isRunning())
            {
                AddCodeHeader::GetInstance()->EnableControl(false);
                m_addFileHeadThread.start();
            }
        }
    }
    else if (strCmd == CMD_SCAN_FILE)
    {
        m_editor.ScanFiles();
    }
    else if (strCmd == CMD_REMOVE_COMMENT_EXCUTE)
    {
        bool removeComment = m_pMainFrame->IsItemChecked(CMD_RemoveCommentCheck);
        bool removeSpace = m_pMainFrame->IsItemChecked(CMD_RemoveSpaceCheck);
        bool removeEmptyLine = m_pMainFrame->IsItemChecked(CMD_RemoveEmptyLineCheck);
        if (!removeComment && !removeSpace && !removeEmptyLine)
        {
            QMessageBox::warning(&m_mainWidget, nullptr, u8"你至少应该选中“删除注释”、“移除多余的空格”和“移除多余的回车”中的一项！", QMessageBox::Ok);
            return;
        }

        if (!m_removeCommentThread.isRunning())
        {
            AddCodeHeader::GetInstance()->EnableControl(false);
            m_removeCommentThread.start();
        }
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
    m_pMainFrame->SetItemEnable(CMD_RemoveCommentCheck, enable);
    m_pMainFrame->SetItemEnable(CMD_RemoveSpaceCheck, enable);
    m_pMainFrame->SetItemEnable(CMD_RemoveEmptyLineCheck, enable);
    m_pMainFrame->SetItemEnable(CMD_KeepEmptyLineNum, enable);
    m_pMainFrame->SetItemEnable(CMD_SCAN_FILE, enable);
    m_pMainFrame->SetItemEnable(CMD_REMOVE_COMMENT_EXCUTE, enable);
}


void AddCodeHeader::OnRemoveCommentsComplete()
{
    EnableControl(true);

    QString info;
    const auto& result{ m_removeCommentThread.m_removeResult };
    info = QString(u8"完成，已处理 %1 个文件。已移除：").arg(m_removeCommentThread.m_fileCount);
    if (m_pMainFrame->IsItemChecked(CMD_RemoveCommentCheck))
        info += QString(u8"\r\n%1 个单行注释，%2 个多行注释").arg(result.single_line_comment_removed).arg(result.multi_line_comment_removed);
    if (m_pMainFrame->IsItemChecked(CMD_RemoveSpaceCheck))
        info += QString(u8"\r\n%1 个空格").arg(result.space_removed);
    if (m_pMainFrame->IsItemChecked(CMD_RemoveEmptyLineCheck))
        info += QString(u8"\r\n%1 个空白行").arg(result.return_removed);
    QMessageBox::information(&m_mainWidget, nullptr, info, QMessageBox::Ok);
}


void AddCodeHeader::OnAddFileHeadComplete()
{
    EnableControl(true);
    QString info = QString(u8"完成，已处理 %1 个文件").arg(m_addFileHeadThread.m_fileCount);
    QMessageBox::information(&m_mainWidget, nullptr, info, QMessageBox::Ok);
}

IModule * CreateInstance()
{
    return new AddCodeHeader();
}
