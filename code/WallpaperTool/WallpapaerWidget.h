#pragma once

#include <QWidget>
#include <QLineEdit>
#include "ImageLabel.h"

class CWallpapaerWidget  : public QWidget
{
    Q_OBJECT

public:
    CWallpapaerWidget(QWidget *parent);
    ~CWallpapaerWidget();
    void SetWallpaper(const QString& strPath);

private:
    QLineEdit m_pathEdit;
    CImageLabel m_imgLabel;
};
