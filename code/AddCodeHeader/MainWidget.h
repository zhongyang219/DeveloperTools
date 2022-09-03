#pragma once

#include <QWidget>
#include <QTextEdit>
#include <QTableView>
#include <QLineEdit>
#include <QComboBox>
//#include <QTextBrowser>
#include <QCheckBox>

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
    QCheckBox* GetRemoveSpaceCheck();
    QCheckBox* GetRemoveReturnCheck();
    void ShowAddCodeHeader(bool show);

private slots:
    void OnBrowseBtnClicked();

private:
    QTextEdit* m_pTemplateTextEdit;
    QLineEdit* m_pFolderPathEidt;
    QComboBox* m_pFileTypeCombo;
    QTableView* m_pFileTableView;
    QComboBox* m_pOutFormatCombo;
    QWidget* m_pAddCodeHeadWidget;      //ср╡Ю©ь╪Ч
    QCheckBox* m_pRemoveSpace;
    QCheckBox* m_pRemoveReturn;
};
