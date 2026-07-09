//
// Created by zhongyang219 on 2026/7/4.
//

#ifndef DEVELOPERTOOLS_COLORPICKERWINDOW_H
#define DEVELOPERTOOLS_COLORPICKERWINDOW_H

#include <memory>
#include <QWidget>
#include <QMenu>
#include "widgets/ColorPickerOverlay.h"
#include "ColorTableHelper.h"

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

    void SetColor(const QColor& color);
    const QColor& GetColor() const;

    void SetUseHex(bool use_hex);
    bool GetUseHex() const;
    void SetHexLowerCase(bool hex_lower_case);
    bool GetHexLowerCase();

    void CopyRgbValue();
    void CopyHexValue();
    void PasteRgbValue();
    void PasteHexValue();

    ColorTableHelper* GetColorTableHelper();

private slots:
    void on_selectColorBtn_clicked();
    void OnColorPicked(const QColor &color);
    void OnColorHovered(const QColor &color);

    void on_colorValueColorRefEdit_textEdited(const QString& text);
    void on_colorValueREdit_textEdited(const QString& text);
    void on_colorValueGEdit_textEdited(const QString& text);
    void on_colorValueBEdit_textEdited(const QString& text);
    void on_colorValueHexEdit_textEdited(const QString& text);
    void on_colorValueHEdit_valueChanged(int i);
    void on_colorValueSEdit_valueChanged(int i);
    void on_colorValueLEdit_valueChanged(int i);

    void UpdateColorValue();
    void UpdateColorRefValue();
    void UpdateRValue();
    void UpdateGValue();
    void UpdateBValue();
    void UpdateHexValue();
    void UpdateHValue();
    void UpdateSValue();
    void UpdateLValue();

    void onTreeContextMenu(const QPoint& pos);

    void on_actionAddGroup_triggered();
    void on_actionAddCurColor_triggered();
    void on_actionDelete_triggered();
    void on_addToColorTableBtn_clicked();
    void on_delectSelColorBtn_clicked();

private:
    QString ValueToString(unsigned int value);

private:
    Ui::ColorPickerWindow* ui;
    std::unique_ptr<ColorPickerOverlay> m_overlay;
    bool m_use_hex{};
    bool m_hex_lower_case{ true };
    std::unique_ptr<ColorTableHelper> m_color_table_helper;
    QMenu m_context_menu;
};

#endif //DEVELOPERTOOLS_COLORPICKERWINDOW_H
