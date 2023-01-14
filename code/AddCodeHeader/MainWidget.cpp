#include "MainWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSplitter>
#include <QPushButton>
#include <QFileDialog>
#include <QTabWidget>
#include <QGroupBox>

CMainWidget::CMainWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout* pLayout = new QVBoxLayout();
    QSplitter* pSpliter = new QSplitter(this);
    pSpliter->setChildrenCollapsible(false);
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
    m_pBrowseBtn = new QPushButton(u8"浏览(&B)...");
    pPathEditLayout->addWidget(m_pBrowseBtn);
    pLeftLayout->addLayout(pPathEditLayout);
    pLeftLayout->addWidget(new QLabel(u8"扫描文件的格式："));
    pLeftLayout->addWidget(m_pFileTypeCombo = new QComboBox());

    QGroupBox* pOptionGroupBox = new QGroupBox(u8"删除注释选项");
    QHBoxLayout* pOptionsLayout = new QHBoxLayout();
    pOptionGroupBox->setLayout(pOptionsLayout);
    pOptionsLayout->addWidget(m_pRemoveSpace = new QCheckBox(u8"移除多余的空格"));
    pOptionsLayout->addWidget(m_pRemoveReturn = new QCheckBox(u8"移除多余的空白行"));
    pOptionsLayout->addWidget(new QLabel(u8"保留连续的空白行的个数："));
    pOptionsLayout->addWidget(m_pReturnNumEdit = new QSpinBox());
    m_pReturnNumEdit->setMinimumWidth(DPI(40));
    m_pReturnNumEdit->setRange(0, 4);
    pOptionsLayout->addStretch();
    pLeftLayout->addWidget(pOptionGroupBox);

    pLeftLayout->addWidget(new QLabel(u8"文件列表："));
    QHBoxLayout* pHLayout2 = new QHBoxLayout();
    pLeftLayout->addLayout(pHLayout2);
    pHLayout2->addWidget(m_pFileTableView = new QTableView());
    QVBoxLayout* pVLayout2 = new QVBoxLayout();
    pHLayout2->addLayout(pVLayout2);
    pVLayout2->addWidget(m_pAddFileBtn = new QPushButton(u8"添加"));
    pVLayout2->addWidget(m_pRemoveFileBtn = new QPushButton(u8"移除"));
    pVLayout2->addWidget(m_pClearFileBtn = new QPushButton(u8"清空"));
    pVLayout2->addStretch();
    //添加进度条
    QHBoxLayout* pHLayout3 = new QHBoxLayout();
    pLeftLayout->addLayout(pHLayout3);
    pHLayout3->addWidget(m_pProgressBar = new QProgressBar());
    m_pProgressBar->setRange(0, 10000);
    m_pProgressBar->setMaximumHeight(DPI(16));
    m_pProgressBar->setTextVisible(false);
    pHLayout3->addWidget(m_pProgressLabel = new QLabel());
    m_pProgressBar->setVisible(false);
    m_pProgressLabel->setVisible(false);

    //创建右侧控件
    m_pAddCodeHeadWidget = new QWidget();
    pSpliter->addWidget(m_pAddCodeHeadWidget);
    QVBoxLayout* pRightLayout = new QVBoxLayout();
    m_pAddCodeHeadWidget->setLayout(pRightLayout);
    pRightLayout->addWidget(new QLabel(u8"输入文件头模板："));
    pRightLayout->addWidget(m_pTemplateTextEdit = new QTextEdit());
    QLabel* pInfoLabel = new QLabel(u8"可用宏说明：\r\n<%file_name%>: 文件名\r\n<%time[format]%>: 日期/时间，format为格式，例如“yyyyMMdd”");
    pInfoLabel->setWordWrap(true);
    pRightLayout->addWidget(pInfoLabel);
    QHBoxLayout* pOptionHLayout = new QHBoxLayout();
    pOptionHLayout->addWidget(new QLabel(u8"输出编码格式："));
    pOptionHLayout->addWidget(m_pOutFormatCombo = new QComboBox());
    pRightLayout->addLayout(pOptionHLayout);


    //m_pFolderPathEidt->setReadOnly(true);
    m_pFileTypeCombo->setEditable(true);

    //
    connect(m_pBrowseBtn, SIGNAL(clicked()), this, SLOT(OnBrowseBtnClicked()));
}

CMainWidget::~CMainWidget()
{
}

void CMainWidget::ShowAddCodeHeader(bool show)
{
    if (show)
        m_pAddCodeHeadWidget->show();
    else
        m_pAddCodeHeadWidget->hide();
}

void CMainWidget::SetProgress(double progress)
{
    bool showProgress = (progress > 0 && progress < 100);
    m_pProgressBar->setVisible(showProgress);
    m_pProgressLabel->setVisible(showProgress);

    if (showProgress)
    {
        m_pProgressBar->setValue(static_cast<int>(progress * 100));
        m_pProgressLabel->setText(QString(u8"正在处理……已完成 %1%").arg(progress, 0, 'f', 1));
    }
}

void CMainWidget::OnBrowseBtnClicked()
{
    QString strNewPath = QFileDialog::getExistingDirectory(this, QString(), m_pFolderPathEidt->text());
    if (!strNewPath.isEmpty())
        m_pFolderPathEidt->setText(strNewPath);
}
