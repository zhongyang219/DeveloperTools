#pragma once
#include <QWidget>
#include <QColor>

class ColorWidget : public QWidget
{
    Q_OBJECT
public:
    ColorWidget(QWidget* pParent = nullptr);
    void SetColor(const QColor& color);

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QColor m_color;
};
