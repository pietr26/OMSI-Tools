#ifndef OTUPDATER_H
#define OTUPDATER_H

#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QPair>
#include <QProcess>
#include <QStringList>
#include <QThread>

#include "OTInformation.h"
#include "OTLinks.h"
#include "OTNetworkConnection.h"
#include "OTPlatform.h"

class OTUpdater : public QThread
{
    Q_OBJECT
public:
    OTUpdater() { }

    void run() Q_DECL_OVERRIDE
    {
        qDebug() << "OTUpdater: Perform updateMt...";

        QStringList args;
        args << "--accept-licenses" << "--default-answer" << "--confirm-command" << "update" << "maintenancetool";

        {
            QProcess process;
            process.start(OTPlatform::updaterExecutable(), args);

            qDebug() << "mt update status:" << process.waitForFinished();
        }

        qDebug() << "OTUpdater: updated maintenacetool.";

        // -------------------------------------------------

        qDebug() << "OTUpdater: Perform updateAll...";

        args.clear();
        args << "--accept-licenses" << "--default-answer" << "--confirm-command" << "update";

        // The updater has to keep running once this function returns and the
        // application shuts down. startDetached() is the API for that - the previous
        // "new QProcess() and never delete it" only worked by accident, because a
        // QProcess kills its child process in the destructor.
        if (!QProcess::startDetached(OTPlatform::updaterExecutable(), args))
            qCritical() << "Could not start the updater!";

        QApplication::quit();
    }


    //  first:
    //      -2: maintenance
    //      -1: manually update error
    //      0: no updates available
    //      1: update only manually available
    //      2: update via mt available
    //  second: latestVersion
    QPair<int, QString> getUpdateInformation()
    {
        qInfo() << "Check for updates...";

        int status;

        if (!QFile::exists(OTPlatform::updaterExecutable()))
        {
            qInfo() << "Maintenance tool is not available.";
            status = -1;
        }
        else
        {
            qInfo() << "Maintenance tool is available.";
            qInfo() << "Starting maintenance tool - checking for updates";

            QStringList args;
            args << "ch";

            QProcess process;
            process.start(OTPlatform::updaterExecutable(), args);
            qInfo() << "Dir:" << process.workingDirectory();

            process.waitForFinished(10000);
            QString output = process.readAllStandardOutput();
            qDebug() << "mt output:" << output;

            if (output.contains("id=\"latest\"")) status = 1;
            else if (output.contains("There are currently no updates available")) status = 0;
            else status = -1;

            qDebug() << "mt status:" << status;
        }

        OTNetworkConnection nc;
        const QString latestVersion = nc.post(OTLinks::latestVersion);
        QPair<int, QString> list;

        // fill first:
        if (nc.lastSuccess == -2) list.first = -2;
        else if (status != -1)
        {
            if (status == 0) list.first = 0;
            else if (status == 1) list.first = 2;
        }
        else if (status == -1)
        {
            if (latestVersion == "") list.first = -1;
            else if (latestVersion == OTInformation::versions::currentVersion.first) list.first = 0;
            else list.first = 1;
        }

        // fill second:
        list.second = latestVersion;

        qDebug() << "Update information:" << list;

        return list;
    }

    void openMaintenancetool()
    {
        QStringList args;
        args << "--start-updater";

        if (!QProcess::startDetached(OTPlatform::updaterExecutable(), args))
            qCritical() << "Could not start the maintenance tool!";

        QApplication::quit();
    }
};

#endif // OTUPDATER_H
