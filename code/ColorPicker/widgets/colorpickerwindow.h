//
// Created by zhongyang219 on 2026/7/4.
//

#ifndef DEVELOPERTOOLS_COLORPICKERWINDOW_H
#define DEVELOPERTOOLS_COLORPICKERWINDOW_H

#include <QWidget>

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

private:
    Ui::ColorPickerWindow* ui;
};

#endif //DEVELOPERTOOLS_COLORPICKERWINDOW_H
