#pragma once

#include "ccommontools_global.h"
#include <QFileInfo>
#include <QString>

class CCommonTools
{
public:
    CCommonTools();

    static QFileInfoList FindFile(const QString &strFilePath, const QStringList &nameFilters);
};
