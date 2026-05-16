#include "bingwallpapermanager.h"
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QByteArray>
#include <QBuffer>

BingWallpaperManager::BingWallpaperManager(QObject *parent)
    : QObject(parent)
    , m_networkManager(new QNetworkAccessManager(this))
{
}

BingWallpaperManager::~BingWallpaperManager()
{
    // QNetworkAccessManager 会被自动清理
}

void BingWallpaperManager::fetchWallpaper(int idx)
{
    // 1. 构建API请求
    QUrl apiUrl("http://cn.bing.com/HPImageArchive.aspx");
    QUrlQuery query;
    query.addQueryItem("format", "js");
    query.addQueryItem("idx", QString::number(idx));
    query.addQueryItem("n", "1");      // 返回数量
    query.addQueryItem("mkt", "zh-CN");
    apiUrl.setQuery(query);

    QNetworkRequest request(apiUrl);
    request.setHeader(QNetworkRequest::UserAgentHeader,
                      "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36");

    // 2. 发送异步请求（非阻塞！）
    QNetworkReply *reply = m_networkManager->get(request);
    if (reply != nullptr)
    {
        connect(reply, SIGNAL(finished()), this, SLOT(onJsonReplyFinished()));
    }
}

const QByteArray& BingWallpaperManager::GetImageData()
{
    return m_imageData;
}

void BingWallpaperManager::onJsonReplyFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;

    if (reply->error() == QNetworkReply::NoError) {
        // 解析JSON
        QByteArray jsonData = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(jsonData);

        if (!doc.isNull() && doc.isObject()) {
            QJsonObject root = doc.object();
            QJsonArray images = root["images"].toArray();

            if (!images.isEmpty()) {
                QJsonObject imageInfo = images[0].toObject();
                QString urlPath = imageInfo["url"].toString();
                m_copyrightInfo = imageInfo["copyright"].toString();
                m_wallpaperDate = imageInfo["enddate"].toString();

                // 3. 提取并拼接完整图片URL
                QString imageUrl = extractImageUrl(urlPath);

                // 4. 异步下载图片
                QNetworkRequest imgRequest{ QUrl(imageUrl) };
                imgRequest.setHeader(QNetworkRequest::UserAgentHeader,
                                     "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36");

                QNetworkReply *imgReply = m_networkManager->get(imgRequest);
                connect(imgReply, SIGNAL(finished()), this, SLOT(onImageReplyFinished()));
                // connect(imgReply, &QNetworkReply::finished,
                //         this, &BingWallpaperManager::onImageReplyFinished);
                // connect(imgReply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
                //         [imgReply](QNetworkReply::NetworkError code){
                //             qWarning() << "图片下载错误:" << code << imgReply->errorString();
                //             imgReply->deleteLater();
                //         });
            } else {
                emit errorOccurred(u8"未找到图片数据");
            }
        } else {
            emit errorOccurred(u8"JSON解析失败");
        }
    } else {
        emit errorOccurred(u8"网络请求失败: " + reply->errorString());
    }

    reply->deleteLater();
}

void BingWallpaperManager::onImageReplyFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;

    if (reply->error() == QNetworkReply::NoError) {
        m_imageData = reply->readAll();
        QPixmap pixmap;

        if (pixmap.loadFromData(m_imageData)) {
            // 5. 发射成功信号，传递图片和版权信息
            emit wallpaperReady(pixmap, m_wallpaperDate + ' ' + m_copyrightInfo);
        } else {
            emit errorOccurred(u8"图片加载失败");
        }
    }

    reply->deleteLater();
}

void BingWallpaperManager::onNetworkReplayError(QNetworkReply::NetworkError code)
{

}

QString BingWallpaperManager::extractImageUrl(const QString &url)
{
    // 截取从开头到 ".jpg" (包含)
    int jpgIndex = url.indexOf(".jpg", Qt::CaseInsensitive);
    if (jpgIndex != -1) {
        QString path = url.left(jpgIndex + 4);  // +4 因为 ".jpg" 长度为4
        // 拼接基础域名，注意处理双斜杠问题
        return "http://cn.bing.com" + path;
    }
    return url;  // 如果没找到 .jpg，返回原字符串
}
