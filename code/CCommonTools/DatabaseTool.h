#ifndef CDATABASEOPERATOR_H
#define CDATABASEOPERATOR_H

#include <QtSql/QSqlDatabase>
#include <QMap>
#include <QVariant>
#include <QVariantList>
#include <QtSql/QSqlQuery>

//用于数据库读写的基类
//继承此类并重写虚函数后可以实现对数据库中一张表的读写
class CDataBaseTool
{
public:
    CDataBaseTool();
    CDataBaseTool(const QSqlDatabase& database);

public:
    virtual const char* GetTableName() = 0;         //获取表的名称
    virtual void GetTableColumeList(QMap<QString, QString>& mapColList) = 0;          //获取表的列，key是列的名称，value是类型
    virtual const char* GetPrimaryKey() = 0;        //获取主键的名称

    void CreateTable();         //初始化数据库表

    //执行一句SQL语句，成功返回true，否则返回false
    bool ExecuteSql(const QString& strSql);

    //执行一句SQL语句，返回QSqlQuery对象
    QSqlQuery ExecuteSql2(const QString& strSql);

    //查询一个值
    //primaryValue：主键的值
    //strKey：要查询的值
    virtual QVariant QueryValue(QVariant primaryValue, const char* strKey);

    //查询表中所有项目的主键
    void QueryPrimaryKeyList(QVariantList& keyList);

    //查询整个表格
    void QueryWholeTable(QList<QMap<QString, QString>>& listTable);

    //通过某个键键值查询数据
    void QueryValueByFiled(const char* strFiled, const QString& strValue, QList<QMap<QString, QString>>& resList);


    //插入一行记录
    //mapColValues：该行每一列的值。key：数据库表格列的字段，value：字段的值
    virtual bool AddRecord(const QMap<QString, QString>& mapColValues);

    //删除一行记录
    bool DeleteRecord(QVariant primaryValue);

    //判断一个记录是否存在于表中
    bool IsRecordExist(QVariant primaryValue);

    //更新一个值
    //primaryValue：数据行的主键
    //filed：要更新的字段
    //value：更新后的值
    virtual bool UpdateRecode(QVariant primaryValue, const char* filed, const QString& value);

    QVector<QString> GetFiled();

private:
    QSqlDatabase m_database;

};

#endif // CDATABASEOPERATOR_H
