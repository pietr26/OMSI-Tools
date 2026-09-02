#ifndef OTMESSAGE_H
#define OTMESSAGE_H

#include <QDebug>
#include <QMessageBox>
#include <QWidget>

/// Unsaved code snippet for closeEvents:
/*
    if (unsaved)
    {
        int msgResult = msg.unsavedContent(this);
        if (msgResult == -1)
        {
            event->ignore();
            return;
        }
        else if (msgResult == 1)
            save(...);
    }
*/

class OTMessage
{
public:
    void tempDBOpenError(QWidget *parent, QString errorTitle)
    {
        QMessageBox::critical(parent, errorTitle, QObject::tr("There was an error while open the temporary database. Please try it again or contact the developer."));
    }

    bool setMainDir(QWidget *parent)
    {
        qDebug() << "Message: Action needs main directory. Set now (Yes/No)?";
        QMessageBox::StandardButton reply = QMessageBox::question(parent, QObject::tr("OMSI main directory not found"), QObject::tr("To continue the application needs the OMSI main directory. Should it be done now?"), QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No)
            return false;

        return true;
    }

    int unsavedChanges(QWidget *parent)
    {
        qDebug() << "Message: Save unsaved changes (Save/Discard/Cancel)?";
        QMessageBox::StandardButton reply = QMessageBox::question(parent, QObject::tr("Unsaved changes"), QObject::tr("There are unsaved changes."), QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (reply == QMessageBox::Save)
        {
            qDebug() << "User pressed Save";
            return 1;
        }
        else if (reply == QMessageBox::Discard)
        {
            qDebug() << "User pressed Discard";
            return 0;
        }
        else
        {
            qDebug() << "User pressed Cancel";
            return -1;
        }
    }

    bool unsavedContentLeaveYesNo(QWidget *parent)
    {
        qDebug() << "Message: Save unsaved changes / leave (Yes/No)?";
        QMessageBox::StandardButton reply = QMessageBox::question(parent, QObject::tr("Unsaved changes"), QObject::tr("There are unsaved changes. Close anyway?"), QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            qDebug() << "User pressed Yes";
            return true;
        }
        else
        {
            qDebug() << "User pressed No";
            return false;
        }
    }

    void fileOpenError(QWidget *parent) { QMessageBox::information(parent, QObject::tr("Error while opening file"), QObject::tr("The selected file doesn't exists or is read-protected.")); }

    void fileOpenErrorCloseOMSI(QWidget *parent, QString filename)
    {
        QMessageBox::critical(parent, QObject::tr("Error while opening file"), QString(QObject::tr("There was an error while opening '%1'. If OMSI is running, please close it and retry it. Furthermore, check if the file still exists.")).arg(filename));
    }

    void noCharsInFont(QWidget *parent)
    {
        QMessageBox::warning(parent, QObject::tr("No chars in font"), QObject::tr("There are no chars in the font."));
    }

    void fileSaveError(QString filename, QWidget *parent = 0)
    {
        QMessageBox::warning(parent, QObject::tr("Save error"), QString(QObject::tr("There was an save error '%1'. If OMSI is running, please close it and retry it.")).arg(filename));
    }

    void moduleDisabled(QWidget *parent = 0)
    {
        QMessageBox::information(parent, QObject::tr("Module still deactivated"), QObject::tr("Unfortunately, this module is still deactivated due to a time shortage in the developments. Please check for new updates in the next few days."));
    }
};

#endif // OTMESSAGE_H
