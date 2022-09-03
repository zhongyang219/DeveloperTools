﻿#include "AddCodeHeaderEditor.h"
#include <QFile>
#include <QSettings>
#include "../CCommonTools/CommonTools.h"
#include "RemoveCommentHelper.h"

CAddCodeHeaderEditor::CAddCodeHeaderEditor(QObject *parent)
    : QObject(parent)
{
}

CAddCodeHeaderEditor::~CAddCodeHeaderEditor()
{
}

void CAddCodeHeaderEditor::ConnectWidget(CMainWidget* pWidget)
{
    m_pWidget = pWidget;
    //加载模板
    QFile file("Template.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString strTemplate = QString::fromUtf8(file.readAll());
        pWidget->GetTemplateTextEdit()->setText(strTemplate);
    }

    //加载设置
    QSettings ini("config.ini", QSettings::IniFormat);
    ini.setIniCodec("UTF-8");
    pWidget->GetFolderPathEidt()->setText(ini.value("AddCodeHeader/folderPath").toString());

    QString strTypeList = ini.value("AddCodeHeader/fileTypeList", "*.c;*.cpp;*.h").toString();
    QStringList typeList = strTypeList.split('|', QString::SkipEmptyParts);
    pWidget->GetFileTypeCombo()->addItems(typeList);

    int outputFormat = ini.value("AddCodeHeader/outoputFormat", OF_UTF8).toInt();
    pWidget->GetOutFormatCombo()->addItem(u8"UTF8", OF_UTF8);
    pWidget->GetOutFormatCombo()->addItem(u8"ANSI", OF_ANSI);
    pWidget->GetOutFormatCombo()->addItem(u8"UTF16", OF_UTF16);
    int index = pWidget->GetOutFormatCombo()->findData(outputFormat);
    pWidget->GetOutFormatCombo()->setCurrentIndex(index);

    pWidget->GetRemoveSpaceCheck()->setChecked(ini.value("AddCodeHeader/removeSpace", true).toBool());
    pWidget->GetRemoveReturnCheck()->setChecked(ini.value("AddCodeHeader/removeReturn", true).toBool());

    //默认不显示“添加代码头”
    pWidget->ShowAddCodeHeader(false);

    //设置列表样式
    pWidget->GetFileTableView()->setSelectionBehavior(QAbstractItemView::SelectRows);      //设置选中行
    pWidget->GetFileTableView()->setSelectionMode(QAbstractItemView::SingleSelection);     //设置只允许选中单行
    pWidget->GetFileTableView()->setEditTriggers(QAbstractItemView::NoEditTriggers);       //设置不允许编辑
    pWidget->GetFileTableView()->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);

    //连接数据模型
    pWidget->GetFileTableView()->setModel(&m_fileListModel);

    //初始化模型
    InitFileTable();
}

void CAddCodeHeaderEditor::ExitWidget()
{
    //保存模板
    QFile file("Template.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        file.write(m_pWidget->GetTemplateTextEdit()->toPlainText().toUtf8());
    }

    //保存设置
    QSettings ini("config.ini", QSettings::IniFormat);
    ini.setIniCodec("UTF-8");
    ini.setValue("AddCodeHeader/folderPath", m_pWidget->GetFolderPathEidt()->text());

    QString strTypeList;
    for (int i = 0; i < m_pWidget->GetFileTypeCombo()->count(); i++)
    {
        strTypeList += m_pWidget->GetFileTypeCombo()->itemText(i);
        strTypeList += '|';
    }
    if (!strTypeList.isEmpty())
        strTypeList.chop(1);
    ini.setValue("AddCodeHeader/fileTypeList", strTypeList);

    ini.setValue("AddCodeHeader/outoputFormat", m_pWidget->GetOutFormatCombo()->currentData());

    ini.setValue("AddCodeHeader/removeSpace", m_pWidget->GetRemoveSpaceCheck()->isChecked());
    ini.setValue("AddCodeHeader/removeReturn", m_pWidget->GetRemoveReturnCheck()->isChecked());
}

void CAddCodeHeaderEditor::AdjustColumeWidth()
{
    if (m_pWidget != nullptr)
    {
        int width = m_pWidget->GetFileTableView()->width();
        int width0 = width / 5;
        int width2 = width0;
        int width1 = width - width0 - width2;
        m_pWidget->GetFileTableView()->setColumnWidth(COL_FILENAME, width0);
        m_pWidget->GetFileTableView()->setColumnWidth(COL_FILEPATH, width1);
        m_pWidget->GetFileTableView()->setColumnWidth(COL_EXTENSION, width2);
    }
}

void CAddCodeHeaderEditor::ScanFiles()
{
    QStringList filterList = m_pWidget->GetFileTypeCombo()->currentText().split(';');
    QFileInfoList fileInfoList = CCommonTools::FindFile(m_pWidget->GetFolderPathEidt()->text(), filterList);
    m_fileListModel.removeRows(0, m_fileListModel.rowCount());
    Q_FOREACH(const QFileInfo& fileInfo, fileInfoList)
    {
        QList<QStandardItem*> itemList;
        for (int i = 0; i < COL_MAX; i++)
        {
            switch (i)
            {
            case COL_FILENAME:
                itemList.append(new QStandardItem(fileInfo.fileName()));
                break;
            case COL_FILEPATH:
                itemList.append(new QStandardItem(fileInfo.filePath()));
                break;
            case COL_EXTENSION:
                itemList.append(new QStandardItem(fileInfo.suffix()));
                break;
            }
        }
        m_fileListModel.appendRow(itemList);
    }
}

int CAddCodeHeaderEditor::RemoveComments(CRemoveCommentHelper::RemoveResult& remove_result)
{
    //遍历所有文件
    int row_count = m_fileListModel.rowCount();
    int file_count = 0;
    for (int i = 0; i < row_count; i++)
    {
        QString strFilePath = m_fileListModel.item(i, COL_FILEPATH)->text();
        CRemoveCommentHelper::RemoveResult cur_result;
        if (CRemoveCommentHelper::RemoveComment(strFilePath, m_pWidget->GetRemoveSpaceCheck()->isChecked(), m_pWidget->GetRemoveReturnCheck()->isChecked(), cur_result))
        {
            file_count++;
            remove_result += cur_result;
        }
    }
    return file_count;
}

void CAddCodeHeaderEditor::InitFileTable()
{
    m_fileListModel.setColumnCount(COL_MAX);
    m_fileListModel.setHeaderData(COL_FILENAME, Qt::Horizontal, u8"文件名");
    m_fileListModel.setHeaderData(COL_FILEPATH, Qt::Horizontal, u8"文件路径");
    m_fileListModel.setHeaderData(COL_EXTENSION, Qt::Horizontal, u8"扩展名");

    //m_fileListModel.
}
