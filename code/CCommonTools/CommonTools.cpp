#include "CommonTools.h"
#include <QDir>
#include <QDirIterator>
#include <QApplication>
#include <QApplication>
#include <QDateTime>
#include <QTextStream>
#include <QEventLoop>
#include <QTimer>

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

void CCommonTools::WriteLog(const QString& strLogInfo, const QString& strFilePath)
{
    QString strLogPath = strFilePath;
    if (strLogPath.isEmpty())
        strLogPath = QString("%1/%2.log").arg(qApp->applicationDirPath()).arg(qApp->applicationName());
    QFile file(strLogPath);
    if (file.open(QIODevice::ReadWrite | QIODevice::Append))
    {
        QTextStream textOutput(&file);
        textOutput.setCodec("UTF-8");
        QString currentTime = QDateTime::currentDateTime().toString("yyyyMMdd hh:mm:ss ");
        textOutput << currentTime << strLogInfo << "\r\n";
        file.close();
    }
}

QIcon CCommonTools::CreateIcon(const QString& strPath, int size)
{
    QPixmap image(strPath);
    image = image.scaled(size, size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    return QIcon(image);
}

void CCommonTools::DelayNotBlocked(int msec)
{
    QEventLoop loop;
    QTimer::singleShot(msec, &loop, SLOT(quit()));
    loop.exec();
}
