#ifndef DATABASE_H
#define DATABASE_H

#include <QSql>
#include <QSqlQuery>
#include <QSqlQueryModel>

class database
{
public:

    void execQuery(const QString& strQuery);

    QSqlQueryModel* execSelectQuery(const QString& strQuery);

    QStringList execQueryPRAGMA(const QString& strTable);
};

#endif // DATABASE_H
