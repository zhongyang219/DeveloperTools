#pragma once

#include <QThread>
#include "FileHeadHelper.h"

class CAddFileHeadThread : public QThread
{
    Q_OBJECT

public:
    CAddFileHeadThread(QObject *parent = nullptr);
    ~CAddFileHeadThread();

    void Stop();
    bool IsStopped();

public:
    int m_fileCount{};

private:
    bool m_stoped{ false };
    CFileHeadHelper m_helper{};

protected:
    virtual void run() override;
};
