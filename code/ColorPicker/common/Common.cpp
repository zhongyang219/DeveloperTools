#include "common/Common.h"
#include <QtGlobal>
#include <QMap>
#include <QTreeWidgetItem>
#include "ColorConvert.h"

#ifdef Q_OS_WIN
#include <dwmapi.h>
#pragma comment(lib,"Dwmapi.lib")
#endif

QColor Common::GetSystemThemeColor()
{
#ifdef Q_OS_WIN
    DWORD crColorization;
    BOOL fOpaqueBlend;
    QColor theme_color{};
    HRESULT result = DwmGetColorizationColor(&crColorization, &fOpaqueBlend);
    if (result == S_OK)
    {
        BYTE r, g, b;
        r = (crColorization >> 16) % 256;
        g = (crColorization >> 8) % 256;
        b = crColorization % 256;
        theme_color = QColor(r, g, b);
    }
    return theme_color;

#endif
    return QColor();
}

QList<Common::ColorItem> Common::GetWindowsSysColorList()
{
#ifdef Q_OS_WIN
    const QMap<int, QString> sys_color_index {
        { COLOR_3DDKSHADOW, "COLOR_3DDKSHADOW"},
        { COLOR_3DFACE, "COLOR_3DFACE" },
        { COLOR_3DHIGHLIGHT, "COLOR_3DHIGHLIGHT" },
        { COLOR_3DHILIGHT, "COLOR_3DHILIGHT" },
        { COLOR_3DLIGHT, "COLOR_3DLIGHT" },
        { COLOR_3DSHADOW, "COLOR_3DSHADOW" },
        { COLOR_ACTIVEBORDER, "COLOR_ACTIVEBORDER" },
        { COLOR_ACTIVECAPTION, "COLOR_ACTIVECAPTION" },
        { COLOR_APPWORKSPACE, "COLOR_APPWORKSPACE" },
        { COLOR_BACKGROUND, "COLOR_BACKGROUND" },
        { COLOR_BTNFACE, "COLOR_BTNFACE" },
        { COLOR_BTNHIGHLIGHT, "COLOR_BTNHIGHLIGHT" },
        { COLOR_BTNHILIGHT, "COLOR_BTNHILIGHT" },
        { COLOR_BTNSHADOW, "COLOR_BTNSHADOW" },
        { COLOR_BTNTEXT, "COLOR_BTNTEXT" },
        { COLOR_CAPTIONTEXT, "COLOR_CAPTIONTEXT" },
        { COLOR_DESKTOP, "COLOR_DESKTOP" },
        { COLOR_GRADIENTACTIVECAPTION, "COLOR_GRADIENTACTIVECAPTION" },
        { COLOR_GRADIENTINACTIVECAPTION, "COLOR_GRADIENTINACTIVECAPTION" },
        { COLOR_GRAYTEXT, "COLOR_GRAYTEXT" },
        { COLOR_HIGHLIGHT, "COLOR_HIGHLIGHT" },
        { COLOR_HIGHLIGHTTEXT, "COLOR_HIGHLIGHTTEXT" },
        { COLOR_HOTLIGHT, "COLOR_HOTLIGHT" },
        { COLOR_INACTIVEBORDER, "COLOR_INACTIVEBORDER" },
        { COLOR_INACTIVECAPTION, "COLOR_INACTIVECAPTION" },
        { COLOR_INACTIVECAPTIONTEXT, "COLOR_INACTIVECAPTIONTEXT" },
        { COLOR_INFOBK, "COLOR_INFOBK" },
        { COLOR_INFOTEXT, "COLOR_INFOTEXT" },
        { COLOR_MENU, "COLOR_MENU" },
        { COLOR_MENUHILIGHT, "COLOR_MENUHILIGHT" },
        { COLOR_MENUBAR, "COLOR_MENUBAR" },
        { COLOR_MENUTEXT, "COLOR_MENUTEXT" },
        { COLOR_SCROLLBAR, "COLOR_SCROLLBAR" },
        { COLOR_WINDOW, "COLOR_WINDOW" },
        { COLOR_WINDOWFRAME, "COLOR_WINDOWFRAME" },
        { COLOR_WINDOWTEXT, "COLOR_WINDOWTEXT" }
    };

    QList<ColorItem> color_list;
    for (auto iter = sys_color_index.begin(); iter != sys_color_index.end(); ++iter)
    {
        ColorItem color_item;
        color_item.name = iter.value();
        auto colorref = GetSysColor(iter.key());
        color_item.color = ColorConvert::ColorrefToColor(colorref);
        color_list.push_back(color_item);
    }
    return color_list;

#else
    return QList<ColorItem>();
#endif
}

void Common::RemoveChildTreeItemByName(QTreeWidgetItem* parent, const QString& name)
{
    if (parent == nullptr)
        return;
    QList<QTreeWidgetItem*> child_to_removed;
    for (int i = 0; i < parent->childCount(); i++)
    {
        auto* child = parent->child(i);
        if (name == child->text(0))
            child_to_removed.append(child);
    }

    for (auto* child : child_to_removed)
    {
        parent->removeChild(child);
    }
}
