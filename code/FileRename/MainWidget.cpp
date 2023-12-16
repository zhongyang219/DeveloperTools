#include "MainWidget.h"
#include <QFileInfo>
#include "FileRenameHelper.h"
#include "MainWidget.h"
#include "FileRename.h"
#include "define.h"
#include "../CCommonTools/Config.h"

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

}

MainWidget::~MainWidget()
{}

void MainWidget::LoadConfig()
{
	CConfig settings(QString::fromUtf8(FileRename::Instance()->GetModuleName()));
	ui.addPrefixCheck->setChecked(settings.GetValue("addPrefrex").toBool());
	ui.prefixEdit->setText(settings.GetValue("prefex").toString());
	ui.autoResoveConflictCheck->setChecked(settings.GetValue("autoResoveConflict").toBool());
}

void MainWidget::SaveConfig() const
{
	CConfig settings(QString::fromUtf8(FileRename::Instance()->GetModuleName()));
	settings.WriteValue("addPrefrex", ui.addPrefixCheck->isChecked());
	settings.WriteValue("prefex", ui.prefixEdit->text());
	settings.WriteValue("autoResoveConflict", ui.autoResoveConflictCheck->isChecked());
}

int MainWidget::RenameWithModifiedTime()
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
		if (FileRenameHelper::FileRenameByCreateTime(file, newName, strPrefix, ui.autoResoveConflictCheck->isChecked()))
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

