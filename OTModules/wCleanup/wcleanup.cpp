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
        QDirIterator scoFolder(mainDir + "/Sceneryobjects", QDir::Dirs | QDir::NoDotAndDotDot);
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
        QDirIterator sliFolder(mainDir + "/Splines", QDir::Dirs | QDir::NoDotAndDotDot);
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
        QDirIterator vehFolder(mainDir + "/Vehicles", QDir::Dirs | QDir::NoDotAndDotDot);
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
            // Sceneryobjects
            ui->pgbProgress->setValue(1);
            ui->statusbar->showMessage(tr("Move sceneryobjects..."));
            qInfo().noquote() << QString("Move selected sceneryobjects to '%1'...").arg(destinationFolder);

            for (int i = 0; i < objectIDs.count(); i++)
            {
                ui->statusbar->showMessage(tr("Move sceneryobjects (%1 of %2)...").arg(QString::number(i + 1), QString::number(objectIDs.count())));

                QDirIterator makePaths(set.read("main", "mainDir").toString() + "/" + ui->lwgObjects->item(objectIDs[i])->text(), QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
                while (makePaths.hasNext())
                    QDir().mkpath(destinationFolder + "/" + makePaths.next().remove(0, cutCount));


                QDirIterator moveFiles(set.read("main", "mainDir").toString() + "/" + ui->lwgObjects->item(objectIDs[i])->text(), QDir::Files, QDirIterator::Subdirectories);
                while (moveFiles.hasNext())
                {
                    qApp->processEvents();

                    QString current = moveFiles.next().remove(0, cutCount);
                    QDir().rename(set.read("main", "mainDir").toString() + "/" + current, destinationFolder + "/" + current);
                }

                QDir(set.read("main", "mainDir").toString() + "/" + ui->lwgObjects->item(objectIDs[i])->text()).removeRecursively();
            }

            std::sort(objectIDs.begin(), objectIDs.end(), std::greater<int>());
            for (int i = 0; i < objectIDs.count(); i++) ui->lwgObjects->takeItem(objectIDs[i]);

            // Splines
            ui->pgbProgress->setValue(2);
            qInfo().noquote() << QString("Move selected splines to '%1'...").arg(destinationFolder);

            for (int i = 0; i < splineIDs.count(); i++)
            {
                ui->statusbar->showMessage(tr("Move splines (%1 of %2)...").arg(QString::number(i + 1), QString::number(splineIDs.count())));

                QDirIterator makePaths(set.read("main", "mainDir").toString() + "/" + ui->lwgSplines->item(splineIDs[i])->text(), QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
                while (makePaths.hasNext())
                    QDir().mkpath(destinationFolder + "/" + makePaths.next().remove(0, cutCount));

                QDirIterator moveFiles(set.read("main", "mainDir").toString() + "/" + ui->lwgSplines->item(splineIDs[i])->text(), QDir::Files, QDirIterator::Subdirectories);
                while (moveFiles.hasNext())
                {
                    qApp->processEvents();

                    QString current = moveFiles.next().remove(0, cutCount);
                    QDir().rename(set.read("main", "mainDir").toString() + "/" + current, destinationFolder + "/" + current);
                }

                QDir(set.read("main", "mainDir").toString() + "/" + ui->lwgSplines->item(splineIDs[i])->text()).removeRecursively();
            }

            std::sort(splineIDs.begin(), splineIDs.end(), std::greater<int>());
            for (int i = 0; i < splineIDs.count(); i++) ui->lwgSplines->takeItem(splineIDs[i]);

            // Vehicles
            ui->pgbProgress->setValue(3);
            qInfo().noquote() << QString("Move selected vehicles to '%1'...").arg(destinationFolder);

            for (int i = 0; i < vehicleIDs.count(); i++)
            {
                ui->statusbar->showMessage(tr("Move vehicles (%1 of %2)...").arg(QString::number(i + 1), QString::number(vehicleIDs.count())));

                QDirIterator makePaths(set.read("main", "mainDir").toString() + "/" + ui->lwgVehicles->item(vehicleIDs[i])->text(), QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
                while (makePaths.hasNext())
                    QDir().mkpath(destinationFolder + "/" + makePaths.next().remove(0, cutCount));

                QDirIterator moveFiles(set.read("main", "mainDir").toString() + "/" + ui->lwgVehicles->item(vehicleIDs[i])->text(), QDir::Files, QDirIterator::Subdirectories);
                while (moveFiles.hasNext())
                {
                    qApp->processEvents();

                    QString current = moveFiles.next().remove(0, cutCount);
                    QDir().rename(set.read("main", "mainDir").toString() + "/" + current, destinationFolder + "/" + current);
                }

                QDir(set.read("main", "mainDir").toString() + "/" + ui->lwgVehicles->item(vehicleIDs[i])->text()).removeRecursively();
            }

            std::sort(vehicleIDs.begin(), vehicleIDs.end(), std::greater<int>());
            for (int i = 0; i < vehicleIDs.count(); i++) ui->lwgVehicles->takeItem(vehicleIDs[i]);

            ui->pgbProgress->setValue(4);
            ui->statusbar->showMessage(tr("Moved selected folders to '%1'.").arg(destinationFolder), 10000);
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
                QDir(set.read("main", "mainDir").toString() + "/" + ui->lwgObjects->item(objectIDs[i])->text()).removeRecursively();
            }

            std::sort(objectIDs.begin(), objectIDs.end(), std::greater<int>());
            for (int i = 0; i < objectIDs.count(); i++) ui->lwgObjects->takeItem(objectIDs[i]);

            // Splines
            for (int i = 0; i < splineIDs.count(); i++)
            {
                qApp->processEvents();
                ui->statusbar->showMessage(tr("Delete splines (%1 of %2)...").arg(QString::number(i + 1), QString::number(splineIDs.count())));
                QDir(set.read("main", "mainDir").toString() + "/" + ui->lwgSplines->item(splineIDs[i])->text()).removeRecursively();
            }

            std::sort(splineIDs.begin(), splineIDs.end(), std::greater<int>());
            for (int i = 0; i < splineIDs.count(); i++) ui->lwgSplines->takeItem(splineIDs[i]);

            // Vehicles
            for (int i = 0; i < vehicleIDs.count(); i++)
            {
                qApp->processEvents();
                ui->statusbar->showMessage(tr("Delete vehicles (%1 of %2)...").arg(QString::number(i + 1), QString::number(vehicleIDs.count())));
                QDir(set.read("main", "mainDir").toString() + "/" + ui->lwgVehicles->item(vehicleIDs[i])->text()).removeRecursively();
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
    misc.openInExplorer(set.read("main", "mainDir").toString() + "/" + item->text());
}

void wCleanup::on_lwgSplines_itemDoubleClicked(QListWidgetItem *item)
{
    misc.openInExplorer(set.read("main", "mainDir").toString() + "/" + item->text());
}

void wCleanup::on_lwgVehicles_itemDoubleClicked(QListWidgetItem *item)
{
    misc.openInExplorer(set.read("main", "mainDir").toString() + "/" + item->text());
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

