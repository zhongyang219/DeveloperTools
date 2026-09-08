#pragma once
#include <QString>
#include <QByteArray>
class CRemoveCommentHelper
{
public:
    CRemoveCommentHelper();

    //移除注释的结果
    struct RemoveResult
    {
        int single_line_comment_removed{};
        int multi_line_comment_removed{};
        int space_removed{};
        int return_removed{};
        int comment_num_moved{};

        void operator+=(const RemoveResult result)
        {
            single_line_comment_removed += result.single_line_comment_removed;
            multi_line_comment_removed += result.multi_line_comment_removed;
            space_removed += result.space_removed;
            return_removed += result.return_removed;
            comment_num_moved += result.comment_num_moved;
        }
    };

    struct FormatPara
    {
        bool removeComment = false;
        bool removeSpace = false;
        bool removeReturn = false;
        int keepReturnNum = 0;
        bool commonToIndividualLine = false;
    };

    static bool CodeFileFormat(const QString& file_path, FormatPara para, RemoveResult& result);
    static void RemoveComment(QByteArray& file_contents, bool removeComment, bool bRemoveSpace, bool bRemoveReturn, int keepReturnNum, RemoveResult& result);

    //将注释移动到单独的行
    static void CommentToIndividualLine(QByteArray& file_contents, RemoveResult& result);


private:
    static int FindFirstOf(const QByteArray& contents, const QByteArray& strFind, int index);
    static int FindStringNotInQuotation(const QByteArray& contents, const char* strFind, int index);		//查找指定字符串，但是确保指定的字符串不在引号内
};
