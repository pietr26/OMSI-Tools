﻿#include "wverifymap.h"
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

    setWindowTitle(tr("Map verification") + " - " + OTInformation::name);

    DiscordGameSDK::setModule(tr("Map verification"));
    DiscordGameSDK::setStatus(tr("Checking a map"));
    DiscordGameSDK::setStart();
    DiscordGameSDK::setIcon("wverifymap");
    DiscordGameSDK::update();

    // init scanner
    _checker = new OTMapChecker(this);
    _scanner = new OTMapScanner(this, _checker);

    connect(_scanner, &QThread::finished, this, &wVerifyMap::onScannerFinished);
    connect(_checker, &QThread::finished, this, &wVerifyMap::onCheckerFinished);

    connect(_scanner, &OTMapScanner::initActionCount, ui->pgbProgress, &QProgressBar::setMaximum);
    connect(_scanner, &OTMapScanner::statusUpdate, this, &wVerifyMap::onStatusUpdate);

    ui->statusbar->addPermanentWidget(ui->pgbProgress);

    loadMapList();
    on_cobxMapName_currentIndexChanged(ui->cobxMapName->currentIndex());

    // Connect S&S
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
    }

    enableView(false);
    ui->twgVerfying->setCurrentIndex(0);

    if (!QFile::exists(filehandler.getMapPath()))
    {
        QMessageBox::warning(this, tr("Map file doesn't exist"), tr("The selected map file doesn't exist."));
        qWarning() << QString("Could not found map file '%1'!").arg(filehandler.getMapPath());
        return;
    }
    qInfo().noquote() << QString("Map: %1").arg(filehandler.getMapPath());

    selectAllAndClear();
    ui->pgbProgress->setVisible(true);

    _checker->setOmsiDir(set.read("main", "mainDir").toString());

    _scanner->start(QThread::HighPriority);
    _checker->start(QThread::HighPriority);
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

void wVerifyMap::onScannerFinished() {
    qDebug() << "scanner finished";

    ui->pgbProgress->setMaximum(0);
    ui->statusbar->showMessage(tr("Checking sceneryobjects and splines..."));

    ui->wdgTiles->addAll(_scanner->allTiles());
    ui->wdgTiles->addMissing(_scanner->missingTiles());
    ui->wdgTiles->apply();
    ui->wdgTilesOverview->setTotal(_scanner->allTilesCount());
    ui->wdgTilesOverview->setInvalidAndMissing(_scanner->invalidTilesCount(), _scanner->missingTilesCount());
}

void wVerifyMap::onCheckerFinished() {
    qDebug() << "checker finished";

    // SCO:
    ui->wdgSceneryobjects->addAll(_checker->allSceneryobjects());
    ui->wdgSceneryobjects->addMissing(_checker->missingSceneryobjects());
    ui->wdgSceneryobjects->apply();
    ui->wdgSceneryobjectsOverview->setTotal(_checker->allSceneryobjectsCount());
    ui->wdgSceneryobjectsOverview->setInvalidAndMissing(_checker->invalidSceneryobjectsCount(), _checker->missingSceneryobjectsCount());

    // SLI:
    ui->wdgSplines->addAll(_checker->allSplines());
    ui->wdgSplines->addMissing(_checker->missingSplines());
    ui->wdgSplines->apply();
    ui->wdgSplinesOverview->setTotal(_checker->allSplinesCount());
    ui->wdgSplinesOverview->setInvalidAndMissing(_checker->invalidSplinesCount(), _checker->missingSplinesCount());

    // VEH:
    ui->wdgVehicles->addAll(_checker->allVehicles());
    ui->wdgVehicles->addMissing(_checker->missingVehicles());
    ui->wdgVehicles->apply();
    ui->wdgVehiclesOverview->setTotal(_checker->allVehiclesCount());
    ui->wdgVehiclesOverview->setInvalidAndMissing(_checker->invalidVehiclesCount(), _checker->missingVehiclesCount());

    // HUM:
    ui->wdgHumans->addAll(_checker->allHumans());
    ui->wdgHumans->addMissing(_checker->missingHumans());
    ui->wdgHumans->apply();
    ui->wdgHumansOverview->setTotal(_checker->allHumansCount());
    ui->wdgHumansOverview->setInvalidAndMissing(_checker->invalidHumansCount(), _checker->missingHumansCount());

    // GLOBAL TEX:
    ui->wdgTextures->addAll(_scanner->allTextures());
    ui->wdgTextures->addMissing(_scanner->missingTextures());
    ui->wdgTextures->apply();
    ui->wdgTexturesOverview->setTotal(_scanner->allTexturesCount());
    ui->wdgTexturesOverview->setInvalidAndMissing(_scanner->invalidTexturesCount(), _scanner->missingTexturesCount());

    qInfo() << "Verification finished.";
    ui->statusbar->showMessage(tr("Verification finished."), 5000);

    ui->pgbProgress->setVisible(false);
    enableView(true);
}

void wVerifyMap::onStatusUpdate(int i, QString message) {
    ui->pgbProgress->setValue(i);
    ui->statusbar->showMessage(message);
}

void wVerifyMap::loadMapList()
{
    mapListSetupFinished = false;
    ui->cobxMapName->clear();

    qDebug() << "Reload map list...";
    mapList = filehandler.listMaps();

    qDebug().noquote() << "Map count:" << mapList.size();
    QString lastMap = set.read(objectName(), "mapPath").toString();

    for (int i = 0; i < mapList.size(); i++)
    {
        ui->cobxMapName->addItem(mapList[i].first);
        if (mapList[i].second == lastMap)
        {
            ui->cobxMapName->setCurrentIndex(i);
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

    _scanner->setMapDir(mapList[index].second.remove("global.cfg"));

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
