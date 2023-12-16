#pragma once
#include "CommonData.h"
class CFileHeadHelper
{
public:
    CFileHeadHelper();
    ~CFileHeadHelper();

    bool AddFileHead(const QString& filePath, eOutputFormat outputFormat);

private:
    eOutputFormat JudgeCode(const QByteArray& data, bool& hasBom);

    //�Ƴ��ļ�ԭ�����ļ�ͷ
    void RemoveFileHead(QString& strContents);

    //�����ļ�ͷ������
    QString GenerateFileHeadContents(const QString& filePath) const;

    //�����ļ�ͷ���ַ�������ÿ�����з�ǰ����ӡ�* ��
    static void ProcessHeadString(QString& str);
};

