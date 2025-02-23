#include "HistoryWallpaperWidget.h"
#include <QVBoxLayout>
#include <QPushButton>
#include "define.h"
#include "WallpaperTool.h"
#include <QFileInfo>
#include <QDialog>
#include <QMessageBox>
#include <QWheelEvent>
#include <QScrollBar>
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

//自定义的QListWidget，用于实现一次只滚动一行
class CustomListWidget : public QListWidget
{
public:
    CustomListWidget(QWidget* parent = nullptr) : QListWidget(parent) {}

protected:
    void wheelEvent(QWheelEvent* event) override
    {
        // 获取当前垂直滚动条的值
        QScrollBar* scrollBar = verticalScrollBar();
        int currentValue = scrollBar->value();

        // 计算每次滚动的步长（以一行的高度为单位）
        int step = event->angleDelta().y() > 0 ? -1 : 1; // 根据滚轮方向决定向上或向下滚动
        int rowHeight = sizeHintForRow(0); // 获取第一行的高度
        if (rowHeight <= 0)
        {
            rowHeight = 100; // 如果获取失败，设置一个默认值
        }

        // 计算新的滚动位置
        int newValue = currentValue + step * rowHeight;

        // 确保滚动位置在合法范围内
        newValue = qMax(scrollBar->minimum(), qMin(newValue, scrollBar->maximum()));

        // 设置新的滚动位置
        scrollBar->setValue(newValue);

        // 接受事件，阻止默认的滚动行为
        event->accept();
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
HistoryWallpaperWidget::HistoryWallpaperWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout* pLayout = new QVBoxLayout();
    setLayout(pLayout);
    m_listWidget = new CustomListWidget(this);
    pLayout->addWidget(m_listWidget);

    m_listWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_listWidget->setProperty("showDropIndicator", QVariant(false));
    m_listWidget->setDefaultDropAction(Qt::IgnoreAction);
    m_listWidget->setIconSize(QSize(DPI(256), DPI(256)));
    m_listWidget->setViewMode(QListView::IconMode);
    m_listWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(m_listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(OnItemDoubleClicked(QListWidgetItem*)));
    connect(m_listWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(OnContextMenu(const QPoint&)));

    m_menu.addAction(QIcon("://res/image.png"), u8"查看", this, SLOT(OnViewWallpaper()));
    m_menu.addAction(QIcon("://res/save.png"), u8"另存为...", this, SLOT(OnSaveWallpaper()));
    m_menu.addAction(QIcon("://res/delete.png"), u8"删除记录...", this, SLOT(OnRemoveRecord()));
    m_menu.addAction(QIcon("://res/delete1.png"), u8"删除文件...", this, SLOT(OnDeleteWallpaper()));
}

HistoryWallpaperWidget::~HistoryWallpaperWidget()
{}

void HistoryWallpaperWidget::Refresh()
{
    //清空列表
    m_listWidget->clear();

    //填充列表控件
    for (const auto& path : WallpaperTool::Instance()->GetHistoryWallpaperMgr().GetHistoryWallpapers())
    {
        QListWidgetItem* pItem = new QListWidgetItem();
        //pItem->setText(QFileInfo(path).fileName());
        const QIcon& icon = WallpaperTool::Instance()->GetHistoryWallpaperThread().GetWallpaperIcon(path);
        pItem->setIcon(icon);
        pItem->setData(Qt::UserRole, path);
        pItem->setData(Qt::ToolTipRole, path);
        m_listWidget->addItem(pItem);
    }
}

void HistoryWallpaperWidget::OnContextMenu(const QPoint& pos)
{
    m_selectedItem = m_listWidget->itemAt(pos);
    if (m_selectedItem != nullptr)
    {
        m_menu.exec(m_listWidget->viewport()->mapToGlobal(pos));
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

void HistoryWallpaperWidget::OnRemoveRecord()
{
    if (m_selectedItem != nullptr)
    {
        QString path = m_selectedItem->data(Qt::UserRole).toString();
        if (!path.isEmpty())
        {
            if (QMessageBox::question(this, QString(), QString(u8"确实要从列表中移除选中记录 %1 吗？").arg(path), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
            {
                WallpaperTool::Instance()->GetHistoryWallpaperMgr().RemoveRecord(path);
                m_listWidget->removeItemWidget(m_selectedItem);
                delete m_selectedItem;
                m_selectedItem = nullptr;
                m_listWidget->setCurrentItem(nullptr);
            }
        }
    }
}

void HistoryWallpaperWidget::OnDeleteWallpaper()
{
    if (m_selectedItem != nullptr)
    {
        QString path = m_selectedItem->data(Qt::UserRole).toString();
        if (!path.isEmpty())
        {
            if (QMessageBox::question(this, QString(), QString(u8"确实要从磁盘中删除文件 %1 吗？").arg(path), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
            {
                QString strLogInfo;
                if (QFile::remove(path))
                {
                    //删除文件后从列表中移除
                    WallpaperTool::Instance()->GetHistoryWallpaperMgr().RemoveRecord(path);
                    m_listWidget->removeItemWidget(m_selectedItem);
                    delete m_selectedItem;
                    m_selectedItem = nullptr;
                    m_listWidget->setCurrentItem(nullptr);
                    strLogInfo = QString(u8"删除壁纸 %1 成功。").arg(path);
                    QMessageBox::information(this, QString(), u8"删除成功！");
                }
                else
                {
                    strLogInfo = QString(u8"删除壁纸 %1 失败。").arg(path);
                    QMessageBox::information(this, QString(), u8"删除失败！");
                }
                WallpaperTool::Instance()->WriteLog(strLogInfo);
            }
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

