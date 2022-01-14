#include "wstartupscreen.h"
#include "OTBackend/OTGlobal.h"

#include <QApplication>
#include <QTranslator>
#include <QDebug>
#include <QFile>
#include "OTBackend/OTLogger.h"

/// \brief Test area for anything
void testArea()
{

}

/// \brief Main function
int main(int argc, char *argv[])
{
    OTSettings set;

    QString newName;
    bool isCrash = false;
    if (set.read("main", "closeCheck") == "false")
    {
        isCrash = true;
        OTMiscellaneous misc;
        newName = QString("logfile_crash_%1.txt").arg(misc.getDate() + "_" + misc.getTime("hh.mm.ss"));

        QFile::copy("logfile.txt", newName);
    }

    if (set.read("main", "logfileMode").isValid() && (set.read("main", "logfileMode") != -1))
    {
        Logger::attach();
        Logger::logging = true;
    }

    set.write("main", "closeCheck", false);

    qInfo().noquote() << "Starting " + OTName + "...";

    QApplication a(argc, argv);

    set.loadLanguage(a);

    if (isCrash)
    {
        qWarning() << "Crash on last start detected! A logfile has been saved. Please contact the developer.";
        QMessageBox::StandardButton reply = QMessageBox::question(NULL, QObject::tr("Crash detected"), QObject::tr("%1 seems to have crashed on last launch. The logfile of the last start was saved separately. Please contact the developer with this.\nOpen the path of the logfile?").arg(OTName));

        if (reply == QMessageBox::Yes)
        {
            QStringList args;
            args << "/select," << QDir::toNativeSeparators(newName);

            QProcess *process = new QProcess(NULL);
            process->start("explorer.exe", args);
        }
    }
    else
        qDebug() << "No crash detected.";

    testArea();


    // If the application was updated, remove the temporary directory in the temp folder
    if ((QCoreApplication::arguments().count() >= 2) && (QCoreApplication::arguments().at(1) == "updateInstalled"))
        QDir(QDir::tempPath() + "/OMSI-Tools_tempAppDir").removeRecursively();

/*    QFile convert("C:/Users/pietr/Desktop/Testfile.o3d");
    convert.open(QFile::ReadOnly);
    QByteArray test = convert.readAll();
    test.toInt();

    qDebug() << test;*/

    wStartUpScreen w;
    w.show();

    int exec = a.exec();

    qInfo().noquote() << OTName + " is closing...";

    set.write("main", "closeCheck", true);
    return exec;
}

