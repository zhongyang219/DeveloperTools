#pragma once

#include <QDialog>
#include "ccommontools_global.h"

class CCOMMONTOOLS_EXPORT DialogBase  : public QDialog
{
public:
	DialogBase(QString dialogName = QString(), QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
	virtual ~DialogBase();
	bool IsDialogSizeLoadSucceed() const;

private:
	QString m_dialogName;
	bool m_sizeLoadSucceed{};
};
