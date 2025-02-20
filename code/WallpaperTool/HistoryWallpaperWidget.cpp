#include "HistoryWallpaperWidget.h"
#include <QVBoxLayout>
#include <QPushButton>
#include "define.h"
#include "WallpaperTool.h"
#include <QFileInfo>
#include <QDialog>
#include "ImageLabel.h"

class WalpaperViewDlg : public QDialog
{
public:
    WalpaperViewDlg(const QString& path, QWidget* parent = nullptr)
        : QDialog(parent)
    {
        setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);
        setWindowTitle(u8"查看壁纸");

        m_pixMap.load(path);
        m_imgLabel.SetImage(m_pixMap);
        QVBoxLayout* pLayout = new QVBoxLayout();
        pLayout->setContentsMargins(0, 0, 0, 0);
        setLayout(pLayout);
        pLayout->addWidget(&m_imgLabel);
        resize(QSize(DPI(800), DPI(450)));
    }

private:
    CImageLabel m_imgLabel;
    QPixmap m_pixMap;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
HistoryWallpaperWidget::HistoryWallpaperWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout* pLayout = new QVBoxLayout();
    setLayout(pLayout);
    pLayout->addWidget(&m_listWidget);

    m_listWidget.setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_listWidget.setProperty("showDropIndicator", QVariant(false));
    m_listWidget.setDefaultDropAction(Qt::IgnoreAction);
    m_listWidget.setIconSize(QSize(DPI(256), DPI(256)));
    m_listWidget.setViewMode(QListView::IconMode);
    m_listWidget.setContextMenuPolicy(Qt::CustomContextMenu);

    connect(&m_listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(OnItemDoubleClicked(QListWidgetItem*)));
    connect(&m_listWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(OnContextMenu(const QPoint&)));

    m_menu.addAction(QIcon("://res/image.png"), u8"查看", this, SLOT(OnViewWallpaper()));
    m_menu.addAction(QIcon("://res/save.png"), u8"另存为...", this, SLOT(OnSaveWallpaper()));
}

HistoryWallpaperWidget::~HistoryWallpaperWidget()
{}

void HistoryWallpaperWidget::Refresh()
{
    //清空列表
    m_listWidget.clear();

    //填充列表控件
    for (const auto& path : WallpaperTool::Instance()->GetHistoryWallpaperMgr().GetHistoryWallpapers())
    {
        QListWidgetItem* pItem = new QListWidgetItem();
        //pItem->setText(QFileInfo(path).fileName());
        pItem->setIcon(QIcon(path));
        pItem->setData(Qt::UserRole, path);
        pItem->setData(Qt::ToolTipRole, path);
        m_listWidget.addItem(pItem);
    }
}

void HistoryWallpaperWidget::OnContextMenu(const QPoint& pos)
{
    m_selectedItem = m_listWidget.itemAt(pos);
    if (m_selectedItem != nullptr)
    {
        m_menu.exec(m_listWidget.viewport()->mapToGlobal(pos));
    }
}

void HistoryWallpaperWidget::OnViewWallpaper()
{
    if (m_selectedItem != nullptr)
    {
        QString path = m_selectedItem->data(Qt::UserRole).toString();
        WalpaperViewDlg dlg(path);
        dlg.exec();
    }
}

void HistoryWallpaperWidget::OnSaveWallpaper()
{
    if (m_selectedItem != nullptr)
    {
        QString path = m_selectedItem->data(Qt::UserRole).toString();
        if (!path.isEmpty())
        {
            WallpaperTool::Instance()->WallpaperSaveAs(path);
        }
    }
}

void HistoryWallpaperWidget::OnItemDoubleClicked(QListWidgetItem* item)
{
    m_selectedItem = item;
    if (item != nullptr)
    {
        QString path = item->data(Qt::UserRole).toString();
        WalpaperViewDlg dlg(path);
        dlg.exec();
    }
}

