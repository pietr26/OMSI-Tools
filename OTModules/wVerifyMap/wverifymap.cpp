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

    setWindowTitle(OTInformation::name + " - " + tr("Map verification"));

    DiscordGameSDK::setModule(tr("Map verification"));
    DiscordGameSDK::setStatus(tr("Checking a map"));
    DiscordGameSDK::start();
    DiscordGameSDK::setIcon("wverifymap");
    DiscordGameSDK::update();

    ui->statusbar->addPermanentWidget(ui->pgbProgress);

    loadMapList();
    on_cobxMapName_currentIndexChanged(ui->cobxMapName->currentIndex());

    // ---
    ui->hlaTiles->insertWidget(1, new verifyMapTools(ui->lwgTilesAll, ui->lwgTilesMissing, this));
    ui->hlaTextures->insertWidget(1, new verifyMapTools(ui->lwgTexturesAll, ui->lwgTexturesMissing, this));
    ui->hlaObjects->insertWidget(1, new verifyMapTools(ui->lwgObjectsAll, ui->lwgObjectsMissing, this));
    ui->hlaSplines->insertWidget(1, new verifyMapTools(ui->lwgSplinesAll, ui->lwgSplinesMissing, this));
    ui->hlaVehicles->insertWidget(1, new verifyMapTools(ui->lwgVehiclesAll, ui->lwgVehiclesMissing, this));
    ui->hlaHumans->insertWidget(1, new verifyMapTools(ui->lwgHumansAll, ui->lwgHumansMissing, this));

    // Connect S&S
    connect(watchProgress, SIGNAL(timeout()), this, SLOT(reloadProgress()));
    ui->pgbProgress->setVisible(false);
    ui->statusbar->showMessage(QString(tr("Press %1 to start the verification.")).arg("\"" + ui->btnStartVerifying->text() + "\""));
    ui->twgVerfying->setCurrentIndex(0);

    ui->lwgTilesAll->installEventFilter(new EventFilterCopyElements(ui->lwgTilesAll));
    ui->lwgTilesMissing->installEventFilter(new EventFilterCopyElements(ui->lwgTilesMissing));
    ui->lwgTexturesAll->installEventFilter(new EventFilterCopyElements(ui->lwgTexturesAll));
    ui->lwgTexturesMissing->installEventFilter(new EventFilterCopyElements(ui->lwgTexturesMissing));

    ui->lwgObjectsAll->installEventFilter(new EventFilterCopyElements(ui->lwgObjectsAll));
    ui->lwgObjectsMissing->installEventFilter(new EventFilterCopyElements(ui->lwgObjectsMissing));
    ui->lwgSplinesAll->installEventFilter(new EventFilterCopyElements(ui->lwgSplinesAll));
    ui->lwgSplinesMissing->installEventFilter(new EventFilterCopyElements(ui->lwgSplinesMissing));
    ui->lwgVehiclesAll->installEventFilter(new EventFilterCopyElements(ui->lwgVehiclesAll));
    ui->lwgVehiclesMissing->installEventFilter(new EventFilterCopyElements(ui->lwgVehiclesMissing));
    ui->lwgHumansAll->installEventFilter(new EventFilterCopyElements(ui->lwgHumansAll));
    ui->lwgHumansMissing->installEventFilter(new EventFilterCopyElements(ui->lwgHumansMissing));

    setToButtons();

    qInfo().noquote() << objectName() + " started";
}

wVerifyMap::~wVerifyMap()
{
    delete ui;
}

/// Opens bug report module
void wVerifyMap::on_actionSendFeedback_triggered()
{
    wFeedback *WFEEDBACK = new wFeedback(this, OTLinks::Wiki::verifyMap);
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

    ui->pgbProgress->setVisible(false);

    setToButtons();
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
    ui->btnVerificationPreferences->setEnabled(enable);
    ui->cobxMapName->setEnabled(enable);
    ui->btnReloadMaps->setEnabled(enable);

    if (enable)
        ui->btnStartVerifying->setText(tr("Start verifying"));
    else
        ui->btnStartVerifying->setText(tr("Running..."));
}

void wVerifyMap::setToButtons()
{
    ui->btnToTiles->setVisible(!ui->ledMissingTiles->text().remove("0").isEmpty());
    ui->btnToTextures->setVisible(!ui->ledMissingTextures->text().remove("0").isEmpty());
    ui->btnToObjects->setVisible(!ui->ledMissingObjects->text().remove("0").isEmpty());
    ui->btnToSplines->setVisible(!ui->ledMissingSplines->text().remove("0").isEmpty());
    ui->btnToVehicles->setVisible(!ui->ledMissingVehicles->text().remove("0").isEmpty());
    ui->btnToHumans->setVisible(!ui->ledMissingHumans->text().remove("0").isEmpty());
}

