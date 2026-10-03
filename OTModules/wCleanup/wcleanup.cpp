#include "wcleanup.h"
#include "ui_wcleanup.h"

#include <QDir>
#include <QDirIterator>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>

#include "OTBackend/DiscordGameSDK.h"
#include "OTBackend/OTInformation.h"
#include "OTBackend/OTLinks.h"
#include "OTBackend/OTPath.h"

wCleanup::wCleanup(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wCleanup)
{
    qInfo().noquote() << "Starting " + objectName() + "...";

    qDebug() << "Set up UI...";
    ui->setupUi(this);
    //adjustSize();
    qDebug() << "UI set";

    // Load prefs
    setWindowTitle(tr("Directory cleanup") + " - " + OTInformation::name);

    DiscordGameSDK::setModule(tr("Directory cleanup"));
    DiscordGameSDK::setStatus(tr("Tidying up the OMSI directory"));
    DiscordGameSDK::setStart();
    DiscordGameSDK::setIcon("wcleanup");
    DiscordGameSDK::update();

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

/*!
    The list entries show the folders in OMSI's backslash notation (see
    on_actionAnalyze_triggered()). Every file system access has to turn that back into a
    path the platform accepts: on Windows the backslash separates components, on Linux
    it is an ordinary character in a file name - so the concatenated path pointed
    nowhere there and every action ran through without any effect.

    The displayed text itself stays unchanged.
*/
QString wCleanup::itemPath(const QString &itemText)
{
    return set.read("main", "mainDir").toString() + "/" + QString(itemText).replace('\\', '/');
}

/*!
    Moves the folder of a list entry into destinationFolder, keeping its path relative to
    the main directory.

    A file which can't be moved stays where it is - e.g. because the destination already
    contains a file with the same name (Qt never overwrites) or because the copy to
    another drive fails. Afterwards only the directories which ran empty are removed, so
    nothing that is still inside the main directory gets lost. Hidden and system files are
    moved as well; before, they weren't listed and got deleted together with the folder.

    Returns the files which could not be moved, relative to the main directory.
*/
QStringList wCleanup::moveFolder(const QString &itemText, const QString &destinationFolder)
{
    const QString mainDir = set.read("main", "mainDir").toString();
    const QString sourceFolder = itemPath(itemText);
    const QDir::Filters includeHidden = QDir::Hidden | QDir::System;

    QStringList directories;
    QDirIterator makePaths(sourceFolder, QDir::Dirs | QDir::NoDotAndDotDot | includeHidden, QDirIterator::Subdirectories);
    while (makePaths.hasNext())
    {
        const QString current = makePaths.next();
        directories << current;
        QDir().mkpath(destinationFolder + "/" + QString(current).remove(0, cutCount));
    }

    // Collect first - the directory listing isn't reliable while its entries are moved away.
    QStringList files;
    QDirIterator listFiles(sourceFolder, QDir::Files | includeHidden, QDirIterator::Subdirectories);
    while (listFiles.hasNext()) files << listFiles.next().remove(0, cutCount);

    QStringList failedFiles;
    for (const QString &current : files)
    {
        qApp->processEvents();

        if (!QDir().rename(mainDir + "/" + current, destinationFolder + "/" + current))
        {
            qWarning().noquote() << QString("Could not move '%1' to '%2'.").arg(current, destinationFolder);
            failedFiles << current;
        }
    }

    // Deepest directories first - rmdir() refuses every directory which isn't empty.
    std::sort(directories.begin(), directories.end(), [](const QString &a, const QString &b) { return a.size() > b.size(); });
    for (const QString &directory : directories) QDir().rmdir(directory);
    QDir().rmdir(sourceFolder);

    return failedFiles;
}

void wCleanup::on_btnAnalyze_clicked()
{
    on_actionAnalyze_triggered();
}

void wCleanup::on_actionAnalyze_triggered()
{
    qInfo() << "Initialize...";
    ui->statusbar->showMessage(tr("Initialize..."));

    if (set.read("main", "mainDir").toString() == "")
    {
        if (msg.setMainDir(this))
            set.write("main", "mainDir", set.getOmsiPath(this));
        else
        {
            ui->statusbar->showMessage(tr("No main directory selected."), 10000);
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
    ui->lwgVehicles->clear();
    filehandler.stuffobj.clear();

    cutCount = set.read("main", "mainDir").toString().size() + 1;

    QList<QPair<QString, QString>> globals = filehandler.listMaps();

    qInfo() << "Read maps...";

    ui->pgbProgress->setMaximum(ui->pgbProgress->maximum() + globals.size());

    for (int i = 0; i < globals.size(); i++)
    {
        filehandler.setMapPath(globals.at(i).second);

        ui->statusbar->showMessage(tr("Read map %1 of %2 (%3)").arg(i + 1).arg(globals.size()).arg(globals.at(i).first));
        qInfo().noquote() << QString("Read map '%1' (%2)").arg(globals.at(i).first, filehandler.getMapPath().remove(0, cutCount));
        ui->pgbProgress->setValue(ui->pgbProgress->value() + 1);

        filehandler.getTiles();
        filehandler.getItems(filehandler.stuffobj.existing.tiles, false);
        filehandler.getVehicles();
    }

    filehandler.stuffobj.existing.removeDuplicates();
    filehandler.stuffobj.toBackslash();

    const QString mainDir = set.read("main", "mainDir").toString();

    // Sceneryobjects:
    if (!filehandler.stuffobj.existing.sceneryobjects.isEmpty())
    {
        qInfo() << "Analyze sceneryobjects...";
        ui->pgbProgress->setValue(ui->pgbProgress->value() + 1);
        ui->statusbar->showMessage(tr("Analyze sceneryobject folder..."));

        QStringList scoFolders;
        QDirIterator scoFolder(OTPath::resolve(mainDir, "Sceneryobjects"), QDir::Dirs | QDir::NoDotAndDotDot);
        while (scoFolder.hasNext()) scoFolders << scoFolder.next().replace("/", "\\");

        ui->pgbProgress->setValue(ui->pgbProgress->value() + 1);
        ui->statusbar->showMessage(tr("Comparing sceneryobjects..."));
        foreach (QString current, scoFolders)
        {
            qApp->processEvents();
            QStringListIterator iterator(filehandler.stuffobj.existing.sceneryobjects);

            int i = 0;
            while (iterator.hasNext())
                if (QString(mainDir + "/" + iterator.next()).replace("/", "\\").contains(current, Qt::CaseInsensitive))
                    i++;

            if (i == 0) ui->lwgObjects->addItem(current.remove(0, cutCount));
        }
    }

    // Splines:
    if (!filehandler.stuffobj.existing.splines.isEmpty())
    {
        qInfo() << "Analyze splines...";
        ui->pgbProgress->setValue(ui->pgbProgress->value() + 1);
        ui->statusbar->showMessage(tr("Analyze spline folder..."));

        QStringList sliFolders;
        QDirIterator sliFolder(OTPath::resolve(mainDir, "Splines"), QDir::Dirs | QDir::NoDotAndDotDot);
        while (sliFolder.hasNext()) sliFolders << sliFolder.next().replace("/", "\\");

        ui->pgbProgress->setValue(ui->pgbProgress->value() + 1);
        ui->statusbar->showMessage(tr("Comparing splines..."));

        foreach (QString current, sliFolders)
        {
            qApp->processEvents();
            QStringListIterator iterator(filehandler.stuffobj.existing.splines);

            int i = 0;
            while (iterator.hasNext())
                if (QString(QFileInfo(mainDir + "/" + iterator.next()).absolutePath()).replace("/", "\\").contains(current, Qt::CaseInsensitive))
                    i++;

            if (i == 0) ui->lwgSplines->addItem(current.remove(0, cutCount));
        }
    }


    // Vehicles:
    if (!filehandler.stuffobj.existing.sceneryobjects.isEmpty() || !filehandler.stuffobj.existing.vehicles.isEmpty())
    {
        QStringList vehiclePaths;

        qInfo() << "Analyze vehicles...";
        ui->pgbProgress->setValue(ui->pgbProgress->value() + 1);
        ui->statusbar->showMessage(tr("Analyze vehicle folder..."));

        QStringList vehFolders;
        QDirIterator vehFolder(OTPath::resolve(mainDir, "Vehicles"), QDir::Dirs | QDir::NoDotAndDotDot);
        while (vehFolder.hasNext()) vehFolders << vehFolder.next().replace("/", "\\");

        ui->pgbProgress->setValue(ui->pgbProgress->value() + 1);
        ui->statusbar->showMessage(tr("Comparing vehicles..."));

        QStringList allExistingVehFolders = filehandler.stuffobj.existing.vehicles + filehandler.stuffobj.existing.sceneryobjects;
        allExistingVehFolders.removeDuplicates();

        foreach (QString current, vehFolders)
        {
            qApp->processEvents();
            QStringListIterator iterator(allExistingVehFolders);

            int i = 0;
            while (iterator.hasNext())
                if (QString(mainDir + "/" + iterator.next()).replace("/", "\\").contains(current, Qt::CaseInsensitive))
                    i++;

            if (i == 0) vehiclePaths << current.remove(0, cutCount);
        }

        vehiclePaths.removeOne("Vehicles\\Announcements");
        vehiclePaths.removeOne("Vehicles\\Anzeigen");

        ui->lwgVehicles->addItems(vehiclePaths);
    }

    ui->btnAnalyze->setEnabled(true);
    ui->actionAnalyze->setEnabled(true);
    ui->pgbProgress->setValue(ui->pgbProgress->maximum());
    ui->statusbar->showMessage(tr("Done."), 10000);

    if ((ui->lwgObjects->count() != 0) || (ui->lwgSplines->count() != 0) || (ui->lwgVehicles->count() != 0))
        ui->gbxActions->setVisible(true);

    for (int i = 0; i < ui->lwgObjects->count(); i++) ui->lwgObjects->item(i)->setCheckState(Qt::Unchecked);
    for (int i = 0; i < ui->lwgSplines->count(); i++) ui->lwgSplines->item(i)->setCheckState(Qt::Unchecked);
    for (int i = 0; i < ui->lwgVehicles->count(); i++) ui->lwgVehicles->item(i)->setCheckState(Qt::Unchecked);

    qInfo() << "Finished.";
}

void wCleanup::on_btnStartAction_clicked()
{
    ui->btnStartAction->setEnabled(false);
    ui->btnAnalyze->setEnabled(false);
    ui->actionAnalyze->setEnabled(false);

    ui->pgbProgress->setValue(0);
    ui->pgbProgress->setMaximum(4);

    ui->lwgObjects->setEnabled(false);
    ui->lwgSplines->setEnabled(false);
    ui->lwgVehicles->setEnabled(false);

    QList<int> objectIDs;
    for (int i = 0; i < ui->lwgObjects->count(); i++) if (ui->lwgObjects->item(i)->checkState() == Qt::Checked) objectIDs << i;

    QList<int> splineIDs;
    for (int i = 0; i < ui->lwgSplines->count(); i++) if (ui->lwgSplines->item(i)->checkState() == Qt::Checked) splineIDs << i;

    QList<int> vehicleIDs;
    for (int i = 0; i < ui->lwgVehicles->count(); i++) if (ui->lwgVehicles->item(i)->checkState() == Qt::Checked) vehicleIDs << i;

    if (ui->rbtnMoveToFolder->isChecked())
    {
        QString destinationFolder = QFileDialog::getExistingDirectory(this, tr("Select destination folder..."));

        if (!destinationFolder.isEmpty())
        {
            // Folders with files left behind stay in their list.
            QStringList failedFiles;

            // Sceneryobjects
            ui->pgbProgress->setValue(1);
            ui->statusbar->showMessage(tr("Move sceneryobjects..."));
            qInfo().noquote() << QString("Move selected sceneryobjects to '%1'...").arg(destinationFolder);

            QList<int> keptObjectIDs;
            for (int i = 0; i < objectIDs.count(); i++)
            {
                ui->statusbar->showMessage(tr("Move sceneryobjects (%1 of %2)...").arg(QString::number(i + 1), QString::number(objectIDs.count())));

                const QStringList failed = moveFolder(ui->lwgObjects->item(objectIDs[i])->text(), destinationFolder);
                if (!failed.isEmpty())
                {
                    failedFiles << failed;
                    keptObjectIDs << objectIDs[i];
                }
            }

            for (int id : keptObjectIDs) objectIDs.removeOne(id);
            std::sort(objectIDs.begin(), objectIDs.end(), std::greater<int>());
            for (int i = 0; i < objectIDs.count(); i++) ui->lwgObjects->takeItem(objectIDs[i]);

            // Splines
            ui->pgbProgress->setValue(2);
            qInfo().noquote() << QString("Move selected splines to '%1'...").arg(destinationFolder);

            QList<int> keptSplineIDs;
            for (int i = 0; i < splineIDs.count(); i++)
            {
                ui->statusbar->showMessage(tr("Move splines (%1 of %2)...").arg(QString::number(i + 1), QString::number(splineIDs.count())));

                const QStringList failed = moveFolder(ui->lwgSplines->item(splineIDs[i])->text(), destinationFolder);
                if (!failed.isEmpty())
                {
                    failedFiles << failed;
                    keptSplineIDs << splineIDs[i];
                }
            }

            for (int id : keptSplineIDs) splineIDs.removeOne(id);
            std::sort(splineIDs.begin(), splineIDs.end(), std::greater<int>());
            for (int i = 0; i < splineIDs.count(); i++) ui->lwgSplines->takeItem(splineIDs[i]);

            // Vehicles
            ui->pgbProgress->setValue(3);
            qInfo().noquote() << QString("Move selected vehicles to '%1'...").arg(destinationFolder);

            QList<int> keptVehicleIDs;
            for (int i = 0; i < vehicleIDs.count(); i++)
            {
                ui->statusbar->showMessage(tr("Move vehicles (%1 of %2)...").arg(QString::number(i + 1), QString::number(vehicleIDs.count())));

                const QStringList failed = moveFolder(ui->lwgVehicles->item(vehicleIDs[i])->text(), destinationFolder);
                if (!failed.isEmpty())
                {
                    failedFiles << failed;
                    keptVehicleIDs << vehicleIDs[i];
                }
            }

            for (int id : keptVehicleIDs) vehicleIDs.removeOne(id);
            std::sort(vehicleIDs.begin(), vehicleIDs.end(), std::greater<int>());
            for (int i = 0; i < vehicleIDs.count(); i++) ui->lwgVehicles->takeItem(vehicleIDs[i]);

            ui->pgbProgress->setValue(4);

            if (failedFiles.isEmpty())
                ui->statusbar->showMessage(tr("Moved selected folders to '%1'.").arg(destinationFolder), 10000);
            else
            {
                ui->statusbar->showMessage(tr("Not all files could be moved."), 10000);

                QMessageBox box(QMessageBox::Warning, tr("Not all files could be moved"),
                                tr("%n file(s) could not be moved to '%1' and stayed in the OMSI directory, e.g. because the destination folder already contains a file with the same name. The affected folders are still listed.", "", failedFiles.count()).arg(destinationFolder),
                                QMessageBox::Ok, this);
                box.setDetailedText(failedFiles.join("\n"));
                box.exec();
            }
        }
    }
    else if (ui->rbtnDelete->isChecked())
    {
        QMessageBox::StandardButton reply = QMessageBox::warning(this, tr("Delete files permanently"), tr("Please keep in mind that this option will delete all files PERMANENTLY. After deletion there is no possibility to restore them. Continue?"), QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes)
        {
            // Sceneryobjects
            for (int i = 0; i < objectIDs.count(); i++)
            {
                qApp->processEvents();
                ui->statusbar->showMessage(tr("Delete sceneryobjects (%1 of %2)...").arg(QString::number(i + 1), QString::number(objectIDs.count())));
                QDir(itemPath(ui->lwgObjects->item(objectIDs[i])->text())).removeRecursively();
            }

            std::sort(objectIDs.begin(), objectIDs.end(), std::greater<int>());
            for (int i = 0; i < objectIDs.count(); i++) ui->lwgObjects->takeItem(objectIDs[i]);

            // Splines
            for (int i = 0; i < splineIDs.count(); i++)
            {
                qApp->processEvents();
                ui->statusbar->showMessage(tr("Delete splines (%1 of %2)...").arg(QString::number(i + 1), QString::number(splineIDs.count())));
                QDir(itemPath(ui->lwgSplines->item(splineIDs[i])->text())).removeRecursively();
            }

            std::sort(splineIDs.begin(), splineIDs.end(), std::greater<int>());
            for (int i = 0; i < splineIDs.count(); i++) ui->lwgSplines->takeItem(splineIDs[i]);

            // Vehicles
            for (int i = 0; i < vehicleIDs.count(); i++)
            {
                qApp->processEvents();
                ui->statusbar->showMessage(tr("Delete vehicles (%1 of %2)...").arg(QString::number(i + 1), QString::number(vehicleIDs.count())));
                QDir(itemPath(ui->lwgVehicles->item(vehicleIDs[i])->text())).removeRecursively();
            }

            std::sort(vehicleIDs.begin(), vehicleIDs.end(), std::greater<int>());
            for (int i = 0; i < vehicleIDs.count(); i++) ui->lwgVehicles->takeItem(vehicleIDs[i]);
        }

        ui->statusbar->showMessage(tr("Deleted selected folders."), 10000);
    }

    qInfo() << "Finished.";

    if ((ui->lwgObjects->count() == 0) && (ui->lwgSplines->count() == 0) && (ui->lwgVehicles->count() == 0))
        ui->gbxActions->setVisible(false);


    ui->lwgObjects->setEnabled(true);
    ui->lwgSplines->setEnabled(true);
    ui->lwgVehicles->setEnabled(true);

    ui->btnStartAction->setEnabled(true);
    ui->btnAnalyze->setEnabled(true);
    ui->actionAnalyze->setEnabled(true);
}

void wCleanup::on_rbtnMoveToFolder_clicked()
{
    ui->btnStartAction->setText(tr("Select destination folder && move..."));
}

void wCleanup::on_rbtnDelete_clicked()
{
    ui->btnStartAction->setText(tr("Delete permanently"));
}

void wCleanup::on_actionClose_triggered()
{
    close();
}

void wCleanup::on_actionPreferences_triggered()
{
    WPREFERENCES = new wPreferences();
    WPREFERENCES->setWindowModality(Qt::ApplicationModal);
    WPREFERENCES->show();
}

void wCleanup::on_actionSendFeedback_triggered()
{
    wFeedback *WFEEDBACK = new wFeedback(this, OTLinks::Wiki::cleanup);
    WFEEDBACK->setWindowModality(Qt::ApplicationModal);
    WFEEDBACK->show();
}

void wCleanup::on_actionBackToHome_triggered()
{
    close();
    emit backToHome();
}

void wCleanup::on_lwgObjects_itemDoubleClicked(QListWidgetItem *item)
{
    misc.openInExplorer(itemPath(item->text()));
}

void wCleanup::on_lwgSplines_itemDoubleClicked(QListWidgetItem *item)
{
    misc.openInExplorer(itemPath(item->text()));
}

void wCleanup::on_lwgVehicles_itemDoubleClicked(QListWidgetItem *item)
{
    misc.openInExplorer(itemPath(item->text()));
}

void wCleanup::on_actionBulkMarkInCurrentList_triggered()
{
    if (ui->lwgObjects->hasFocus())
    {
        for (int i = 0; i < ui->lwgObjects->selectedItems().count(); i++)
        {
            if (ui->lwgObjects->selectedItems()[i]->checkState() == Qt::Unchecked) ui->lwgObjects->selectedItems()[i]->setCheckState(Qt::Checked);
            else ui->lwgObjects->selectedItems()[i]->setCheckState(Qt::Unchecked);
        }
    }
    else if (ui->lwgSplines->hasFocus())
    {
        for (int i = 0; i < ui->lwgSplines->selectedItems().count(); i++)
        {
            if (ui->lwgSplines->selectedItems()[i]->checkState() == Qt::Unchecked) ui->lwgSplines->selectedItems()[i]->setCheckState(Qt::Checked);
            else ui->lwgSplines->selectedItems()[i]->setCheckState(Qt::Unchecked);
        }
    }
    else if (ui->lwgVehicles->hasFocus())
    {
        for (int i = 0; i < ui->lwgVehicles->selectedItems().count(); i++)
        {
            if (ui->lwgVehicles->selectedItems()[i]->checkState() == Qt::Unchecked) ui->lwgVehicles->selectedItems()[i]->setCheckState(Qt::Checked);
            else ui->lwgVehicles->selectedItems()[i]->setCheckState(Qt::Unchecked);
        }
    }
}

