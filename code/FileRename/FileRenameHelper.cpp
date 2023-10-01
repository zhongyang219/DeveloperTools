#include "FileRenameHelper.h"
#include <QFileInfo>
#include <QDateTime>
#include <QDir>

bool FileRenameHelper::FileRenameByCreateTime(const QString& filePath, QString& newFileName, const QString& strPrefex, bool autoResoveConflict)
{
	QFileInfo fileInfo(filePath);
	if (!fileInfo.isFile())
		return false;

	QDateTime lastModified = fileInfo.lastModified();
	newFileName = strPrefex + lastModified.toString("yyyyMMdd_hhmmss");
	newFileName += '.';
	newFileName += fileInfo.completeSuffix();
	QString newFilePath = fileInfo.dir().filePath(newFileName);
	if (autoResoveConflict)
	{
		FileAutoRename(newFilePath);
		newFileName = QFileInfo(newFilePath).fileName();
	}
	if (filePath == newFilePath)
		return true;
	return QFile::rename(filePath, newFilePath);
}

bool FileRenameHelper::StrIsNumber(const QString& str)
{
	if (str.isEmpty())
		return false;
	for (const auto& ch : str)
	{
		if (ch < '0' || ch > '9')
			return false;
	}
	return true;
}

bool FileRenameHelper::IsFileNameNumbered(const QString& file_name, int& number, int& index)
{
	if (file_name.isEmpty())
		return false;
	if (file_name.back() != ')')
		return false;
	int size{ file_name.size() };
	index = file_name.lastIndexOf('(');		//��ǰ����������
	if (index <= 0)		//�����Ų������ַ�����ͷ
		return false;
	QString number_str{ file_name.mid(index + 1, size - index - 2) };		//��ȡ��������֮����ı�
	if (StrIsNumber(number_str))				//�ж��ı��Ƿ�������
	{
		number = number_str.toInt();
		return true;
	}
	else
	{
		return false;
	}
}

void FileRenameHelper::FileAutoRename(QString& file_path)
{
	while (QFileInfo(file_path).isFile())
	{
		//�ж��ļ�����ĩβ�Ƿ���ϡ�(����)������ʽ
		QString file_name;		//�ļ�����������չ����
		QFileInfo c_file_path(file_path);
		file_name = c_file_path.baseName();
		QString ext{ c_file_path.completeSuffix() };
		int num;
		int index;
		bool is_numbered{ IsFileNameNumbered(file_name, num, index) };		//�ļ�����ĩβ�Ƿ���ϡ�(����)������ʽ
		if (!is_numbered)		//����ļ���ĩβû�С�(����)��������ĩβ��ӡ� (1)��
		{
			file_name += " (1)";
		}
		else		//���򣬽�ԭ�������ּ�1
		{
			file_name = file_name.mid(0, index);
			QString num_str = QString("(%d)").arg(num + 1);
			file_name += num_str;
		}
		file_path = c_file_path.dir().filePath(file_name + '.' + ext);
	}
}
