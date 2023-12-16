#ifndef CTABLEITEMMODEL_H
#define CTABLEITEMMODEL_H

#include <QTableView>
#include <QAbstractTableModel>
#include "ccommontools_global.h"

typedef QMap<int, QVariant> ColumeItemMap;
class TableItemModelPrivate;

//提供一个表格的数据模型
class CCOMMONTOOLS_EXPORT CTableItemModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit CTableItemModel(int columeCount, QObject *parent = nullptr);
    virtual ~CTableItemModel();

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    /**
     * @brief       设置是否启用鼠标提示
     * @param[in]	enable 是否启用鼠标提示
     */
    void SetMouseTooltipEnable(bool enable);

    //清空表格数据
    void ClearData();

    /**
     * @brief       删除一行数据
     * @param[in]	row: 要删除的行号
     */
    void DeleteItem(int row);

    /**
     * @brief       修改指定行的数据
     * @param[in]	row: 要修改的行号
     * @param[in]	itemMap: 每一列的列号和其对应的数据
     * @return      返回值
     */
    void ModifyItem(int row, const ColumeItemMap& itemMap);

    /**
     * @brief       向表格的末尾追加一行数据
     * @param[in]	itemMap: 每一列的列号和其对应的数据
     */
    void AddItem(const ColumeItemMap& itemMap);

    /**
     * @brief       为表格中的一行设置用户自定义数据
     * @param[in]	row: 要修改的行号
     * @param[in]	key: 用户数据的key
     * @param[in]	value: 用户数据的值
     * @return      是否成功。如果行号为无效值则会返回false。
     */
    bool SetItemData(int row, const QString& key, const QVariant& value);

    /**
     * @brief       获取表格中一行的用户数据
     * @param[in]	row: 要修改的行号
     * @param[in]	key: 用户数据的key
     * @return      用户数据的值
     */
    QVariant ItemData(int row, const QString& key);

    /**
     * @brief       获取指定行的数据
     * @param[in]	row: 行号
     * @param[in]	itemMap: 每一列的列号和其对应的数据
     * @return      返回值
     */
    void GetItem(int row, ColumeItemMap& itemMap);

    /**
     * @brief       获取指定单元格的数据
     * @param[in]	row: 行号
     * @param[out]	col: 列号
     * @return      单元格的文本
     */
    QString GetItemText(int row, int col) const;

    /**
     * @brief       设置指定单元格的数据
     * @param[in]	row: 行号
     * @param[out]	col: 列号
     * @param[out]	str: 要设置的字符串
     */
    void SetItemText(int row, int col, const QString& str);

    /**
     * @brief       获取表格的行数
     * @return      表格的行数
     */
    int ItemCount() const;

    /**
     * @brief       查找一个数据所在行
     * @param[in]	str: 要查找的名称
     * @param[in]	col: 要查找的列
     * @return      找到的行号。未找到返回负数
     */
    int FindItem(const QString& str, int col);

    /**
     * @brief       对表格进行快速搜索，将不匹配的项目隐藏
     * @param[in]	strText: 搜索的文文本
     * @param[in]	pTableView: 表格控件
     */
    void QuickSearch(const QString& text, QTableView* pTableView);

protected:
    TableItemModelPrivate* d;
};

#endif // CTABLEITEMMODEL_H
