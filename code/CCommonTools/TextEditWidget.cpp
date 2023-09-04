#include "TextEditWidget.h"
#include <QTextEdit>
#include <QVBoxLayout>

CTextEditWidget::CTextEditWidget(QWidget*parent)
    : QWidget(parent)
{
    QVBoxLayout* pLayout = new QVBoxLayout;
    setLayout(pLayout);
    pLayout->addWidget(m_label = new QLabel(this));
    pLayout->addWidget(m_edit = new QTextEdit(this));
    m_label->hide();
}

CTextEditWidget::~CTextEditWidget()
{}

void CTextEditWidget::SetLabelText(const QString& text)
{
    m_label->setVisible(!text.isEmpty());
    m_label->setText(text);
}

QTextEdit* CTextEditWidget::GetEdit()
{
    return m_edit;
}
