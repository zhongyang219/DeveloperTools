#pragma once

#include <QWidget>
#include "ui_MainWidget.h"
#include "../CCommonTools/tableitemmodel.h"


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

    int ExcuteFileRename();

    void AddFile(const QString& filePath);
    void AddFolder(const QString& folderPath);
    QStringList GetFileList() const;
    CTableItemModel& GetModel();
    QTableView* GetTableView();

protected:
    virtual void dragEnterEvent(QDragEnterEvent* event) override;
    virtual void dropEvent(QDropEvent* event) override;

private slots:
    void OnSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

private:
    Ui::MainWidgetClass ui;
    CTableItemModel m_model{ COL_MAX };
};
