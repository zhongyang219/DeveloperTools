#include "DatabaseTool.h"
#include <QtSql/QSqlError>
#include <QVector>
#include <QtDebug>

CDataBaseTool::CDataBaseTool()
{

    //从文件打开数据库
    {
        const QString connName = "B9096853-8DC0-4CC3-9A88-99DBCCCE15BD";
        m_database = QSqlDatabase::addDatabase("QSQLITE", connName);
        m_database.setDatabaseName(".\\database.db");
        if (!m_database.open())
        {
            qDebug() << u8"本地数据库文件打开失败！";
        }
    }
}

CDataBaseTool::CDataBaseTool(const QSqlDatabase& database)
    : m_database(database)
{

}

void CDataBaseTool::CreateTable()
{
    if (m_database.isOpen())
    {
        //创建表
        //生成查询字符串
        QString strSql = "create table ";
        strSql += GetTableName();
        strSql += '(';

        QMap<QString, QString> mapColList;
        GetTableColumeList(mapColList);

        QMap<QString, QString>::const_iterator i = mapColList.constBegin();
        while (i != mapColList.constEnd())
        {
            strSql += i.key();      //表的列的名称
            strSql += ' ';
            strSql += i.value();     //表的列的类型
            strSql += ", ";
            ++i;
        }

        if (!mapColList.isEmpty())      //删除末尾多余的“, ”
        {
            strSql.chop(2);
        }

        strSql += ')';

        m_database.exec(strSql);
    }
}

bool CDataBaseTool::ExecuteSql(const QString &strSql)
{
    QSqlQuery query(m_database);
    bool rtn = query.exec(strSql);

    if (!rtn)   //SQL语句执行失败，写入错误日志
    {
        QString strErrorText = m_database.lastError().text();
        QString strInfo = u8"执行SQL语句“" + strSql + u8"”失败，错误信息：" + strErrorText + "\r\n";
        qDebug() << strInfo;
    }

    return rtn;
}

QSqlQuery CDataBaseTool::ExecuteSql2(const QString &strSql)
{
    QSqlQuery rtn = m_database.exec(strSql);

    QSqlError error = m_database.lastError();
    if (error.type() != QSqlError::NoError)   //SQL语句执行失败，写入错误日志
    {
        QString strInfo = u8"执行SQL语句“" + strSql + u8"”失败，错误信息：" + error.text() + "\r\n";
        qDebug() << strInfo;
    }

    return rtn;
}

QVariant CDataBaseTool::QueryValue(QVariant primaryValue, const char *strKey)
{
    //生成查询字符串
    QString strSql;
    strSql += "select ";
    strSql += strKey;
    strSql += " from ";
    strSql += GetTableName();
    strSql += " where ";
    strSql += GetPrimaryKey();
    strSql += " = '";
    strSql += primaryValue.toString();
    strSql += '\'';

    QVariant val;
    QSqlQuery query(strSql, m_database);

    if (query.exec())
    {
        if (query.next())
        {
            val = query.value(0);
        }
    }

    return val;

}

void CDataBaseTool::QueryPrimaryKeyList(QVariantList &keyList)
{
    keyList.clear();

    //生成查询字符串
    QString strSql;
    strSql += "select ";
    strSql += GetPrimaryKey();
    strSql += " from ";
    strSql += GetTableName();

    QSqlQuery query = ExecuteSql2(strSql);
    while (query.next())
    {
        QVariant val;
        val = query.value(0);
        keyList.append(val);
    }

}

void CDataBaseTool::QueryWholeTable(QList<QMap<QString, QString>>& listTable)
{
    listTable.clear();

    //生成查询字符串
    QString strSql;

    //获取列的字段
    QVector<QString> filedList = GetFiled();

    strSql += "select ";

    Q_FOREACH(const QString& strFiled, filedList)
    {
        strSql += strFiled;
        strSql += ", ";
    }
    if (!filedList.isEmpty())      //删除末尾多余的“, ”
    {
        strSql.chop(2);
    }

    strSql += " from ";
    strSql += GetTableName();

    QSqlQuery query = ExecuteSql2(strSql);
    while (query.next())
    {
        QMap<QString, QString> rowData;
        for (int i = 0; i< filedList.size(); i++)
        {
            QVariant val;
            val = query.value(i);
            rowData[filedList[i]] = val.toString();
        }
        listTable.append(rowData);
    }

}

