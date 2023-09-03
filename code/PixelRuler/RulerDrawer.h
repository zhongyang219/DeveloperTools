#pragma once
#include <QPainter>
#include <QPalette>
class CRulerDrawer
{
public:
    //刻度的位置
    enum ScaleDirection
    {
        Top,
        Bottom,
        Left,
        Right,
    };

    CRulerDrawer(QPainter& painter, QRect rect, int scalePercent);
    void Draw(ScaleDirection direction, bool drawText = true);

private:
    void DrawHorizontalScale(int top, bool drawText);
    void DrawVerticalScale(int left, bool drawText);
    int DPI(int pixel);

private:
    int m_scalePercent = 100;   //尺子缩放百分比
    QRect m_rect;
    QPainter& m_painter;
    QPalette m_palette;
    int m_scaleUnitSize = 10;
};

