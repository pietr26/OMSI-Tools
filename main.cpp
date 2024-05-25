#include "OTModules/OTGeneric/wstartupscreen.h"
#include "OTBackend/OTGlobal.h"

#include <QApplication>
#include <QTranslator>
#include <QDebug>
#include <QFile>
#include "OTBackend/OTLogger.h"

/// Test area for anything
void testArea()
{

}

/// Cleanup actions after updates
void updateCleanup()
{
    qInfo() << "Update installed, remove old files...";

    // Necessary for every update:
    QDir(QDir::tempPath() + "/OMSI-Tools_tempAppDir").removeRecursively();

    // Necessary for specified versions:

    // 001: >  1.0.0-lite - Remove unused image libaries
    // Moved to Updater (permission conflicts)
}

/// Main function
int main(int argc, char *argv[])
{
    OTSettings set;
    OTMiscellaneous misc;
    OTFileOperations fop;

    QApplication a(argc, argv);

    /// Setup
    {
        // If application crashed, save logfile
        QString newName;
        bool isCrash = false;
        if (set.read("main", "closeCheck") == "false")
        {
            isCrash = true;
            newName = QString("logfile_crash_%1.txt").arg(misc.getDate("yyyy-MM-dd") + "_" + misc.getTime("hh-mm-ss"));

            QFile::copy("logfile.txt", newName);
        }

        // Initialize logger
        if ((set.read("main", "logfileMode") != -1))
        {
            OTLogger::attach("logfile.txt", OTInformation::name);
            OTLogger::logging = true;
        }

        // Print all prefs to the logfile
        qDebug().noquote() << "Preferences" + set.getAllPreferencesFormatted() + "\n================================================\n";

        set.setDefaultPreferences();

        qInfo().noquote() << "Starting " + OTInformation::name + "...";

        qApp->setStyle(QStyleFactory::create(set.read("main", "theme").toString()));

        set.loadTranslator();

        if (isCrash)
        {
            qWarning() << "Crash on last start detected! A logfile has been saved.";
            QMessageBox::StandardButton reply = QMessageBox::question(NULL, QObject::tr("Crash detected"), QObject::tr("%1 seems to have crashed on last launch. The logfile of the last start was saved separately. Please contact the developer with this.\nOpen the path of the logfile?").arg(OTInformation::name));

            if (reply == QMessageBox::Yes)
                fop.showInExplorer(newName);
        }
        else
            qDebug() << "No crash detected.";

        set.write("main", "closeCheck", false);

        testArea();

        // If the application was updated, remove the temporary directory in the temp folder
        if ((QCoreApplication::arguments().size() >= 2) && (QCoreApplication::arguments().at(1) == "updateInstalled"))
            updateCleanup();

        // Start counter
        if (set.read("main", "startCount").isValid())
            set.write("main", "startCount", set.read("main", "startCount").toInt() + 1);
        else
            set.write("main", "startCount", 1);

        // Reset advanced verification:
        set.write("wVerifyMap", "advVerifying", false);
    }

    wStartUpScreen *WSTARTUPSCREEN;

//    if ((QCoreApplication::arguments().size() >= 2) && (QCoreApplication::arguments().at(1) == "..."))
//    {
//        // ...
//    }
//    else
    {
        WSTARTUPSCREEN = new wStartUpScreen();
        WSTARTUPSCREEN->show();
    }

    int exec = a.exec();

    qInfo().noquote() << OTInformation::name + " is closing...";
    set.write("main", "closeCheck", true);
    return exec;
}
