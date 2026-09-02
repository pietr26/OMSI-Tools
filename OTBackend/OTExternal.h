#ifndef OTEXTERNAL_H
#define OTEXTERNAL_H

#include "OTGlobal.h"
#include "OTDdsImage.h"
#include <QImage>
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
            return QSqlQuery(db);
        }

        if (automaticOpenClose)
        {
            if (!db.open())
            {
                qCritical() << "OTDatabaseHandler error: Cannot open database automatically";
                return QSqlQuery(db);
            }

            QSqlQuery query(action, db);
            db.close();
            return query;
        }
        else
        {
            if (db.isOpen())
                return QSqlQuery(action, db);
            else
            {
                qCritical() << "OTDatabaseHandler error: Open database before executing an action!";
                return QSqlQuery(db);
            }
        }
    }

    bool openDB() { return db.open(); }

    void closeDB() { db.close(); }

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

        /*
            Every handler gets its own named connection. Without a name they all shared
            qt_sql_default_connection, so the second module to open a database removed
            the first one's connection - Qt then reports "connection is still in use,
            all queries will cease to work" and the older handle turns invalid.

            Two handlers on the same file deliberately share one connection, which is
            what wDBCopyrights and wAddPath need. No connection is removed here: the
            handlers outlive their windows, and a shared connection must not be pulled
            away from the other holder.
        */
        const QString connection = "OTDatabaseHandler:" + dbPath;

        if (QSqlDatabase::contains(connection))
            db = QSqlDatabase::database(connection, false);
        else
            db = QSqlDatabase::addDatabase("QSQLITE", connection);

        db.setDatabaseName(dbPath);

        if (firstSetup && !queryForFirstSetup.isEmpty()) doAction(queryForFirstSetup, true);

        return true;
    }

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
        // DirectXTex is a Windows tool. Everywhere else Qt has to do the job.
        if (OTPlatform::texconvExecutable().isEmpty())
            return convertWithQt(format, input, tFile);

        QPair<QString, QString> output = exec(QStringList() << "-y" << "-ft" << format << "-o" << QDir::tempPath() << input);

        QString newFile = QDir::tempPath() + "/" + QFileInfo(input).fileName();
        while (!newFile.endsWith(".")) newFile = newFile.removeLast();
        newFile.append(format);

        tFile.resize(0);

        QFile nFile(newFile);

        // Both failures leave the caller with an empty texture. See TODO.md - reporting
        // that back through the return value would change what the callers show.
        if (!nFile.open(QFile::ReadOnly))
            qWarning().noquote() << "Could not open the converted texture '" + newFile + "'.";

        QByteArray bla = nFile.readAll();

        if (!tFile.open())
            qWarning().noquote() << "Could not open the temporary file for the converted texture.";

        QDataStream in(&tFile);
        in.writeRawData(bla.constData(), bla.size());

        return true;
    }

    QPair<QString, QString> exec(QStringList args)
    {
        const QString executable = OTPlatform::texconvExecutable();

        if (executable.isEmpty() || !QFile(executable).exists())
        {
            qCritical() << "Could not find texconv.exe!";
            return QPair<QString, QString>("", "ERR: Could not find texconv.exe!");
        }

        QProcess texconvProcess;
        texconvProcess.setWorkingDirectory(QApplication::applicationDirPath());
        texconvProcess.start(executable, args);
        texconvProcess.waitForFinished();

        QString info = texconvProcess.readAllStandardOutput();
        QString error = texconvProcess.readAllStandardError();

        if (info.trimmed() != "") qDebug() << "execTexconv Info:" << info;
        if (error.trimmed() != "") qDebug() << "execTexconv Error:" << error;

        return QPair<QString, QString>(info, error);
    }

private:
    /// Reads the texture through Qt's image plugins and falls back to the own DDS
    /// reader, because Qt does not bring an image plugin for that format any more.
    bool convertWithQt(QString format, QString input, QTemporaryFile &tFile)
    {
        QImage image(input);

        if (image.isNull()) image = OTDdsImage::read(input);

        if (image.isNull())
        {
            qCritical().noquote() << "Could not read '" + input + "'.";
            return false;
        }

        if (!tFile.isOpen() && !tFile.open()) return false;
        tFile.resize(0);
        tFile.seek(0);

        if (!image.save(&tFile, format.toUpper().toLatin1().constData())) return false;

        // The callers pick the result up by its file name, so it has to be on disk.
        tFile.flush();

        return true;
    }
};

#endif // OTEXTERNAL_H


