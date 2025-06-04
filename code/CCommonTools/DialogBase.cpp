#include "DialogBase.h"
#include "Config.h"

DialogBase::DialogBase(QString dialogName, QWidget* parent, Qt::WindowFlags f)
	: QDialog(parent, f),
	m_dialogName(dialogName)
{
	//载入窗口大小
	if (!m_dialogName.isEmpty())
	{
		CConfig config;
		int width = config.GetValue(QString("DialogSize/%1/Width").arg(m_dialogName)).toInt();
		int height = config.GetValue(QString("DialogSize/%1/Height").arg(m_dialogName)).toInt();
		if (width > 0 && height > 0)
		{
			resize(width, height);
			m_sizeLoadSucceed = true;
		}
	}
}

DialogBase::~DialogBase()
{
	//保存窗口大小
	if (!m_dialogName.isEmpty())
	{
		CConfig config;
		QSize windowSize = size();
		config.WriteValue(QString("DialogSize/%1/Width").arg(m_dialogName), windowSize.width());
		config.WriteValue(QString("DialogSize/%1/Height").arg(m_dialogName), windowSize.height());
	}
}

bool DialogBase::IsDialogSizeLoadSucceed() const
{
	return m_sizeLoadSucceed;
}
