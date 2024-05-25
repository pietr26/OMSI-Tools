#ifndef OTEXTERNAL_H
#define OTEXTERNAL_H

#include "OTGlobal.h"
#include <QtSql>
#include <QSqlDriver>


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

    QString lastError() { return db.lastError().text(); }

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

class OTGit
{
public:
    QPair<QString, QString> exec(QStringList args)
    {
        if (projectFolder.isEmpty())
        {
            qWarning() << "Git: projectFolder is empty!";
            return QPair<QString, QString>("", "projectFolder is empty!");
        }
        else if (!QDir(projectFolder).exists())
        {
            qWarning() << "Git: projectFolder is invalid!";
            return QPair<QString, QString>("", "projectFolder is invalid!");
        }

        QProcess gitProcess;
        gitProcess.setWorkingDirectory(projectFolder);
        gitProcess.start("git", args);
        gitProcess.waitForFinished();

        QString info = gitProcess.readAllStandardOutput();
        QString error = gitProcess.readAllStandardError();

        if (info.trimmed() != "") qDebug() << "execGit Info:" << info;
        if (error.trimmed() != "") qDebug() << "execGit Error:" << error;

        return QPair<QString, QString>(info, error);
    }

    QString projectFolder;
};

class OTDirectXTexConv
{
public:
    bool convert(QString format, QString input, QTemporaryFile &tFile)
    {
        QPair<QString, QString> output = exec(QStringList() << "-y" << "-ft" << format << "-o" << QDir::tempPath() << input);

        QString newFile = QDir::tempPath() + "/" + QFileInfo(input).fileName();
        while (!newFile.endsWith(".")) newFile = newFile.removeLast();
        newFile.append(format);

        tFile.resize(0);

        QFile nFile(newFile);
        nFile.open(QFile::ReadOnly);
        QByteArray bla = nFile.readAll();
        tFile.open();
        QDataStream in(&tFile);
        in.writeRawData(bla.constData(), bla.size());

        //tFile.close();

        //QFile(newFile).remove();

        return true;
    }

    QPair<QString, QString> exec(QStringList args)
    {
        if (!QFile("texconv.exe").exists())
        {
            qCritical() << "Could not find texconv.exe!";
            return QPair<QString, QString>("", "ERR: Could not find texconv.exe!");
        }

        QProcess texconvProcess;
        texconvProcess.setWorkingDirectory(QApplication::applicationDirPath());
        texconvProcess.start("texconv.exe", args);
        texconvProcess.waitForFinished();

        QString info = texconvProcess.readAllStandardOutput();
        QString error = texconvProcess.readAllStandardError();

        if (info.trimmed() != "") qDebug() << "execTexconv Info:" << info;
        if (error.trimmed() != "") qDebug() << "execTexconv Error:" << error;

        return QPair<QString, QString>(info, error);
    }

private:
};

#endif // OTEXTERNAL_H


