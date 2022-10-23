#include "OTModules/OTGeneric/wstartupscreen.h"
#include "OTBackend/OTGlobal.h"

#include <QApplication>
#include <QTranslator>
#include <QDebug>
#include <QFile>
#include "OTBackend/OTLogger.h"
#include "OTBackend/OTStyleDraw.h"

/// Test area for anything
void testArea()
{
    //qDebug() << "+&+& TEST AREA &+&+&+&+&+&+&+&+&+&+&+&+&+&+&+&+&";

    // Copies all directories (and subdirectories) without content
//    OTSettings set;
//    int cutCount = set.read("main", "mainDir").toString().size() + 1;
//    QDirIterator makePaths(set.read("main", "mainDir").toString(), QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
//    while (makePaths.hasNext())
//        QDir().mkpath("C:/Users/pietr/Desktop/testOMSI/" + makePaths.next().remove(0, cutCount));

    //qDebug() << "+&+&+&+&+&+&+&+&+&+&+&+&+&+&+&+&+&+&+&+&+&+&+&+&";
}

/// Cleanup actions after updates
void updateCleanup()
{
    qInfo() << "Update installed, remove old files...";

    // Necessary for every update:
    QDir(QDir::tempPath() + "/OMSI-Tools_tempAppDir").removeRecursively();

    // Necessary for specified versionsf:

    // 001: >  1.0.0-lite - Remove unused image libaries
    // Moved to Updater (permission conflicts)
}

/// Main function
int main(int argc, char *argv[])
{
    OTSettings set;
    OTMiscellaneous misc;
    OTFileOperations fop;

    // If application crashed, save logfile
    QString newName;
    bool isCrash = false;
    if (set.read("main", "closeCheck") == "false")
    {
        isCrash = true;
        newName = QString("logfile_crash_%1.txt").arg(misc.getDate("dd-MM-yyyy") + "_" + misc.getTime("hh-mm-ss"));

        QFile::copy("logfile.txt", newName);
    }

    // Initialize logger
    if ((set.read("main", "logfileMode") != -1))
    {
        Logger::attach();
        Logger::logging = true;
    }

    // Print all settings to the logfile
    qDebug().noquote() << "Settings" + set.getAllSettings() + "\n================================================\n";

    set.write("main", "closeCheck", false);
    set.setDefaultSettings();

    qInfo().noquote() << "Starting " + OTName + "...";

    QApplication a(argc, argv);

    QApplication::setStyle(new OTStyleDraw);

    set.loadTranslator();

    if (isCrash)
    {
        qWarning() << "Crash on last start detected! A logfile has been saved. Please contact the developer.";
        QMessageBox::StandardButton reply = QMessageBox::question(NULL, QObject::tr("Crash detected"), QObject::tr("%1 seems to have crashed on last launch. The logfile of the last start was saved separately. Please contact the developer with this.\nOpen the path of the logfile?").arg(OTName));

        if (reply == QMessageBox::Yes)
            fop.showInExplorer(newName);
    }
    else
        qDebug() << "No crash detected.";

    testArea();

    // If the application was updated, remove the temporary directory in the temp folder
    if ((QCoreApplication::arguments().size() >= 2) && (QCoreApplication::arguments().at(1) == "updateInstalled"))
        updateCleanup();

    // Start counter
    if (set.read("main", "startCount").isValid())
        set.write("main", "startCount", set.read("main", "startCount").toInt() + 1);
    else
        set.write("main", "startCount", 1);

    wStartUpScreen *WSTARTUPSCREEN;
    wFbh *WFBH;

    if ((QCoreApplication::arguments().size() >= 2) && (QCoreApplication::arguments().at(1) == "openFbh"))
    {
        WFBH = new wFbh();
        WFBH->show();
    }
    else
    {
        WSTARTUPSCREEN = new wStartUpScreen();
        WSTARTUPSCREEN->show();
    }

    int exec = a.exec();

    qInfo().noquote() << OTName + " is closing...";

    set.write("main", "closeCheck", true);

    return exec;
}
