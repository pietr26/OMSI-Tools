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
    DiscordGameSDK::setStart();
    DiscordGameSDK::setIcon("wverifymap");
    DiscordGameSDK::update();

    ui->statusbar->addPermanentWidget(ui->pgbProgress);

    loadMapList();
    on_cobxMapName_currentIndexChanged(ui->cobxMapName->currentIndex());

    // Connect S&S
    connect(watchProgress, SIGNAL(timeout()), this, SLOT(reloadProgress()));
    ui->pgbProgress->setVisible(false);
    ui->statusbar->showMessage(QString(tr("Press %1 to start the verification.")).arg("\"" + ui->btnStartVerifying->text() + "\""));
    ui->twgVerfying->setCurrentIndex(0);

    // Set overview tiles
    ui->wdgTilesOverview->setElementType(ElementType::Tile);
    ui->wdgSceneryobjectsOverview->setElementType(ElementType::Sceneryobject);
    ui->wdgSplinesOverview->setElementType(ElementType::Spline);
    ui->wdgVehiclesOverview->setElementType(ElementType::Vehicle);
    ui->wdgHumansOverview->setElementType(ElementType::Human);
    ui->wdgTexturesOverview->setElementType(ElementType::Texture);
    connect(ui->wdgTilesOverview, &wdgOverviewTile::goTo, this, [this](){ ui->twgVerfying->setCurrentIndex(1); });
    connect(ui->wdgSceneryobjectsOverview, &wdgOverviewTile::goTo, this, [this](){ ui->twgVerfying->setCurrentIndex(2); });
    connect(ui->wdgSplinesOverview, &wdgOverviewTile::goTo, this, [this](){ ui->twgVerfying->setCurrentIndex(3); });
    connect(ui->wdgVehiclesOverview, &wdgOverviewTile::goTo, this, [this](){ ui->twgVerfying->setCurrentIndex(4); });
    connect(ui->wdgHumansOverview, &wdgOverviewTile::goTo, this, [this](){ ui->twgVerfying->setCurrentIndex(5); });
    connect(ui->wdgTexturesOverview, &wdgOverviewTile::goTo, this, [this](){ ui->twgVerfying->setCurrentIndex(6); });

    // Set tabs
    ui->wdgTiles->setName(tr("Tiles"));
    ui->wdgSceneryobjects->setName(tr("Sceneryobjects"));
    ui->wdgSplines->setName(tr("Splines"));
    ui->wdgVehicles->setName(tr("Vehicles"));
    ui->wdgHumans->setName(tr("Humans"));
    ui->wdgTextures->setName(tr("Textures"));

    qInfo().noquote() << objectName() + " started";
}

wVerifyMap::~wVerifyMap()
{
    delete ui;
}

void wVerifyMap::on_actionSendFeedback_triggered()
{
    wFeedback *WFEEDBACK = new wFeedback(this, OTLinks::Wiki::verifyMap);
    WFEEDBACK->show();
}

void wVerifyMap::on_actionStartVerifying_triggered()
{
    on_btnStartVerifying_clicked();
}

void wVerifyMap::selectAllAndClear()
{
    qDebug() << "Clear view...";

    // Clear ListWidgets:
    ui->wdgTiles->clear();
    ui->wdgSceneryobjects->clear();
    ui->wdgSplines->clear();
    ui->wdgVehicles->clear();
    ui->wdgHumans->clear();
    ui->wdgTextures->clear();

    ui->pgbProgress->setVisible(false);

    ui->wdgTilesOverview->clear();
    ui->wdgSceneryobjectsOverview->clear();
    ui->wdgSplinesOverview->clear();
    ui->wdgVehiclesOverview->clear();
    ui->wdgHumansOverview->clear();
    ui->wdgTexturesOverview->clear();
}


