#include "wcleanup.h"
#include "ui_wcleanup.h"

#include <QDir>
#include <QDirIterator>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>

#include "OTBackend/DiscordGameSDK.h"
#include "OTBackend/OTFolderTransfer.h"
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
    setWindowTitle(tr("Directory sync") + " - " + OTInformation::name);

    DiscordGameSDK::setModule(tr("Directory sync"));
    DiscordGameSDK::setStatus(tr("Syncing the OMSI directory"));
    DiscordGameSDK::setStart();
    DiscordGameSDK::setIcon("wcleanup");
    DiscordGameSDK::update();

    ui->statusbar->addPermanentWidget(ui->pgbProgress);
    ui->pgbProgress->setVisible(false);
    ui->gbxActions->setVisible(false);
    ui->gbxRestoreActions->setVisible(false);
    on_rbtnMoveToFolder_clicked();

    ui->ledReferenceDir->setText(set.read("wCleanup", "referenceDir").toString());

    for (QListWidget *list : { ui->lwgRestoreObjects, ui->lwgRestoreSplines, ui->lwgRestoreVehicles })
    {
        connect(list, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem *item) {
            misc.openInExplorer(OTPath::resolve(ui->ledReferenceDir->text(), item->data(Qt::UserRole).toString()));
        });
    }

    updateTabTitles();

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

QString wCleanup::topLevelFolder(const QString &relativePath)
{
    const QStringList segments = QString(relativePath).replace('/', '\\').split('\\', Qt::SkipEmptyParts);
    if (segments.size() < 3) return QString();

    return QString(segments.at(0) + "\\" + segments.at(1)).toLower();
}

