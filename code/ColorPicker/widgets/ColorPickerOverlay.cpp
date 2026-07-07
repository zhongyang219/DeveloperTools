//
// Created by zhongyang219 on 2026/7/7.
//

#include "ColorPickerOverlay.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QScreen>
#include <QGuiApplication>
#include <QPainter>

ColorPickerOverlay::ColorPickerOverlay(QWidget *parent) : QWidget(parent)
{
    // 1. 设置窗口属性：无边框、置顶、不在任务栏显示
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);

    // 2. 设置背景完全透明
    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet("background-color: transparent;");

    // 3. 计算所有显示器的虚拟总区域（完美支持多显示器）
    QRect virtualRect;
    for (QScreen *screen : QGuiApplication::screens()) {
        virtualRect = virtualRect.united(screen->geometry());
    }
    setGeometry(virtualRect);

    // 4. 设置自定义吸管光标 (假设吸管尖端在图片左下角)
    QPixmap pipettePixmap(":/res/pickColor.png");
    setCursor(QCursor(pipettePixmap, 0, pipettePixmap.height()));

    setMouseTracking(true);
}

const QPoint& ColorPickerOverlay::GetPickingPos()
{
    return m_picking_pos;
}

// 关键：重写 paintEvent 且什么都不画，确保窗口绝对透明，不干扰截屏
void ColorPickerOverlay::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
}

// 拦截鼠标移动，实时获取颜色
void ColorPickerOverlay::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pos = event->globalPosition().toPoint();
    if (pos != m_picking_pos)
    {
        m_picking_pos = pos;
        QColor color = getColorAtPos(pos);
        emit colorHovered(color);
    }
}

// 拦截鼠标点击，防止事件穿透到底层窗口
void ColorPickerOverlay::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        // 左键：确认取色
        QColor color = getColorAtPos(event->globalPosition().toPoint());
        emit colorPicked(color);
        this->close(); // 关闭覆盖窗口，退出取色模式
    }
    else if (event->button() == Qt::RightButton)
    {
        // 右键：取消取色
        emit canceled();
        this->close();
    }
    // 注意：这里没有调用 QWidget::mousePressEvent，事件被“吞噬”了
}

// 拦截键盘事件，支持 ESC 取消
void ColorPickerOverlay::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        emit canceled();
        this->close();
    }
}

// 核心取色逻辑
QColor ColorPickerOverlay::getColorAtPos(const QPoint &pos)
{
    QScreen *screen = QGuiApplication::screenAt(pos);
    if (!screen) screen = QGuiApplication::primaryScreen();

    // 截取 1x1 像素
    QPixmap pixmap = screen->grabWindow(0, pos.x(), pos.y(), 1, 1);
    return pixmap.toImage().pixelColor(0, 0);
}