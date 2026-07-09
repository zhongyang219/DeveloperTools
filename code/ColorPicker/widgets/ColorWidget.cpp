#include "widgets/ColorWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QColorDialog>

ColorWidget::ColorWidget(QWidget* pParent)
    : QWidget(pParent)
{
}

void ColorWidget::SetColor(const QColor& color)
{
    m_color = color;
    update();
}

const QColor& ColorWidget::GetColor() const
{
    return m_color;
}

void ColorWidget::SetClickable(bool clickable)
{
    m_clickable = clickable;
    if (m_clickable)
    {
        setCursor(Qt::PointingHandCursor);
    }
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

void ColorWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (m_clickable)
    {
        if (event->button() == Qt::LeftButton)
        {
            QColorDialog colorDlg(m_color, this);
            if (colorDlg.exec() == QDialog::Accepted)
            {
                m_color = colorDlg.currentColor();
                emit colorEdited();
            }
        }
    }
}
