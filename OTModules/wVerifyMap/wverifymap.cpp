#include "wverifymap.h"
#include "ui_wverifymap.h"

wVerifyMap::wVerifyMap(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wVerifyMap)
{
    qInfo().noquote() << "Starting " + objectName() + "...";
    qDebug() << "Set up UI...";
    ui->setupUi(this);
    //adjustSize();
    qDebug() << "UI set";

    setWindowTitle(OTName + " - " + tr("map verify"));

    // Set default settings
    if (!set.read(objectName(), "advVerifying").isValid())
        set.write(objectName(), "advVerifying", false);

    if (!set.read(objectName(), "onlyMapTextures").isValid())
        set.write(objectName(), "onlyMapTextures", false);

    loadMapList();
    on_cobxMapName_currentIndexChanged(ui->cobxMapName->currentIndex());

    // ---
    ui->hlaTiles->insertWidget(1, new verifyMapTools(ui->lwgTilesAll, ui->lwgTilesMissing, this));
    ui->hlaTextures->insertWidget(1, new verifyMapTools(ui->lwgTexturesAll, ui->lwgTexturesMissing, this));
    ui->hlaObjects->insertWidget(1, new verifyMapTools(ui->lwgObjectsAll, ui->lwgObjectsMissing, this));
    ui->hlaSplines->insertWidget(1, new verifyMapTools(ui->lwgSplinesAll, ui->lwgSplinesMissing, this));
    ui->hlaVehicles->insertWidget(1, new verifyMapTools(ui->lwgVehiclesAll, ui->lwgVehiclesMissing, this));
    ui->hlaHumans->insertWidget(1, new verifyMapTools(ui->lwgHumansAll, ui->lwgHumansMissing, this));

    // Load settings
    setStyleSheet(set.read("main", "theme").toString());

    // Hide detail buttons
    ui->btnTilesDetails->setVisible(false);
    ui->btnTexturesDetails->setVisible(false);
    ui->btnObjectsDetails->setVisible(false);
    ui->btnSplinesDetails->setVisible(false);
    ui->btnVehiclesDetails->setVisible(false);
    ui->btnHumansDetails->setVisible(false);

    // Connect S&S
    connect(watchProgress, SIGNAL(timeout()), this, SLOT(reloadProgress()));
    ui->pgbProgress->setVisible(false);
    ui->statusbar->showMessage(QString(tr("Press %1 to start the verification.")).arg("\"" + ui->btnStartVerifying->text() + "\""));
    ui->twgVerfying->setCurrentIndex(0);

    qInfo().noquote() << objectName() + " started";

    ui->lwgTilesAll->installEventFilter(this);
}

wVerifyMap::~wVerifyMap()
{
    delete ui;
}

/// Copies a entry from listWidget.
void wVerifyMap::on_lwgTilesAll_itemDoubleClicked(QListWidgetItem *item)
{
    misc.copy(item->text());
}

/// Copies a entry from listWidget.
void wVerifyMap::on_lwgTilesMissing_itemDoubleClicked(QListWidgetItem *item)
{
    misc.copy(item->text());
}

/// Copies a entry from listWidget.
void wVerifyMap::on_lwgTexturesAll_itemDoubleClicked(QListWidgetItem *item)
{
    misc.copy(item->text());
}

/// Copies a entry from listWidget.
void wVerifyMap::on_lwgTexturesMissing_itemDoubleClicked(QListWidgetItem *item)
{
    misc.copy(item->text());
}

/// Copies a entry from listWidget.
void wVerifyMap::on_lwgObjectsAll_itemDoubleClicked(QListWidgetItem *item)
{
    misc.copy(item->text());
}

/// Copies a entry from listWidget.
void wVerifyMap::on_lwgObjectsMissing_itemDoubleClicked(QListWidgetItem *item)
{
    misc.copy(item->text());
}

/// Copies a entry from listWidget.
void wVerifyMap::on_lwgSplinesAll_itemDoubleClicked(QListWidgetItem *item)
{
    misc.copy(item->text());
}

