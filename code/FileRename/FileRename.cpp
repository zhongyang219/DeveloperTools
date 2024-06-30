#include "FileRename.h"
#include <QFileDialog>
#include <QMessageBox>
#include <set>

static FileRename* pIns = nullptr;
FileRename::FileRename()
{
}

FileRename* FileRename::Instance()
{
    return pIns;
}

IMainFrame* FileRename::GetMainFrame()
{
    return m_pMainFrame;
}

void FileRename::InitInstance()
{
    m_mainWidget.LoadConfig();
}

void FileRename::UiInitComplete(IMainFrame* pMainFrame)
{
    m_pMainFrame = pMainFrame;
    m_pMainFrame->SetItemEnable(CMD_FileRenameDelete, false);

    //QWidget* pMainWindow = dynamic_cast<QWidget*>(pMainFrame);
    //if (pMainWindow != nullptr)
    //    pMainWindow->setAcceptDrops(true);
}

void FileRename::UnInitInstance()
{
    m_mainWidget.SaveConfig();
}

void* FileRename::GetMainWindow()
{
    return &m_mainWidget;
}

IModule::eMainWindowType FileRename::GetMainWindowType() const
{
    return IModule::MT_QWIDGET;
}

const char* FileRename::GetModuleName()
{
    return "FileRename";
}

void FileRename::OnCommand(const char* strCmd, bool checked)
{
    QString cmd(QString::fromUtf8(strCmd));
    if (cmd == CMD_FileRenameAddFiles)
    {
        QStringList files = QFileDialog::getOpenFileNames(&m_mainWidget, QString(), QString(), u8"所有文件 (*.*);;图片文件 (*.png *.bmp *.jpg)");
        if (!files.isEmpty())
        {
            for (const auto& file : files)
                m_mainWidget.AddFile(file);
        }
    }
    else if (cmd == CMD_FileRenameAddFolder)
    {
        QString strDir = QFileDialog::getExistingDirectory(&m_mainWidget);
        if (!strDir.isEmpty())
        {
            m_mainWidget.AddFolder(strDir);
        }
    }
    else if (cmd == CMD_FileRenameDelete)
    {
        auto selections = m_mainWidget.GetTableView()->selectionModel()->selectedRows();
        if (selections.isEmpty())
            return;

        if (QMessageBox::information(&m_mainWidget, nullptr, QString(u8"确实要移除选中的 %1 个文件吗？").arg(selections.size()), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            std::set<int> selectedRows;
            for (const auto& selection : selections)
            {
                selectedRows.insert(selection.row());
            }

            for (auto iter = selectedRows.rbegin(); iter != selectedRows.rend(); ++iter)
            {
                int row = *iter;
                m_mainWidget.GetModel().DeleteItem(row);
            }
        }
    }
    else if (cmd == CMD_FileRenameClear)
    {
        m_mainWidget.GetModel().ClearData();
    }
    else if (cmd == CMD_RenameWithModifiedTime)
    {
        int count = m_mainWidget.ExcuteFileRename();
        QString info = QString(u8"完成，已重命名 %1 个文件。").arg(count);
        QMessageBox::information(&m_mainWidget, QString(), info);
    }
}


IModule* CreateInstance()
{
    pIns = new FileRename();
    return pIns;
}
