#include "FileHeadHelper.h"
#include <QFile>
#include "../CCommonTools/CommonTools.h"
#include <QTextStream>
#include "AddCodeHeader.h"
#include <QFileInfo>
#include <QDateTime>

const QString FILE_HEAD_START = "/**************************************************************************************";
const QString FILE_HEAD_END = "**************************************************************************************/";

CFileHeadHelper::CFileHeadHelper()
{
}


CFileHeadHelper::~CFileHeadHelper()
{
}

bool CFileHeadHelper::AddFileHead(const QString& filePath, eOutputFormat outputFormat)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
        return false;
    QByteArray fileData = file.readAll();
    file.close();
    //�жϱ�������
    bool hasBom;
    eOutputFormat codeType = JudgeCode(fileData, hasBom);
    //�Ƴ��ļ�BOM
    if (hasBom)
    {
        if (codeType == OF_UTF8)
            fileData.remove(0, 3);
        else if (codeType == OF_UTF16)
            fileData.remove(0, 2);
    }
    //���ݱ����ʽת��ΪQString
    QString fileContents;
    if (codeType == OF_UTF8)
    {
        fileContents = QString::fromUtf8(fileData);
    }
    else if (codeType == OF_UTF16)
    {
        fileContents = QString::fromWCharArray((const wchar_t*)fileData.constData());
    }
    else
    {
        fileContents = QString::fromLocal8Bit(fileData);
    }
    //ȥ��ԭ�����ļ�ͷ
    RemoveFileHead(fileContents);
    //����ļ�ͷ
    QString strFileHead = GenerateFileHeadContents(filePath);
    fileContents.push_front(strFileHead);
    //���������ʽת����QByteArray
    QByteArray outputData;
    if (outputFormat == OF_UTF8)
    {
        if (hasBom)
        {
            outputData.append('\xef');
            outputData.append('\xbb');
            outputData.append('\xbf');
        }
        outputData.append(fileContents.toUtf8());
    }
    else if (outputFormat == OF_UTF16)
    {
        outputData.append('\xff');
        outputData.append('\xfe');
        std::wstring contentsWcs = fileContents.toStdWString();
        outputData.append((const char*)contentsWcs.c_str(), contentsWcs.size() * 2);
    }
    else
    {
        outputData.append(fileContents.toLocal8Bit());
    }
    //д���ļ�
    if (!file.open(QIODevice::WriteOnly))
        return false;
    file.write(outputData);
    file.close();
    return true;
}

eOutputFormat CFileHeadHelper::JudgeCode(const QByteArray& data, bool& hasBom)
{
    hasBom = false;
    if (data.size() >= 3 && data[0] == '\xef' && data[1] == '\xbb' && data[2] == '\xbf')
    {
        hasBom = true;
        return OF_UTF8;
    }
    if (data.size() >= 2 && data[0] == '\xff' && data[1] == '\xfe')
    {
        hasBom = false;
        return OF_UTF16;
    }
    if (CCommonTools::IsUtf8Bytes(data))
    {
        return OF_UTF8;
    }
    return OF_ANSI;
}

void CFileHeadHelper::RemoveFileHead(QString& strContents)
{
    //�Ƴ��ļ�ͷ
    if (strContents.startsWith(FILE_HEAD_START))
    {
        int endIndex = strContents.indexOf(FILE_HEAD_END, FILE_HEAD_START.size());
        if (endIndex > 0)
            strContents.remove(0, endIndex + FILE_HEAD_END.size());
    }
    //ɾ���ļ���ǰ���һ�����з�
    if (strContents.startsWith('\r'))
        strContents = strContents.mid(1);
    if (strContents.startsWith('\n'))
        strContents = strContents.mid(1);
}

QString CFileHeadHelper::GenerateFileHeadContents(const QString& filePath) const
{
    QFileInfo fileInfo(filePath);
    QString str;
    const auto& data = AddCodeHeader::GetInstance()->GetEditor().GetHeadItemTableModel().GetData();
    QTextStream contents(&str);
    contents << FILE_HEAD_START << endl;
    //��������ļ�ͷ
    for (const auto& headItem : data)
    {
        if (!headItem.enable)
            continue;
        switch (headItem.item)
        {
        case HI_COPYRIGHT:
            contents << "* @copyright   ";
            break;
        case HI_FILE:
            contents << "* @file        ";
            break;
        case HI_BRIEF:
            contents << "* @brief       ";
            break;
        case HI_AUTHOR:
            contents << "* @author      ";
            break;
        case HI_EMAL:
            contents << "* @email       ";
            break;
        case HI_VERSION:
            contents << "* @version     ";
            break;
        case HI_DATE:
            contents << "* @date        ";
            break;
        case HI_LICENSE:
            contents << "* @license     ";
            break;
        default:
            contents << "* ";
            break;
        }

        if (headItem.item == HI_FILE)
        {
            contents << fileInfo.fileName() << endl;
        }
        else if (headItem.item == HI_DATE)
        {
            QDateTime fileDate = fileInfo.lastModified();
            contents << fileDate.toString("yyyy-MM-dd") << endl;
        }
        else
        {
            QString strValue = headItem.value;
            ProcessHeadString(strValue);
            contents << strValue << endl;
        }
    }
    contents << FILE_HEAD_END << endl;
    return str;
}

void CFileHeadHelper::ProcessHeadString(QString& str)
{
    int index = -1;
    while (true)
    {
        index = str.indexOf('\n', index + 1);
        if (index >= 0)
            str.insert(index + 1, "* ");
        else
            break;
    }
}
