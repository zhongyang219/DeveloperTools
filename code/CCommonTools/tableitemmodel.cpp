#include "tableitemmodel.h"

//表格中一行的数据
struct TableItemModelRowInfo
{
    ColumeItemMap columeMap;
    QMap<QString, QVariant> extendData;
    QMap<int, bool> itemEditable;
};

//类的私有数据
class TableItemModelPrivate
{
public:
    QList<TableItemModelRowInfo> m_data;
    int m_columeCount;
    QMap<int, QString> m_headerData;
    bool m_tooltipEnable = false;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
CTableItemModel::CTableItemModel(int columeCount, QObject *parent)
    : QAbstractTableModel(parent),
      d(new TableItemModelPrivate)
{
    d->m_columeCount = columeCount;
}

CTableItemModel::~CTableItemModel()
{
    delete d;
}

QVariant CTableItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
            return d->m_headerData.value(section);
        else
            return section + 1;
    }
    return QVariant();
}

bool CTableItemModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (orientation == Qt::Horizontal && value != headerData(section, orientation, role))
    {
        d->m_headerData[section] = value.toString();
        emit headerDataChanged(orientation, section, section);
        return true;
    }
    return false;
}

int CTableItemModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return d->m_data.size();
}

int CTableItemModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return d->m_columeCount;
}

QVariant CTableItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole || (d->m_tooltipEnable && role == Qt::ToolTipRole))
    {
        if (index.row() >= 0 && index.row() < d->m_data.size())
        {
            const ColumeItemMap& colItemMap = d->m_data.value(index.row()).columeMap;
            return colItemMap.value(index.column());
        }
    }

    return QVariant();
}

bool CTableItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value)
    {
        if (index.row() >= 0 && index.row() < d->m_data.size())
        {
            ColumeItemMap& colItemMap = d->m_data[index.row()].columeMap;
            colItemMap[index.column()] = value;
            emit dataChanged(index, index, QVector<int>() << role);
            return true;
        }
    }
    return false;
}

void CTableItemModel::SetMouseTooltipEnable(bool enable)
{
    d->m_tooltipEnable = enable;
}

void CTableItemModel::ClearData()
{
    if (!d->m_data.isEmpty())
    {
        d->m_data.clear();
        layoutChanged();
    }
}

void CTableItemModel::DeleteItem(int row)
{
    if (row >= 0 && row < d->m_data.size())
    {
        d->m_data.removeAt(row);
        layoutChanged();
    }
}

void CTableItemModel::ModifyItem(int row, const ColumeItemMap &itemMap)
{
    if (row >= 0 && row < d->m_data.size())
    {
        for (auto iter = itemMap.begin(); iter != itemMap.end(); ++iter)
            d->m_data[row].columeMap[iter.key()] = iter.value();
        layoutChanged();
    }
}

void CTableItemModel::AddItem(const ColumeItemMap &itemMap)
{
    TableItemModelRowInfo rowInfo;
    rowInfo.columeMap = itemMap;
    d->m_data.append(rowInfo);
    layoutChanged();
}

bool CTableItemModel::SetItemData(int row, const QString &key, const QVariant &value)
{
    if (row >= 0 && row < d->m_data.size())
    {
        auto& extendData = d->m_data[row].extendData;
        extendData[key] = value;
        return true;
    }
    return false;
}

QVariant CTableItemModel::ItemData(int row, const QString &key)
{
    if (row >= 0 && row < d->m_data.size())
    {
        const auto& extendData = d->m_data[row].extendData;
        return extendData.value(key);
    }
    return QVariant();
}

void CTableItemModel::GetItem(int row, ColumeItemMap &itemMap)
{
    if (row >= 0 && row < d->m_data.size())
        itemMap = d->m_data[row].columeMap;
}

QString CTableItemModel::GetItemText(int row, int col) const
{
    if (row >= 0 && row < d->m_data.size())
        return d->m_data[row].columeMap.value(col).toString();
    return QString();
}

void CTableItemModel::SetItemText(int row, int col, const QString &str)
{
    if (row >= 0 && row < d->m_data.size() && col >= 0 && col < d->m_columeCount)
    {
        d->m_data[row].columeMap[col] = str;
        layoutChanged();
    }
}

int CTableItemModel::ItemCount() const
{
    return d->m_data.size();
}

int CTableItemModel::FindItem(const QString &str, int col)
{
    int iItemCount = ItemCount();
    for (int i = 0; i < iItemCount; i++)
    {
        if (GetItemText(i, col) == str)
            return i;
    }
    return -1;
}

void CTableItemModel::QuickSearch(const QString &text, QTableView *pTableView)
{
    int iItemCount = ItemCount();
    //遍历列表中的每一行，判断该行是否匹配要搜索的字符串
    for (int i = 0; i < iItemCount; i++)
    {
        bool bMatch = false;     //当前行是否匹配
        if (text.isEmpty())
        {
            bMatch = true;  //如果查找字符串为空，则认为匹配
        }
        else
        {
            //遍历当前行的每一列
            for (int j = 0; j < d->m_columeCount; j++)
            {
                QString strCellText = GetItemText(i, j);    //单元格的文本
                if (strCellText.contains(text, Qt::CaseInsensitive)) //该行有一个单元格匹配，则说明该行匹配
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
