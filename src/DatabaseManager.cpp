#include "DatabaseManager.h"

DatabaseManager::DatabaseManager() : db(QSqlDatabase::addDatabase("QPSQL")) {
    db.setHostName(DatabaseInfo::host);
    db.setDatabaseName(DatabaseInfo::database_name);
    db.setUserName(DatabaseInfo::user);
    db.setPassword(DatabaseInfo::password);
    db.setPort(DatabaseInfo::port);
    if (db.open()) {
        qDebug() << "DatabaseManager: Successfully connected to database " << DatabaseInfo::database_name;
    } else {
        throw std::runtime_error(db.lastError().text().toStdString());
    }
}

DatabaseManager::~DatabaseManager() {
db.close();
}

QList<QString> DatabaseManager::selectAllUsers() const {
    QList<QString> res;
    QSqlQuery query;
    query.prepare("SELECT id, username FROM public.user;");
    query.exec();
    while (query.next()) {
        res.push_back(query.value(0).toString() + " | " + query.value(1).toString());
    }
    return res;
}
