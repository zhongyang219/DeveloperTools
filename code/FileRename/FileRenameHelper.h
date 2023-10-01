#pragma once
#include <QString>
class FileRenameHelper
{
public:
	static bool FileRenameByCreateTime(const QString& filePath, QString& newFileName, const QString& strPrefex = QString(), bool autoResoveConflict = false);

	//判断一个字符串是否是数字
	static bool StrIsNumber(const QString& str);

	//判断文件名是末尾是否符合“(数字)”的形式
	//file_name: 要判断的文件名，不包含扩展名
	//number: 接收括号中的数字
	//index: 接收左括号在字符串中的索引
	static bool IsFileNameNumbered(const QString& file_name, int& number, int& index);

	//对一个文件路径自动重命名，如果file_path存在，则在其后面加上“(1)”，如果文件名后面存在带括号的数字的形式，则括号内的数字加1
	static void FileAutoRename(QString& file_path);

private:
};

