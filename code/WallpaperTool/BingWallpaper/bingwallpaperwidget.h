#ifndef BINGWALLPAPERWIDGET_H
#define BINGWALLPAPERWIDGET_H
#include <QLineEdit>
#include "ImageLabel.h"
#include "bingwallpapermanager.h"
#include <QWidget>

class CBingWallpaperWidget : public QWidget
{
    Q_OBJECT
public:
    CBingWallpaperWidget();

private slots:
    void onWallpaperReady(const QPixmap &pixmap, const QString &copyright);
    void onErrorOccurred(const QString &errorMsg);

private:
    QLineEdit m_infoEdit;
    CImageLabel m_mainImgLabel;
    BingWallpaperManager *m_wallpaperManager;
};

#endif // BINGWALLPAPERWIDGET_H
