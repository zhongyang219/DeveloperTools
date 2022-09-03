#pragma once

#include <QObject>
#include <QStandardItemModel>
#include "MainWidget.h"
#include "RemoveCommentHelper.h"

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

private:
    void InitFileTable();

private:
    QStandardItemModel m_fileListModel;

    CMainWidget* m_pWidget{};

};
