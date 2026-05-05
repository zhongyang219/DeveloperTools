#include "bingwallpaperwidget.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QFileDialog>
#include <QMessageBox>
#include "../../CCommonTools/CommonTools.h"

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

void CBingWallpaperWidget::CurrentWallpaperSaveAs()
{
    QString wallpaperName = m_infoEdit.text();
    CCommonTools::FileNameNormalize(wallpaperName);
    QString strDir = QFileDialog::getExistingDirectory(this, QString(), QString(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!strDir.isEmpty())
    {
        QString filePath = strDir + "/BingWallpaper_" + wallpaperName + ".jpg";
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly))
        {
            qint64 written = file.write(m_wallpaperManager->GetImageData());
            file.close();

            if (written != m_wallpaperManager->GetImageData().size())
            {
                QMessageBox::warning(this, u8"必应壁纸另存为", u8"文件写入不完整");
            }
        }
        else
        {
            QMessageBox::warning(this, u8"必应壁纸另存为", u8"无法打开文件: " + file.errorString());
        }
    }
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
    m_infoEdit.setText(u8"加载失败: " + errorMsg);
    qWarning() << u8"壁纸加载错误:" << errorMsg;
}
