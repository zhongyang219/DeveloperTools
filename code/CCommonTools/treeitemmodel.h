#ifndef CTREEITEMMODEL_H
#define CTREEITEMMODEL_H

#include "TreeItem.h"
#include <QAbstractItemModel>

class CCOMMONTOOLS_EXPORT CTreeItemModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit CTreeItemModel(QObject *parent = nullptr);

    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    bool IsModified() const;
    void SetModified(bool modified = true);

    void SetReadOnly(bool readOnly);

    CTreeItemGroup& GetTreeData();
    const CTreeItemGroup& GetTreeData() const;

    //根据QModelIndex获取节点对象
    CTreeItem* getItem(const QModelIndex &index) const;

    /**
     * @brief       遍历所有子节点
     * @param[in]	func: 传递一个函数对象或lambda表达式，当遍历到一个节点时，此函数对象或lambda表达式会被调用。
     *              如果需要中止遍历，则返回true，否则返回false
     * @param[in]   parent 遍历的父节点
     * @param[in]   column 遍历的Index的列
     */
    void IterateIndexs(std::function<bool(QModelIndex)> func, QModelIndex parent = QModelIndex(), int column = 0) const;           //遍历指定节点下的所有节点

    //判断一个单独的节点是否已勾选（需要在派生类中重写）
    virtual bool isSingleItemChecked(CTreeItem* pItem) const;

    //设置单独一个节点的勾选状态（需要在派生类中重写）
    virtual void setSingleItemChecked(CTreeItem* pItem, bool checked);


    //判断一个节点是否已勾选
    int isItemChecked(CTreeItem* pItem) const;

    /*
     * 函数说明: 设置一个节点的勾选状态
     * 输入参数:
     *   pItem: 节点对象的指针
     *   checked: 是否勾选
     */
    void setItemChecked(CTreeItem* pItem, bool checked);

protected:
    CTreeItemGroup m_treeData;
    bool m_modified = false;
    bool m_readOnly = false;
};

#endif // CTREEITEMMODEL_H
