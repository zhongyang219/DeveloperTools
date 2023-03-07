#ifndef STYLEMANAGER_H
#define STYLEMANAGER_H
#include <QString>
#include <QWidget>
#include <QComboBox>
#include "ccommontools_global.h"

class CCOMMONTOOLS_EXPORT CStyle
{
public:
    CStyle(const QString& strPath, bool hasPaletteColor = true);

    /**
        @brief 为一个窗口应用样式表
        @param pWidget - 窗口的指针
    **/
    void ApplyStyleSheet(QWidget* pWidget) const;

    /**
        @brief 对一个样式表中所有像素值根据系统DPI设定进行放大
        @param strStyle - 样式表
    **/
    static void StyleDpiChange(QString& strStyle);

private:
    QString m_strQss;             //样式表
    QString m_strPaletteColor;    //调色板颜色
    bool m_hasPaletteColor;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
class CCOMMONTOOLS_EXPORT CStyleManager
{
public:
    static CStyleManager* Instance();
    static void ApplyQComboboxItemStyle(QComboBox* pCombobox);

    enum Style
    {
        S_LIGHT,
        S_DARK,
        S_DEFAULT
    };

    void SetStyle(Style style, QWidget* pWidget);

    const CStyle& GetLightStyle() const;
    const CStyle& GetDarkStyle() const;

private:
    CStyleManager();

    static CStyleManager* m_instance;
    CStyle m_lightStyle;
    CStyle m_darkStyle;
};

#endif // STYLEMANAGER_H
