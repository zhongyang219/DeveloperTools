#include "RemoveCommentHelper.h"
#include <QFile>

CRemoveCommentHelper::CRemoveCommentHelper()
{
}

bool CRemoveCommentHelper::RemoveComment(const QString& file_path, bool bRemoveSpace, bool bRemoveReturn, RemoveResult& result)
{
    QFile file(file_path);
    if (!file.open(QFile::ReadOnly))
        return false;
    QByteArray file_contents = file.readAll();
    file.close();

    RemoveComment(file_contents, bRemoveSpace, bRemoveReturn, result);

    file.setFileName(file_path);
    if (!file.open(QFile::WriteOnly))
        return false;
    file.write(file_contents);
    file.close();
    return true;
}

void CRemoveCommentHelper::RemoveComment(QByteArray& file_contents, bool bRemoveSpace, bool bRemoveReturn, RemoveResult& result)
{
    result = RemoveResult();
    //删除“//”
    int index1 = -1, index2 = -1;
    while (true)
    {
        index1 = file_contents.indexOf("//", index1 + 1);
        index2 = file_contents.indexOf("\n", index1 + 1);
        if (index1 < 0 || index2 < 0)
            break;
        file_contents.remove(index1, index2 - index1);
        result.single_line_comment_removed++;
    }
    //删除“/**/”
    index1 = -1, index2 = -1;
    while (true)
    {
        index1 = file_contents.indexOf("/*", index1 + 1);
        index2 = file_contents.indexOf("*/", index1 + 1);
        if (index1 < 0 || index2 < 0)
            break;
        file_contents.remove(index1, index2 - index1 + 2);
        result.multi_line_comment_removed++;
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
    }

    //移除多余的回车
    if (bRemoveReturn)
    {
        while (file_contents.contains("\r\n\r\n\r\n\r\n"))
        {
            file_contents.replace("\r\n\r\n\r\n\r\n", "\r\n\r\n\r\n");
            result.return_removed++;
        }
        while (file_contents.contains("\n\n\n\n"))
        {
            file_contents.replace("\n\n\n\n", "\n\n\n");
            result.return_removed++;
        }
    }
}
