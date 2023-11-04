#ifndef OTDATABASEHANDLER_H
#define OTDATABASEHANDLER_H
#include "OTGlobal.h"
#include <QtSql>
#include <QSqlDriver>
#include <QDebug>


/* FORMAL DATABASE SETUP IN HEADER

    // Setup database
    dbHandler.dbPath = "<dbPath>";
    dbHandler.setupDatabase("<SQLCommandForPotentialFirstSetup>");

----------------------------------

    // Setup database
    dbHandler.dbPath = "<dbPath>";
    dbHandler.setupDatabase(&temporaryFile, <dbPathInRessources>);

*/

class OTDatabaseHandler
{
public:
    /// Opens a database, executes a databse action and close them
    QSqlQuery doAction(QString action, bool automaticOpenClose = false)
    {
        if (dbPath.isEmpty()) qWarning() << "Attention: dbPath is empty!";

        if (action.length() > 63999)
        {
            qCritical() << "OTDatabaseHandler error: action string ist too long (> 64000)!";
            return QSqlQuery();
        }

        if (automaticOpenClose)
        {
            if (!db.open())
            {
                qCritical() << "OTDatabaseHandler error: Cannot open database automatically";
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
                qCritical() << "OTDatabaseHandler error: Open database before executing an action!";
                return QSqlQuery();
            }
        }
    }

    bool openDB() { return db.open(); }

    void closeDB() { db.close(); }

    /// [OVERLOAD] Setup a database
    bool setupDatabase(QString queryForFirstSetup = "")
    {
        bool firstSetup = false;

        if (dbPath.isEmpty())
        {
            qCritical() << "dbPath is empty!";
            return false;
        }
        else if (!QFile(QDir(dbPath).absolutePath()).exists()) firstSetup = true;

        if (firstSetup)
        {
            if (!QDir().exists(QFileInfo(dbPath).absolutePath())) QDir().mkpath(QFileInfo(dbPath).absolutePath());

            QFile database(dbPath);
            if (!database.open(QIODevice::WriteOnly))
            {
                qCritical() << "Could not create database!";
                return false;
            }

            database.close();
        }

        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(dbPath);

        if (firstSetup && !queryForFirstSetup.isEmpty()) doAction(queryForFirstSetup, true);

        return true;
    }

    /// [OVERLOAD] Setup a database
    bool setupDatabase(QTemporaryFile &tempDB, QString dbInResscoures)
    {
        if (!tempDB.open())
        {
            qCritical() << "Could not open temporary database!";
            qDebug() << "Temporary file";
            return false;
        }
        else
        {
            QFile databaseInRessources(dbInResscoures);
            if (!databaseInRessources.open(QFile::ReadOnly))
            {
                qCritical() << "Could not open ressource database!";
                qDebug() << "Resource file";
                return false;
            }
            else
            {
                QByteArray data = databaseInRessources.readAll();
                tempDB.write(data);

                databaseInRessources.close();
            }
        }

        dbPath = tempDB.fileName();
        setupDatabase();

        return true;
    }

    /// Copys and renames a database
    void createBackup()
    {
        if (dbPath.isEmpty()) qWarning() << "dbPath is empty!";

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


