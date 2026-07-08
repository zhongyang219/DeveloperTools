//
// Created by zhongyang219 on 2026/7/4.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ColorPickerWindow.h" resolved

#include "colorpickerwindow.h"
#include "ui_ColorPickerWindow.h"
#include <QScreen>
#include <QClipboard>
#include "ColorPicker.h"
#include "common/ColorConvert.h"

bool IsCharHex(char ch)
{
    return (ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F');
}

bool IsStringHex(const QString& strHex)
{
    QString strHex1 = strHex.trimmed();
    if (strHex1.isEmpty())
        return false;
    if (strHex1[0] == L'#')
        strHex1 = strHex1.mid(1);
    for (auto& ch : strHex1)
    {
        if (!IsCharHex(ch.toLatin1()))
            return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

const QColor& ColorPickerWindow::GetColor() const
{
    return ui->selectColorWidget->GetColor();
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

bool ColorPickerWindow::GetUseHex() const
{
    return m_use_hex;
}

void ColorPickerWindow::SetHexLowerCase(bool hex_lower_case)
{
    m_hex_lower_case = hex_lower_case;
    UpdateColorValue();
}

bool ColorPickerWindow::GetHexLowerCase()
{
    return m_hex_lower_case;
}

void ColorPickerWindow::CopyRgbValue()
{
    const QColor& color = ui->selectColorWidget->GetColor();
    QString strRgb = ColorConvert::GetRgbString(color);
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(strRgb);
}

void ColorPickerWindow::CopyHexValue()
{
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(ui->colorValueHexEdit->text());
}

void ColorPickerWindow::PasteRgbValue()
{
    QClipboard* clipboard = QApplication::clipboard();
    QString strRgb = clipboard->text();
    QStringList strRgbList = strRgb.split(',', Qt::SkipEmptyParts);
    if (strRgbList.size() >= 3)
    {
        int r = strRgbList[0].toInt(nullptr, 0);
        int g = strRgbList[1].toInt(nullptr, 0);
        int b = strRgbList[2].toInt(nullptr, 0);
        ui->selectColorWidget->SetColor(QColor(r, g, b));
        UpdateColorValue();
        ColorPicker::Instance()->GetMainFrame()->SetStatusBarText(u8"RGB值粘贴成功。", 2000);
    }
    else
    {
        ColorPicker::Instance()->GetMainFrame()->SetStatusBarText(u8"RGB值粘贴失败！", 2000);
    }
}

void ColorPickerWindow::PasteHexValue()
{
    QClipboard* clipboard = QApplication::clipboard();
    QString strHex = clipboard->text();
    if (IsStringHex(strHex))
    {
        if (strHex[0] != '#')
            strHex = '#' + strHex;
        ui->selectColorWidget->SetColor(QColor(strHex));
        UpdateColorValue();
        ColorPicker::Instance()->GetMainFrame()->SetStatusBarText(u8"十六进制值粘贴成功。", 2000);
    }
    else
    {
        ColorPicker::Instance()->GetMainFrame()->SetStatusBarText(u8"十六进制值粘贴失败！", 2000);
    }

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
