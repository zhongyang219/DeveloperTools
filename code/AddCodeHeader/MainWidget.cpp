﻿#include "MainWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSplitter>
#include <QPushButton>
#include <QFileDialog>
#include <QTabWidget>

CMainWidget::CMainWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout* pLayout = new QVBoxLayout();
    QSplitter* pSpliter = new QSplitter(this);
    pLayout->addWidget(pSpliter);
    pLayout->setMargin(0);
    this->setLayout(pLayout);

    //创建左侧控件
    QWidget* pLeftWidget = new QWidget();
    pSpliter->addWidget(pLeftWidget);

    QVBoxLayout* pLeftLayout = new QVBoxLayout();
    pLeftWidget->setLayout(pLeftLayout);
    pLeftLayout->addWidget(new QLabel(u8"设置扫描路径："));
    QHBoxLayout* pPathEditLayout = new QHBoxLayout();
    pPathEditLayout->addWidget(m_pFolderPathEidt = new QLineEdit());
    QPushButton* pBrowseBtn = new QPushButton(u8"浏览(&B)...");
    pPathEditLayout->addWidget(pBrowseBtn);
    pLeftLayout->addLayout(pPathEditLayout);
    pLeftLayout->addWidget(new QLabel(u8"扫描文件的格式："));
    pLeftLayout->addWidget(m_pFileTypeCombo = new QComboBox());
    pLeftLayout->addWidget(new QLabel(u8"文件列表："));
    pLeftLayout->addWidget(m_pFileTableView = new QTableView());

    //创建右侧控件

    QWidget* pAddCodeHeadWidget = new QWidget();
    pSpliter->addWidget(pAddCodeHeadWidget);
    QVBoxLayout* pRightLayout = new QVBoxLayout();
    pAddCodeHeadWidget->setLayout(pRightLayout);
    pRightLayout->addWidget(new QLabel(u8"输入文件头模板："));
    pRightLayout->addWidget(m_pTemplateTextEdit = new QTextEdit());
    QLabel* pInfoLabel = new QLabel(u8"可用宏说明：\r\n<%file_name%>: 文件名\r\n<%time[format]%>: 日期/时间，format为格式，例如“yyyyMMdd”");
    pInfoLabel->setWordWrap(true);
    pRightLayout->addWidget(pInfoLabel);
    QHBoxLayout* pOptionHLayout = new QHBoxLayout();
    pOptionHLayout->addWidget(new QLabel(u8"输出编码格式："));
    pOptionHLayout->addWidget(m_pOutFormatCombo = new QComboBox());
    pRightLayout->addLayout(pOptionHLayout);


    m_pFolderPathEidt->setReadOnly(true);
    m_pFileTypeCombo->setEditable(true);

    //
    connect(pBrowseBtn, SIGNAL(clicked()), this, SLOT(OnBrowseBtnClicked()));
}

CMainWidget::~CMainWidget()
{
}

QTextEdit * CMainWidget::GetTemplateTextEdit()
{
    return m_pTemplateTextEdit;
}

QLineEdit * CMainWidget::GetFolderPathEidt()
{
    return m_pFolderPathEidt;
}

QComboBox * CMainWidget::GetFileTypeCombo()
{
    return m_pFileTypeCombo;
}

QTableView * CMainWidget::GetFileTableView()
{
    return m_pFileTableView;
}

QComboBox* CMainWidget::GetOutFormatCombo()
{
    return m_pOutFormatCombo;
}

void CMainWidget::OnBrowseBtnClicked()
{
    QString strNewPath = QFileDialog::getExistingDirectory(this, QString(), m_pFolderPathEidt->text());
    if (!strNewPath.isEmpty())
        m_pFolderPathEidt->setText(strNewPath);
}
