#pragma once

#include <QMainWindow>
#include <QList>
#include "WallpapaerWidget.h"
#include <QGridLayout>

class MainWidget : public QMainWindow
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
    //设置所有壁纸的路径
    void SetWallpapers(QList<QString>& wallpapersPath);

private:
    QList<CWallpapaerWidget*> m_wallpaperWidgets;
    QGridLayout* m_pLayout;
};
