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

    //移除文件原来的文件头
    void RemoveFileHead(QString& strContents);

    //生成文件头的内容
    QString GenerateFileHeadContents(const QString& filePath) const;

    //处理文件头的字符串，在每个换行符前面添加“* ”
    static void ProcessHeadString(QString& str);
};

