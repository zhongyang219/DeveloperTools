#include "FileRename.h"

FileRename::FileRename()
{
}

void FileRename::InitInstance()
{
}

void FileRename::UnInitInstance()
{
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
    return nullptr;
}

void FileRename::OnCommand(const char* strCmd, bool checked)
{
    QString cmd(QString::fromUtf8(strCmd));
    if ()
}


IModule* CreateInstance()
{
    return new FileRename();
}