void CDataBaseTool::QueryValueByFiled(const char *strFiled, const QString &strValue, QList<QMap<QString, QString>>& resList)
{
    resList.clear();

    //生成查询字符串
    QString strSql;

    //获取列的字段
    QVector<QString> filedList = GetFiled();
    strSql += "select ";

    Q_FOREACH(const QString& strFiled, filedList)
    {
        strSql += strFiled;
        strSql += ", ";
    }
    if (!filedList.isEmpty())      //删除末尾多余的“, ”
    {
        strSql.chop(2);
    }

    strSql += " from ";
    strSql += GetTableName();

    strSql += " where ";
    strSql += strFiled;
    strSql += " = \'";
    strSql += strValue;
    strSql += '\'';

    QSqlQuery query = ExecuteSql2(strSql);
    while (query.next())
    {
        QMap<QString, QString> rowData;
        for (int i = 0; i< filedList.size(); i++)
        {
            QVariant val;
            val = query.value(i);
            rowData[filedList[i]] = val.toString();
        }
        resList.append(rowData);
    }

}

bool CDataBaseTool::AddRecord(const QMap<QString, QString> &mapColValues)
{
    //生成查询字符串
    QString strSql;
    strSql += "insert into ";
    strSql += GetTableName();
    strSql += '(';

    QMap<QString, QString> mapColList;
    GetTableColumeList(mapColList);
    QMap<QString, QString>::const_iterator iter = mapColList.constBegin();
    while (iter != mapColList.constEnd())
    {
        strSql += iter.key();      //表的列的名称
        strSql += ", ";

        ++iter;
    }
    if (!mapColList.empty())        //删除最后一个多余的", "
    {
        strSql.chop(2);
    }

    strSql += ") values(";

    iter = mapColList.constBegin();
    while (iter != mapColList.constEnd())
    {
        strSql += '\'';
        QString strValue;
        QMap<QString, QString>::const_iterator iter1 = mapColValues.find(iter.key());
        if (iter1 != mapColValues.constEnd())
        {
            strValue = iter1.value();
        }
        strSql += strValue;
        strSql += "', ";

        ++iter;

    }
    if (!mapColList.empty())        //删除最后一个多余的", "
    {
        strSql.chop(2);
    }
    strSql += ')';

    return ExecuteSql(strSql);
}

bool CDataBaseTool::DeleteRecord(QVariant primaryValue)
{
    //生成查询字符串
    QString strSql;
    strSql += "delete from ";
    strSql += GetTableName();
    strSql += " where ";
    strSql += GetPrimaryKey();
    strSql += " = '";
    strSql += primaryValue.toString();
    strSql += '\'';

    return ExecuteSql(strSql);
}

bool CDataBaseTool::IsRecordExist(QVariant primaryValue)
{
    //return false;
    QString strSql;
    strSql += "select ";
    strSql += GetPrimaryKey();
    strSql += " from ";
    strSql += GetTableName();
    strSql += " where ";
    strSql += GetPrimaryKey();
    strSql += " = '";
    strSql += primaryValue.toString();
    strSql += '\'';

    QSqlQuery query = ExecuteSql2(strSql);
    bool exist = (query.next());

    return exist;
}

bool CDataBaseTool::UpdateRecode(QVariant primaryValue, const char *filed, const QString &value)
{
    QString strSql;
    strSql += "update ";
    strSql += GetTableName();
    strSql += " set ";
    strSql += filed;
    strSql += " = '";
    strSql += value;
    strSql += "' where ";
    strSql += GetPrimaryKey();
    strSql += " = '";
    strSql += primaryValue.toString();
    strSql += '\'';

    return ExecuteSql(strSql);
}

QVector<QString> CDataBaseTool::GetFiled()
{
    QVector<QString> filedList;
    QMap<QString, QString> mapColList;
    GetTableColumeList(mapColList);
    QMap<QString, QString>::const_iterator i = mapColList.constBegin();
    while (i != mapColList.constEnd())
    {
        filedList.push_back(i.key());
        ++i;
    }
    return filedList;
}