/// Copies a entry from listWidget.
void wVerifyMap::on_lwgSplinesMissing_itemDoubleClicked(QListWidgetItem *item)
{
    misc.copy(item->text());
}

/// Copies a entry from listWidget.
void wVerifyMap::on_lwgVehiclesAll_itemDoubleClicked(QListWidgetItem *item)
{
    misc.copy(item->text());
}

/// Copies a entry from listWidget.
void wVerifyMap::on_lwgVehiclesMissing_itemDoubleClicked(QListWidgetItem *item)
{
    misc.copy(item->text());
}

/// Copies a entry from listWidget.
void wVerifyMap::on_lwgHumansAll_itemDoubleClicked(QListWidgetItem *item)
{
    misc.copy(item->text());
}

/// Copies a entry from listWidget.
void wVerifyMap::on_lwgHumansMissing_itemDoubleClicked(QListWidgetItem *item)
{
    misc.copy(item->text());
}

/// Shows details for tiles
void wVerifyMap::on_btnTilesDetails_clicked()
{
    ui->twgVerfying->setCurrentIndex(1);
}

/// Shows details for tiles
void wVerifyMap::on_btnTexturesDetails_clicked()
{
    ui->twgVerfying->setCurrentIndex(2);
}

/// Shows details for objects
void wVerifyMap::on_btnObjectsDetails_clicked()
{
    ui->twgVerfying->setCurrentIndex(3);
}

/// Shows details for splines
void wVerifyMap::on_btnSplinesDetails_clicked()
{
    ui->twgVerfying->setCurrentIndex(4);
}

/// Shows details for vehicles
void wVerifyMap::on_btnVehiclesDetails_clicked()
{
    ui->twgVerfying->setCurrentIndex(5);
}

/// Shows details for humans
void wVerifyMap::on_btnHumansDetails_clicked()
{
    ui->twgVerfying->setCurrentIndex(6);
}

/// Adjusts the size of the window
void wVerifyMap::on_actionAdjustWindowSize_triggered()
{
    adjustSize();
}

/// Opens bug report module
void wVerifyMap::on_actionSendFeedback_triggered()
{
    wFeedback *WFEEDBACK = new wFeedback(this, OTLinks::wiki::verifyMap);
    WFEEDBACK->setWindowModality(Qt::ApplicationModal);
    WFEEDBACK->show();
}

/// Starts verifying
void wVerifyMap::on_actionStartVerifying_triggered()
{
    on_btnStartVerifying_clicked();
}

