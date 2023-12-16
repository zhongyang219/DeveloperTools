#include "HeadItemTableModel.h"
#include <QComboBox>
#include <QDataStream>
#include "../CCommonTools/LongTextEdit.h"


CHeadItemTableModel::CHeadItemTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

CHeadItemTableModel::~CHeadItemTableModel()
{
}

QVariant CHeadItemTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch (section)
        {
        case COL_HEAD_ITEM_NAME:
            return u8"名称";
        case COL_HEAD_ITEM_VALUE:
            return u8"值";
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);

}

int CHeadItemTableModel::rowCount(const QModelIndex & parent) const
{
    return m_data.size();
}

int CHeadItemTableModel::columnCount(const QModelIndex & parent) const
{
    return COL_HEAD_ITEM_MAX;
}

QVariant CHeadItemTableModel::data(const QModelIndex & index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= 0 && index.row() < m_data.size())
    {
        if (role == Qt::DisplayRole)
        {
            const HeadItemItem& headItem = m_data.value(index.row());
            switch (index.column())
            {
            case COL_HEAD_ITEM_NAME:
                return headItem.name();
            case COL_HEAD_ITEM_VALUE:
            {
                if (headItem.item == HI_FILE || headItem.item == HI_DATE)
                    return QVariant();
                else
                    return headItem.value;
            }
            }
        }
        else if (role == Qt::EditRole)
        {
            const HeadItemItem& headItem = m_data.value(index.row());
            switch (index.column())
            {
            case COL_HEAD_ITEM_NAME:
                return static_cast<int>(headItem.item);
            case COL_HEAD_ITEM_VALUE:
                return headItem.value;
            }
        }

        //设置复选框的选中状态
        else if (role == Qt::CheckStateRole && index.column() == 0)
        {
            const HeadItemItem& headItem = m_data.value(index.row());
            if (headItem.enable)
                return Qt::Checked;
            else
                return Qt::Unchecked;
        }
    }

    return QVariant();
}

bool CHeadItemTableModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (index.row() >= 0 && index.row() < m_data.size())
    {
        if (role == Qt::EditRole)
        {
            HeadItemItem& headItem = m_data[index.row()];
            switch (index.column())
            {
            case COL_HEAD_ITEM_NAME:
                headItem.item = static_cast<eHeadItem>(value.toInt());
                return true;
            case COL_HEAD_ITEM_VALUE:
                headItem.value = value.toString();
                return true;
            }
        }
        else if (role == Qt::CheckStateRole && index.column() == 0)
        {
            HeadItemItem& headItem = m_data[index.row()];
            headItem.enable = (value == Qt::Checked);
            emit dataChanged(index, index);
        }
    }

    return false;
}

Qt::ItemFlags CHeadItemTableModel::flags(const QModelIndex & index) const
{
    Qt::ItemFlags flag = QAbstractTableModel::flags(index);
    const HeadItemItem& headItem = m_data.value(index.row());
    if (index.column() == COL_HEAD_ITEM_VALUE && (headItem.item != HI_FILE && headItem.item != HI_DATE))
        flag = flag | Qt::ItemIsEditable;

    flag = flag | Qt::ItemIsUserCheckable;

    return flag;
}

QList<HeadItemItem>& CHeadItemTableModel::GetData()
{
    return m_data;
}

void CHeadItemTableModel::AddItem()
{
    m_data.append(HeadItemItem());
    layoutChanged();
}

QByteArray CHeadItemTableModel::ToByteArray() const
{
    QByteArray data;
    QDataStream dataStream(&data, QIODevice::WriteOnly);
    //写入版本
    dataStream << 2;
    //写入长度
    dataStream << m_data.size();
    //写入数据
    for (const auto& item : m_data)
    {
        dataStream << item.item;
        dataStream << item.value;
        dataStream << item.enable;
    }
    return data;
}

