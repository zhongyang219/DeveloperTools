#ifndef BINGWALLPAPERMANAGER_H
#define BINGWALLPAPERMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPixmap>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class BingWallpaperManager : public QObject
{
    Q_OBJECT
public:
    explicit BingWallpaperManager(QObject *parent = nullptr);
    ~BingWallpaperManager();

    /**
     * 获取壁纸（异步）
     * @param idx 壁纸的索引，0=今天，1=昨天，以此类推
     */
    void fetchWallpaper(int idx = 0);

    const QByteArray& GetImageData();

signals:
    // 壁纸获取成功信号
    void wallpaperReady(const QPixmap &pixmap, const QString &copyright);
    // 错误信号
    void errorOccurred(const QString &errorMessage);

private slots:
    void onJsonReplyFinished();    // 处理JSON响应
    void onImageReplyFinished();   // 处理图片响应
    void onNetworkReplayError(QNetworkReply::NetworkError code);

private:
    QString extractImageUrl(const QString &url);
    QNetworkAccessManager *m_networkManager;
    QString m_copyrightInfo;
    QString m_wallpaperDate;
    QByteArray m_imageData;
};

#endif // BINGWALLPAPERMANAGER_H
