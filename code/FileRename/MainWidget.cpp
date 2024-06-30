#include "MainWidget.h"
#include <QFileInfo>
#include "FileRenameHelper.h"
#include "MainWidget.h"
#include "FileRename.h"
#include "define.h"
#include "../CCommonTools/Config.h"
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QList>
#include <QFileInfo>
#include "../CCommonTools/CommonTools.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    ui.groupBox->layout()->setContentsMargins(DPI(8), DPI(16), DPI(8), DPI(8));
    //设置列表样式
    ui.tableView->setSelectionBehavior(QAbstractItemView::SelectRows);      //设置选中行
    ui.tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);       //设置不允许编辑
    ui.tableView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);

    ui.tableView->setModel(&m_model);
    m_model.setHeaderData(COL_FILENAME, Qt::Horizontal, u8"文件名");
    m_model.setHeaderData(COL_FILEPATH, Qt::Horizontal, u8"文件路径");
    m_model.setHeaderData(COL_NEW_NAME, Qt::Horizontal, u8"新文件名");

    ui.tableView->setColumnWidth(COL_FILENAME, DPI(140));
    ui.tableView->setColumnWidth(COL_FILEPATH, DPI(300));
    ui.tableView->setColumnWidth(COL_NEW_NAME, DPI(140));

    connect(ui.tableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this, SLOT(OnSelectionChanged(QItemSelection, QItemSelection)));

    //响应文件拖放
    setAcceptDrops(true);
}

MainWidget::~MainWidget()
{}

void MainWidget::LoadConfig()
{
    CConfig settings(QString::fromUtf8(FileRename::Instance()->GetModuleName()));
    int mode = settings.GetValue("renameMode").toInt();
    switch (mode)
    {
    case 0: ui.renameByModifyTimeRadioBtn->click(); break;
    case 1: ui.renameByNumberRadioBtn->click(); break;
    case 2: ui.renameReplaceRadioBtn->click(); break;
    }

    ui.addPrefixCheck->setChecked(settings.GetValue("addPrefrex").toBool());
    ui.prefixEdit->setText(settings.GetValue("prefex").toString());
    ui.autoResoveConflictCheck->setChecked(settings.GetValue("autoResoveConflict").toBool());

    ui.startValueSpin->setValue(settings.GetValue("startValue").toInt());
    ui.digitNumSpin->setValue(settings.GetValue("digitNum").toInt());

    ui.findStrEdit->setText(settings.GetValue("findStr").toString());
    ui.replaceStrEdit->setText(settings.GetValue("replaceStr").toString());
}

void MainWidget::SaveConfig() const
{
    CConfig settings(QString::fromUtf8(FileRename::Instance()->GetModuleName()));
    int mode = 0;
    if (ui.renameByModifyTimeRadioBtn->isChecked())
        mode = 0;
    else if (ui.renameByNumberRadioBtn->isChecked())
        mode = 1;
    else if (ui.renameReplaceRadioBtn->isChecked())
        mode = 2;
    settings.WriteValue("renameMode", mode);

    settings.WriteValue("addPrefrex", ui.addPrefixCheck->isChecked());
    settings.WriteValue("prefex", ui.prefixEdit->text());
    settings.WriteValue("autoResoveConflict", ui.autoResoveConflictCheck->isChecked());

    settings.WriteValue("startValue", ui.startValueSpin->value());
    settings.WriteValue("digitNum", ui.digitNumSpin->value());

    settings.WriteValue("findStr", ui.findStrEdit->text());
    settings.WriteValue("replaceStr", ui.replaceStrEdit->text());
}

int MainWidget::ExcuteFileRename()
{
    QStringList fileList = GetFileList();
    int count = 0;
    int i = 0;
    for (const auto& file : fileList)
    {
        QString newName;
        QString strPrefix;
        if (ui.addPrefixCheck->isChecked())
            strPrefix = ui.prefixEdit->text();

        bool rtn = false;
        //使用修改时间重命名
        if (ui.renameByModifyTimeRadioBtn->isChecked())
            rtn = FileRenameHelper::FileRenameByCreateTime(file, newName, strPrefix, ui.autoResoveConflictCheck->isChecked());
        else if (ui.renameByNumberRadioBtn->isChecked())
            rtn = FileRenameHelper::FileRenameByNumber(file, ui.startValueSpin->value() + i, ui.digitNumSpin->value(), newName, strPrefix, ui.autoResoveConflictCheck->isChecked());
        else if (ui.renameReplaceRadioBtn->isChecked())
            rtn = FileRenameHelper::FileRenameReplace(file, ui.findStrEdit->text(), ui.replaceStrEdit->text(), newName, ui.autoResoveConflictCheck->isChecked());

        if (rtn)
        {
            m_model.SetItemText(i, COL_NEW_NAME, newName);
            count++;
        }
        else
        {
            m_model.SetItemText(i, COL_NEW_NAME, u8"<重命名失败>");
        }
        i++;
    }
    return count;
}

void MainWidget::AddFile(const QString & filePath)
{
    QFileInfo fileInfo(filePath);
    ColumeItemMap colMap;
    colMap[COL_FILENAME] = fileInfo.fileName();
    colMap[COL_FILEPATH] = fileInfo.filePath();
    m_model.AddItem(colMap);
}

void MainWidget::AddFolder(const QString& folderPath)
{
    QFileInfoList fileInfoList = CCommonTools::FindFile(folderPath);
    Q_FOREACH(const QFileInfo & fileInfo, fileInfoList)
    {
        AddFile(fileInfo.filePath());
    }
}

QStringList MainWidget::GetFileList() const
{
    QStringList fileList;
    int fileCount = m_model.ItemCount();
    for (int i = 0; i < fileCount; i++)
    {
        fileList.push_back(m_model.GetItemText(i, COL_FILEPATH));
    }
    return fileList;
}

CTableItemModel& MainWidget::GetModel()
{
    return m_model;
}

QTableView* MainWidget::GetTableView()
{
    return ui.tableView;
}

void MainWidget::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasUrls())
    {
        event->acceptProposedAction();
    }
}

void MainWidget::dropEvent(QDropEvent* event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    for (const QUrl& url : urls)
    {
        //获取每个拖放文件的路径
        QString filePath = url.toLocalFile();
        //判断是文件还是文件夹
        QFileInfo fileInfo(filePath);
        if (fileInfo.isDir())
        {
            AddFolder(filePath);
        }
        else
        {
            AddFile(filePath);
        }
    }
}

void MainWidget::OnSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
        int row = -1;
        if (!selected.empty())
        {
            QModelIndexList indexs = selected.front().indexes();
            if (!indexs.empty())
                row = indexs.front().row();
        }

        FileRename::Instance()->GetMainFrame()->SetItemEnable(CMD_FileRenameDelete, row >= 0);
}

