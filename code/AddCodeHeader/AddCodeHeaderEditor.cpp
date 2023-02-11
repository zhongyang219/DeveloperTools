#include "AddCodeHeaderEditor.h"
#include <QFile>
#include "../CCommonTools/CommonTools.h"
#include "RemoveCommentHelper.h"
#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <QHeaderView>
#include "AddCodeHeader.h"
#include "../CCommonTools/Config.h"

CAddCodeHeaderEditor::CAddCodeHeaderEditor(QObject *parent)
    : QObject(parent), m_fileListModel(COL_MAX)
{
}

CAddCodeHeaderEditor::~CAddCodeHeaderEditor()
{
}

void CAddCodeHeaderEditor::ConnectWidget(CMainWidget* pWidget)
{
    m_pWidget = pWidget;
    //加载模板
    QFile file(qApp->applicationDirPath() + "/Template.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString strTemplate = QString::fromUtf8(file.readAll());
        pWidget->GetTemplateTextEdit()->setText(strTemplate);
    }

    //加载设置
    CConfig settings(QString::fromWCharArray(AddCodeHeader::GetInstance()->GetModuleName()));
    pWidget->GetFolderPathEidt()->setText(settings.GetValue("folderPath").toString());

    QString strTypeList = settings.GetValue("fileTypeList", "*.c;*.cpp;*.h").toString();
    QStringList typeList = strTypeList.split('|', QString::SkipEmptyParts);
    pWidget->GetFileTypeCombo()->addItems(typeList);

    int outputFormat = settings.GetValue("outoputFormat", OF_UTF8).toInt();
    pWidget->GetOutFormatCombo()->addItem(u8"UTF8", OF_UTF8);
    pWidget->GetOutFormatCombo()->addItem(u8"ANSI", OF_ANSI);
    pWidget->GetOutFormatCombo()->addItem(u8"UTF16", OF_UTF16);
    int index = pWidget->GetOutFormatCombo()->findData(outputFormat);
    pWidget->GetOutFormatCombo()->setCurrentIndex(index);

    pWidget->GetRemoveSpaceCheck()->setChecked(settings.GetValue("removeSpace", true).toBool());
    pWidget->GetRemoveReturnCheck()->setChecked(settings.GetValue("removeReturn", true).toBool());
    pWidget->GetReturnNumEdit()->setValue(settings.GetValue("keepReturnNum", 2).toInt());

    //默认不显示“添加代码头”
    pWidget->ShowAddCodeHeader(false);
    CCommonTools::SetActionEnable(CMD_ADD_CODE_HEADER_EXCUTE, false);

    //设置列表样式
    pWidget->GetFileTableView()->setSelectionBehavior(QAbstractItemView::SelectRows);      //设置选中行
    pWidget->GetFileTableView()->setSelectionMode(QAbstractItemView::SingleSelection);     //设置只允许选中单行
    pWidget->GetFileTableView()->setEditTriggers(QAbstractItemView::NoEditTriggers);       //设置不允许编辑
    pWidget->GetFileTableView()->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);

    //连接数据模型
    pWidget->GetFileTableView()->setModel(&m_fileListModel);

    //初始化模型
    InitFileTable();

    m_pWidget->GetRemoveFileBtn()->setEnabled(false);
    connect(m_pWidget->GetFileTableView()->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this, SLOT(OnSelectionChanged(QItemSelection, QItemSelection)));

    connect(m_pWidget->GetAddFileBtn(), SIGNAL(clicked()), this, SLOT(OnAddFileBtnClicked()));
    connect(m_pWidget->GetRemoveFileBtn(), SIGNAL(clicked()), this, SLOT(OnRemoveFileBtnClicked()));
    connect(m_pWidget->GetClearFileBtn(), SIGNAL(clicked()), this, SLOT(OnClearFileBtnClicked()));
}

