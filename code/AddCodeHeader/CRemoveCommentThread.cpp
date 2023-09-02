#include "CRemoveCommentThread.h"
#include "AddCodeHeader.h"

CRemoveCommentThread::CRemoveCommentThread(QObject *parent)
    : QThread(parent)
{
}

CRemoveCommentThread::~CRemoveCommentThread()
{
}

void CRemoveCommentThread::Stop()
{
    m_stoped = true;
}

bool CRemoveCommentThread::IsStopped()
{
    return m_stoped;
}

void CRemoveCommentThread::run()
{
    m_removeResult = CRemoveCommentHelper::RemoveResult();
    m_fileCount = AddCodeHeader::GetInstance()->GetEditor().RemoveComments(m_removeResult);
}
