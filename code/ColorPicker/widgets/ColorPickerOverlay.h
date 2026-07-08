//
// Created by zhongyang219 on 2026/7/7.
//

#ifndef DEVELOPERTOOLS_COLORPICKEROVERLAY_H
#define DEVELOPERTOOLS_COLORPICKEROVERLAY_H

#include <QWidget>
#include <QColor>

//取色时的全屏覆盖窗口
class ColorPickerOverlay : public QWidget
{
    Q_OBJECT
public:
    explicit ColorPickerOverlay(QWidget *parent = nullptr);
    const QPoint& GetPickingPos();

signals:
    void colorPicked(const QColor &color);   // 左键确认取色
    void colorHovered(const QColor &color);  // 鼠标移动实时预览
    void canceled();                         // 右键或ESC取消

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override; // 关键：防止绘制背景

private:
    QPoint m_picking_pos;
    QColor m_color;
};

#endif //DEVELOPERTOOLS_COLORPICKEROVERLAY_H
