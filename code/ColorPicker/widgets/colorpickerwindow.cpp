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

void ColorPickerWindow::SetColor(const QColor& color)
{
    //设置当前颜色
    ui->selectColorWidget->SetColor(color);
    UpdateColorValue();
}

void ColorPickerWindow::UpdateColorValue()
{
    //设置COLORREF值
    UpdateColorRefValue();
    //设置RGB值
    UpdateRValue();
    UpdateGValue();
    UpdateBValue();
    //设置十六进制值
    UpdateHexValue();
}

void ColorPickerWindow::UpdateColorRefValue()
{
    const QColor& color = ui->selectColorWidget->GetColor();
    auto colorrefValue = ColorConvert::GetColorrefValue(color);
    ui->colorValueColorRefEdit->setText(ValueToString(colorrefValue));
}

void ColorPickerWindow::UpdateRValue()
{
    const QColor& color = ui->selectColorWidget->GetColor();
    ui->colorValueREdit->setText(ValueToString(color.red()));
}

void ColorPickerWindow::UpdateGValue()
{
    const QColor& color = ui->selectColorWidget->GetColor();
    ui->colorValueGEdit->setText(ValueToString(color.green()));
}

void ColorPickerWindow::UpdateBValue()
{
    const QColor& color = ui->selectColorWidget->GetColor();
    ui->colorValueBEdit->setText(ValueToString(color.blue()));
}

void ColorPickerWindow::UpdateHexValue()
{
    const QColor& color = ui->selectColorWidget->GetColor();
    QString strHex = color.name();
    if (!m_hex_lower_case)
        strHex = strHex.toUpper();
    ui->colorValueHexEdit->setText(strHex);
}

void ColorPickerWindow::SetUseHex(bool use_hex)
{
    m_use_hex = use_hex;
    UpdateColorValue();
}

void ColorPickerWindow::SetHexLowerCase(bool hex_lower_case)
{
    m_hex_lower_case = hex_lower_case;
    UpdateColorValue();
}

void ColorPickerWindow::on_selectColorBtn_clicked()
{
    StartPicking();
}

void ColorPickerWindow::OnColorPicked(const QColor& color)
{
    SetColor(color);
}

void ColorPickerWindow::OnColorHovered(const QColor& color)
{
    ui->labelCurColorRgb->setText(ColorConvert::GetRgbString(color));
    ui->labelCurColorHex->setText(color.name());
    ui->curColorWidget->SetColor(color);

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

void ColorPickerWindow::on_colorValueColorRefEdit_textEdited(const QString& text)
{
    bool ok{};
    auto colorRefValue = text.toInt(&ok, 0);
    if (ok)
    {
        QColor color = ColorConvert::ColorrefToColor(colorRefValue);
        ui->selectColorWidget->SetColor(color);
        UpdateRValue();
        UpdateGValue();
        UpdateBValue();
        UpdateHexValue();
    }
}

void ColorPickerWindow::on_colorValueREdit_textEdited(const QString& text)
{
    bool ok{};
    auto rValue = text.toInt(&ok, 0);
    if (ok)
    {
        QColor color = ui->selectColorWidget->GetColor();
        color.setRed(rValue);
        ui->selectColorWidget->SetColor(color);
        UpdateColorRefValue();
        UpdateHexValue();
    }
}

void ColorPickerWindow::on_colorValueGEdit_textEdited(const QString& text)
{
    bool ok{};
    auto gValue = text.toInt(&ok, 0);
    if (ok)
    {
        QColor color = ui->selectColorWidget->GetColor();
        color.setGreen(gValue);
        ui->selectColorWidget->SetColor(color);
        UpdateColorRefValue();
        UpdateHexValue();
    }
}

void ColorPickerWindow::on_colorValueBEdit_textEdited(const QString& text)
{
    bool ok{};
    auto bValue = text.toInt(&ok, 0);
    if (ok)
    {
        QColor color = ui->selectColorWidget->GetColor();
        color.setBlue(bValue);
        ui->selectColorWidget->SetColor(color);
        UpdateColorRefValue();
        UpdateHexValue();
    }
}

void ColorPickerWindow::on_colorValueHexEdit_textEdited(const QString& text)
{
    bool ok{};
    QString strHex = text;
    if (!strHex.isEmpty() && strHex[0] != '#')
        strHex = "#" + strHex;
    QColor color = QColor(strHex);
    ui->selectColorWidget->SetColor(color);
    UpdateColorRefValue();
    UpdateRValue();
    UpdateGValue();
    UpdateBValue();
}

QString ColorPickerWindow::ValueToString(unsigned int value)
{
    QString strValue = QString::number(value, m_use_hex ? 16 : 10);
    if (m_use_hex && !m_hex_lower_case)
        strValue = strValue.toUpper();
    if (m_use_hex)
        strValue = "0x" + strValue;
    return strValue;
}
