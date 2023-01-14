#include "CommonTools.h"
#include <QDir>
#include <QDirIterator>
#include <QApplication>
#include "mainwindowinterface.h"

CCommonTools::CCommonTools()
{
}

QFileInfoList CCommonTools::FindFile(const QString & strFilePath, const QStringList & nameFilters)
{
    QFileInfoList fileList;
    if (strFilePath.isEmpty() || nameFilters.isEmpty())
    {
        return fileList;
    }

    QDir dir;
    dir.setPath(strFilePath);
    dir.setNameFilters(nameFilters);
    QDirIterator iter(dir, QDirIterator::Subdirectories);
    while (iter.hasNext())
    {
        iter.next();
        QFileInfo info = iter.fileInfo();
        if (info.isFile())
        {
            fileList.append(info);
        }
    }
    return fileList;
}

QMainWindow * CCommonTools::GetMainWindow()
{
    Q_FOREACH(QWidget* w, qApp->topLevelWidgets())
    {
        QMainWindow* pMainWindow = qobject_cast<QMainWindow*>(w);
        if (pMainWindow != nullptr)
            return pMainWindow;
    }
    return nullptr;
}

QAction * CCommonTools::GetMainFrameAction(const QString & strCmdId)
{
    IMainWindowInterface* pMainWindowInterface = dynamic_cast<IMainWindowInterface*>(GetMainWindow());
    if (pMainWindowInterface != nullptr)
        return pMainWindowInterface->GetAction(strCmdId);
    return nullptr;
}

void CCommonTools::SetActionEnable(const QString& strCmdId, bool enable)
{
    QAction* pAction = CCommonTools::GetMainFrameAction(strCmdId);
    if (pAction != nullptr)
        pAction->setEnabled(enable);
}
