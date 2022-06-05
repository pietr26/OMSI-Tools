#include "wcleanup.h"
#include "ui_wcleanup.h"

wCleanup::wCleanup(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wCleanup)
{
    qInfo().noquote() << "Starting " + moduleName + "...";

    qDebug() << "Set up UI...";
    ui->setupUi(this);
    //adjustSize();
    qDebug() << "UI set";

    // Load settings
    setStyleSheet(set.read("main","theme").toString());
    setWindowTitle(OTName + " - " + tr("cleanup"));

    qInfo().noquote() << moduleName + " started";
}

wCleanup::~wCleanup()
{
    delete ui;
}

/* NEEDS:
 * getTiles();
 *  --> getItems();
 */

void wCleanup::on_btnAnalyze_clicked()
{
    QStringList globals;

    QDirIterator mapFolder(set.read("main", "mainDir").toString() + "/maps", QStringList() << "global.cfg", QDir::Files, QDirIterator::Subdirectories);
    while (mapFolder.hasNext())
        globals << mapFolder.next();

    foreach (QString current, globals)
    {
        filehandler.setMapPath(current);

        filehandler.getTiles();

        filehandler.getItems(filehandler.stuffobj.existing.tiles);
    }

    filehandler.stuffobj.existing.sceneryobjects.removeDuplicates();
    filehandler.stuffobj.existing.splines.removeDuplicates();

    // Sceneryobjects
    QStringList scoFolders;
    QDirIterator scoFolder(set.read("main", "mainDir").toString() + "/Sceneryobjects", QDir::Dirs | QDir::NoDotAndDotDot);
    while (scoFolder.hasNext())
        scoFolders << scoFolder.next();

    foreach (QString current, scoFolders)
    {
        QStringListIterator iterator(filehandler.stuffobj.existing.sceneryobjects);

        int i = 0;
        while (iterator.hasNext())
            if (QString(set.read("main", "mainDir").toString() + "/" + iterator.next()).contains(current, Qt::CaseInsensitive))
                i++;

        if (i == 0)
            ui->lwgObjects->addItem(current);
    }

    // Splines:
    QStringList sliFolders;
    QDirIterator sliFolder(set.read("main", "mainDir").toString() + "/Splines", QDir::Dirs | QDir::NoDotAndDotDot);
    while (sliFolder.hasNext())
        sliFolders << sliFolder.next();

    foreach (QString current, sliFolders)
    {
        QStringListIterator iterator(filehandler.stuffobj.existing.splines);

        int i = 0;
        while (iterator.hasNext())
            if (QString(QFileInfo(set.read("main", "mainDir").toString() + "/" + iterator.next()).absolutePath()).contains(current, Qt::CaseInsensitive))
                i++;

        if (i == 0)
            ui->lwgSplines->addItem(current);
    }

    qInfo() << "+++ Finished.";
}

