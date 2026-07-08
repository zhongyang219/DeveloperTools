#pragma once
#include "ccommontools_global.h"
#include <QSettings>

class CCOMMONTOOLS_EXPORT CConfig
{
public:
    CConfig(const QString& strModuleName = QString());
    ~CConfig();

    void WriteValue(const QString& strKeyName, const QVariant& value);
    QVariant GetValue(const QString& strKeyName, const QVariant& defaultValue = QVariant()) const;

private:
    QString GetKeyName(const QString strKeyName) const;

private:
    QSettings* m_settings{};
    QString m_module_name;
};