/// Selects all items from lwg's and deletes them (and more)
void wVerifyMap::selectAllAndClear()
{
    qDebug() << "Clear view...";

    // Clear ListWidgets:
    ui->lwgTilesAll->selectAll();
    qDeleteAll (ui->lwgTilesAll->selectedItems());
    ui->lwgTilesMissing->selectAll();
    qDeleteAll (ui->lwgTilesMissing->selectedItems());

    ui->lwgTexturesAll->selectAll();
    qDeleteAll (ui->lwgTexturesAll->selectedItems());
    ui->lwgTexturesMissing->selectAll();
    qDeleteAll (ui->lwgTexturesMissing->selectedItems());

    ui->lwgObjectsAll->selectAll();
    qDeleteAll (ui->lwgObjectsAll->selectedItems());
    ui->lwgObjectsMissing->selectAll();
    qDeleteAll (ui->lwgObjectsMissing->selectedItems());

    ui->lwgSplinesAll->selectAll();
    qDeleteAll (ui->lwgSplinesAll->selectedItems());
    ui->lwgSplinesMissing->selectAll();
    qDeleteAll (ui->lwgSplinesMissing->selectedItems());

    ui->lwgVehiclesAll->selectAll();
    qDeleteAll (ui->lwgVehiclesAll->selectedItems());
    ui->lwgVehiclesMissing->selectAll();
    qDeleteAll (ui->lwgVehiclesMissing->selectedItems());

    ui->lwgHumansAll->selectAll();
    qDeleteAll (ui->lwgHumansAll->selectedItems());
    ui->lwgHumansMissing->selectAll();
    qDeleteAll (ui->lwgHumansMissing->selectedItems());

    // Clear all lineEdits:
    ui->ledTotalTiles->clear();
    ui->ledMissingTiles->clear();
    ui->ledTotalTextures->clear();
    ui->ledMissingTextures->clear();

    ui->ledTotalObjects->clear();
    ui->ledMissingObjects->clear();
    ui->ledTotalSplines->clear();
    ui->ledMissingSplines->clear();

    ui->ledTotalHumans->clear();
    ui->ledMissingHumans->clear();
    ui->ledTotalVehicles->clear();
    ui->ledMissingVehicles->clear();

    // set Detail buttons unvisible
    ui->btnTilesDetails->setVisible(false);
    ui->btnTexturesDetails->setVisible(false);
    ui->btnObjectsDetails->setVisible(false);
    ui->btnSplinesDetails->setVisible(false);
    ui->btnVehiclesDetails->setVisible(false);
    ui->btnHumansDetails->setVisible(false);

    // Set ingore-Int's to 0:
    ignoredTiles = 0;
    ignoredTextures = 0;
    ignoredObjects = 0;
    ignoredSplines = 0;
    ignoredVehicles = 0;
    ignoredHumans = 0;

    // Reset the ignore-Labels
    ui->lblIgnoredTiles->setText("-");
    ui->lblIgnoredTextures->setText("-");
    ui->lblIgnoredObjects->setText("-");
    ui->lblIgnoredSplines->setText("-");
    ui->lblIgnoredVehicles->setText("-");
    ui->lblIgnoredHumans->setText("-");

    ui->pgbProgress->setVisible(false);
}


/// Reloads progress state
void wVerifyMap::reloadProgress()
{
    ui->pgbProgress->setMinimum(0);
    ui->pgbProgress->setMaximum(filehandler.maxProgress);
    ui->pgbProgress->setValue(filehandler.currentProgress);
    ui->statusbar->showMessage(filehandler.progressName);
}

/// Starts / ends the progress
void wVerifyMap::startEndWatchProgress(bool state)
{
    if (state)
        watchProgress->start(50);
    else
    {
        watchProgress->stop();
        ui->pgbProgress->setMinimum(0);
        ui->pgbProgress->setMaximum(100);
        ui->pgbProgress->setValue(100);
    }
}

/// Starts verifying
void wVerifyMap::enableView(bool enable)
{
    ui->btnStartVerifying->setEnabled(enable);
    ui->btnVerifycationSettings->setEnabled(enable);
    ui->cobxMapName->setEnabled(enable);
    ui->btnReloadMaps->setEnabled(enable);

    if (enable)
        ui->btnStartVerifying->setText(tr("Start verifying"));
    else
        ui->btnStartVerifying->setText(tr("Running..."));
}

