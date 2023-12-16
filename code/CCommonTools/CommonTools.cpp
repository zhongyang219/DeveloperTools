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
    if (strFilePath.isEmpty())
    {
        return fileList;
    }

    QDir dir;
    dir.setPath(strFilePath);
	if (!nameFilters.isEmpty())
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

bool CCommonTools::IsUtf8Bytes(const QByteArray& data)
{
    int charByteCounter = 1;  //计算当前正分析的字符应还有的字节数
    unsigned char curByte; //当前分析的字节.
    bool ascii = true;
    for (int i = 0; i < data.size(); i++)
    {
        curByte = static_cast<unsigned char>(data[i]);
        if (charByteCounter == 1)
        {
            if (curByte >= 0x80)
            {
                ascii = false;
                //判断当前
                while (((curByte <<= 1) & 0x80) != 0)
                {
                    charByteCounter++;
                }
                //标记位首位若为非0 则至少以2个1开始 如:110XXXXX...........1111110X
                if (charByteCounter == 1 || charByteCounter > 6)
                {
                    return false;
                }
            }
        }
        else
        {
            //若是UTF-8 此时第一位必须为1
            if ((curByte & 0xC0) != 0x80)
            {
                return false;
            }
            charByteCounter--;
        }
    }
    if (ascii) return false;		//如果全是ASCII字符，返回false
    else return true;
}

//int CCommonTools::QStringFindFirstOf(const QString& str, const char* findStr, int start)
//{
//
//}
