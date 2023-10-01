#pragma once

#include <QWidget>
#include "ui_MainWidget.h"
#include "../CCommonTools/TableDataModel.h"

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

	enum Column
	{
		COL_FILENAME,
		COL_FILEPATH,
		COL_NEW_NAME,
		COL_MAX
	};

	void LoadConfig();
	void SaveConfig() const;

	int RenameWithModifiedTime();

	void AddFile(const QString& filePath);
	QStringList GetFileList() const;
	CTableDataModel& GetModel();
	QTableView* GetTableView();

private slots:
	void OnSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

private:
    Ui::MainWidgetClass ui;
	CTableDataModel m_model{ COL_MAX };
};
