#include "MainWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSplitter>
#include <QPushButton>
#include <QFileDialog>

CMainWidget::CMainWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout* pLayout = new QVBoxLayout();
    QSplitter* pSpliter = new QSplitter(this);
    pLayout->addWidget(pSpliter);
    pLayout->setMargin(0);
    this->setLayout(pLayout);

    QWidget* pLeftWidget = new QWidget();
    QWidget* pRightWidget = new QWidget();
    pSpliter->addWidget(pLeftWidget);
    pSpliter->addWidget(pRightWidget);

    //�������ؼ�
    QVBoxLayout* pLeftLayout = new QVBoxLayout();
    pLeftWidget->setLayout(pLeftLayout);
    pLeftLayout->addWidget(new QLabel(u8"����ɨ��·����"));
    QHBoxLayout* pPathEditLayout = new QHBoxLayout();
    pPathEditLayout->addWidget(m_pFolderPathEidt = new QLineEdit());
    QPushButton* pBrowseBtn = new QPushButton(u8"���(&B)...");
    pPathEditLayout->addWidget(pBrowseBtn);
    pLeftLayout->addLayout(pPathEditLayout);
    pLeftLayout->addWidget(new QLabel(u8"ɨ���ļ��ĸ�ʽ��"));
    pLeftLayout->addWidget(m_pFileTypeCombo = new QComboBox());
    pLeftLayout->addWidget(new QLabel(u8"�ļ��б�"));
    pLeftLayout->addWidget(m_pFileTableView = new QTableView());

    //�����Ҳ�ؼ�
    QVBoxLayout* pRightLayout = new QVBoxLayout();
    pRightWidget->setLayout(pRightLayout);
    pRightLayout->addWidget(new QLabel(u8"�����ļ�ͷģ�壺"));
    pRightLayout->addWidget(m_pTemplateTextEdit = new QTextEdit());
    QLabel* pInfoLabel = new QLabel(u8"���ú�˵����\r\n<%file_name%>: �ļ���\r\n<%time[format]%>: ����/ʱ�䣬formatΪ��ʽ�����硰yyyyMMdd��");
    pInfoLabel->setWordWrap(true);
    pRightLayout->addWidget(pInfoLabel);
    QHBoxLayout* pOptionHLayout = new QHBoxLayout();
    pOptionHLayout->addWidget(new QLabel(u8"��������ʽ��"));
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
