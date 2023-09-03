#pragma once

#include <QWidget>

class CRulerWidget  : public QWidget
{
public:
    CRulerWidget(Qt::Orientation orientation, QWidget *parent = nullptr);
    ~CRulerWidget();

private:
    virtual void paintEvent(QPaintEvent* event) override;

protected:
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void keyReleaseEvent(QKeyEvent* event) override;

private:
    Qt::Orientation m_orientation;
    QPoint m_mousePoint;
    bool m_mousePressed{};
};