/*!
    Returns the folders directly below rootFolder (e.g. "Sceneryobjects") which aren't
    contained in usedFolders (see topLevelFolder()), in OMSI's backslash notation relative
    to the main directory.
*/
QStringList wCleanup::unusedFolders(const QString &rootFolder, const QSet<QString> &usedFolders)
{
    QStringList unused;

    QDirIterator folder(OTPath::resolve(set.read("main", "mainDir").toString(), rootFolder), QDir::Dirs | QDir::NoDotAndDotDot);
    while (folder.hasNext())
    {
        const QString current = folder.next().remove(0, cutCount).replace('/', '\\');
        if (!usedFolders.contains(current.toLower())) unused << current;
    }

    return unused;
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
    ui->pgbProgress->setMaximum(7);
    ui->pgbProgress->setValue(1);

    ui->btnAnalyze->setEnabled(false);
    ui->actionAnalyze->setEnabled(false);
    ui->gbxActions->setVisible(false);
    ui->gbxRestoreActions->setVisible(false);

    ui->lwgObjects->clear();
    ui->lwgSplines->clear();
    ui->lwgVehicles->clear();
    ui->lwgRestoreObjects->clear();
    ui->lwgRestoreSplines->clear();
    ui->lwgRestoreVehicles->clear();
    filehandler.stuffobj.clear();
    missingFolders.clear();

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

        // The tile paths are relative to their map, so only the current map's may be read.
        filehandler.stuffobj.existing.tiles.clear();
        filehandler.getTiles();
        filehandler.getItems(filehandler.stuffobj.existing.tiles);
        filehandler.getVehicles();
    }

    filehandler.stuffobj.removeDuplicates();
    filehandler.stuffobj.toBackslash();

    // A folder counts as used as soon as any file inside it is used, no matter of which
    // kind - there are sceneryobjects inside spline folders and splines inside object
    // folders, and parked vehicles are sceneryobjects inside the vehicle folder.
    QSet<QString> usedFolders;
    const QStringList usedFiles = filehandler.stuffobj.existing.sceneryobjects
                                + filehandler.stuffobj.existing.splines
                                + filehandler.stuffobj.existing.vehicles;
    for (const QString &file : usedFiles) usedFolders << topLevelFolder(file);
    usedFolders.remove(QString());

    // Without any used file (e.g. no map could be read) every folder would be listed.
    if (!usedFolders.isEmpty())
    {
        // Sceneryobjects:
        qInfo() << "Analyze sceneryobjects...";
        ui->pgbProgress->setValue(ui->pgbProgress->value() + 1);
        ui->statusbar->showMessage(tr("Analyze sceneryobject folder..."));
        ui->pgbProgress->setValue(ui->pgbProgress->value() + 1);
        ui->statusbar->showMessage(tr("Comparing sceneryobjects..."));
        ui->lwgObjects->addItems(unusedFolders("Sceneryobjects", usedFolders));

        // Splines:
        qInfo() << "Analyze splines...";
        ui->pgbProgress->setValue(ui->pgbProgress->value() + 1);
        ui->statusbar->showMessage(tr("Analyze spline folder..."));
        ui->pgbProgress->setValue(ui->pgbProgress->value() + 1);
        ui->statusbar->showMessage(tr("Comparing splines..."));
        ui->lwgSplines->addItems(unusedFolders("Splines", usedFolders));

        // Vehicles:
        qInfo() << "Analyze vehicles...";
        ui->pgbProgress->setValue(ui->pgbProgress->value() + 1);
        ui->statusbar->showMessage(tr("Analyze vehicle folder..."));
        ui->pgbProgress->setValue(ui->pgbProgress->value() + 1);
        ui->statusbar->showMessage(tr("Comparing vehicles..."));

        QStringList vehiclePaths = unusedFolders("Vehicles", usedFolders);
        vehiclePaths.removeOne("Vehicles\\Announcements");
        vehiclePaths.removeOne("Vehicles\\Anzeigen");

        ui->lwgVehicles->addItems(vehiclePaths);
    }

    // Missing files, grouped like the cleanup by their top-level folder. Placeholders like
    // "[Aigroup] ..." have no such folder and drop out here.
    qInfo() << "Analyze missing folders...";
    ui->pgbProgress->setValue(ui->pgbProgress->value() + 1);
    ui->statusbar->showMessage(tr("Looking up missing folders in the reference folder..."));

    const QStringList missingFiles = filehandler.stuffobj.missing.sceneryobjects
                                   + filehandler.stuffobj.missing.splines
                                   + filehandler.stuffobj.missing.vehicles;
    for (const QString &file : missingFiles)
    {
        const QString key = topLevelFolder(file);
        if (!QStringList({ "sceneryobjects", "splines", "vehicles" }).contains(key.section('\\', 0, 0)))
            continue;

        MissingFolder &folder = missingFolders[key];
        if (folder.name.isEmpty()) folder.name = file.section('\\', 0, 1);
        if (!folder.files.contains(file, Qt::CaseInsensitive)) folder.files << file;
    }

    updateRestoreLists();

    ui->btnAnalyze->setEnabled(true);
    ui->actionAnalyze->setEnabled(true);
    ui->pgbProgress->setValue(ui->pgbProgress->maximum());
    ui->statusbar->showMessage(tr("Done."), 10000);

    if ((ui->lwgObjects->count() != 0) || (ui->lwgSplines->count() != 0) || (ui->lwgVehicles->count() != 0))
        ui->gbxActions->setVisible(true);

    for (int i = 0; i < ui->lwgObjects->count(); i++) ui->lwgObjects->item(i)->setCheckState(Qt::Unchecked);
    for (int i = 0; i < ui->lwgSplines->count(); i++) ui->lwgSplines->item(i)->setCheckState(Qt::Unchecked);
    for (int i = 0; i < ui->lwgVehicles->count(); i++) ui->lwgVehicles->item(i)->setCheckState(Qt::Unchecked);

    updateTabTitles();

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
            const QString mainDir = set.read("main", "mainDir").toString();

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

                const OTFolderTransfer::Result result = OTFolderTransfer::transfer(mainDir, destinationFolder, ui->lwgObjects->item(objectIDs[i])->text(), OTFolderTransfer::Move);
                const QStringList failed = result.skipped + result.failed;
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

                const OTFolderTransfer::Result result = OTFolderTransfer::transfer(mainDir, destinationFolder, ui->lwgSplines->item(splineIDs[i])->text(), OTFolderTransfer::Move);
                const QStringList failed = result.skipped + result.failed;
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

                const OTFolderTransfer::Result result = OTFolderTransfer::transfer(mainDir, destinationFolder, ui->lwgVehicles->item(vehicleIDs[i])->text(), OTFolderTransfer::Move);
                const QStringList failed = result.skipped + result.failed;
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

    updateTabTitles();


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
    for (QListWidget *list : { ui->lwgObjects, ui->lwgSplines, ui->lwgVehicles, ui->lwgRestoreObjects, ui->lwgRestoreSplines, ui->lwgRestoreVehicles })
    {
        if (!list->hasFocus()) continue;

        for (QListWidgetItem *item : list->selectedItems())
        {
            if (!(item->flags() & Qt::ItemIsUserCheckable)) continue;
            item->setCheckState(item->checkState() == Qt::Unchecked ? Qt::Checked : Qt::Unchecked);
        }
    }
}

QListWidget *wCleanup::restoreList(const QString &folderName)
{
    const QString root = folderName.section('\\', 0, 0).toLower();
    if (root == "sceneryobjects") return ui->lwgRestoreObjects;
    if (root == "splines") return ui->lwgRestoreSplines;
    return ui->lwgRestoreVehicles;
}