void CHeadItemTableModel::FromByteArray(const QByteArray& byteArray)
{
    QDataStream dataStream(byteArray);
    //读取版本号
    int version;
    dataStream >> version;
    //读取长度
    int length;
    dataStream >> length;
    //读取数据
    for (int i = 0; i < length; i++)
    {
        HeadItemItem item;
        int itemType;
        dataStream >> itemType;
        item.item = static_cast<eHeadItem>(itemType);
        dataStream >> item.value;
        if (version >= 2)
            dataStream >> item.enable;
        m_data.append(item);
    }
    //添加其他不存在的项
    AppendItemIfNotExist(HI_COPYRIGHT);
    AppendItemIfNotExist(HI_FILE);
    AppendItemIfNotExist(HI_BRIEF);
    AppendItemIfNotExist(HI_AUTHOR);
    AppendItemIfNotExist(HI_EMAL);
    AppendItemIfNotExist(HI_VERSION);
    AppendItemIfNotExist(HI_DATE);
    AppendItemIfNotExist(HI_USER_DEFINE);
}

void CHeadItemTableModel::AppendItemIfNotExist(eHeadItem item)
{
    bool itemExist = false;
    for (const auto& headItem : m_data)
    {
        if (headItem.item == item)
        {
            itemExist = true;
            break;
        }
    }
    if (!itemExist)
        m_data.append(HeadItemItem(item));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
QWidget *CHeadItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
        QWidget* pEditorWidget = nullptr;
    switch (index.column())
    {
    //case CHeadItemTableModel::COL_HEAD_ITEM_NAME:
    //{
    //    QComboBox* pCombo = new QComboBox(parent);
    //    for (int i = 0; i < HI_MAX; i++)
    //    {
    //        QString strName = HeadItemItem::GetHeadItemName(static_cast<eHeadItem>(i));
    //        pCombo->addItem(strName);
    //    }
    //    pEditorWidget = pCombo;
    //}
    //break;
    case CHeadItemTableModel::COL_HEAD_ITEM_VALUE:
    {
        CLongTextEdit* pEdit = new CLongTextEdit(parent);
        pEditorWidget = pEdit;
    }
    break;
    default:
        pEditorWidget = QStyledItemDelegate::createEditor(parent, option, index);
        break;
    }
    return pEditorWidget;
}

void CHeadItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QVariant value = index.model()->data(index, Qt::EditRole);
    m_curEditData = value;  //将设置给控件的数据保存
    switch (index.column())
    {
    //case CHeadItemTableModel::COL_HEAD_ITEM_NAME:
    //{
    //    QComboBox* pCombo = dynamic_cast<QComboBox*>(editor);
    //    if (pCombo != nullptr)
    //    {
    //        pCombo->setCurrentIndex(value.toInt());
    //    }
    //}
    //break;
    case CHeadItemTableModel::COL_HEAD_ITEM_VALUE:
    {
        CLongTextEdit* pEdit = dynamic_cast<CLongTextEdit*>(editor);
        pEdit->setText(value.toString());
    }
    break;
    default:
        QStyledItemDelegate::setEditorData(editor, index);
        break;
    }
}

void CHeadItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    switch (index.column())
    {
    //case CHeadItemTableModel::COL_HEAD_ITEM_NAME:
    //{
    //    QComboBox* pCombo = dynamic_cast<QComboBox*>(editor);
    //    if (pCombo != nullptr && m_curEditData.toInt() != pCombo->currentIndex())
    //    {
    //        model->setData(index, pCombo->currentIndex(), Qt::EditRole);    //数据发生改变才调用setData
    //    }
    //}
    //break;
    case CHeadItemTableModel::COL_HEAD_ITEM_VALUE:
    {
        CLongTextEdit* pEdit = dynamic_cast<CLongTextEdit*>(editor);
        model->setData(index, pEdit->text(), Qt::EditRole);
    }
    break;
    default:
        QStyledItemDelegate::setModelData(editor, model, index);
        break;
    }
}
