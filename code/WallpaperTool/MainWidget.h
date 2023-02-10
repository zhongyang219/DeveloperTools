#pragma once

#include <QMainWindow>
#include "ui_MainWidget.h"

class MainWidget : public QMainWindow
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
    void SetWallpaper(const QString& strPath);

private:
    Ui::MainWidgetClass ui;
};
