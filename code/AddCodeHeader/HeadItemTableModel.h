#pragma once

#include <QAbstractTableModel>
#include <QStyledItemDelegate>
#include <QByteArray>
#include "CommonData.h"

class CHeadItemTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    CHeadItemTableModel(QObject *parent = nullptr);
    ~CHeadItemTableModel();

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
        int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;


    enum eFileHeadTableCol
    {
        COL_HEAD_ITEM_NAME,
        COL_HEAD_ITEM_VALUE,
        COL_HEAD_ITEM_MAX
    };

    QList<HeadItemItem>& GetData();

    void AddItem();

    //序列化
    QByteArray ToByteArray() const;

    //反序列化
    void FromByteArray(const QByteArray& byteArray);

private:
    void AppendItemIfNotExist(eHeadItem item);

private:
    QList<HeadItemItem> m_data;
};



class CHeadItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
        // QAbstractItemDelegate interface
public:
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

private:
    mutable QVariant m_curEditData;
};
