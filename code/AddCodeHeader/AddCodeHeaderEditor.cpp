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
    : QObject(parent)
    , m_fileListModel(COL_MAX)
{
}

CAddCodeHeaderEditor::~CAddCodeHeaderEditor()
{
}

void CAddCodeHeaderEditor::ConnectWidget(CMainWidget* pWidget, IMainFrame* pMainFrame)
{
    m_pWidget = pWidget;
    m_pMainFrame = pMainFrame;

    //加载设置
    CConfig settings(QString::fromUtf8(AddCodeHeader::GetInstance()->GetModuleName()));
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

    m_pMainFrame->SetItemChecked(CMD_RemoveCommentCheck, settings.GetValue("removeComment", true).toBool());
    m_pMainFrame->SetItemChecked(CMD_RemoveSpaceCheck, settings.GetValue("removeSpace", true).toBool());
    m_pMainFrame->SetItemChecked(CMD_RemoveEmptyLineCheck, settings.GetValue("removeReturn", true).toBool());
    m_pMainFrame->SetItemCurIIndex(CMD_KeepEmptyLineNum, settings.GetValue("keepReturnNum", 2).toInt());

    m_fileHeadItemModel.FromByteArray(settings.GetValue("fileHeadItem").toByteArray());

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

    m_pWidget->GetRemoveFileBtn()->setEnabled(false);
    connect(m_pWidget->GetFileTableView()->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this, SLOT(OnSelectionChanged(QItemSelection, QItemSelection)));

    connect(m_pWidget->GetAddFileBtn(), SIGNAL(clicked()), this, SLOT(OnAddFileBtnClicked()));
    connect(m_pWidget->GetRemoveFileBtn(), SIGNAL(clicked()), this, SLOT(OnRemoveFileBtnClicked()));
    connect(m_pWidget->GetClearFileBtn(), SIGNAL(clicked()), this, SLOT(OnClearFileBtnClicked()));

    //初始化添加代码头
    pWidget->GetTemplateItemTable()->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_pWidget->GetTemplateItemTable()->setModel(&m_fileHeadItemModel);
    CHeadItemDelegate* pDelegate = new CHeadItemDelegate();
    m_pWidget->GetTemplateItemTable()->setItemDelegate(pDelegate);
    m_pWidget->GetTemplateItemTable()->setColumnWidth(COL_HEAD_ITEM_VALUE, DPI(180));

    connect(m_pWidget->GetTemplateMoveUpBtn(), SIGNAL(clicked()), this, SLOT(OnTemplateMoveUpBtnClicked()));
    connect(m_pWidget->GetTemplateMoveDownBtn(), SIGNAL(clicked()), this, SLOT(OnTemplateMoveDownBtnClicked()));
}

void CAddCodeHeaderEditor::ExitWidget()
{
    //保存设置
    CConfig settings(QString::fromUtf8(AddCodeHeader::GetInstance()->GetModuleName()));
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

    settings.WriteValue("removeComment", m_pMainFrame->IsItemChecked(CMD_RemoveCommentCheck));
    settings.WriteValue("removeSpace", m_pMainFrame->IsItemChecked(CMD_RemoveSpaceCheck));
    settings.WriteValue("removeReturn", m_pMainFrame->IsItemChecked(CMD_RemoveEmptyLineCheck));
    settings.WriteValue("keepReturnNum", m_pMainFrame->GetItemText(CMD_KeepEmptyLineNum));

    settings.WriteValue("fileHeadItem", m_fileHeadItemModel.ToByteArray());
}

void CAddCodeHeaderEditor::AdjustColumeWidth()
{
    if (m_pWidget != nullptr)
    {
        int width = m_pWidget->GetFileTableView()->width() - DPI(21);
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
    m_fileListModel.ClearData();
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
        bool removeComment = m_pMainFrame->IsItemChecked(CMD_RemoveCommentCheck);
        bool removeSpace = m_pMainFrame->IsItemChecked(CMD_RemoveSpaceCheck);
        bool removeEmptyLine = m_pMainFrame->IsItemChecked(CMD_RemoveEmptyLineCheck);
        int keepEmptyLineNum = QString(m_pMainFrame->GetItemText(CMD_KeepEmptyLineNum)).toInt();
        if (CRemoveCommentHelper::RemoveFileComment(strFilePath, removeComment, removeSpace, removeEmptyLine, keepEmptyLineNum, cur_result))
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

QStringList CAddCodeHeaderEditor::GetFilePathList()
{
    QStringList filePathList;
    int row_count = m_fileListModel.rowCount();
    for (int i = 0; i < row_count; i++)
    {
        QString strFilePath = m_fileListModel.GetItemText(i, COL_FILEPATH);
        filePathList.push_back(strFilePath);
    }
    return filePathList;
}


eOutputFormat CAddCodeHeaderEditor::GetOutputFormat()
{
    return static_cast<eOutputFormat>(m_pWidget->GetOutFormatCombo()->currentData().toInt());
}


CHeadItemTableModel& CAddCodeHeaderEditor::GetHeadItemTableModel()
{
    return m_fileHeadItemModel;
}

void CAddCodeHeaderEditor::InitFileTable()
{
    m_fileListModel.setHeaderData(COL_FILENAME, Qt::Horizontal, u8"文件名");
    m_fileListModel.setHeaderData(COL_FILEPATH, Qt::Horizontal, u8"文件路径");
    m_fileListModel.setHeaderData(COL_EXTENSION, Qt::Horizontal, u8"扩展名");

    //m_fileListModel.
}

void CAddCodeHeaderEditor::AddFile(const QFileInfo & fileInfo)
{
    ColumeItemMap colMap;
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
        m_fileListModel.ClearData();
    }
}

void CAddCodeHeaderEditor::OnTemplateMoveUpBtnClicked()
{
    auto& data = m_fileHeadItemModel.GetData();
    int currentRow = m_pWidget->GetTemplateItemTable()->currentIndex().row();
    if (currentRow > 0 && currentRow < data.size())
    {
        std::swap(data[currentRow], data[currentRow - 1]);
        m_pWidget->GetTemplateItemTable()->selectionModel()->setCurrentIndex(m_fileHeadItemModel.index(currentRow - 1, 0), QItemSelectionModel::SelectCurrent);
        m_fileHeadItemModel.layoutChanged();
    }
}

void CAddCodeHeaderEditor::OnTemplateMoveDownBtnClicked()
{
    auto& data = m_fileHeadItemModel.GetData();
    int currentRow = m_pWidget->GetTemplateItemTable()->currentIndex().row();
    if (currentRow >= 0 && currentRow < data.size() - 1)
    {
        std::swap(data[currentRow], data[currentRow + 1]);
        m_pWidget->GetTemplateItemTable()->selectionModel()->setCurrentIndex(m_fileHeadItemModel.index(currentRow + 1, 0), QItemSelectionModel::SelectCurrent);
        m_fileHeadItemModel.layoutChanged();
    }
}