void wVerifyMap::on_btnStartVerifying_clicked()
{
    enableIgnoreLabels(true);

    qInfo() << "Start verifying...";

    if (set.read("main", "mainDir").toString() == "")
    {
        if (msg.setMainDir(this))
            set.write("main", "mainDir", set.getOmsiPath(this));
        else
            return;
        cutCount = set.read("main", "mainDir").toString().count() + 1;
    }

    enableView(false);
    startEndWatchProgress(true);
    ui->twgVerfying->setCurrentIndex(0);

    if (!QFile::exists(filehandler.getMapPath()))
    {
        QMessageBox::warning(this, tr("Map file doesn't exist"), tr("The selected map file doesn't exist."));
        qWarning() << QString("Could not found map file '%1'!").arg(filehandler.getMapPath());
        return endVerifying();
    }
    selectAllAndClear();
    ui->pgbProgress->setVisible(true);

    // MAP:
    qInfo() << "Get tiles...";
    filehandler.getTiles(this);
    qDebug() << "Got tiles.";

    filehandler.stuffobj.missing.tiles = iglF.check(filehandler.stuffobj.missing.tiles, ignoredTiles);
    filehandler.stuffobj.existing.tiles = iglF.check(filehandler.stuffobj.existing.tiles, ignoredTiles);

    // SCO and SLI:
    qInfo() << "Get sceneryobjects / splines...";
    filehandler.getItems(filehandler.stuffobj.existing.tiles);
    qDebug() << "Got sceneryobjects / splines.";

    filehandler.stuffobj.missing.sceneryobjects = iglF.check(filehandler.stuffobj.missing.sceneryobjects, ignoredObjects);
    filehandler.stuffobj.existing.sceneryobjects = iglF.check(filehandler.stuffobj.existing.sceneryobjects, ignoredObjects);

    filehandler.stuffobj.missing.splines = iglF.check(filehandler.stuffobj.missing.splines, ignoredSplines);
    filehandler.stuffobj.existing.splines = iglF.check(filehandler.stuffobj.existing.splines, ignoredSplines);

    filehandler.stuffobj.removeDuplicates();

    // SCO and SLI (advanced)
    if (set.read(objectName(), "advVerifying").toBool())
    {
        qInfo() << "Checking sceneryobjects...";
        filehandler.verifyObjects(filehandler.stuffobj.existing.sceneryobjects);
        qDebug() << "Checked sceneryobjects.";

        qInfo() << "Checking splines...";
        filehandler.verifySplines(filehandler.stuffobj.existing.splines);
        qDebug() << "Checked splines.";
    }

    // VEH:
    qInfo() << "Get vehicles...";
    filehandler.getVehicles(this);
    qDebug() << "Got vehicles.";

    filehandler.stuffobj.missing.vehicles = iglF.check(filehandler.stuffobj.missing.vehicles, ignoredVehicles);
    filehandler.stuffobj.existing.vehicles = iglF.check(filehandler.stuffobj.existing.vehicles, ignoredVehicles);

    // HUM:
    qInfo() << "Get humans...";
    filehandler.getHumans(this);
    qDebug() << "Got humans.";

    filehandler.stuffobj.missing.humans = iglF.check(filehandler.stuffobj.missing.humans, ignoredHumans);
    filehandler.stuffobj.existing.humans = iglF.check(filehandler.stuffobj.existing.humans, ignoredHumans);

    // TEX:
    if (set.read(objectName(), "advVerifying").toBool() && !set.read(objectName(), "onlyMapTextures").toBool())
    {
        qDebug() << "Read all textures...";
        filehandler.stuffobj.missing.textures = iglF.check(filehandler.stuffobj.missing.textures, ignoredTextures);
        filehandler.stuffobj.existing.textures = iglF.check(filehandler.stuffobj.existing.textures, ignoredTextures);
    }

    // GLOBAL TEX:
    qInfo() << "Get map textures...";
    filehandler.checkTextureLayers(this);
    qDebug() << "Got map textures.";

    filehandler.stuffobj.missing.globalTextures = iglF.check(filehandler.stuffobj.missing.globalTextures, ignoredTextures);
    filehandler.stuffobj.existing.globalTextures = iglF.check(filehandler.stuffobj.existing.globalTextures, ignoredTextures);

    // SET UI:
    {
        qDebug() << "Set view...";
        filehandler.stuffobj.removeDuplicates();

        // MAP:
        ui->lwgTilesAll->addItems(filehandler.stuffobj.missing.tiles);
        ui->lwgTilesAll->addItems(filehandler.stuffobj.existing.tiles);
        ui->lwgTilesAll->sortItems();
        ui->lwgTilesMissing->addItems(filehandler.stuffobj.missing.tiles);
        ui->lwgTilesMissing->sortItems();

        ui->ledTotalTiles->setText(QString::number(ui->lwgTilesAll->count()));
        ui->ledMissingTiles->setText(QString::number(ui->lwgTilesMissing->count()));

        // TEX:
        if (set.read(objectName(), "advVerifying").toBool() && !set.read(objectName(), "onlyMapTextures").toBool())
        {
            ui->lwgTexturesAll->addItems(filehandler.stuffobj.missing.textures);
            ui->lwgTexturesAll->addItems(filehandler.stuffobj.existing.textures);

            ui->lwgTexturesMissing->addItems(filehandler.stuffobj.missing.textures);
        }

        // GLOBAL TEX:
        ui->lwgTexturesAll->addItems(filehandler.stuffobj.missing.globalTextures);
        ui->lwgTexturesAll->addItems(filehandler.stuffobj.existing.globalTextures);
        ui->lwgTexturesAll->sortItems();
        ui->lwgTexturesMissing->addItems(filehandler.stuffobj.missing.globalTextures);
        ui->lwgTexturesMissing->sortItems();

        ui->ledTotalTextures->setText(QString::number(ui->lwgTexturesAll->count()));
        ui->ledMissingTextures->setText(QString::number(ui->lwgTexturesMissing->count()));

        // SCO:
        ui->lwgObjectsAll->addItems(filehandler.stuffobj.missing.sceneryobjects);
        ui->lwgObjectsAll->addItems(filehandler.stuffobj.existing.sceneryobjects);
        ui->lwgObjectsAll->sortItems();
        ui->lwgObjectsMissing->addItems(filehandler.stuffobj.missing.sceneryobjects);
        ui->lwgObjectsMissing->sortItems();

        ui->ledTotalObjects->setText(QString::number(ui->lwgObjectsAll->count()));
        ui->ledMissingObjects->setText(QString::number(ui->lwgObjectsMissing->count()));

        // SLI:
        ui->lwgSplinesAll->addItems(filehandler.stuffobj.missing.splines);
        ui->lwgSplinesAll->addItems(filehandler.stuffobj.existing.splines);
        ui->lwgSplinesAll->sortItems();

        ui->lwgSplinesMissing->addItems(filehandler.stuffobj.missing.splines);
        ui->lwgSplinesMissing->sortItems();

        ui->ledTotalSplines->setText(QString::number(ui->lwgSplinesAll->count()));
        ui->ledMissingSplines->setText(QString::number(ui->lwgSplinesMissing->count()));

        // VEH:
        ui->lwgVehiclesAll->addItems(filehandler.stuffobj.missing.vehicles);
        ui->lwgVehiclesAll->addItems(filehandler.stuffobj.existing.vehicles);
        ui->lwgVehiclesAll->sortItems();

        ui->lwgVehiclesMissing->addItems(filehandler.stuffobj.missing.vehicles);
        ui->lwgVehiclesMissing->sortItems();

        ui->ledTotalVehicles->setText(QString::number(ui->lwgVehiclesAll->count()));
        ui->ledMissingVehicles->setText(QString::number(ui->lwgVehiclesMissing->count()));

        // HUM:
        ui->lwgHumansAll->addItems(filehandler.stuffobj.missing.humans);
        ui->lwgHumansAll->addItems(filehandler.stuffobj.existing.humans);
        ui->lwgHumansAll->sortItems();

        ui->lwgHumansMissing->addItems(filehandler.stuffobj.missing.humans);
        ui->lwgHumansMissing->sortItems();

        ui->ledTotalHumans->setText(QString::number(ui->lwgHumansAll->count()));
        ui->ledMissingHumans->setText(QString::number(ui->lwgHumansMissing->count()));

        // ----------

        setDetailButtons();

        ui->lblIgnoredTiles->setText(QString::number(ignoredTiles));
        ui->lblIgnoredTextures->setText(QString::number(ignoredTextures));
        ui->lblIgnoredObjects->setText(QString::number(ignoredObjects));
        ui->lblIgnoredSplines->setText(QString::number(ignoredSplines));
        ui->lblIgnoredVehicles->setText(QString::number(ignoredVehicles));
        ui->lblIgnoredHumans->setText(QString::number(ignoredHumans));

        filehandler.stuffobj.clear();
    }

    qInfo() << "Verifycation finished.";
    ui->statusbar->showMessage(tr("Verifycation finished."), 5000);

    endVerifying();
}

