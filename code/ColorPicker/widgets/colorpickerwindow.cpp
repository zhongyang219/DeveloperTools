//
// Created by zhongyang219 on 2026/7/4.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ColorPickerWindow.h" resolved

#include "colorpickerwindow.h"
#include "ui_ColorPickerWindow.h"
#include <QScreen>
#include "ColorPicker.h"
#include "common/ColorConvert.h"

ColorPickerWindow::ColorPickerWindow(QWidget* parent) : QWidget(parent), ui(new Ui::ColorPickerWindow)
{
    ui->setupUi(this);
    // connect(&m_picker_timer, SIGNAL(timeout()), this, SLOT(OnPickerTimeout()));
}

ColorPickerWindow::~ColorPickerWindow()
{
    delete ui;
}

void ColorPickerWindow::LoadConfig()
{
}

void ColorPickerWindow::SaveConfig() const
{
}

void ColorPickerWindow::StartPicking()
{
    m_overlay = std::make_unique<ColorPickerOverlay>(this);
    connect(m_overlay.get(), SIGNAL(colorPicked(const QColor&)), SLOT(OnColorPicked(const QColor&)));
    connect(m_overlay.get(), SIGNAL(colorHovered(const QColor&)), SLOT(OnColorHovered(const QColor&)));
    connect(m_overlay.get(), SIGNAL(canceled()), SLOT(OnColorCanceled()));
    m_overlay->show();
}

void ColorPickerWindow::on_selectColorBtn_clicked()
{
    StartPicking();
}

void ColorPickerWindow::OnColorPicked(const QColor& color)
{
}

void ColorPickerWindow::OnColorHovered(const QColor& color)
{
    ui->labelCurColorRgb->setText(ColorConvert::GetRgbString(color));
    ui->labelCurColorHex->setText(color.name());

    if (m_overlay != nullptr)
    {
        QPoint pos = m_overlay->GetPickingPos();
        QString strPos = QString("x: %1, y: %2").arg(pos.x()).arg(pos.y());
        ColorPicker::Instance()->GetMainFrame()->SetStatusBarText(strPos.toUtf8().constData(), 2000);
    }
}

void ColorPickerWindow::OnColorCanceled()
{
}
