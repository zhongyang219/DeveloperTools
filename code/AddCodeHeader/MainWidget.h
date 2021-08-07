#pragma once

#include <QWidget>
#include <QTextEdit>
#include <QTableView>
#include <QLineEdit>
#include <QComboBox>
//#include <QTextBrowser>

class CMainWidget : public QWidget
{
    Q_OBJECT

public:
    CMainWidget(QWidget *parent = Q_NULLPTR);
    ~CMainWidget();

    QTextEdit* GetTemplateTextEdit();
    QLineEdit* GetFolderPathEidt();
    QComboBox* GetFileTypeCombo();
    QTableView* GetFileTableView();
    QComboBox* GetOutFormatCombo();

private slots:
    void OnBrowseBtnClicked();

private:
    QTextEdit* m_pTemplateTextEdit;
    QLineEdit* m_pFolderPathEidt;
    QComboBox* m_pFileTypeCombo;
    QTableView* m_pFileTableView;
    QComboBox* m_pOutFormatCombo;
};
