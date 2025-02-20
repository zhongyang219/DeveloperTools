#pragma once

#include <QWidget>
#include <QListWidget>
#include <QMenu>

class HistoryWallpaperWidget  : public QWidget
{
    Q_OBJECT

public:
    HistoryWallpaperWidget(QWidget *parent = nullptr);
    ~HistoryWallpaperWidget();
    void Refresh();

private slots:
    void OnItemDoubleClicked(QListWidgetItem* item);
    void OnContextMenu(const QPoint& pos);
    void OnViewWallpaper();
    void OnSaveWallpaper();

private:
    QListWidget m_listWidget;
    QMenu m_menu;
    QListWidgetItem* m_selectedItem{};
};
