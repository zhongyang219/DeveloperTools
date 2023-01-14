#pragma once

#include "ccommontools_global.h"
#include <QFileInfo>
#include <QString>
#include <QAction>
#include <QMainWindow>

class CCommonTools
{
public:
    CCommonTools();

    static QFileInfoList FindFile(const QString &strFilePath, const QStringList &nameFilters);

    static QMainWindow* GetMainWindow();
    static QAction* GetMainFrameAction(const QString& strCmdId);
    static void SetActionEnable(const QString& strCmdId, bool enable);
};