/// Sets detail buttons
void wVerifyMap::setDetailButtons()
{
    qDebug() << "Set detail buttons...";

    if (!(ui->ledMissingTiles->text() == "" || ui->ledMissingTiles->text() == "0"))
        ui->btnTilesDetails->setVisible(true);

    if (!(ui->ledMissingTextures->text() == "" || ui->ledMissingTextures->text() == "0"))
        ui->btnTexturesDetails->setVisible(true);

    if (!(ui->ledMissingObjects->text() == "" || ui->ledMissingObjects->text() == "0"))
        ui->btnObjectsDetails->setVisible(true);

    if (!(ui->ledMissingSplines->text() == "" || ui->ledMissingSplines->text() == "0"))
        ui->btnSplinesDetails->setVisible(true);

    if (!(ui->ledMissingVehicles->text() == "" || ui->ledMissingVehicles->text() == "0"))
        ui->btnVehiclesDetails->setVisible(true);

    if (!(ui->ledMissingHumans->text() == "" || ui->ledMissingHumans->text() == "0"))
        ui->btnHumansDetails->setVisible(true);
}

/// Ends verifying
void wVerifyMap::endVerifying()
{
    startEndWatchProgress(false);
    enableView(true);
}

/// Closes the application
void wVerifyMap::on_actionClose_triggered()
{
    QApplication::quit();
}

