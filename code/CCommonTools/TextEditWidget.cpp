#include "TextEditWidget.h"
#include <QTextEdit>
#include <QVBoxLayout>

CTextEditDialog::CTextEditDialog(QWidget*parent)
    : QDialog(parent)
{
    QVBoxLayout* pLayout = new QVBoxLayout;
    setLayout(pLayout);
    pLayout->addWidget(m_label = new QLabel(this));
    pLayout->addWidget(m_edit = new QTextEdit(this));
    m_label->hide();
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
