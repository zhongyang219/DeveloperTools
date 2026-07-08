//
// Created by zhongyang219 on 2026/7/4.
//

#ifndef DEVELOPERTOOLS_COLORPICKERWINDOW_H
#define DEVELOPERTOOLS_COLORPICKERWINDOW_H

#include <QWidget>
#include "widgets/ColorPickerOverlay.h"
#include <memory>

QT_BEGIN_NAMESPACE

namespace Ui
{
    class ColorPickerWindow;
}

QT_END_NAMESPACE

class ColorPickerWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ColorPickerWindow(QWidget* parent = nullptr);
    ~ColorPickerWindow() override;

    virtual void LoadConfig();
    virtual void SaveConfig() const;

    //开始取色
    void StartPicking();

private slots:
    void on_selectColorBtn_clicked();
    void OnColorPicked(const QColor &color);
    void OnColorHovered(const QColor &color);
    void OnColorCanceled();

private:
    Ui::ColorPickerWindow* ui;
    std::unique_ptr<ColorPickerOverlay> m_overlay;
};

#endif //DEVELOPERTOOLS_COLORPICKERWINDOW_H
