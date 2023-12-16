#pragma once

#include "ccommontools_global.h"
#include <QDialog>
#include <QTextEdit>
#include <QLabel>

class CCOMMONTOOLS_EXPORT CTextEditDialog  : public QDialog
{
public:
    CTextEditDialog(QWidget* parent = nullptr, bool showButtons = false);
    ~CTextEditDialog();
    void SetLabelText(const QString& text);
    QTextEdit* GetEdit();

private:
    QTextEdit* m_edit;
    QLabel* m_label;
};
