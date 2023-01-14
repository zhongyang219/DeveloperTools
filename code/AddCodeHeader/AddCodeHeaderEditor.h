#pragma once

#include <QObject>
#include <QStandardItemModel>
#include "MainWidget.h"
#include "RemoveCommentHelper.h"
#include "../CCommonTools/TableDataModel.h"
#include <QFileInfo>
#include <functional>

class CAddCodeHeaderEditor : public QObject
{
    Q_OBJECT

public:
    CAddCodeHeaderEditor(QObject *parent = Q_NULLPTR);
    ~CAddCodeHeaderEditor();

    //对界面进行初始化操作
    void ConnectWidget(CMainWidget* pWidget);

    //界面关闭时执行的操作
    void ExitWidget();

    void AdjustColumeWidth();

    //扫描文件
    void ScanFiles();

    //移除所有文件的注释
    //remove_result：保存结果
    //func：一个函数对象，参数为已处理的文件个数
    //返回值：总共已处理的文件个数
    int RemoveComments(CRemoveCommentHelper::RemoveResult& remove_result);

    enum eOutputFormat      //输出的文本格式
    {
        OF_UTF8,
        OF_ANSI,
        OF_UTF16
    };

    enum eFileListColume
    {
        COL_FILENAME,
        COL_FILEPATH,
        COL_EXTENSION,

        COL_MAX
    };

signals:
    void signalRemoveCommentProgress(double percent);

private:
    void InitFileTable();
    void AddFile(const QFileInfo& fileInfo);

private slots:
    void OnSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);
    void OnAddFileBtnClicked();
    void OnRemoveFileBtnClicked();
    void OnClearFileBtnClicked();

private:
    CTableDataModel m_fileListModel;

    CMainWidget* m_pWidget{};

};
