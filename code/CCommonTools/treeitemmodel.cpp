#include "treeitemmodel.h"

CTreeItemModel::CTreeItemModel(QObject *parent)
    : QAbstractItemModel(parent)
{
}

QModelIndex CTreeItemModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!parent.isValid())
    {
        if (row >= 0 && row < m_treeData.GetChildList().size())
            return createIndex(row, column, m_treeData.GetChild(row).get());
    }
    else
    {
        CTreeItem* pItem = getItem(parent);
        if (pItem != nullptr)
        {
            if (row >= 0 && row < pItem->GetChildList().size())
            {
                return createIndex(row, column, pItem->GetChildList()[row].get());
            }
        }
    }

    return QModelIndex();
}

QModelIndex CTreeItemModel::parent(const QModelIndex &index) const
{
    CTreeItem* pItem = getItem(index);
    if (pItem != nullptr)
    {
        CTreeItem* pParent = pItem->GetParent();
        if (pParent != nullptr)
        {
            return createIndex(pParent->IndexOf(pItem), 0, pParent);
        }
    }
    return QModelIndex();
}

int CTreeItemModel::rowCount(const QModelIndex &parent) const
{
    CTreeItem* pItem = getItem(parent);
    if (pItem == nullptr)
    {
        // 根节点下的数据行数
        return m_treeData.GetChildList().size();
    }
    else
    {
        // 节点下的数据行数
        return pItem->GetChildList().size();
    }
}

bool CTreeItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && data(index, role) != value)
    {
        m_modified = true;
        return true;
    }

    return false;
}

bool CTreeItemModel::IsModified() const
{
    return m_modified;
}

void CTreeItemModel::SetModified(bool modified)
{
    m_modified = modified;
}

void CTreeItemModel::SetReadOnly(bool readOnly)
{
    m_readOnly = readOnly;
}

CTreeItemGroup &CTreeItemModel::GetTreeData()
{
    return m_treeData;
}

const CTreeItemGroup &CTreeItemModel::GetTreeData() const
{
    return m_treeData;
}

CTreeItem *CTreeItemModel::getItem(const QModelIndex &index) const
{
    if (index.isValid())
    {
        CTreeItem* pItem = static_cast<CTreeItem*>(index.internalPointer());
        if (pItem != nullptr)
            return pItem;
    }
    return nullptr;
}

void CTreeItemModel::IterateIndexs(std::function<bool (QModelIndex)> func, QModelIndex parent, int column) const
{
    if (func(parent))       //func返回true，则终止遍历
        return;
    int childrenCount = rowCount(parent);
    for (int i = 0; i < childrenCount; i++)
    {
        //获取parent的每个子index
        QModelIndex childIndex = index(i, column, parent);
        if (childIndex.isValid())
        {
            //递归调用此函数
            IterateIndexs(func, childIndex, column);
        }
    }
}

bool CTreeItemModel::isSingleItemChecked(CTreeItem *pItem) const
{
    Q_UNUSED(pItem)
    return false;
}

void CTreeItemModel::setSingleItemChecked(CTreeItem *pItem, bool checked)
{
    Q_UNUSED(pItem)
    Q_UNUSED(checked)
}

int CTreeItemModel::isItemChecked(CTreeItem *pItem) const
{
    if (pItem != nullptr)
    {
        if (!pItem->HasChildren()) //子节点返回该节点的选中状态
        {
            return (isSingleItemChecked(pItem) ? Qt::Checked : Qt::Unchecked);
        }
        else    //父节点当子节点全部选中时为选中状态
        {
            bool hasChecked = false;
            bool hasUnchecked = false;
            for (const auto& pSubItem : pItem->GetChildList())
            {
                if (isItemChecked(pSubItem.get()))
                    hasChecked = true;
                else
                    hasUnchecked = true;
            }
            if (hasChecked && !hasUnchecked)        //如果子节点只有选中而没有未选中的节点，则为选中状态
                return Qt::Checked;
            else if (!hasChecked && hasUnchecked)   //如果子节点没有选中只有未选中的节点，则为未选中状态
                return Qt::Unchecked;
            else
                return Qt::PartiallyChecked;        //其他情况则为部分选中状态
        }
    }
    return Qt::Unchecked;
}

void CTreeItemModel::setItemChecked(CTreeItem *pItem, bool checked)
{
    if (pItem != nullptr)
    {
        setSingleItemChecked(pItem, checked);
        for (const auto& pSubItem : pItem->GetChildList())
        {
            setItemChecked(pSubItem.get(), checked);
        }
    }
}
