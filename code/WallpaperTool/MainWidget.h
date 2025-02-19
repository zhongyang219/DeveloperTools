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

    //回到宫格布局
    void ShowGridLayout();

    int GetWallpaperNum();

signals:
    //布局在单窗口布局和宫格布局间发生了变化。
    //bGrid: 如果改变后的布局为宫格布局，则为true，否则为false
    //curWallpapaerPath：bGrid为false时有效，表示当前选中的壁纸路径
    void widgetLayoutChanged(bool bGrid, const QString& curWallpapaerPath);

private slots:
    void OnWallpapaerWidgetClicked();

private:
    QList<CWallpapaerWidget*> m_wallpaperWidgets;
    QGridLayout* m_pLayout;
    bool m_isGridLayout{ true };
};
