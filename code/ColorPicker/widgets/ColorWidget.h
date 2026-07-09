#pragma once
#include <QWidget>
#include <QColor>

class ColorWidget : public QWidget
{
    Q_OBJECT
public:
    ColorWidget(QWidget* pParent = nullptr);
    void SetColor(const QColor& color);
    const QColor& GetColor() const;
    void SetClickable(bool clickable);

signals:
    void colorEdited();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;

private:
    QColor m_color;
    bool m_clickable{};
};
