#include "TextEditWidget.h"
#include <QTextEdit>
#include <QVBoxLayout>
#include <QPushButton>

CTextEditDialog::CTextEditDialog(QWidget*parent, bool showButtons)
    : QDialog(parent)
{
    QVBoxLayout* pLayout = new QVBoxLayout;
    setLayout(pLayout);
    pLayout->addWidget(m_label = new QLabel(this));
    pLayout->addWidget(m_edit = new QTextEdit(this));
    m_label->hide();
    if (showButtons)
    {
        QHBoxLayout* pButtonsLayout = new QHBoxLayout;
        pLayout->addLayout(pButtonsLayout);
        QPushButton* pOkBtn = new QPushButton(u8"确定", this);
        QPushButton* pCancelBtn = new QPushButton(u8"取消", this);
        connect(pOkBtn, SIGNAL(clicked()), this, SLOT(accept()));
        connect(pCancelBtn, SIGNAL(clicked()), this, SLOT(reject()));
        pButtonsLayout->addStretch();
        pButtonsLayout->addWidget(pOkBtn);
        pButtonsLayout->addWidget(pCancelBtn);
    }
}

CTextEditDialog::~CTextEditDialog()
{}

void CTextEditDialog::SetLabelText(const QString& text)
{
    m_label->setVisible(!text.isEmpty());
    m_label->setText(text);
}

QTextEdit* CTextEditDialog::GetEdit()
{
    return m_edit;
}
