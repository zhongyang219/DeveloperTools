﻿#pragma once

#include "ccommontools_global.h"
#include <QFileInfo>
#include <QString>
#include <QAction>
#include <QMainWindow>

class CCOMMONTOOLS_EXPORT CCommonTools
{
public:
    CCommonTools();

    static QFileInfoList FindFile(const QString &strFilePath, const QStringList &nameFilters);

    static QMainWindow* GetMainWindow();
    static QAction* GetMainFrameAction(const QString& strCmdId);
    static void SetActionEnable(const QString& strCmdId, bool enable);
    static void WriteLog(const QString& strLogInfo, const QString& strFilePath = QString());

    static QIcon CreateIcon(const QString& strPath, int size);
};
