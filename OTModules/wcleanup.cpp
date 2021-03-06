#include "wcleanup.h"
#include "ui_wcleanup.h"

wCleanup::wCleanup(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wCleanup)
{
    qInfo().noquote() << "Starting " + objectName() + "...";

    qDebug() << "Set up UI...";
    ui->setupUi(this);
    //adjustSize();
    qDebug() << "UI set";

    // Load settings
    setStyleSheet(set.read("main","theme").toString());
    setWindowTitle(OTName + " - " + tr("cleanup"));

    ui->statusbar->addPermanentWidget(ui->pgbProgress);
    ui->pgbProgress->setVisible(false);
    ui->gbxActions->setVisible(false);
    on_rbtnMoveToFolder_clicked();

    qInfo().noquote() << objectName() + " started";
}

wCleanup::~wCleanup()
{
    delete ui;
}

void wCleanup::on_btnAnalyze_clicked()
{
    on_actionAnalyze_triggered();
}

void wCleanup::on_actionAnalyze_triggered()
{
    qInfo() << "Starting...";
    ui->statusbar->showMessage(tr("Initialize..."));

    if (set.read("main", "mainDir").toString() == "")
    {
        if (msg.setMainDir(this))
            set.write("main", "mainDir", set.getOmsiPath(this));
        else
        {
            ui->statusbar->showMessage(tr("No OMSI Main Directory selected."), 10000);
            return;
        }
    }

    ui->pgbProgress->setVisible(true);
    ui->pgbProgress->setMaximum(6);
    ui->pgbProgress->setValue(1);

    ui->btnAnalyze->setEnabled(false);
    ui->actionAnalyze->setEnabled(false);
    ui->gbxActions->setVisible(false);

    ui->lwgObjects->clear();
    ui->lwgSplines->clear();
    filehandler.stuffobj.clear();

    cutCount = set.read("main", "mainDir").toString().count() + 1;

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
        filehandler.getItems(filehandler.stuffobj.existing.tiles, false, false);

        filehandler.stuffobj.existing.removeDuplicates();
    }

    // Sceneryobjects
    if (!filehandler.stuffobj.existing.sceneryobjects.isEmpty())
    {
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
                ui->lwgObjects->addItem(current.remove(0, cutCount));
        }
    }


    // Splines:
    if (!filehandler.stuffobj.existing.splines.isEmpty())
    {
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
                ui->lwgSplines->addItem(current.remove(0, cutCount));
        }
    }

    ui->btnAnalyze->setEnabled(true);
    ui->actionAnalyze->setEnabled(true);
    ui->pgbProgress->setValue(ui->pgbProgress->maximum());
    ui->statusbar->showMessage(tr("Done."), 10000);

    if ((ui->lwgObjects->count() != 0) || (ui->lwgSplines->count() != 0))
        ui->gbxActions->setVisible(true);

    qInfo() << "Finished.";
}

