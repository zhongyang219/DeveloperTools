//
// Created by zhongyang219 on 2026/7/7.
//

#include "ColorPickerOverlay.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QScreen>
#include <QGuiApplication>
#include <QPainter>
#include "define.h"

 //由于取色时有1/256不透明度的窗口，因此需要对取到的颜色进行校准
 QColor VerifyPickedColor(const QColor& color)
 {
     //将0~254映射到0~255
     int r = qRound(color.red() * 255.0 / 254.0);
     int g = qRound(color.green() * 255.0 / 254.0);
     int b = qRound(color.blue() * 255.0 / 254.0);
     // 限制在 0~255 范围内，防止溢出
     r = qBound(0, r, 255);
     g = qBound(0, g, 255);
     b = qBound(0, b, 255);
     return QColor(r, g, b);
 }

ColorPickerOverlay::ColorPickerOverlay(QWidget *parent) : QWidget(parent)
{

    // 设置窗口属性：无边框、置顶、不在任务栏显示
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);

#ifdef Q_OS_WIN
    // 设置背景透明（设置为不透明度1/256，完全不透明会导致窗口直接消失无法响应鼠标事件）
     setWindowOpacity(0.004);
     setStyleSheet("background-color: rgb(0, 0, 0);");
#else

    setAttribute(Qt::WA_TransparentForMouseEvents, false);
    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet("background-color: transparent;");
#endif

    // 计算所有显示器的虚拟总区域
    QRect virtualRect;
    for (QScreen *screen : QGuiApplication::screens()) {
        virtualRect = virtualRect.united(screen->geometry());
    }
    setGeometry(virtualRect);

    // 设置自定义吸管光标 (吸管尖端在图片左下角)
    QPixmap pipettePixmap(":res/pickCursor.png");
    int cursor_size = DPI(16);
    setCursor(QCursor(pipettePixmap.scaled(cursor_size, cursor_size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation), 0, cursor_size));

    setMouseTracking(true);
}

const QPoint& ColorPickerOverlay::GetPickingPos()
{
    return m_picking_pos;
}

void ColorPickerOverlay::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
}

// 拦截鼠标移动，实时获取颜色
void ColorPickerOverlay::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pos = event->globalPos();
    if (pos != m_picking_pos)
    {
        m_picking_pos = pos;
        QScreen* screen = QGuiApplication::screenAt(pos);
        if (!screen)
            screen = QGuiApplication::primaryScreen();

        // 截取 1x1 像素
        QPixmap pixmap = screen->grabWindow(0, pos.x(), pos.y(), 1, 1);
        m_color = pixmap.toImage().pixelColor(0, 0);
#ifdef Q_OS_WIN
        //windows下设置了1/256不透明度，因此这里要转换一下
        m_color = VerifyPickedColor(m_color);
#endif
        emit colorHovered(m_color);
    }
}

// 拦截鼠标点击，防止事件穿透到底层窗口
void ColorPickerOverlay::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        // 左键：确认取色
        emit colorPicked(m_color);
        this->close(); // 关闭覆盖窗口，退出取色模式
    }
    else if (event->button() == Qt::RightButton)
    {
        // 右键：取消取色
        emit canceled();
        this->close();
    }
    // 不调用 QWidget::mousePressEvent
}

// ESC取消取色
void ColorPickerOverlay::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        emit canceled();
        this->close();
    }
}
