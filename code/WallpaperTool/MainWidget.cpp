#include "MainWidget.h"
#include "define.h"

MainWidget::MainWidget(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget* pWidget = new QWidget(this);
    setCentralWidget(pWidget);

    m_pLayout = new QGridLayout();
    pWidget->setLayout(m_pLayout);
}

MainWidget::~MainWidget()
{}

void MainWidget::SetWallpapers(QList<QString>&wallpapersPath)
{
    //清空布局
    for (auto* pWidget : m_wallpaperWidgets)
    {
        m_pLayout->removeWidget(pWidget);
        delete pWidget;
    }
    m_wallpaperWidgets.clear();

    //添加Widgets
    int row = 0;    //子窗口在布局中的行
    int col = 0;    //子窗口在布局中的列
    int colMax = 0; //每行的窗口数
    //根据窗口总数确定每行显示多少个窗口
    if (wallpapersPath.size() <= 1)
        colMax = 1;
    else if (wallpapersPath.size() <= 4)
        colMax = 2;
    else if (wallpapersPath.size() <= 6)
        colMax = 3;
    else if (wallpapersPath.size() <= 8)
        colMax = 4;
    else
        colMax = 5;
    int index = 0;  //循环的序号
    for (const QString& path : wallpapersPath)
    {
        //计算窗口的行和列
        col = index % colMax;
        row = index / colMax;
        CWallpapaerWidget* pWidget = new CWallpapaerWidget(this);
        m_wallpaperWidgets.push_back(pWidget);
        pWidget->SetWallpaper(path);
        pWidget->show();
        m_pLayout->addWidget(pWidget, row, col, 1, 1);
        index++;
    }
}
