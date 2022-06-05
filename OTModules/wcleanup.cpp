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

void wCleanup::on_btnAnalyze_clicked()
{
    qInfo() << "Starting...";
    ui->statusbar->showMessage(tr("Initialize..."));
    ui->pgbProgress->setValue(1);
    ui->btnAnalyze->setEnabled(false);

    ui->lwgObjects->clear();
    ui->lwgSplines->clear();

    QStringList globals;

    QDirIterator mapFolder(set.read("main", "mainDir").toString() + "/maps", QStringList() << "global.cfg", QDir::Files, QDirIterator::Subdirectories);
    while (mapFolder.hasNext())
        globals << mapFolder.next();

    qInfo() << "Read maps...";

    ui->pgbProgress->setMaximum(ui->pgbProgress->maximum() + globals.count());
    int i = 0;
    foreach (QString current, globals)
    {
        i++;
        ui->statusbar->showMessage(tr("Read maps (%1 of %2)...").arg(i).arg(globals.count()));
        ui->pgbProgress->setValue(ui->pgbProgress->value() + 1);

        filehandler.setMapPath(current);
        filehandler.getTiles();
        filehandler.getItems(filehandler.stuffobj.existing.tiles);
    }

    filehandler.stuffobj.existing.sceneryobjects.removeDuplicates();
    filehandler.stuffobj.existing.splines.removeDuplicates();

    // Sceneryobjects
    qInfo() << "Analyze sceneryobjects...";
    ui->pgbProgress->setValue(ui->pgbProgress->value() + 1);
    ui->statusbar->showMessage(tr("Analyze sceneryobject folder..."));

    QStringList scoFolders;
    QDirIterator scoFolder(set.read("main", "mainDir").toString() + "/Sceneryobjects", QDir::Dirs | QDir::NoDotAndDotDot);
    while (scoFolder.hasNext())
        scoFolders << scoFolder.next();

    ui->pgbProgress->setValue(ui->pgbProgress->value() + 1);
    ui->statusbar->showMessage(tr("Comparing sceneryobjects..."));
    foreach (QString current, scoFolders)
    {
        qApp->processEvents();
        QStringListIterator iterator(filehandler.stuffobj.existing.sceneryobjects);

        int i = 0;
        while (iterator.hasNext())
            if (QString(set.read("main", "mainDir").toString() + "/" + iterator.next()).contains(current, Qt::CaseInsensitive))
                i++;

        if (i == 0)
            ui->lwgObjects->addItem(current);
    }

    // Splines:
    qInfo() << "Analyze splines...";
    ui->pgbProgress->setValue(ui->pgbProgress->value() + 1);
    ui->statusbar->showMessage(tr("Analyze spline folder..."));

    QStringList sliFolders;
    QDirIterator sliFolder(set.read("main", "mainDir").toString() + "/Splines", QDir::Dirs | QDir::NoDotAndDotDot);
    while (sliFolder.hasNext())
        sliFolders << sliFolder.next();

    ui->pgbProgress->setValue(ui->pgbProgress->value() + 1);
    ui->statusbar->showMessage(tr("Comparing splines..."));
    foreach (QString current, sliFolders)
    {
        qApp->processEvents();
        QStringListIterator iterator(filehandler.stuffobj.existing.splines);

        int i = 0;
        while (iterator.hasNext())
            if (QString(QFileInfo(set.read("main", "mainDir").toString() + "/" + iterator.next()).absolutePath()).contains(current, Qt::CaseInsensitive))
                i++;

        if (i == 0)
            ui->lwgSplines->addItem(current);
    }

    ui->btnAnalyze->setEnabled(true);
    ui->pgbProgress->setValue(ui->pgbProgress->value() + 1);
    ui->statusbar->showMessage(tr("Done."), 10000);

    qInfo() << "Finished.";
}

