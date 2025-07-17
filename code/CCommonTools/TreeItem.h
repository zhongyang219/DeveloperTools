#ifndef TREEITEM_H
#define TREEITEM_H

#include <memory>
#include <functional>
#include <QList>
#include <QString>
#include <QVariant>
#include "ccommontools_global.h"

class CTreeItem;
typedef std::shared_ptr<CTreeItem> CTreeItemPtr;
typedef QList<CTreeItemPtr> TreeItemList;

class CCOMMONTOOLS_EXPORT CTreeItem
{
public:
    CTreeItem();
    ~CTreeItem();

public:
    //获取属性
    QVariant GetProperty(const QString& strProperty) const;

    //设置属性
    void SetProperty(const QString& strProperty, QVariant value);

    //清除属性
    void ClearPorperty(const QString& strProperty);

    CTreeItem* GetParent() const;
    void SetParent(CTreeItem* pParent);

    //添加一个子节点
    void AddItem(CTreeItemPtr pItem);

    //获取子节点列表
    const TreeItemList& GetChildList();

    //查找一个节点在子节点中的索引
    int IndexOf(CTreeItem* pItem);

    //节点是否有子节点
    bool HasChildren() const;

    //获取节点级别。根节点为0，往下一级依次加1
    int GetLevel() const;

    //设置节点内部指针
    void SetInternalPointer(void* pInternalPointer);

    //获取节点内部指针
    void* GetInternalPointer() const;

    //从另一个节点对象复制所有属性
    void CopyProperties(const CTreeItem& another);

    /**
     * @brief       遍历所有子节点
     * @param[in]	func: 传递一个函数对象或lambda表达式，当遍历到一个节点时，此函数对象或lambda表达式会被调用。
     *              此函数对象或lambda表达式的第1个参数为遍历到的节点；第2个参数为当前遍历到的席位序号（从0开始）；如果需要中止遍历，则返回true，否则返回false
     * @return      已遍历的节点的个数
     */
    int IterateItems(std::function<bool(CTreeItem*, int)> func);


private:
    CTreeItem* m_pParent;       //父节点
    TreeItemList m_childList;   //子节点列表
    QMap<QString, QVariant> m_propertyMap;  //节点属性
    void* m_internalPointer = nullptr;
};

//用于管理所有节点
class CCOMMONTOOLS_EXPORT CTreeItemGroup
{
public:
    CTreeItemGroup();
    void AddItem(CTreeItemPtr pItem);           //添加一个节点
    CTreeItemPtr GetChild(int n) const;         //获取一个子节点
    int IndexOf(CTreeItem* pItem);              //查找一个节点在子节点中的索引
    const TreeItemList& GetChildList() const;   //获取子节点列表
    TreeItemList& ChildList();

    /**
     * @brief       根据属性查找一个节点
     * @param[in]	strProperty: 节点的属性
     * @param[in]	value: 属性的值
     * @return      返回值
     */
    CTreeItem* FindItem(const QString& strProperty, QVariant value);

    /**
     * @brief       遍历所有子节点
     * @param[in]	func: 传递一个函数对象或lambda表达式，当遍历到一个节点时，此函数对象或lambda表达式会被调用。
     *              此函数对象或lambda表达式的第1个参数为遍历到的节点；第2个参数为当前遍历到的席位序号（从0开始）；如果需要中止遍历，则返回true，否则返回false
     * @return      已遍历的节点的个数
     */
    int IterateItems(std::function<bool(CTreeItem*, int)> func) const;           //遍历指定节点下的所有节点
private:
    TreeItemList m_childList;   //子节点列表
};

#endif // TREEITEM_H
