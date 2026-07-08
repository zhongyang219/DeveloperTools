#include "widgets/ColorWidget.h"
#include <QPainter>

ColorWidget::ColorWidget(QWidget* pParent)
    : QWidget(pParent)
{
}

void ColorWidget::SetColor(const QColor& color)
{
    m_color = color;
    update();
}

void ColorWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QRect widget_rect = rect();
    widget_rect.adjust(0, 0, -1, -1);
    //绘制颜色矩形
    painter.fillRect(widget_rect, m_color);
    //绘制边框
    painter.setPen(QColor(128, 128, 128));
    painter.drawRect(widget_rect);
}