void wVerifyMap::reloadProgress()
{
    ui->pgbProgress->setMinimum(0);
    ui->pgbProgress->setMaximum(filehandler.maxProgress);
    ui->pgbProgress->setValue(filehandler.currentProgress);
    ui->statusbar->showMessage(filehandler.progressName);
}

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
    filehandler.stuffobj.clear();
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
        ui->wdgTiles->add(filehandler.stuffobj.existing.tiles, false);
        ui->wdgTiles->add(filehandler.stuffobj.missing.tiles, true);
        ui->wdgTiles->apply();
        ui->wdgTilesOverview->setTotal(ui->wdgTiles->getData().total);
        ui->wdgTilesOverview->setMissing(ui->wdgTiles->getData().missing);

        // SCO:
        ui->wdgSceneryobjects->add(filehandler.stuffobj.existing.sceneryobjects, false);
        ui->wdgSceneryobjects->add(filehandler.stuffobj.missing.sceneryobjects, true);
        ui->wdgSceneryobjects->apply();
        ui->wdgSceneryobjectsOverview->setTotal(ui->wdgSceneryobjects->getData().total);
        ui->wdgSceneryobjectsOverview->setMissing(ui->wdgSceneryobjects->getData().missing);

        // SLI:
        ui->wdgSplines->add(filehandler.stuffobj.existing.splines, false);
        ui->wdgSplines->add(filehandler.stuffobj.missing.splines, true);
        ui->wdgSplines->apply();
        ui->wdgSplinesOverview->setTotal(ui->wdgSplines->getData().total);
        ui->wdgSplinesOverview->setMissing(ui->wdgSplines->getData().missing);

        // VEH:
        ui->wdgVehicles->add(filehandler.stuffobj.existing.vehicles, false);
        ui->wdgVehicles->add(filehandler.stuffobj.missing.vehicles, true);
        ui->wdgVehicles->apply();
        ui->wdgVehiclesOverview->setTotal(ui->wdgVehicles->getData().total);
        ui->wdgVehiclesOverview->setMissing(ui->wdgVehicles->getData().missing);

        // HUM:
        ui->wdgHumans->add(filehandler.stuffobj.existing.humans, false);
        ui->wdgHumans->add(filehandler.stuffobj.missing.humans, true);
        ui->wdgHumans->apply();
        ui->wdgHumansOverview->setTotal(ui->wdgHumans->getData().total);
        ui->wdgHumansOverview->setMissing(ui->wdgHumans->getData().missing);

        // TEX:
        if (set.read(objectName(), "advVerifying").toBool() && !set.read(objectName(), "onlyMapTextures").toBool())
        {
            ui->wdgTextures->add(filehandler.stuffobj.existing.textures, false);
            ui->wdgTextures->add(filehandler.stuffobj.missing.textures, true);
        }

        // GLOBAL TEX:
        ui->wdgTextures->add(filehandler.stuffobj.existing.globalTextures, false);
        ui->wdgTextures->add(filehandler.stuffobj.missing.globalTextures, true);
        ui->wdgTextures->apply();
        ui->wdgTexturesOverview->setTotal(ui->wdgTextures->getData().total);
        ui->wdgTexturesOverview->setMissing(ui->wdgTextures->getData().missing);
    }

    qInfo() << "Verification finished.";
    ui->statusbar->showMessage(tr("Verification finished."), 5000);

    endVerifying();
}

void wVerifyMap::endVerifying()
{
    startEndWatchProgress(false);
    enableView(true);
}

void wVerifyMap::on_actionClose_triggered()
{
    QApplication::quit();
}

void wVerifyMap::on_actionPreferences_triggered()
{
    WPREFERENCES = new wPreferences();
    WPREFERENCES->show();
}

void wVerifyMap::on_btnVerificationPreferences_clicked()
{
    WPREFERENCES = new wPreferences(this, "wVerifyMap");
    WPREFERENCES->setWindowModality(Qt::ApplicationModal);
    WPREFERENCES->show();
}

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
