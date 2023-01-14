#pragma once

#include <QWidget>
#include <QTextEdit>
#include <QTableView>
#include <QLineEdit>
#include <QComboBox>
//#include <QTextBrowser>
#include <QCheckBox>
#include <QPushButton>
#include <QSpinBox>
#include <QProgressBar>
#include <QLabel>
#include "define.h"

class CMainWidget : public QWidget
{
    Q_OBJECT

public:
    CMainWidget(QWidget *parent = Q_NULLPTR);
    ~CMainWidget();

    DECLARE_CLASS_GET_ATTR(TemplateTextEdit, m_pTemplateTextEdit, QTextEdit*)
    DECLARE_CLASS_GET_ATTR(FolderPathEidt, m_pFolderPathEidt, QLineEdit*)
    DECLARE_CLASS_GET_ATTR(BrowseBtn, m_pBrowseBtn, QPushButton*)
    DECLARE_CLASS_GET_ATTR(FileTypeCombo, m_pFileTypeCombo, QComboBox*)
    DECLARE_CLASS_GET_ATTR(FileTableView, m_pFileTableView, QTableView*)
    DECLARE_CLASS_GET_ATTR(OutFormatCombo, m_pOutFormatCombo, QComboBox*)
    DECLARE_CLASS_GET_ATTR(RemoveSpaceCheck, m_pRemoveSpace, QCheckBox*)
    DECLARE_CLASS_GET_ATTR(RemoveReturnCheck, m_pRemoveReturn, QCheckBox*)
    DECLARE_CLASS_GET_ATTR(ReturnNumEdit, m_pReturnNumEdit, QSpinBox*)
    DECLARE_CLASS_GET_ATTR(AddFileBtn, m_pAddFileBtn, QPushButton*)
    DECLARE_CLASS_GET_ATTR(RemoveFileBtn, m_pRemoveFileBtn, QPushButton*)
    DECLARE_CLASS_GET_ATTR(ClearFileBtn, m_pClearFileBtn, QPushButton*)
    DECLARE_CLASS_GET_ATTR(ProgressBar, m_pProgressBar, QProgressBar*)

    void ShowAddCodeHeader(bool show);

private slots:
    void OnBrowseBtnClicked();
    void SetProgress(double progress);

private:
    QTextEdit* m_pTemplateTextEdit;
    QLineEdit* m_pFolderPathEidt;
    QPushButton* m_pBrowseBtn;
    QComboBox* m_pFileTypeCombo;
    QTableView* m_pFileTableView;
    QComboBox* m_pOutFormatCombo;
    QWidget* m_pAddCodeHeadWidget;      //右侧控件
    QCheckBox* m_pRemoveSpace;
    QCheckBox* m_pRemoveReturn;
    QSpinBox* m_pReturnNumEdit;
    QPushButton* m_pAddFileBtn;			//添加文件按钮
    QPushButton* m_pRemoveFileBtn;		//删除文件按钮
    QPushButton* m_pClearFileBtn;		//删除文件按钮
    QProgressBar* m_pProgressBar;
    QLabel* m_pProgressLabel;
};