void CAddCodeHeaderEditor::ExitWidget()
{
    //保存模板
    QFile file(qApp->applicationDirPath() + "/Template.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        file.write(m_pWidget->GetTemplateTextEdit()->toPlainText().toUtf8());
    }

    //保存设置
    CConfig settings(QString::fromWCharArray(AddCodeHeader::GetInstance()->GetModuleName()));
    settings.WriteValue("folderPath", m_pWidget->GetFolderPathEidt()->text());

    QString strTypeList;
    for (int i = 0; i < m_pWidget->GetFileTypeCombo()->count(); i++)
    {
        strTypeList += m_pWidget->GetFileTypeCombo()->itemText(i);
        strTypeList += '|';
    }
    if (!strTypeList.isEmpty())
        strTypeList.chop(1);
    settings.WriteValue("fileTypeList", strTypeList);

    settings.WriteValue("outoputFormat", m_pWidget->GetOutFormatCombo()->currentData());

    settings.WriteValue("removeSpace", m_pWidget->GetRemoveSpaceCheck()->isChecked());
    settings.WriteValue("removeReturn", m_pWidget->GetRemoveReturnCheck()->isChecked());
    settings.WriteValue("keepReturnNum", m_pWidget->GetReturnNumEdit()->value());
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
    if (!QFileInfo(m_pWidget->GetFolderPathEidt()->text()).isDir())
    {
        QMessageBox::warning(m_pWidget, nullptr, u8"扫描目录不存在！");
        return;
    }
    QFileInfoList fileInfoList = CCommonTools::FindFile(m_pWidget->GetFolderPathEidt()->text(), filterList);
    m_fileListModel.DeleteAllItems();
    Q_FOREACH(const QFileInfo& fileInfo, fileInfoList)
    {
        AddFile(fileInfo);
    }
}

int CAddCodeHeaderEditor::RemoveComments(CRemoveCommentHelper::RemoveResult& remove_result)
{
    //遍历所有文件
    int row_count = m_fileListModel.rowCount();
    int file_count = 0;
    for (int i = 0; i < row_count; i++)
    {
        QString strFilePath = m_fileListModel.GetItemText(i, COL_FILEPATH);
        CRemoveCommentHelper::RemoveResult cur_result;
        if (CRemoveCommentHelper::RemoveFileComment(strFilePath, m_pWidget->GetRemoveSpaceCheck()->isChecked(), m_pWidget->GetRemoveReturnCheck()->isChecked(), m_pWidget->GetReturnNumEdit()->value(), cur_result))
        {
            file_count++;
            remove_result += cur_result;
            emit signalRemoveCommentProgress(static_cast<double>(file_count) * 100 / row_count);
            if (AddCodeHeader::GetInstance()->GetRemoveCommentThread().IsStopped())
                break;
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

void CAddCodeHeaderEditor::AddFile(const QFileInfo & fileInfo)
{
    CTableDataModel::ColumeItemMap colMap;
    colMap[COL_FILENAME] = fileInfo.fileName();
    colMap[COL_FILEPATH] = fileInfo.filePath();
    colMap[COL_EXTENSION] = fileInfo.suffix();
    m_fileListModel.AddItem(colMap);
}

void CAddCodeHeaderEditor::OnSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
    if (m_pWidget != nullptr)
    {
        int row = -1;
        if (!selected.empty())
        {
            QModelIndexList indexs = selected.front().indexes();
            if (!indexs.empty())
                row = indexs.front().row();
        }
        
        m_pWidget->GetRemoveFileBtn()->setEnabled(row >= 0);
    }
}

void CAddCodeHeaderEditor::OnAddFileBtnClicked()
{
    QString strFilters;
    for (int i = 0; i < m_pWidget->GetFileTypeCombo()->count(); i++)
    {
        strFilters += m_pWidget->GetFileTypeCombo()->itemText(i);
        strFilters += ";;";
    }
    strFilters += u8"所有文件 (*.*)";

    QString strFilePath = QFileDialog::getOpenFileName(m_pWidget, nullptr, nullptr, strFilters);
    if (!strFilePath.isEmpty())
    {
        AddFile(QFileInfo(strFilePath));
    }
}

void CAddCodeHeaderEditor::OnRemoveFileBtnClicked()
{
    auto selections = m_pWidget->GetFileTableView()->selectionModel()->selectedIndexes();
    if (selections.isEmpty())
        return;

    if (QMessageBox::information(m_pWidget, nullptr, u8"确实要移除选中文件吗？", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        m_fileListModel.DeleteItem(selections.front().row());
    }
}

void CAddCodeHeaderEditor::OnClearFileBtnClicked()
{
    if (QMessageBox::information(m_pWidget, nullptr, u8"确实要移除所有文件吗？", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        m_fileListModel.DeleteAllItems();
    }
}
