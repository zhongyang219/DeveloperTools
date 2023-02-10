#ifndef CTABLEDATAEDITOR_H
#define CTABLEDATAEDITOR_H
#include <QMap>
#include  <QStandardItemModel>
#include "ccommontools_global.h"
#include <QTableView>

class CCOMMONTOOLS_EXPORT CTableDataModel : public QStandardItemModel
{
public:
    CTableDataModel(int iColumeCount);
    virtual ~CTableDataModel();

    typedef QMap<int, QString> ColumeItemMap;

    //删除一行数据
    //输入参数：
    //  row: 要删除的行号
    void DeleteItem(int row);

    void DeleteAllItems();

    //修改指定行的数据
    //输入参数：
    //  row: 要修改的行号
    //  itemMap: 每一列的列号和其对应的数据
    void ModifyItem(int row, ColumeItemMap& itemMap, bool bModifyDatabase = true);

    //追加一行数据
    //输入参数：
    //  itemMap: 每一列的列号和其对应的数据
    virtual bool AddItem(const ColumeItemMap& itemMap);

    //获取指定行的数据
    //输入参数：
    //  row: 要修改的行号
    //输出参数：
    //  itemMap: 接收每一列的列号和其对应的数据
    void GetItem(int row, ColumeItemMap& itemMap);

    //获取指定单元格的数据
    //输入参数：
    //  row: 行号
    //  col: 列号
    QString GetItemText(int row, int col) const;

    //获取指定单元格的数据
    //输入参数：
    //  row: 行号
    //  col: 列号
    //  str: 要设置的字符串
    virtual void SetItemText(int row, int col, const QString& str);

    //获取表格的行数
    //返回值：表格的行数
    int ItemCount() const;

    /*
     * 函数说明: 查找一个数据所在行
     * 输入参数:
     *   str: 要查找的名称
     *   col: 要查找的列
     * 返回值:
     *   结果所在行
     */
    int FindItem(const QString& str, int col);

protected:

    /*
     * 函数说明: 对表格进行快速搜索，将不匹配的项目隐藏
     * 输入参数:
     *   strText: 搜索的文文本
     *   pTableView: 表格控件
     *   iColMax: 表格列数
     */
    virtual void QuickSearch(const QString& strText, QTableView* pTableView, int iColMax);

protected:
    int m_iColumeCount;                 //表格的列数

};

#endif // CTABLEDATAEDITOR_H
