#ifndef IMESSAGE_DATABASEMANAGER_H
#define IMESSAGE_DATABASEMANAGER_H

#include <iostream>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QString>
#include <QDebug>
#include <QVector>
#include "ConnectionInfo.h"

class DatabaseManager {
public:
    DatabaseManager();

    ~DatabaseManager();

    DatabaseManager(const DatabaseManager &rhs) = delete;

    DatabaseManager &operator=(const DatabaseManager &rhs) = delete;

    [[nodiscard]] QList<QString> selectAllUsers() const;

private:
    QSqlDatabase db;
};


#endif //IMESSAGE_DATABASEMANAGER_H
