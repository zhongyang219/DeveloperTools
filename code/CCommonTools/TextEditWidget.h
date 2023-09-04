#pragma once

#include "ccommontools_global.h"
#include <QWidget>
#include <QTextEdit>
#include <QLabel>

class CCOMMONTOOLS_EXPORT CTextEditWidget  : public QWidget
{
public:
    CTextEditWidget(QWidget* parent = nullptr);
    ~CTextEditWidget();
    void SetLabelText(const QString& text);
    QTextEdit* GetEdit();

private:
    QTextEdit* m_edit;
    QLabel* m_label;
};
