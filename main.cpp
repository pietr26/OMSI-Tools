#include "OTModules/OTGeneric/wstart.h"
#include "OTModules/OTGeneric/wfirstsetup.h"
#include "OTBackend/OTGlobal.h"

#include <QApplication>
#include <QTranslator>
#include <QDebug>
#include <QFile>
#include "OTBackend/OTLogger.h"
#include "OTBackend/DiscordGameSDK.h"
#include <QThreadPool>

void testArea()
{

}

void updateCleanup()
{
    qInfo() << "Update installed, remove old files...";

    // Necessary for every update up to 1.1.2-lite:
    QDir(QDir::tempPath() + "/OMSI-Tools_tempAppDir").removeRecursively();
}

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

        set.setPreferencesOnStart();

        qInfo().noquote() << "Starting " + OTInformation::name + "...";

        qApp->setStyle(QStyleFactory::create(set.read("main", "theme").toString()));
        set.loadBaseStyleheet();

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

        // Start DiscordGameSDK:
        DiscordGameSDK *discord = new DiscordGameSDK();

        QFuture<void> discordFuture = QtConcurrent::run([discord]() {
            discord->exec();
        });

#ifdef QT_DEBUG
        DiscordGameSDK::clearActivity();
        DiscordGameSDK::setModule("Debugging");
        DiscordGameSDK::setStatus("all the way to hell");
        DiscordGameSDK::update();
        DiscordGameSDK::setBlockUpdate(true);
#else
        if (OTInformation::build == OTBuildOptions::Dev)
        {
            DiscordGameSDK::clearActivity();
            DiscordGameSDK::setModule("Developing");
            DiscordGameSDK::setStatus(QString("a brand new version of %1").arg(OTInformation::name));
            DiscordGameSDK::update();
            DiscordGameSDK::setBlockUpdate(true);
        }
        else DiscordGameSDK::clearActivity();
#endif
    }

    wStart *WSTART;
    wFirstSetup *WFIRSTSETUP;

//    if ((QCoreApplication::arguments().size() >= 2) && (QCoreApplication::arguments().at(1) == "..."))
//    {
//        // ...
//    }
//    else

    if (!set.read("main", "language").isValid())
    {
        WFIRSTSETUP = new wFirstSetup();
        WFIRSTSETUP->show();
    }
    else
    {
        WSTART = new wStart();
        WSTART->show();
    }

    int exec = a.exec();

    // --------------------------------------------
    // Actions on close

    DiscordGameSDK::stop();

    qInfo().noquote() << OTInformation::name + " is closing...";
    set.write("main", "closeCheck", true);

    return exec;
}
