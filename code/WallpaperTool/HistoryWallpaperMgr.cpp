#include "HistoryWallpaperMgr.h"
#include "../CCommonTools/Config.h"
#include "WallpaperTool.h"
#include <QFileInfo>

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
