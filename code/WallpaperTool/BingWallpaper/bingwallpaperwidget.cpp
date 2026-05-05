#include "bingwallpaperwidget.h"
#include <QVBoxLayout>
#include <QLineEdit>

CBingWallpaperWidget::CBingWallpaperWidget()
{
    QVBoxLayout* pLayout = new QVBoxLayout();
    pLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(pLayout);
    pLayout->addWidget(&m_infoEdit);
    m_infoEdit.setReadOnly(true);
    pLayout->addWidget(&m_mainImgLabel);

    m_wallpaperManager = new BingWallpaperManager(this);
    // 连接信号槽
    connect(m_wallpaperManager, &BingWallpaperManager::wallpaperReady,
            this, &CBingWallpaperWidget::onWallpaperReady);
    connect(m_wallpaperManager, &BingWallpaperManager::errorOccurred,
            this, &CBingWallpaperWidget::onErrorOccurred);

    // 启动获取
    m_wallpaperManager->fetchTodayWallpaper();
}

void CBingWallpaperWidget::onWallpaperReady(const QPixmap &pixmap, const QString &copyright)
{
    // 在GUI线程更新UI（信号槽机制保证线程安全）
    m_mainImgLabel.SetImage(pixmap);
    m_infoEdit.setText(copyright);

    // 可选：保存本地缓存
    // pixmap.save(QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)
    //            + "/bing_wallpaper.jpg");
}

void CBingWallpaperWidget::onErrorOccurred(const QString &errorMsg)
{
    m_infoEdit.setText("加载失败: " + errorMsg);
    qWarning() << "壁纸加载错误:" << errorMsg;
}
