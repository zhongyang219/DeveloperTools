#include "AddCodeHeader.h"
#include <QMessageBox>
#include "RemoveCommentHelper.h"

AddCodeHeader* AddCodeHeader::m_pInstance{};

AddCodeHeader::AddCodeHeader()
{
    m_pInstance = this;
}

void AddCodeHeader::InitModule()
{
    m_editor.ConnectWidget(&m_mainWidget);
}

void AddCodeHeader::UnInitModule()
{
    m_editor.ExitWidget();
    m_pInstance = nullptr;
}

unsigned long long AddCodeHeader::GetMainWindow()
{
    return (unsigned long long(&m_mainWidget));
}

const wchar_t* AddCodeHeader::GetModuleName()
{
    return L"AddCodeHeader";
}

void AddCodeHeader::CommandTrigerd(const wchar_t* cmd, bool checked)
{
    QString strCmd = QString::fromWCharArray(cmd);
    if (strCmd == "AddCodeHeaderExcute")
    {
        QMessageBox::information(nullptr, nullptr, u8"你点击了“执行”", QMessageBox::Ok);
    }
    else if (strCmd == "AddCodeHeaderScanFile")
    {
        m_editor.ScanFiles();
    }
    else if (strCmd == "RemoveCommentExcute")
    {
        //QMessageBox::information(nullptr, nullptr, u8"你点击了删除注释的“执行”", QMessageBox::Ok);
        CRemoveCommentHelper::RemoveResult result;
        int file_count = m_editor.RemoveComments(result);
        QString info;
        info = QString(u8"完成，已处理 %1 个文件。\r\n移除 %2 个单行注释，%3 个多行注释，%4 个空格，%5 个回车符").arg(file_count)
            .arg(result.single_line_comment_removed).arg(result.multi_line_comment_removed).arg(result.space_removed).arg(result.return_removed);
        QMessageBox::information(nullptr, nullptr, info, QMessageBox::Ok);
    }
}

IModuleInterface::eMainWindowType AddCodeHeader::GetMainWindowType() const
{
    return IModuleInterface::MT_QWIDGET;
}

AddCodeHeader* AddCodeHeader::GetInstance()
{
    return m_pInstance;
}

IModuleInterface * CreateInstance()
{
    return new AddCodeHeader();
}
