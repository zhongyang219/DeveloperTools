#include "WallpaperTool.h"
#include "WallpaperHelper.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include "../CCommonTools/CommonTools.h"
#include "../CCommonTools/Config.h"

WallpaperTool::WallpaperTool()
{
}

void WallpaperTool::InitModule()
{
    m_strCurWallpaperPath = CWallpaperHelper::GetCurrentWallpaperPath();
    m_mainWidget.SetWallpaper(m_strCurWallpaperPath);

    CConfig settings(QString::fromWCharArray(GetModuleName()));
    m_strLastSaveDir = settings.GetValue("lastSaveDir").toString();
}

void WallpaperTool::UnInitModule()
{
    CConfig settings(QString::fromWCharArray(GetModuleName()));
    settings.WriteValue("lastSaveDir", m_strLastSaveDir);
}

unsigned long long WallpaperTool::GetMainWindow()
{
    return (unsigned long long)&m_mainWidget;
}

IModuleInterface::eMainWindowType WallpaperTool::GetMainWindowType() const
{
    return IModuleInterface::MT_QWIDGET;
}

const wchar_t* WallpaperTool::GetModuleName()
{
    return L"WallpaperTool";
}

void WallpaperTool::CommandTrigerd(const wchar_t* strCmd, bool checked)
{
    QString cmd = QString::fromWCharArray(strCmd);

    //��ǰ��ֽ���Ϊ
    if (cmd == CMD_CURRENT_WALLPAPER_SAVE_AS)
    {
        QString strDir = QFileDialog::getExistingDirectory(&m_mainWidget, QString(), m_strLastSaveDir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        if (!strDir.isEmpty())
        {
            QString strNewPath = strDir + "/" + QFileInfo(m_strCurWallpaperPath).fileName();
            QString strLogInfo;
            if (!QFileInfo(strNewPath).isFile())
            {
                if (QFile::copy(m_strCurWallpaperPath, strNewPath))
                {
                    QMessageBox::information(&m_mainWidget, QString(), u8"����ɹ���");
                    strLogInfo = QString(u8"�ɹ�����ֽ %1 ���浽 %2 Ŀ¼�¡�").arg(m_strCurWallpaperPath).arg(strDir);
                    m_strLastSaveDir = strDir;
                }
                else
                {
                    QMessageBox::information(&m_mainWidget, QString(), u8"����ʧ�ܣ�");
                    strLogInfo = QString(u8"�����ֽ�� %1 �� %2 Ŀ¼��ʧ�ܡ�").arg(m_strCurWallpaperPath).arg(strDir);
                }
            }
            else
            {
                QMessageBox::warning(&m_mainWidget, QString(), QString(u8"�ļ� %1 �Ѵ��ڣ�").arg(strNewPath));
                strLogInfo = QString(u8"�����ֽ %1 ʱ��Ŀ���ļ� %2 �Ѵ��ڡ�").arg(m_strCurWallpaperPath).arg(strNewPath);
            }
            WriteLog(strLogInfo);
        }
    }

    //ɾ����ǰ��ֽ
    else if (cmd == CMD_CURRENT_WALLPAPER_DELETE)
    {
        if (QMessageBox::question(&m_mainWidget, QString(), QString(u8"ȷʵҪ�Ӵ�����ɾ���ļ� %1 ��").arg(m_strCurWallpaperPath), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            QString strLogInfo;
            if (QFile::remove(m_strCurWallpaperPath))
            {
                strLogInfo = QString(u8"ɾ����ֽ %1 �ɹ���").arg(m_strCurWallpaperPath);
                QMessageBox::information(&m_mainWidget, QString(), u8"ɾ���ɹ���");
            }
            else
            {
                strLogInfo = QString(u8"ɾ����ֽ %1 ʧ�ܡ�").arg(m_strCurWallpaperPath);
                QMessageBox::information(&m_mainWidget, QString(), u8"ɾ��ʧ�ܣ�");
            }
            WriteLog(strLogInfo);
        }
    }

    //ˢ��
    else if (cmd == CMD_CURRENT_WALLPAPER_REFRESH)
    {
        m_strCurWallpaperPath = CWallpaperHelper::GetCurrentWallpaperPath();
        m_mainWidget.SetWallpaper(m_strCurWallpaperPath);
    }
}

void WallpaperTool::WriteLog(const QString& strLogInfo)
{
    if (!strLogInfo.isEmpty())
    {
        QString strLogFilePath = QString("%1/%2.log").arg(qApp->applicationDirPath()).arg(QString::fromWCharArray(GetModuleName()));
        CCommonTools::WriteLog(strLogInfo, strLogFilePath);
    }
}

IModuleInterface* CreateInstance()
{
    return new WallpaperTool();
}
