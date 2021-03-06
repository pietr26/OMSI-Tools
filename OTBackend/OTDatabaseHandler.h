#ifndef OTDATABASEHANDLER_H
#define OTDATABASEHANDLER_H
#include "OTGlobal.h"
#include <QtSql>
#include <QSqlDriver>
#include <QDebug>

class OTDatabaseHandler
{
public:
    /// Opens a database, executes a databse action and close them
    QSqlQuery doAction(QString action)
    {
        QSqlQuery qry;

        if (!open())
            return qry;

        qry = query(action);

        db.close();

        return qry;
    }

    /// Setup a database
    void setupDatabase(bool firstSetup = false)
    {
        if (firstSetup)
        {
            QDir dir;

            if (!dir.exists(QFileInfo(dbPath).absolutePath()))
                dir.mkpath(QFileInfo(dbPath).absolutePath());

            QFile database(dbPath);
            database.open(QIODevice::WriteOnly);
            database.close();
        }

        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(dbPath);
    }

    /// Copys and renames a database
    void createBackup()
    {
        if (QFile(dbPath).exists())
            QFile(dbPath).copy(QFileInfo(dbPath).absolutePath() + "/" + "backup_" + misc.getDate("yyyy-MM-dd-") + "---" + misc.getTime("hh-mm-ss-z") + ".db");
    }

    QString dbPath;
    QString lastError;
    QSqlDatabase db;

private:

    OTMiscellaneous misc;

    /// Opens a database
    bool open()
    {
        if (!db.open())
            return false;

        return true;
    }

    /// Executes an action
    QSqlQuery query(QString action)
    {
        lastError.clear();
        QSqlQuery query;
        query.prepare(action);

        if (!query.exec())
        {
            lastError = query.lastError().driverText() + " (" + query.lastError().databaseText() + ")";
            qCritical().noquote() << "SQL error: " << lastError;
        }

        return query;
    }
};

#endif // OTDATABASEHANDLER_H


