#include "RulerDrawer.h"
#include <QApplication>
#include "PixelRuler.h"
#include "define.h"

const int SCALE_MARGIN = ::DPI(32);

CRulerDrawer::CRulerDrawer(QPainter& painter, QRect rect, int scalePercent)
    : m_painter(painter), m_rect(rect), m_scalePercent(scalePercent)
{
    m_palette = qApp->palette();
    m_scaleUnitSize = PixelRuler::Instance()->GetScaleUnitSize();
}

void CRulerDrawer::Draw(ScaleDirection direction, bool drawText)
{
    switch (direction)
    {
    case CRulerDrawer::Top:
        DrawHorizontalScale(true, drawText);
        break;
    case CRulerDrawer::Bottom:
        DrawHorizontalScale(false, drawText);
        break;
    case CRulerDrawer::Left:
        DrawVerticalScale(true, drawText);
        break;
    case CRulerDrawer::Right:
        DrawVerticalScale(false, drawText);
        break;
    }
}

void CRulerDrawer::DrawHorizontalScale(int top, bool drawText)
{
    m_painter.save();
    //绘制水平标尺
    int x_pos{ 0 };		//水平刻度
    m_painter.setPen(QPen(m_palette.color(QPalette::Text)));
    //绘制刻度

    int y_pos_start;
    if (top)
        y_pos_start = 0;
    else
        y_pos_start = m_rect.height();

    while (DPI(x_pos) <= m_rect.width() - 2 * SCALE_MARGIN)
    {
        int line_length;		//刻度的长度
        if (x_pos % (m_scaleUnitSize * m_scaleUnitSize) == 0)		//刻度值为100的倍数时绘制刻度的长度为10像素
            line_length = DPI(10);
        else if (x_pos % (m_scaleUnitSize * m_scaleUnitSize / 2) == 0)	//刻度值为50的倍数时绘制刻度的长度为8像素
            line_length = DPI(8);
        else
            line_length = DPI(5);
        int y_pos_end;
        if (top)
            y_pos_end = y_pos_start + line_length;
        else
            y_pos_end = y_pos_start - line_length;
        m_painter.drawLine(QPoint(DPI(x_pos) + SCALE_MARGIN - 1, y_pos_start), QPoint(DPI(x_pos) + SCALE_MARGIN - 1, y_pos_end));
        //画刻度上的文本
        if (drawText && x_pos % (m_scaleUnitSize * m_scaleUnitSize) == 0)		//当刻度值为100的倍数时显示刻度值
        {
            QString str = QString::number(x_pos);
            QSize str_size = m_painter.fontMetrics().size(Qt::TextSingleLine, str);
            int y_pos_text;
            if (top)
                y_pos_text = DPI(12);
            else
                y_pos_text = y_pos_start - DPI(12) - str_size.height();
            QRect rect{ QPoint{ DPI(x_pos) + SCALE_MARGIN - str_size.width() / 2, y_pos_text }, str_size };
            m_painter.drawText(rect, str);
        }
        x_pos += m_scaleUnitSize;
    }
    m_painter.restore();
}

void CRulerDrawer::DrawVerticalScale(int left, bool drawText)
{
    m_painter.save();
    //绘制垂直标尺
    int y_pos{ 0 };		//垂直刻度
    m_painter.setPen(QPen(m_palette.color(QPalette::Text)));
    //绘制刻度

    int x_pos_start;
    if (left)
        x_pos_start = 0;
    else
        x_pos_start = m_rect.width();

    while (DPI(y_pos) <= m_rect.height() - 2 * SCALE_MARGIN)
    {
        int line_length;		//刻度的长度
        if (y_pos % (m_scaleUnitSize * m_scaleUnitSize) == 0)		//刻度值为100的倍数时绘制刻度的长度为10像素
            line_length = DPI(10);
        else if (y_pos % (m_scaleUnitSize * m_scaleUnitSize / 2) == 0)	//刻度值为50的倍数时绘制刻度的长度为8像素
            line_length = DPI(8);
        else
            line_length = DPI(5);
        int x_pos_end;
        if (left)
            x_pos_end = x_pos_start + line_length;
        else
            x_pos_end = x_pos_start - line_length;
        m_painter.drawLine(QPoint(x_pos_start, DPI(y_pos) + SCALE_MARGIN - 1), QPoint(x_pos_end, DPI(y_pos) + SCALE_MARGIN - 1));
        //画刻度上的文本
        if (drawText && y_pos % (m_scaleUnitSize * m_scaleUnitSize) == 0)		//当刻度值为100的倍数时显示刻度值
        {
            QString str = QString::number(y_pos);
            QSize str_size = m_painter.fontMetrics().size(Qt::TextSingleLine, str);
            int x_pos_text;
            if (left)
                x_pos_text = DPI(12);
            else
                x_pos_text = x_pos_start - DPI(12) - str_size.width();
            QRect rect{ QPoint{ x_pos_text, DPI(y_pos) + SCALE_MARGIN - str_size.height() / 2 }, str_size };
            m_painter.drawText(rect, str);
        }
        y_pos += m_scaleUnitSize;
    }
    m_painter.restore();
}

int CRulerDrawer::DPI(int pixel)
{
    return pixel * m_scalePercent / 100;
}
