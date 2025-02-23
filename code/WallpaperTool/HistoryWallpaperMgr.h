#pragma once
#include <QStringList>
#define HISTORY_WALLPAPER_MAX 200

class CHistoryWallpaperMgr
{
public:
    void AddWallpapers(const QStringList& wallpapers);
    void Load();
    void Save() const;

    const QStringList& GetHistoryWallpapers() const;
    void RemoveRecord(const QString& wallpaperPath);

private:
    QStringList m_historyWallpapers;    //历史壁纸列表

};

