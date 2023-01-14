#pragma once

#include <QThread>
#include "RemoveCommentHelper.h"

class CRemoveCommentThread : public QThread
{
    Q_OBJECT

public:
    CRemoveCommentThread(QObject *parent = nullptr);
    ~CRemoveCommentThread();

    void Stop();
    bool IsStopped();

public:
    CRemoveCommentHelper::RemoveResult m_removeResult;
    int m_fileCount{};

private:
    bool m_stoped{ false };

protected:
    virtual void run() override;
};
