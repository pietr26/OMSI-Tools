#ifndef OTFILEOPERATIONS_H
#define OTFILEOPERATIONS_H

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QWidget>

#include "OTInformation.h"
#include "OTMiscellaneous.h"
#include "OTPlatform.h"

class OTFileOperations
{
public:
    Q_DECL_DEPRECATED QString writeFileHeader()
    {
        return "File created with " + OTInformation::name + " " + OTInformation::versions::currentVersion.first + " on " + misc.getDate() + ", " + misc.getTime() + "\n\n";
    }

    void createBackupFolder()
    {
        const QString backup = OTPlatform::applicationFile("backup");

        if (!QDir(backup).exists())
            QDir().mkdir(backup);
    }

    void createShortcut(QString filepath, QString shortcutLocation, QWidget *parent)
    {
        if (QFile(filepath).link(shortcutLocation))
        {
            QMessageBox::information(parent, QObject::tr("Success"), QObject::tr("Successfully created shortcut!"));
            qInfo().noquote() << QString("Created shortcut in '%1'!").arg(shortcutLocation);
        }
        else
        {
            QMessageBox::warning(parent, QString(QObject::tr("Error")), QObject::tr("Could not create shortcut in %1.").arg(QDir().homePath() + "/Desktop"));
            qCritical().noquote() << QString("Could not create a shortcut in '%1'!").arg(shortcutLocation);
        }
    }

    void showInExplorer(QString absolutePath)
    {
        // ATTENTION: This will NOT work in the OneDrive folder - I don't know why.
        qDebug() << "Show in explorer:" << absolutePath;

        OTPlatform::showInFileManager(absolutePath);
    }

private:
    OTMiscellaneous misc;
};

#endif // OTFILEOPERATIONS_H
