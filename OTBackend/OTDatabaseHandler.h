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
    QSqlQuery doAction(QString action, bool automaticOpenClose = false)
    {
        if (automaticOpenClose)
        {
            if (!db.open())
            {
                qWarning() << "OTDatabaseHandler error: Cannot open database automatically";
                return QSqlQuery();
            }

            QSqlQuery query(action);
            db.close();
            return query;
        }
        else
        {
            if (db.isOpen())
                return QSqlQuery(action);
            else
            {
                qWarning() << "OTDatabaseHandler error: Open database before executing an action or use automaticOpenClose";
                return QSqlQuery();
            }
        }
    }

    bool openDB()
    {
        return db.open();
    }

    void closeDB()
    {
        db.close();
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
            QFile(dbPath).copy(QFileInfo(dbPath).absolutePath() + "/backups/" + QFileInfo(dbPath).baseName() + "_" + misc.getDate("yyyy-MM-dd") + "_" + misc.getTime("hh-mm-ss-z") + ".db");
    }

    QString dbPath;


private:
    OTMiscellaneous misc;
    QSqlDatabase db;

    /// Opens a database
    bool open()
    {
        if (!db.open())
            return false;

        return true;
    }
};

#endif // OTDATABASEHANDLER_H


