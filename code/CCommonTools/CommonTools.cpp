#include "CommonTools.h"
#include <QDir>
#include <QDirIterator>

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
