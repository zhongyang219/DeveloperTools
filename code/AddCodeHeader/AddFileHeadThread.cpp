#include "AddFileHeadThread.h"
#include "AddCodeHeader.h"

CAddFileHeadThread::CAddFileHeadThread(QObject *parent)
    : QThread(parent)
{
}

CAddFileHeadThread::~CAddFileHeadThread()
{
}

void CAddFileHeadThread::Stop()
{
    m_stoped = true;
}

bool CAddFileHeadThread::IsStopped()
{
    return m_stoped;
}

void CAddFileHeadThread::run()
{
    //m_removeResult = CRemoveCommentHelper::RemoveResult();
    //m_fileCount = AddCodeHeader::GetInstance()->GetEditor().RemoveComments(m_removeResult);
    QStringList filePathList = AddCodeHeader::GetInstance()->GetEditor().GetFilePathList();
    m_fileCount = 0;
    eOutputFormat outputFormat = AddCodeHeader::GetInstance()->GetEditor().GetOutputFormat();
    for (const QString& filePath : filePathList)
    {
        m_helper.AddFileHead(filePath, outputFormat);
        m_fileCount++;
        AddCodeHeader::GetInstance()->GetEditor().signalRemoveCommentProgress(m_fileCount * 100 / filePathList.size());
    }
}