/// Opens the settings
void wVerifyMap::on_actionSettings_triggered()
{
    WSETTINGS = new wSettings(this);
    WSETTINGS->setWindowModality(Qt::ApplicationModal);
    WSETTINGS->show();
}

/// Opens the ignorelist (UI)
void wVerifyMap::on_actionEditIgnorelist_triggered()
{
    DIGNORELIST = new dIgnoreList();
    DIGNORELIST->show();
}

/// Styles ignore labels
void wVerifyMap::enableIgnoreLabels(bool enable)
{
    QString str;

    if (enable)
        str = "-";
    else
        str = "?";

    ui->lblIgnoredTiles->setText(str);
    ui->lblIgnoredTextures->setText(str);
    ui->lblIgnoredObjects->setText(str);
    ui->lblIgnoredSplines->setText(str);
    ui->lblIgnoredVehicles->setText(str);
    ui->lblIgnoredHumans->setText(str);
}

/// Shows the verifycation settings
void wVerifyMap::on_btnVerifycationSettings_clicked()
{
    WSETTINGS = new wSettings(this, "wVerifyMap");
    WSETTINGS->setWindowModality(Qt::ApplicationModal);
    WSETTINGS->show();
}

/// Loads map list
void wVerifyMap::loadMapList()
{
    mapListSetupFinished = false;
    ui->cobxMapName->clear();
    qDebug() << "Reload map list...";
    mapList = filehandler.listMaps();
    qDebug().noquote() << "Map count:" << mapList.count();

    for (int i = 0; i < mapList.count(); i++)
        ui->cobxMapName->addItem(mapList[i].second);

    for (int i = 0; i < mapList.count(); i++)
    {
        if (mapList[i].first == set.read(objectName(), "mapPath").toString())
        {
            ui->cobxMapName->setCurrentIndex(i);
            i = mapList.count();
        }
    }
    mapListSetupFinished = true;
}

/// Sets UI for a new map
void wVerifyMap::on_cobxMapName_currentIndexChanged(int index)
{
    if (!mapListSetupFinished)
        return;

    set.write(objectName(), "mapPath", mapList[index].first);
    selectAllAndClear();
    filehandler.setMapPath(mapList[index].first);

    QString picture = mapList[index].first.remove(QRegularExpression("global.cfg")) + "picture.jpg";
    if (QFile(picture).exists())
        ui->lblPicture->setPixmap(QPixmap(picture));
    else
        ui->lblPicture->setPixmap(QPixmap(":/rec/data/icons/iconUnvisible.svg").scaled(185, 140));
}

/// Reloads map list
void wVerifyMap::on_btnReloadMaps_clicked()
{
    loadMapList();
    ui->statusbar->showMessage(tr("Reloaded map list."), 5000);
}

