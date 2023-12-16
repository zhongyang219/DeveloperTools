#pragma once

#include <QLineEdit>
#include "ccommontools_global.h"

class CCOMMONTOOLS_EXPORT CLongTextEdit : public QLineEdit
{
    Q_OBJECT

public:
    CLongTextEdit(QWidget *parent);
    ~CLongTextEdit();

protected slots :
    void OnActionTriggerd();
};
