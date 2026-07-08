#include "Config.h"
#include "define.h"

CConfig::CConfig(const QString& strModuleName)
{
#ifdef Q_OS_WIN
    QString strConfigPath = QString("HKEY_CURRENT_USER\\Software\\Apps By ZhongYang\\%1").arg(APP_NAME);
#ifdef QT_DEBUG
    strConfigPath += " (Debug)";
#endif
    if (!strModuleName.isEmpty())
    {
        strConfigPath += '\\';
        strConfigPath += strModuleName;
    }
    m_settings = new QSettings(strConfigPath, QSettings::NativeFormat);
#else
    m_settings = new QSettings("Apps By ZhongYang", APP_NAME);
    m_module_name = strModuleName;
#endif
}

CConfig::~CConfig()
{
    SAFE_DELETE(m_settings);
}

QString CConfig::GetKeyName(const QString strKeyName) const
{
    if (!m_module_name.isEmpty())
    {
        return m_module_name + '/' + strKeyName;
    }
    else
    {
        return strKeyName;
    }
}

void CConfig::WriteValue(const QString& strKeyName, const QVariant& value)
{
    if (value.type() == QVariant::Bool)     //由于bool类型保存到注册表会转换成字符串，这里将它转换成int
        m_settings->setValue(GetKeyName(strKeyName), value.toInt());
    else
        m_settings->setValue(GetKeyName(strKeyName), value);
}

QVariant CConfig::GetValue(const QString& strKeyName, const QVariant& defaultValue) const
{
    return m_settings->value(GetKeyName(strKeyName), defaultValue);
}
