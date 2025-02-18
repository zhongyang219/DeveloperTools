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
        connect(&pWidget->GetImageLabel(), SIGNAL(clicked()), this, SLOT(OnWallpapaerWidgetClicked()));
        pWidget->show();
        pWidget->GetImageLabel().setCursor(QCursor(Qt::PointingHandCursor));
        m_pLayout->addWidget(pWidget, row, col, 1, 1);
        index++;
    }
    m_isGridLayout = true;
    emit widgetLayoutChanged(m_isGridLayout, QString());
}

void MainWidget::ShowGridLayout()
{
    //显示所有子窗口
    for (auto* pWidget : m_wallpaperWidgets)
    {
        pWidget->setVisible(true);
        pWidget->GetImageLabel().setCursor(QCursor(Qt::PointingHandCursor));     //设置鼠标指针为手形
    }
    m_isGridLayout = true;
    emit widgetLayoutChanged(m_isGridLayout, QString());
}

void MainWidget::OnWallpapaerWidgetClicked()
{
    CWallpapaerWidget* pClickedWidget = qobject_cast<CWallpapaerWidget*>(QObject::sender()->parent());
    if (pClickedWidget != nullptr)
    {
        //显示点击的窗口，隐藏其他窗口
        for (auto* pWidget : m_wallpaperWidgets)
        {
            if (pWidget == pClickedWidget)
                pWidget->show();
            else
                pWidget->hide();
        }
        m_isGridLayout = false;
        emit widgetLayoutChanged(m_isGridLayout, pClickedWidget->GetWallpaperPath());
        pClickedWidget->GetImageLabel().setCursor(QCursor(Qt::ArrowCursor));
    }
}