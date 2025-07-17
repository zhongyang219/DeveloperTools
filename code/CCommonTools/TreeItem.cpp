#include "TreeItem.h"

CTreeItem::CTreeItem()
    : m_pParent(nullptr)
{

}

CTreeItem::~CTreeItem()
{

}

QVariant CTreeItem::GetProperty(const QString &strProperty) const
{
    return m_propertyMap.value(strProperty);
}

void CTreeItem::SetProperty(const QString &strProperty, QVariant value)
{
    m_propertyMap[strProperty] = value;
}

void CTreeItem::ClearPorperty(const QString &strProperty)
{
    m_propertyMap.remove(strProperty);
}

CTreeItem *CTreeItem::GetParent() const
{
    return m_pParent;
}

void CTreeItem::SetParent(CTreeItem *pParent)
{
    m_pParent = pParent;
}

void CTreeItem::AddItem(CTreeItemPtr pItem)
{
    m_childList.push_back(pItem);
    pItem->SetParent(this);
}

const TreeItemList &CTreeItem::GetChildList()
{
    return m_childList;
}

int CTreeItem::IndexOf(CTreeItem *pItem)
{
    for (int i = 0; i < m_childList.size(); i++)
    {
        if (m_childList[i].get() == pItem)
            return i;
    }
    return -1;
}

bool CTreeItem::HasChildren() const
{
    return !m_childList.isEmpty();
}

int CTreeItem::GetLevel() const
{
    int level = 0;
    const CTreeItem* pItem = this;
    while (true)
    {
        pItem = pItem->GetParent();
        if (pItem != nullptr)
            level++;
        else
            break;
    }
    return level;
}

void CTreeItem::SetInternalPointer(void *pInternalPointer)
{
    m_internalPointer = pInternalPointer;
}

void *CTreeItem::GetInternalPointer() const
{
    return m_internalPointer;
}

void CTreeItem::CopyProperties(const CTreeItem &another)
{
    m_propertyMap = another.m_propertyMap;
}

int CTreeItem::IterateItems(std::function<bool (CTreeItem*, int)> func)
{
    int itemCount = 0;
    bool rtn = func(this, itemCount);
    itemCount++;
    if (rtn)
        return itemCount;
    Q_FOREACH(auto& item, m_childList)
    {
        itemCount += item->IterateItems(func);
    }
    return itemCount;
}


/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

CTreeItemGroup::CTreeItemGroup()
{

}

void CTreeItemGroup::AddItem(CTreeItemPtr pItem)
{
    m_childList.push_back(pItem);
}

CTreeItemPtr CTreeItemGroup::GetChild(int n) const
{
    if (n >= 0 && n < m_childList.size())
        return m_childList[n];
    else
        return nullptr;
}

int CTreeItemGroup::IndexOf(CTreeItem *pItem)
{
    for (int i = 0; i < m_childList.size(); i++)
    {
        if (m_childList[i].get() == pItem)
            return i;
    }
    return -1;
}

const TreeItemList &CTreeItemGroup::GetChildList() const
{
    return m_childList;
}

TreeItemList &CTreeItemGroup::ChildList()
{
    return m_childList;
}

CTreeItem *CTreeItemGroup::FindItem(const QString &strProperty, QVariant value)
{
    CTreeItem* pFindItem = nullptr;
    IterateItems([&](CTreeItem* pItem, int) ->bool
    {
        if (pItem != nullptr && pItem->GetProperty(strProperty) == value)
        {
            pFindItem = pItem;
            return true;
        }
        return false;
    });
    return pFindItem;
}


int CTreeItemGroup::IterateItems(std::function<bool(CTreeItem*, int)> func) const
{
    int itemCount = 0;
    Q_FOREACH(auto& item, m_childList)
    {
        itemCount += item->IterateItems(func);
    }
    return itemCount;
}
