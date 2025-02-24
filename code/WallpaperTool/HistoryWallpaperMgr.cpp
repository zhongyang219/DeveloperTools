#include "HistoryWallpaperMgr.h"
#include "../CCommonTools/Config.h"
#include "WallpaperTool.h"
#include <QFileInfo>
#include "define.h"

void CHistoryWallpaperMgr::AddWallpapers(const QStringList& wallpapers)
{
    for (const auto& wallpaper : wallpapers)
    {
        if (!m_historyWallpapers.contains(wallpaper))
            m_historyWallpapers.push_front(wallpaper);
    }
    while (m_historyWallpapers.size() > HISTORY_WALLPAPER_MAX)
    {
        m_historyWallpapers.pop_back();
    }
}

void CHistoryWallpaperMgr::Load()
{
    CConfig settings(QString::fromUtf8(WallpaperTool::Instance()->GetModuleName()));
    m_historyWallpapers = settings.GetValue("HistoryWallpapers").toStringList();
}

void CHistoryWallpaperMgr::Save() const
{
    CConfig settings(QString::fromUtf8(WallpaperTool::Instance()->GetModuleName()));
    settings.WriteValue("HistoryWallpapers", m_historyWallpapers);
}

const QStringList& CHistoryWallpaperMgr::GetHistoryWallpapers() const
{
    return m_historyWallpapers;
}

void CHistoryWallpaperMgr::RemoveRecord(const QString& wallpaperPath)
{
    m_historyWallpapers.removeAll(wallpaperPath);
}

//////////////////////////////////////////////////////////////////////////////////////////////
HistoryWallpaperSearchThread::HistoryWallpaperSearchThread(CHistoryWallpaperMgr& manager)
    : m_manager(manager)
{
}

const QIcon& HistoryWallpaperSearchThread::GetWallpaperIcon(const QString& path)
{
    auto iter = m_imageMap.find(path);
    if (iter != m_imageMap.end())
    {
        return iter.value();
    }
    else
    {
        static QIcon emptyIcon;
        return emptyIcon;
    }
}

bool HistoryWallpaperSearchThread::StartThread()
{
    if (!isRunning())
    {
        start();
        return true;
    }
    return false;
}

void HistoryWallpaperSearchThread::run()
{
    //加载历史记录中所有图片并加载到内存
    for (const QString& path : m_manager.GetHistoryWallpapers())
    {
        if (!m_imageMap.contains(path))
        {
            QPixmap pixmap;
            pixmap.load(path);
            int iconSize = DPI(256);
            pixmap = pixmap.scaled(iconSize, iconSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            m_imageMap.insert(path, QIcon(pixmap));
        }
    }
}
