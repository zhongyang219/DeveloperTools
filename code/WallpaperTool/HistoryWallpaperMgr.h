#pragma once
#include <QStringList>
#include <QThread>
#include <QIcon>
#include <QMap>
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

//////////////////////////////////////////////////////////////////////////
class HistoryWallpaperSearchThread : public QThread
{
public:
    HistoryWallpaperSearchThread(CHistoryWallpaperMgr& manager);

    const QIcon& GetWallpaperIcon(const QString& path);

    bool StartThread();

protected:
    virtual void run() override;

private:
    QMap<QString, QIcon> m_imageMap;    //保存所有壁纸的图片
    CHistoryWallpaperMgr& m_manager;
};
