#pragma once

#include <QWidget>
#include "ui_MainWidget.h"

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private slots:

private:
    Ui::MainWidgetClass ui;
};
