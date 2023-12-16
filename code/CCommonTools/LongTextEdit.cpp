#include "LongTextEdit.h"
#include <QIcon>
#include <QAction>
#include "TextEditWidget.h"

CLongTextEdit::CLongTextEdit(QWidget *parent)
    : QLineEdit(parent)
{
    QAction* pEditAction = addAction(QIcon(":/res/more.png"), QLineEdit::TrailingPosition);
    connect(pEditAction, SIGNAL(triggered(bool)), this, SLOT(OnActionTriggerd()));
}

CLongTextEdit::~CLongTextEdit()
{
}

void CLongTextEdit::OnActionTriggerd()
{
    CTextEditDialog dlg(this, true);
    dlg.setWindowTitle(u8"±à¼­ÎÄ±¾");
    dlg.GetEdit()->setPlainText(text());
    if (dlg.exec() == QDialog::Accepted)
    {
        setText(dlg.GetEdit()->toPlainText());
    }
}
