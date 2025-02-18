#pragma once

#include <QMainWindow>
#include <QLineEdit>
#include "ImageLabel.h"

class MainWidget : public QMainWindow
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
    void SetWallpaper(const QString& strPath);

private:
    QLineEdit m_pathEdit;
    CImageLabel m_imgLabel;

};
