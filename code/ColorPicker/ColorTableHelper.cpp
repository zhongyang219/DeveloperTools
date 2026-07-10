#include "ColorTableHelper.h"
#include <QDomDocument>
#include <QPainter>
#include <QStyledItemDelegate>
#include <QSet>
#include "define.h"
#include "common/ColorConvert.h"

///////////////////////////////////////////////////////////////////////////////////////////////
enum Column
{
    COL_NAME,
    COL_RGB,
    COL_HEX
};

static QIcon CreateSingleColorIcon(const QColor& color)
{
    const int icon_size = DPI(16);
    QPixmap pixmap(icon_size, icon_size);
    pixmap.fill(color);
    QPainter painter(&pixmap);
    //绘制边框
    QRect icon_rect(0, 0, icon_size, icon_size);
    icon_rect.adjust(0, 0, -1, -1);
    painter.setPen(QColor(128, 128, 128));
    painter.drawRect(icon_rect);
    return QIcon(pixmap);
}


class ColorItemDelegate : public QStyledItemDelegate
{
public:
    ColorItemDelegate(QObject* parent = nullptr)
        : QStyledItemDelegate(parent)
    {}

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override
    {
        //设置仅名称列可编辑
        if (index.column() == COL_NAME)
        {
            QWidget* widget = QStyledItemDelegate::createEditor(parent, option, index);
            return widget;
        }
        return nullptr;
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////
ColorTableHelper::ColorTableHelper(QTreeWidget* tree_widget)
    : m_tree_widget(tree_widget)
{
    m_tree_widget->setColumnWidth(COL_NAME, DPI(200));
    m_tree_widget->setItemDelegate(new ColorItemDelegate(m_tree_widget));
}

void ColorTableHelper::LoadFromXml(const QString& strXml)
{
    m_tree_widget->clear();

    QDomDocument doc;
    if (doc.setContent(strXml))
    {
        //加载分组
        auto groupNodes = doc.documentElement().childNodes();
        for (int i = 0; i < groupNodes.count(); i++)
        {
            auto groupNode = groupNodes.at(i).toElement();
            QString strName = groupNode.attribute("name");
            QTreeWidgetItem* group_item = CreateGroupItem(strName);
            m_tree_widget->addTopLevelItem(group_item);
            //加载颜色
            auto colorNodes = groupNode.childNodes();
            for (int j = 0; j < colorNodes.count(); j++)
            {
                auto colorNode = colorNodes.at(j).toElement();
                QString strColorName = colorNode.attribute("name");
                QString strColor = colorNode.attribute("color");
                QTreeWidgetItem* subItem = CreateColorItem(strColorName, strColor, group_item);
                group_item->addChild(subItem);
            }
        }
    }

    if (m_tree_widget->topLevelItemCount() == 0)
    {
        QTreeWidgetItem* group_item = CreateGroupItem(u8"默认");
        m_tree_widget->addTopLevelItem(group_item);
    }
    m_tree_widget->expandAll();
}

QString ColorTableHelper::SaveToXml() const
{
    QDomDocument doc;
    auto root = doc.createElement("color_table");
    doc.appendChild(root);
    for (int i = 0; i < m_tree_widget->topLevelItemCount(); i++)
    {
        auto* group_item = m_tree_widget->topLevelItem(i);
        QDomElement group_node = doc.createElement("group");
        group_node.setAttribute("name", group_item->text(COL_NAME));
        root.appendChild(group_node);

        for (int j = 0; j < group_item->childCount(); j++)
        {
            auto* color_item = group_item->child(j);
            QDomElement color_node = doc.createElement("color");
            color_node.setAttribute("name", color_item->text(COL_NAME));
            color_node.setAttribute("color", color_item->data(0, ColorRole).toString());
            group_node.appendChild(color_node);
        }
    }
    return doc.toString();
}

void ColorTableHelper::AddGroup()
{
    auto* cur_item = m_tree_widget->currentItem();
    if (cur_item == nullptr)
        return;
    int index = m_tree_widget->indexOfTopLevelItem(cur_item);
    QTreeWidgetItem* new_item = CreateGroupItem(u8"新建颜色组");
    if (index >= 0 && index < m_tree_widget->topLevelItemCount())
        m_tree_widget->insertTopLevelItem(index, new_item);
    else
        m_tree_widget->addTopLevelItem(new_item);
    m_tree_widget->editItem(new_item);

}

void ColorTableHelper::AddCurColor(const QColor& color)
{
    auto* cur_item = m_tree_widget->currentItem();
    if (cur_item == nullptr)
    {
        if (m_tree_widget->topLevelItemCount() > 0)
            cur_item = m_tree_widget->topLevelItem(0);
    }
    if (cur_item == nullptr)
        return;

    ItemType item_type = static_cast<ItemType>(cur_item->data(0, ItemTypeRole).toInt());
    QTreeWidgetItem* color_item = nullptr;
    QString color_name = u8"颜色 " + color.name().toUpper();
    if (item_type == GroupType)
    {
        color_item = CreateColorItem(color_name, color.name(), cur_item);
        cur_item->addChild(color_item);
    }
    else
    {
        QTreeWidgetItem* parent_item = cur_item->parent();
        color_item = CreateColorItem(color_name, color.name(), nullptr);
        int index = parent_item->indexOfChild(cur_item) + 1;
        if (index >= 0 && index < parent_item->childCount())
            parent_item->insertChild(index, color_item);
        else
            parent_item->addChild(color_item);
    }
}

void ColorTableHelper::DeleteSel()
{
    QList<QTreeWidgetItem*> selectedItems = m_tree_widget->selectedItems();
    if (selectedItems.isEmpty())
        return;

    QSet<QTreeWidgetItem*> selectedSet(selectedItems.begin(), selectedItems.end());

    QList<QTreeWidgetItem*> selectedParents;
    QList<QTreeWidgetItem*> selectedChildren;
    QSet<QTreeWidgetItem*> parentsToDelete; // 记录最终需要删除的父节点

    //分类节点
    for (QTreeWidgetItem* item : selectedItems)
    {
        if (item->parent() == nullptr)
        {
            // 当前是父节点（第一级）
            selectedParents.append(item);

            int childCount = item->childCount();
            if (childCount == 0)
            {
                // 父节点没有子节点，直接删除
                parentsToDelete.insert(item);
            }
            else
            {
                // 父节点有子节点，且当所有子节点都被选中或都未被选中时，要删除父节点
                bool hasChildSelected = false;
                bool hasChildUnSelected = false;
                for (int i = 0; i < childCount; ++i)
                {
                    if (selectedSet.contains(item->child(i)))
                        hasChildSelected = true;
                    else
                        hasChildUnSelected = true;
                }
                // 子节点都被选中或都未被选中时，父节点才加入删除列表
                if (!hasChildSelected || !hasChildUnSelected)
                    parentsToDelete.insert(item);
            }
        }
        else
        {
            // 当前是子节点（第二级）
            selectedChildren.append(item);
        }
    }

    //删除子节点
    for (QTreeWidgetItem* item : selectedChildren)
    {
        delete item;
    }

    //删除父节点
    for (QTreeWidgetItem* item : parentsToDelete)
    {
        int index = m_tree_widget->indexOfTopLevelItem(item);
        if (index != -1)
        {
            delete m_tree_widget->takeTopLevelItem(index);
        }
    }
}

QTreeWidgetItem* ColorTableHelper::GetOrAddGroup(const QString& group_name)
{
    //查找组
    for (int i = 0; i < m_tree_widget->topLevelItemCount(); i++)
    {
        QTreeWidgetItem* cur_group_item = m_tree_widget->topLevelItem(i);
        QString cur_group_name = cur_group_item->text(COL_NAME);
        if (cur_group_name == group_name)
            return cur_group_item;
    }
    //添加组
    QTreeWidgetItem* group_item = CreateGroupItem(group_name);
    m_tree_widget->addTopLevelItem(group_item);
    return group_item;
}

QTreeWidgetItem* ColorTableHelper::CreateGroupItem(const QString& name)
{
    QTreeWidgetItem* group_item = new QTreeWidgetItem(m_tree_widget);
    group_item->setText(COL_NAME, name);
    group_item->setFlags(group_item->flags() | Qt::ItemIsEditable);
    group_item->setData(0, ItemTypeRole, GroupType);
    group_item->setIcon(COL_NAME, QIcon(":res/folder.png"));
    return group_item;
}

QTreeWidgetItem* ColorTableHelper::CreateColorItem(const QString& name, const QString& str_color, QTreeWidgetItem* parent_item)
{
    QTreeWidgetItem* color_item = new QTreeWidgetItem(parent_item);
    color_item->setText(COL_NAME, name);
    color_item->setData(0, ColorRole, str_color);
    color_item->setData(0, ItemTypeRole, ColorType);
    color_item->setFlags(color_item->flags() | Qt::ItemIsEditable);
    QColor color(str_color);
    color_item->setIcon(COL_NAME, CreateSingleColorIcon(color));
    color_item->setText(COL_RGB, ColorConvert::GetRgbString(color));
    color_item->setText(COL_HEX, color.name());
    return color_item;
}