/*!
    A folder can be restored as soon as the reference folder contains at least one of its
    missing files - the whole folder is transferred then, even if some files stay missing.
*/
void wCleanup::updateRestoreLists()
{
    const QString referenceDir = ui->ledReferenceDir->text();
    bool restorable = false;

    ui->lwgRestoreObjects->clear();
    ui->lwgRestoreSplines->clear();
    ui->lwgRestoreVehicles->clear();

    for (const MissingFolder &folder : std::as_const(missingFolders))
    {
        QStringList notFound;
        for (const QString &file : folder.files)
            if (referenceDir.isEmpty() || !OTPath::exists(referenceDir, file)) notFound << file;

        const int found = folder.files.size() - notFound.size();

        QListWidgetItem *item = new QListWidgetItem(QString("%1 (%2/%3)").arg(folder.name).arg(found).arg(folder.files.size()));
        item->setData(Qt::UserRole, folder.name);

        QString toolTip = tr("Missing files found in the reference folder: %1 of %2").arg(found).arg(folder.files.size());
        if (!notFound.isEmpty())
        {
            const int maxLines = 15;
            toolTip += "\n\n" + tr("Not in the reference folder:") + "\n" + notFound.mid(0, maxLines).join("\n");
            if (notFound.size() > maxLines) toolTip += "\n" + tr("... and %n more", "", notFound.size() - maxLines);
        }
        item->setToolTip(toolTip);

        if (found == 0)
            item->setFlags(item->flags() & ~(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable));
        else
        {
            item->setCheckState(Qt::Unchecked);
            restorable = true;
        }

        restoreList(folder.name)->addItem(item);
    }

    ui->gbxRestoreActions->setVisible(restorable);
    updateTabTitles();
}

void wCleanup::updateTabTitles()
{
    const int unused = ui->lwgObjects->count() + ui->lwgSplines->count() + ui->lwgVehicles->count();
    const int missing = ui->lwgRestoreObjects->count() + ui->lwgRestoreSplines->count() + ui->lwgRestoreVehicles->count();

    ui->twgMode->setTabText(ui->twgMode->indexOf(ui->tabCleanup), unused == 0 ? tr("Clean up") : tr("Clean up (%1)").arg(unused));
    ui->twgMode->setTabText(ui->twgMode->indexOf(ui->tabRestore), missing == 0 ? tr("Restore") : tr("Restore (%1)").arg(missing));
}

void wCleanup::on_btnSelectReferenceDir_clicked()
{
    const QString referenceDir = QFileDialog::getExistingDirectory(this, tr("Select reference folder..."), ui->ledReferenceDir->text());
    if (referenceDir.isEmpty()) return;

    if (QDir(referenceDir) == QDir(set.read("main", "mainDir").toString()))
    {
        QMessageBox::warning(this, tr("Invalid reference folder"), tr("The reference folder must not be the OMSI directory itself."));
        return;
    }

    ui->ledReferenceDir->setText(referenceDir);
    set.write("wCleanup", "referenceDir", referenceDir);
    updateRestoreLists();
}

void wCleanup::on_btnStartRestore_clicked()
{
    const QString referenceDir = ui->ledReferenceDir->text();
    const QString mainDir = set.read("main", "mainDir").toString();
    const OTFolderTransfer::Mode mode = ui->rbtnRestoreMove->isChecked() ? OTFolderTransfer::Move : OTFolderTransfer::Copy;

    QStringList folders;
    for (QListWidget *list : { ui->lwgRestoreObjects, ui->lwgRestoreSplines, ui->lwgRestoreVehicles })
        for (int i = 0; i < list->count(); i++)
            if (list->item(i)->checkState() == Qt::Checked) folders << list->item(i)->data(Qt::UserRole).toString();

    if (folders.isEmpty())
    {
        ui->statusbar->showMessage(tr("No folder selected."), 10000);
        return;
    }

    ui->twgMode->setEnabled(false);
    ui->btnAnalyze->setEnabled(false);
    ui->actionAnalyze->setEnabled(false);

    ui->pgbProgress->setValue(0);
    ui->pgbProgress->setMaximum(folders.size());

    qInfo().noquote() << QString("%1 %2 folder(s) from '%3'...").arg(mode == OTFolderTransfer::Move ? "Move" : "Copy").arg(folders.size()).arg(referenceDir);

    int transferred = 0;
    int skipped = 0;
    QStringList failedFiles;
    for (int i = 0; i < folders.size(); i++)
    {
        ui->statusbar->showMessage(tr("Restore folder %1 of %2 (%3)...").arg(i + 1).arg(folders.size()).arg(folders.at(i)));
        ui->pgbProgress->setValue(i);

        const OTFolderTransfer::Result result = OTFolderTransfer::transfer(referenceDir, mainDir, folders.at(i), mode);
        transferred += result.transferred.size();
        skipped += result.skipped.size();
        failedFiles << result.failed;
    }

    qInfo().noquote() << QString("Restored %1 file(s), %2 already existed, %3 failed.").arg(transferred).arg(skipped).arg(failedFiles.size());

    // A restored train only now reveals the vehicles it consists of.
    on_actionAnalyze_triggered();
    ui->twgMode->setEnabled(true);
    ui->twgMode->setCurrentWidget(ui->tabRestore);

    QString status = tr("Restored %n file(s).", "", transferred);
    if (skipped != 0) status += " " + tr("%n file(s) already existed and were kept.", "", skipped);
    ui->statusbar->showMessage(status, 10000);

    if (!failedFiles.isEmpty())
    {
        QMessageBox box(QMessageBox::Warning, tr("Not all files could be restored"),
                        tr("%n file(s) could not be transferred from the reference folder. The other files were restored.", "", failedFiles.count()),
                        QMessageBox::Ok, this);
        box.setDetailedText(failedFiles.join("\n"));
        box.exec();
    }
}
