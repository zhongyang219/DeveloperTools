//
// Created by zhongyang219 on 2026/7/4.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ColorPickerWindow.h" resolved

#include "colorpickerwindow.h"
#include "ui_ColorPickerWindow.h"

ColorPickerWindow::ColorPickerWindow(QWidget* parent) : QWidget(parent), ui(new Ui::ColorPickerWindow)
{
    ui->setupUi(this);
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
