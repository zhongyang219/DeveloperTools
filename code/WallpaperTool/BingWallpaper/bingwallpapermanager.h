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

    // 获取今日壁纸（异步）
    void fetchTodayWallpaper();

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
};

#endif // BINGWALLPAPERMANAGER_H
