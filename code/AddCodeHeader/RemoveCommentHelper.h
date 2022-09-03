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

        void operator+=(const RemoveResult result)
        {
            single_line_comment_removed += result.single_line_comment_removed;
            multi_line_comment_removed += result.multi_line_comment_removed;
            space_removed += result.space_removed;
            return_removed += result.return_removed;
        }
    };

    static bool RemoveComment(const QString& file_path, bool bRemoveSpace, bool bRemoveReturn, RemoveResult& result);

private:
    static void RemoveComment(QByteArray& file_contents, bool bRemoveSpace, bool bRemoveReturn, RemoveResult& result);

};