void wCleanup::on_btnStartAction_clicked()
{
    ui->btnStartAction->setEnabled(false);
    ui->btnAnalyze->setEnabled(false);
    ui->actionAnalyze->setEnabled(false);

    ui->pgbProgress->setValue(0);
    ui->pgbProgress->setMaximum(3);

    ui->lwgObjects->setEnabled(false);
    ui->lwgSplines->setEnabled(false);

    if (ui->rbtnMoveToFolder->isChecked())
    {
        QString destinationFolder = QFileDialog::getExistingDirectory(this, tr("Select destination folder..."));

        if (destinationFolder.isEmpty())
            return;

        // Sceneryobjects
        ui->pgbProgress->setValue(1);
        ui->statusbar->showMessage(tr("Move sceneryobjects..."));
        qInfo() << QString("Move selected sceneryobjects to '%1'...").arg(destinationFolder);

        for (int i = 0; i < ui->lwgObjects->selectedItems().count(); i++)
        {
            qApp->processEvents();

            QDirIterator makePaths(set.read("main", "mainDir").toString() + "/" + ui->lwgObjects->selectedItems().at(i)->text(), QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
            while (makePaths.hasNext())
                QDir().mkpath(destinationFolder + "/" + makePaths.next().remove(0, cutCount));


            QDirIterator moveFiles(set.read("main", "mainDir").toString() + "/" + ui->lwgObjects->selectedItems().at(i)->text(), QDir::Files, QDirIterator::Subdirectories);
            while (moveFiles.hasNext())
            {
                QString current = moveFiles.next().remove(0, cutCount);
                QDir().rename(set.read("main", "mainDir").toString() + "/" + current, destinationFolder + "/" + current);
            }

            QDir(set.read("main", "mainDir").toString() + "/" + ui->lwgObjects->selectedItems().at(i)->text()).removeRecursively();
        }
        qDeleteAll(ui->lwgObjects->selectedItems());

        // Splines
        ui->pgbProgress->setValue(2);
        ui->statusbar->showMessage(tr("Move splines..."));
        qInfo() << QString("Move selected splines to '%1'...").arg(destinationFolder);

        for (int i = 0; i < ui->lwgSplines->selectedItems().count(); i++)
        {
            qApp->processEvents();

            QDirIterator makePaths(set.read("main", "mainDir").toString() + "/" + ui->lwgSplines->selectedItems().at(i)->text(), QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
            while (makePaths.hasNext())
                QDir().mkpath(destinationFolder + "/" + makePaths.next().remove(0, cutCount));

            QDirIterator moveFiles(set.read("main", "mainDir").toString() + "/" + ui->lwgSplines->selectedItems().at(i)->text(), QDir::Files, QDirIterator::Subdirectories);
            while (moveFiles.hasNext())
            {
                QString current = moveFiles.next().remove(0, cutCount);
                QDir().rename(set.read("main", "mainDir").toString() + "/" + current, destinationFolder + "/" + current);
            }

            QDir(set.read("main", "mainDir").toString() + "/" + ui->lwgSplines->selectedItems().at(i)->text()).removeRecursively();
        }
        qDeleteAll(ui->lwgSplines->selectedItems());

        ui->pgbProgress->setValue(3);
        ui->statusbar->showMessage(tr("Moved selected folders to '%1'.").arg(destinationFolder), 10000);
    }
    else if (ui->rbtnDelete->isChecked())
    {
        QMessageBox::StandardButton reply = QMessageBox::warning(this, tr("Delete files permanently"), tr("Please keep in mind that this option will delete all files PERMANENTLY. After deletion there is no possibility to restore them. Continue?"), QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes)
        {
            for (int i = 0; i < ui->lwgObjects->selectedItems().count(); i++)
                QDir(set.read("main", "mainDir").toString() + "/" + ui->lwgObjects->selectedItems().at(i)->text()).removeRecursively();

            qDeleteAll(ui->lwgObjects->selectedItems());

            for (int i = 0; i < ui->lwgSplines->selectedItems().count(); i++)
                QDir(set.read("main", "mainDir").toString() + "/" + ui->lwgSplines->selectedItems().at(i)->text()).removeRecursively();

            qDeleteAll(ui->lwgSplines->selectedItems());
        }

        ui->statusbar->showMessage(tr("Deleted selected folders."), 10000);
    }

    qInfo() << "Finished.";

    if ((ui->lwgObjects->count() == 0) && (ui->lwgSplines->count() == 0))
        ui->gbxActions->setVisible(false);

    ui->lwgObjects->setEnabled(true);
    ui->lwgSplines->setEnabled(true);

    ui->btnStartAction->setEnabled(true);
    ui->btnAnalyze->setEnabled(true);
    ui->actionAnalyze->setEnabled(true);
}

void wCleanup::on_rbtnMoveToFolder_clicked()
{
    ui->btnStartAction->setText(tr("Select destination folder && move"));
}

void wCleanup::on_rbtnDelete_clicked()
{
    ui->btnStartAction->setText(tr("Delete permanently"));
}

void wCleanup::on_actionClose_triggered()
{
    close();
}

void wCleanup::on_actionSettings_triggered()
{
    WSETTINGS = new wSettings(this);
    WSETTINGS->setWindowModality(Qt::ApplicationModal);
    WSETTINGS->show();
}

void wCleanup::on_actionSendFeedback_triggered()
{
    wFeedback *WFEEDBACK = new wFeedback(this);
    WFEEDBACK->setWindowModality(Qt::ApplicationModal);
    WFEEDBACK->show();
}
