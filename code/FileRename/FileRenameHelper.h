#pragma once
#include <QString>
class FileRenameHelper
{
public:
    /**
     * @brief      使用文件创建时间重命名
     * @param[in]  filePath 文件路径
     * @param[out] newFileName 接收新文件名
     * @param[in]  strPrefex 添加的前缀
     * @param[in]  autoResoveConflict 是否自动解决命名冲突
     * @return     是否成功
     */
	static bool FileRenameByCreateTime(const QString& filePath, QString& newFileName, const QString& strPrefex, bool autoResoveConflict);

    /**
     * @brief      使用数字编号重命名
     * @param[in]  filePath 文件路径
     * @param[in]  nuber 数字编号
     * @param[in]  digit 位数
     * @param[out] newFileName 接收新文件名
     * @param[in]  strPrefex 添加的前缀
     * @param[in]  autoResoveConflict 是否自动解决命名冲突
     * @return     是否成功
     */
    static bool FileRenameByNumber(const QString& filePath, int nuber, int digit, QString& newFileName, const QString& strPrefex, bool autoResoveConflict);

    /**
     * @brief      对文件名进行替换
     * @param[in]  filePath 文件路径
     * @param[in]  findStr 要查找的字符串
     * @param[in]  replaceStr 替换的字符串
     * @param[out] newFileName 接收新文件名
     * @param[in]  autoResoveConflict 是否自动解决命名冲突
     * @return     是否成功
     */
    static bool FileRenameReplace(const QString& filePath, const QString& findStr, const QString& replaceStr, QString& newFileName, bool autoResoveConflict);

	//判断一个字符串是否是数字
	static bool StrIsNumber(const QString& str);

    /**
     * @brief      判断文件名是末尾是否符合“(数字)”的形式
     * @param[in]  file_name 要判断的文件名，不包含扩展名
     * @param[out] number 接收括号中的数字
     * @param[out] index 接收左括号在字符串中的索引
     * @return     成功返回true，否则返回false
     */
	static bool IsFileNameNumbered(const QString& file_name, int& number, int& index);

    /**
     * @brief      根据一个文件的路径自动解决命名冲突。如果file_path存在，则在其后面加上“(1)”，如果文件名后面存在带括号的数字的形式，则括号内的数字加1
     * @param[in][out]  file_path 文件的路径
     */
	static void FileAutoResoveConfict(QString& file_path);

private:
    /**
     * @brief      对一个文件重命名
     * @param[in]  filePath 文件路径
     * @param[in]  newFileName 新文件名（含扩展名）
     * @param[in]  autoResoveConflict 是否自动解决命名冲突
     * @return     
     */
    static bool FileRename(const QString& filePath, QString& newFileName, bool autoResoveConflict);
};

