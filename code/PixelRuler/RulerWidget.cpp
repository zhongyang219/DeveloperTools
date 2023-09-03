#include "RulerWidget.h"
#include "RulerDrawer.h"
#include "PixelRuler.h"
#include <QApplication>
#include <QMouseEvent>
#include "define.h"
#include <QScreen>
#include <QKeyEvent>

CRulerWidget::CRulerWidget(Qt::Orientation orientation, QWidget *parent)
    : QWidget(parent), m_orientation(orientation)
{
    //设置无边框
    auto flags = windowFlags();
    flags |= Qt::FramelessWindowHint;       //去掉窗口边框
    flags |= Qt::WindowStaysOnTopHint;      //总是置顶
    this->setWindowFlags(flags);

    //设置窗口大小
    QSize screenSize = QGuiApplication::primaryScreen()->size();
    int rulerSize = DPI(46);
    if (orientation == Qt::Horizontal)
        resize(QSize(screenSize.width(), rulerSize));
    else
        resize(QSize(rulerSize, screenSize.height()));
}

CRulerWidget::~CRulerWidget()
{}

void CRulerWidget::paintEvent(QPaintEvent * event)
{
    QPainter painter(this);
    CRulerDrawer drawer(painter, rect(), PixelRuler::Instance()->GetScalePercent());
    //绘制标尺
    if (m_orientation == Qt::Horizontal)
    {
        drawer.Draw(CRulerDrawer::Top, true);
        drawer.Draw(CRulerDrawer::Bottom, false);
    }
    else if (m_orientation == Qt::Vertical)
    {
        drawer.Draw(CRulerDrawer::Left, true);
        drawer.Draw(CRulerDrawer::Right, false);
    }
    //绘制窗口边框
    painter.save();
    painter.setPen(QPen(qApp->palette().color(QPalette::Text)));
    QRect windowRect = rect();
    windowRect.setRight(windowRect.right() - 1);
    windowRect.setBottom(windowRect.bottom() - 1);
    painter.drawRect(windowRect);
    painter.restore();
}

void CRulerWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (m_mousePressed)
    {
        move(event->globalPos() - m_mousePoint);
    }
}

void CRulerWidget::mousePressEvent(QMouseEvent* event)
{
    if ((event->buttons() & Qt::LeftButton) != 0)
    {
        m_mousePressed = true;
        m_mousePoint = event->globalPos() - this->pos();
    }
}

void CRulerWidget::mouseReleaseEvent(QMouseEvent* event)
{
    m_mousePressed = false;
}

void CRulerWidget::keyReleaseEvent(QKeyEvent* event)
{
    //按下键盘方向键可以以一个像素为单位移动窗口位置
    if (event->key() == Qt::Key_Up)
    {
        QPoint point = this->pos();
        point.setY(point.y() - 1);
        move(point);
    }
    else if (event->key() == Qt::Key_Down)
    {
        QPoint point = this->pos();
        point.setY(point.y() + 1);
        move(point);
    }
    else if (event->key() == Qt::Key_Left)
    {
        QPoint point = this->pos();
        point.setX(point.x() - 1);
        move(point);
    }
    else if (event->key() == Qt::Key_Right)
    {
        QPoint point = this->pos();
        point.setX(point.x() + 1);
        move(point);
    }

    QWidget::keyReleaseEvent(event);
}
