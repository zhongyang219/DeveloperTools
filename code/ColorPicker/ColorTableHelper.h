#pragma once
#include <QTreeWidget>
class ColorTableHelper
{
public:
    ColorTableHelper(QTreeWidget* tree_widget);

    enum ItemRole
    {
        ColorRole = Qt::UserRole,
        ItemTypeRole = Qt::UserRole + 1
    };

    enum ItemType
    {
        GroupType,
        ColorType
    };

    void LoadFromXml(const QString& strXml);
    QString SaveToXml() const;

    void AddGroup();
    void AddCurColor(const QColor& color);
    void DeleteSel();

protected:
    QTreeWidgetItem* CreateGroupItem(const QString& name);
    QTreeWidgetItem* CreateColorItem(const QString& name, const QString& str_color, QTreeWidgetItem* parent_item);

private:
    QTreeWidget* m_tree_widget;
};
