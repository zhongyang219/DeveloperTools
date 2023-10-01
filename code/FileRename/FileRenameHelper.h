#pragma once
#include <QString>
class FileRenameHelper
{
public:
	static bool FileRenameByCreateTime(const QString& filePath, QString& newFileName, const QString& strPrefex = QString(), bool autoResoveConflict = false);

	//�ж�һ���ַ����Ƿ�������
	static bool StrIsNumber(const QString& str);

	//�ж��ļ�����ĩβ�Ƿ���ϡ�(����)������ʽ
	//file_name: Ҫ�жϵ��ļ�������������չ��
	//number: ���������е�����
	//index: �������������ַ����е�����
	static bool IsFileNameNumbered(const QString& file_name, int& number, int& index);

	//��һ���ļ�·���Զ������������file_path���ڣ������������ϡ�(1)��������ļ���������ڴ����ŵ����ֵ���ʽ���������ڵ����ּ�1
	static void FileAutoRename(QString& file_path);

private:
};

