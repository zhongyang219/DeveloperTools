#include "RemoveCommentHelper.h"
#include <QFile>
#include <QRegExp>

CRemoveCommentHelper::CRemoveCommentHelper()
{
}

bool CRemoveCommentHelper::RemoveFileComment(const QString& file_path, bool removeComment, bool bRemoveSpace, bool bRemoveReturn, int keepReturnNum, RemoveResult& result)
{
    QFile file(file_path);
    if (!file.open(QFile::ReadOnly))
        return false;
    QByteArray file_contents(file.readAll());
    file.close();

    RemoveComment(file_contents, removeComment, bRemoveSpace, bRemoveReturn, keepReturnNum, result);

    file.setFileName(file_path);
    if (!file.open(QFile::WriteOnly))
        return false;
    file.write(file_contents);
    file.close();
    return true;
}

void CRemoveCommentHelper::RemoveComment(QByteArray& file_contents, bool removeComment, bool bRemoveSpace, bool bRemoveReturn, int keepReturnNum, RemoveResult& result)
{
    result = RemoveResult();
    if (removeComment)
    {
        //删除“//”
        int index1 = -1, index2 = -1;
        while (true)
        {
            index1 = FindStringNotInQuotation(file_contents, "//", index1 + 1);
            index2 = FindFirstOf(file_contents, "\r\n", index1 + 1);
            if (index1 < 0 || index2 < 0)
                break;

            file_contents.remove(index1, index2 - index1);
            result.single_line_comment_removed++;
        }
        //删除“/**/”
        index1 = -1, index2 = -1;
        while (true)
        {
            index1 = FindStringNotInQuotation(file_contents, "/*", index1 + 1);
            index2 = FindStringNotInQuotation(file_contents, "*/", index1 + 1);
            if (index1 < 0 || index2 < 0)
                break;
            file_contents.remove(index1, index2 - index1 + 2);
            result.multi_line_comment_removed++;
        }
    }

    //移除多余的空格
    if (bRemoveSpace)
    {
        while (file_contents.contains(" \r\n"))
        {
            file_contents.replace(" \r\n", "\r\n");
            result.space_removed++;
        }
        while (file_contents.contains(" \n"))
        {
            file_contents.replace(" \n", "\n");
            result.space_removed++;
        }

        //移除多余的制表符
        while (file_contents.contains("\t\r\n"))
        {
            file_contents.replace("\t\r\n", "\r\n");
            result.space_removed++;
        }
        while (file_contents.contains("\t\n"))
        {
            file_contents.replace("\t\n", "\n");
            result.space_removed++;
        }
    }

    //移除多余的回车
    if (bRemoveReturn)
    {
        auto createString = [](const char* str, int repeatTimes) ->QByteArray
        {
            QByteArray strResult;
            for (int i = 0; i < repeatTimes; i++)
                strResult += str;
            return strResult;
        };
        while (file_contents.contains(createString("\r\n", keepReturnNum + 2)))
        {
            file_contents.replace(createString("\r\n", keepReturnNum + 2), createString("\r\n", keepReturnNum + 1));
            result.return_removed++;
        }
        while (file_contents.contains(createString("\n", keepReturnNum + 2)))
        {
            file_contents.replace(createString("\n", keepReturnNum + 2), createString("\n", keepReturnNum + 1));
            result.return_removed++;
        }
    }
}

int CRemoveCommentHelper::FindFirstOf(const QByteArray& contents, const QByteArray& strFind, int index)
{
    for (int i = index; i < contents.size(); i++)
    {
        if (strFind.contains(contents[i]))
            return i;
    }
    return -1;
}

int CRemoveCommentHelper::FindStringNotInQuotation(const QByteArray& contents, const char * strFind, int index)
{
    index--;
    while (true)
    {
        index = contents.indexOf(strFind, index + 1);
        if (index < 0)
            break;

        int indexPreviousReturn = contents.lastIndexOf('\n', index);
        //QByteArray curLine(contents.mid(indexPreviousReturn + 1, indexReturn));		//当前行

        //判断找到的字符串是否在引号内
        //如果找到的字符串前面的引号个数为偶数，则认为找到的字符串不在引号内
        if (contents.mid(indexPreviousReturn, index - indexPreviousReturn).count('\"') % 2 == 0)
            break;
    }
    return index;
}
