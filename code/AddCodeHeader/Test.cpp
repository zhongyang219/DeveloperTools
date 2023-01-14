#include "Test.h"
#include "RemoveCommentHelper.h"


CTest::CTest()
{
}


CTest::~CTest()
{
}

void CTest::Test()
{
    TestRemoveComment();
}

void CTest::TestRemoveComment()
{
    QByteArray strTest("	int a = 0;\r\n	//aaaaaaa\r\n	printf(\"aaa//bbb\");	//printf\r\n  printf(\"ccc//ddd\"); //\"printf\"\r\n  printf(\"ccc//ddd\");\r\n  printf(\"eeee//ff\");	//i\"444\"//aa\"ccc\r\n/**/\r\n	return 0;\r\n");
    QByteArray strRemoveComment("	int a = 0;\r\n\r\n	printf(\"aaa//bbb\");\r\n  printf(\"ccc//ddd\");\r\n  printf(\"ccc//ddd\");\r\n  printf(\"eeee//ff\");\r\n\r\n	return 0;\r\n");
    CRemoveCommentHelper::RemoveResult result;
    CRemoveCommentHelper::RemoveComment(strTest, true, true, 2, result);
    Q_ASSERT(strTest == strRemoveComment);
}
