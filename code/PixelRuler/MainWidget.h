#pragma once

#include <QWidget>

class CMainWidget  : public QWidget
{
    Q_OBJECT

public:
    CMainWidget(QWidget* parent = nullptr);
    ~CMainWidget();

private:
    virtual void paintEvent(QPaintEvent* event) override;
};
