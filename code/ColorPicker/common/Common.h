#pragma once
#include <QColor>
class QTreeWidgetItem;
class Common
{
public:
    //获取当前系统主题色
    static QColor GetSystemThemeColor();

    struct ColorItem
    {
        QString name;
        QColor color;
    };

    //获取Windows下GetSysColor函数获取到的颜色值
    static QList<ColorItem> GetWindowsSysColorList();

    static void RemoveChildTreeItemByName(QTreeWidgetItem* parent, const QString& name);
};
