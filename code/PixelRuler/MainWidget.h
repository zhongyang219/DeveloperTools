#pragma once

#include <QWidget>
#include <QTextEdit> 

class CMainWidget  : public QWidget
{
    Q_OBJECT

public:
    CMainWidget(QWidget* parent = nullptr);
    ~CMainWidget();

    QTextEdit m_helpEdit;

private:
    virtual void paintEvent(QPaintEvent* event) override;
};
