#include "TableDataModel.h"
#include "TableDataModel.h"

CTableDataModel::CTableDataModel(int iColumeCount)
    : m_iColumeCount(iColumeCount)
{
}

CTableDataModel::~CTableDataModel()
{
}

void CTableDataModel::DeleteItem(int row)
{
    //从模型中删除
    removeRow(row);

}

void CTableDataModel::DeleteAllItems()
{
    removeRows(0, rowCount());
}

void CTableDataModel::ModifyItem(int row, ColumeItemMap &itemMap, bool bModifyDatabase)
{
    ColumeItemMap::iterator iter;
    for (iter = itemMap.begin(); iter != itemMap.end(); ++iter)
    {
        QModelIndex index = this->index(row, iter.key());
        setData(index, iter.value());
    }

}

bool CTableDataModel::AddItem(const ColumeItemMap &itemMap)
{
    //向模型中添加
    QList<QStandardItem*> itemList;
    for (int i = 0; i < m_iColumeCount; i++)
    {
        itemList.append(new QStandardItem(itemMap[i]));
    }
    appendRow(itemList);
    return true;
}

void CTableDataModel::GetItem(int row, ColumeItemMap &itemMap)
{
    for (int col = 0; col < m_iColumeCount; col++)
    {
        itemMap[col] = GetItemText(row, col);
    }
}

QString CTableDataModel::GetItemText(int row, int col) const
{
    QStandardItem* pItem = this->item(row, col);
    QString strText;
    if (pItem != nullptr)
        strText = pItem->text();
    return strText;
}

void CTableDataModel::SetItemText(int row, int col, const QString &str)
{
    //修改模型数据
    QModelIndex index = this->index(row, col);
    setData(index, str);
}

int CTableDataModel::ItemCount() const
{
    return rowCount();
}

int CTableDataModel::FindItem(const QString &str, int col)
{
    int iItemCount = ItemCount();
    for (int i = 0; i < iItemCount; i++)
    {
        if (GetItemText(i, col) == str)
            return i;
    }
    return false;
}

void CTableDataModel::QuickSearch(const QString &strText, QTableView *pTableView, int iColMax)
{
    int iItemCount = ItemCount();
    //遍历列表中的每一行，判断该行是否匹配要搜索的字符串
    for (int i = 0; i < iItemCount; i++)
    {
        bool bMatch = false;     //当前行是否匹配
        if (strText.isEmpty())
        {
            bMatch = true;  //如果查找字符串为空，则认为匹配
        }
        else
        {
            //遍历当前行的每一列
            for (int j = 0; j < iColMax; j++)
            {
                QString strCellText = GetItemText(i, j);    //单元格的文本
                if (strCellText.contains(strText, Qt::CaseInsensitive)) //该行有一个单元格匹配，则说明该行匹配
                {
                    bMatch = true;
                    break;
                }
            }
        }
        //如果不匹配要搜索的字符串，则隐藏此行
        pTableView->setRowHidden(i, !bMatch);
    }
}

