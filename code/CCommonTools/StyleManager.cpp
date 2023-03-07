#include <QFile>
#include <QPalette>
#include <QStyledItemDelegate>
#include "StyleManager.h"
#include "StringHelper.h"
#include "define.h"
#include <QFile>

CStyle::CStyle(const QString& strPath, bool hasPaletteColor)
    : m_hasPaletteColor(hasPaletteColor)
{
    //载入样式表文件
    QFile file(strPath);

    if (file.open(QFile::ReadOnly))
    {
        m_strQss = QString::fromUtf8(file.readAll());
        StyleDpiChange(m_strQss);
        if (hasPaletteColor)
            m_strPaletteColor = m_strQss.mid(20, 7);
        file.close();
    }
}

void CStyle::ApplyStyleSheet(QWidget* pWidget) const
{
    if (pWidget != nullptr)
    {
        //设置样式表
        if (m_hasPaletteColor)
            pWidget->setPalette(QPalette(m_strPaletteColor));
        pWidget->setStyleSheet(m_strQss);
    }
}



static int FindPxStart(const QString& str, int index)
{
    for (int i = index; i >= 0; i--)
    {
        if (!StringHelper::IsNumber(str[i].toLatin1()))
            return i + 1;
    }
    return -1;
}

void CStyle::StyleDpiChange(QString& strStyle)
{
    int index = 0;
    while (true)
    {
        //查找“px”
        index = strStyle.indexOf("px", index + 1);
        if (index < 0)
            break;
        //查找像素的起始位置
        int indexStart = FindPxStart(strStyle, index - 1);
        //获取像素值
        QString strPixel = strStyle.mid(indexStart, index - indexStart);
        int pixel = strPixel.toInt();
        //判断是否为radius属性
        bool isRadius = false;
        int indexLineStart = StringHelper::QStringFindLastOf(strStyle, "\r\n", indexStart);
        if (indexLineStart >= 0)
        {
            QString strAttr = strStyle.mid(indexLineStart + 1, indexStart - indexLineStart - 1);
            isRadius = strAttr.contains("radius");
        }
        //对像素值进行DPI转换
        int newPixel = DPI(pixel);
        //如果是radius属性，则放大后不超过8像素
        if (isRadius)
        {
            int radiusMax = (std::max)(pixel, DPI(8));
            if (newPixel > radiusMax)
                newPixel = radiusMax;
        }
        //更新样式表
        QString strNewPixel = QString::number(newPixel);
        strStyle.replace(indexStart, strPixel.size(), strNewPixel);
        //更新查找位置
        index = indexStart + strNewPixel.size() + 2;
    }
//#ifdef QT_DEBUG
//    //Debug模式下将更改后的样式写入文件
//    QFile file("./styleChange.css");
//    if (file.open(QIODevice::WriteOnly))
//    {
//        file.write(strStyle.toUtf8());
//        file.close();
//    }
//#endif
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
CStyleManager* CStyleManager::m_instance;

CStyleManager *CStyleManager::Instance()
{
    if (m_instance == nullptr)
        m_instance = new CStyleManager();
    return m_instance;
}

void CStyleManager::ApplyQComboboxItemStyle(QComboBox *pCombobox)
{
    QStyledItemDelegate* itemDelegate = new QStyledItemDelegate();
    pCombobox->setItemDelegate(itemDelegate);
}

void CStyleManager::SetStyle(Style style, QWidget* pWidget)
{
    switch (style)
    {
    case CStyleManager::S_LIGHT:
        m_lightStyle.ApplyStyleSheet(pWidget);
        break;
    case CStyleManager::S_DARK:
        m_darkStyle.ApplyStyleSheet(pWidget);
        break;
    default:
        break;
    }
}

const CStyle& CStyleManager::GetLightStyle() const
{
    return m_lightStyle;
}

const CStyle& CStyleManager::GetDarkStyle() const
{
    return m_darkStyle;
}

CStyleManager::CStyleManager()
    : m_lightStyle(":/qss/flatwhite.css"),
    m_darkStyle(":/qss/flatblack.css")
{
}