void wVerifyMap::on_btnStartVerifying_clicked()
{
    qInfo() << "Start verifying...";

    if (set.read("main", "mainDir").toString() == "")
    {
        if (msg.setMainDir(this))
            set.write("main", "mainDir", set.getOmsiPath(this));
        else
            return;
        cutCount = set.read("main", "mainDir").toString().size() + 1;
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
    qInfo().noquote() << QString("Map: %1").arg(filehandler.getMapPath());

    selectAllAndClear();
    ui->pgbProgress->setVisible(true);

    // MAP:
    qInfo() << "Get tiles...";
    filehandler.getTiles(this);
    qDebug() << "Got tiles.";

    // SCO and SLI:
    qInfo() << "Get sceneryobjects / splines...";
    filehandler.getItems(filehandler.stuffobj.existing.tiles);
    qDebug() << "Got sceneryobjects / splines.";

    filehandler.stuffobj.removeDuplicates(); // TODO: Outsource in their class / functions

    QFuture<void> scoFuture;
    QFutureWatcher<void> *scoFutureWatcher  = new QFutureWatcher<void>(this);
    QEventLoop *loop = new QEventLoop();

    // SCO and SLI (advanced)
    if (set.read(objectName(), "advVerifying").toBool())
    {
        qInfo() << "Checking sceneryobjects...";
        //filehandler.verifyObjects(filehandler.stuffobj.existing.sceneryobjects);

        //QFuture<void> future = QtConcurrent::run(&OTOMSIFileHandler::verifyObjects, &filehandler, filehandler.stuffobj.existing.sceneryobjects);
        scoFuture = QtConcurrent::run([=]() {
            filehandler.verifyObjects(filehandler.stuffobj.existing.sceneryobjects);
        });

        scoFutureWatcher->setFuture(scoFuture);
        connect(scoFutureWatcher, &QFutureWatcher<void>::finished, this, [=]() { qDebug() << "Checked sceneryobjects."; loop->quit(); });

        qInfo() << "Checking splines...";
        filehandler.verifySplines(filehandler.stuffobj.existing.splines);
        qDebug() << "Checked splines.";
    }

    // VEH:
    qInfo() << "Get vehicles...";
    filehandler.getVehicles(this);
    qDebug() << "Got vehicles.";

    // HUM:
    qInfo() << "Get humans...";
    filehandler.getHumans(this);
    qDebug() << "Got humans.";

    // GLOBAL TEX:
    qInfo() << "Get map textures...";
    filehandler.checkTextureLayers(this);
    qDebug() << "Got map textures.";

    if (set.read(objectName(), "advVerifying").toBool())
        loop->exec();

    scoFutureWatcher->deleteLater();
    loop->deleteLater();

    // SET UI:
    {
        qDebug() << "Set view...";
        filehandler.stuffobj.removeDuplicates();
        filehandler.stuffobj.toBackslash();

        // MAP:
        QStringList tiles = filehandler.stuffobj.missing.tiles + filehandler.stuffobj.existing.tiles;
        tiles.removeDuplicates();
        ui->lwgTilesAll->addItems(tiles);
        ui->lwgTilesAll->sortItems();
        ui->lwgTilesMissing->addItems(filehandler.stuffobj.missing.tiles);
        ui->lwgTilesMissing->sortItems();

        ui->ledTotalTiles->setText(QString::number(ui->lwgTilesAll->count()));
        ui->ledMissingTiles->setText(QString::number(ui->lwgTilesMissing->count()));

        QStringList textures;
        QStringList missingTextures;
        // TEX:
        if (set.read(objectName(), "advVerifying").toBool() && !set.read(objectName(), "onlyMapTextures").toBool())
        {
            textures = filehandler.stuffobj.missing.textures + filehandler.stuffobj.existing.textures;
            ui->lwgTexturesAll->addItems(textures);

            ui->lwgTexturesMissing->addItems(filehandler.stuffobj.missing.textures);
        }

        // GLOBAL TEX:
        textures.append(filehandler.stuffobj.missing.globalTextures);
        textures.append(filehandler.stuffobj.existing.globalTextures);
        textures.removeDuplicates();
        ui->lwgTexturesAll->addItems(textures);
        ui->lwgTexturesAll->sortItems();

        missingTextures.append(filehandler.stuffobj.missing.globalTextures);
        missingTextures.removeDuplicates();
        ui->lwgTexturesMissing->addItems(missingTextures);
        ui->lwgTexturesMissing->sortItems();

        ui->ledTotalTextures->setText(QString::number(ui->lwgTexturesAll->count()));
        ui->ledMissingTextures->setText(QString::number(ui->lwgTexturesMissing->count()));

        // SCO:
        QStringList objects = filehandler.stuffobj.missing.sceneryobjects + filehandler.stuffobj.existing.sceneryobjects;
        objects.removeDuplicates();
        ui->lwgObjectsAll->addItems(objects);
        ui->lwgObjectsAll->sortItems();

        ui->lwgObjectsMissing->addItems(filehandler.stuffobj.missing.sceneryobjects);
        ui->lwgObjectsMissing->sortItems();

        ui->ledTotalObjects->setText(QString::number(ui->lwgObjectsAll->count()));
        ui->ledMissingObjects->setText(QString::number(ui->lwgObjectsMissing->count()));

        // SLI:
        QStringList splines = filehandler.stuffobj.missing.splines + filehandler.stuffobj.existing.splines;
        splines.removeDuplicates();
        ui->lwgSplinesAll->addItems(splines);
        ui->lwgSplinesAll->sortItems();

        ui->lwgSplinesMissing->addItems(filehandler.stuffobj.missing.splines);
        ui->lwgSplinesMissing->sortItems();

        ui->ledTotalSplines->setText(QString::number(ui->lwgSplinesAll->count()));
        ui->ledMissingSplines->setText(QString::number(ui->lwgSplinesMissing->count()));

        // VEH:
        QStringList vehicles = filehandler.stuffobj.missing.vehicles + filehandler.stuffobj.existing.vehicles;
        vehicles.removeDuplicates();
        ui->lwgVehiclesAll->addItems(vehicles);
        ui->lwgVehiclesAll->sortItems();

        ui->lwgVehiclesMissing->addItems(filehandler.stuffobj.missing.vehicles);
        ui->lwgVehiclesMissing->sortItems();

        ui->ledTotalVehicles->setText(QString::number(ui->lwgVehiclesAll->count()));
        ui->ledMissingVehicles->setText(QString::number(ui->lwgVehiclesMissing->count()));

        // HUM:
        QStringList humans = filehandler.stuffobj.missing.humans + filehandler.stuffobj.existing.humans;
        humans.removeDuplicates();
        ui->lwgHumansAll->addItems(humans);
        ui->lwgHumansAll->sortItems();

        ui->lwgHumansMissing->addItems(filehandler.stuffobj.missing.humans);
        ui->lwgHumansMissing->sortItems();

        ui->ledTotalHumans->setText(QString::number(ui->lwgHumansAll->count()));
        ui->ledMissingHumans->setText(QString::number(ui->lwgHumansMissing->count()));

        // ----------

        filehandler.stuffobj.clear();
    }

    qInfo() << "Verification finished.";
    ui->statusbar->showMessage(tr("Verification finished."), 5000);

    endVerifying();
}

/// Ends verifying
void wVerifyMap::endVerifying()
{
    startEndWatchProgress(false);
    enableView(true);
    setToButtons();
}

/// Closes the application
void wVerifyMap::on_actionClose_triggered()
{
    QApplication::quit();
}

/// Opens the prefs
void wVerifyMap::on_actionPreferences_triggered()
{
    WPREFERENCES = new wPreferences();
    WPREFERENCES->show();
}

/// Shows the verification prefs
void wVerifyMap::on_btnVerificationPreferences_clicked()
{
    WPREFERENCES = new wPreferences(this, "wVerifyMap");
    WPREFERENCES->setWindowModality(Qt::ApplicationModal);
    WPREFERENCES->show();
}

/// Loads map list
void wVerifyMap::loadMapList()
{
    mapListSetupFinished = false;
    ui->cobxMapName->clear();

    qDebug() << "Reload map list...";
    mapList = filehandler.listMaps();

    qDebug().noquote() << "Map count:" << mapList.size();

    for (int i = 0; i < mapList.size(); i++)
        ui->cobxMapName->addItem(mapList[i].first);

    for (int i = 0; i < mapList.size(); i++)
    {
        if (mapList[i].second == set.read(objectName(), "mapPath").toString())
        {
            ui->cobxMapName->setCurrentIndex(i);
            i = mapList.size();
        }
    }
    mapListSetupFinished = true;
}

/// Sets UI for a new map
void wVerifyMap::on_cobxMapName_currentIndexChanged(int index)
{
    if ((!mapListSetupFinished) || mapList.isEmpty())
        return;

    set.write(objectName(), "mapPath", mapList[index].second);
    selectAllAndClear();
    filehandler.setMapPath(mapList[index].second);

    QString picture = mapList[index].second + "picture.jpg";
    picture.remove("global.cfg");

    if (QFile(picture).exists())
        ui->lblPicture->setPixmap(QPixmap(picture));
    else
        ui->lblPicture->setPixmap(QIcon::fromTheme(QIcon::ThemeIcon::CameraPhoto).pixmap(128, 128));
}

/// Reloads map list
void wVerifyMap::on_btnReloadMaps_clicked()
{
    loadMapList();
    ui->statusbar->showMessage(tr("Reloaded map list."), 5000);
}

void wVerifyMap::on_actionBackToHome_triggered()
{
    close();
    emit backToHome();
}

void wVerifyMap::on_btnToTiles_clicked() { ui->twgVerfying->setCurrentIndex(1); }

void wVerifyMap::on_btnToTextures_clicked() { ui->twgVerfying->setCurrentIndex(2); }

void wVerifyMap::on_btnToObjects_clicked() { ui->twgVerfying->setCurrentIndex(3); }

void wVerifyMap::on_btnToSplines_clicked() { ui->twgVerfying->setCurrentIndex(4); }

void wVerifyMap::on_btnToVehicles_clicked() { ui->twgVerfying->setCurrentIndex(5); }

void wVerifyMap::on_btnToHumans_clicked() { ui->twgVerfying->setCurrentIndex(6); }
