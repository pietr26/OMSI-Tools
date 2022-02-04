#include "wverifymap.h"
#include "ui_wverifymap.h"

wVerifyMap::wVerifyMap(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wVerifyMap)
{
    qInfo().noquote() << "Starting " + moduleName + "...";
    qDebug() << "Set up UI...";
    ui->setupUi(this);
    //adjustSize();
    qDebug() << "UI set";

    setWindowTitle(OTName + " - " + tr("map verify"));

    // Load settings
    setStyleSheet(set.read("main", "theme").toString());

    if (set.read(moduleName, "advVerifying").toInt() == 2)
        ui->cbxAdvancedVerifying->setChecked(true);
    qDebug() << "advVerifying checkbox loaded";

    if (set.read(moduleName, "mapPath") != "")
        filehandler.setMapPath(set.read(moduleName, "mapPath").toString());
    qDebug() << "Map path loaded";

    if (set.read(moduleName, "onlyMapTextures") != "")
        ui->cbxOnlyMapTextures->setChecked(set.read(moduleName, "onlyMapTextures").toBool());

    if (ui->cbxAdvancedVerifying->isChecked())
        ui->cbxOnlyMapTextures->setEnabled(true);
    else
        ui->cbxOnlyMapTextures->setEnabled(false);

    qDebug() << "Only global textures checkbox loaded";



    // Load Description and picture from the map
    if ((filehandler.getMapPath() != "") && (QFile(filehandler.getMapPath()).exists()))
    {
        QFile global(filehandler.getMapPath());
        if (!global.open(QFile::ReadOnly | QFile::Text))
            goto mapNotFound;

        ui->ledMapPath->setText(filehandler.getMapPath());
        ui->lblPicture->setPixmap(QPixmap(filehandler.getMapPath().remove(QRegularExpression("global.cfg")) + "/picture.jpg"));
        ui->ledMapName->setText(filehandler.readGlobal("name", this));
    }
    mapNotFound:

    // Hide detail buttons
    ui->btnTilesDetails->setVisible(false);
    ui->btnTexturesDetails->setVisible(false);
    ui->btnObjectsDetails->setVisible(false);
    ui->btnSplinesDetails->setVisible(false);
    ui->btnVehiclesDetails->setVisible(false);
    ui->btnHumansDetails->setVisible(false);

    ui->gbxSettings->setVisible(false);

    // Connect S&S
    connect(watchProgress, SIGNAL(timeout()), this, SLOT(reloadProgress()));
    ui->pgbProgress->setVisible(false);

    ui->statusbar->showMessage(QString(tr("Press %1 to start the verification.")).arg("\"" + ui->btnStartVerifying->text() + "\""));

    toolButtonSetup();
    ui->twgVerfying->setCurrentIndex(0);

    qInfo().noquote() << moduleName + " started successfully.";

    ui->lwgTilesAll->installEventFilter(this);
}

wVerifyMap::~wVerifyMap()
{
    qInfo().noquote() << moduleName + " is closing...";
    delete ui;
}

/// \brief Shows tool button menu
void wVerifyMap::on_tbnTilesTools_clicked()
{
    ui->tbnTilesTools->showMenu();
}

/// \brief Shows tool button menu
void wVerifyMap::on_tbnTexturesTools_clicked()
{
    ui->tbnTexturesTools->showMenu();
}

/// \brief Shows tool button menu
void wVerifyMap::on_tbnObjectsTools_clicked()
{
    ui->tbnObjectsTools->showMenu();
}

/// \brief Shows tool button menu
void wVerifyMap::on_tbnSplinesTools_clicked()
{
    ui->tbnSplinesTools->showMenu();
}

/// \brief Shows tool button menu
void wVerifyMap::on_tbnVehiclesTools_clicked()
{
    ui->tbnVehiclesTools->showMenu();
}

/// \brief Shows tool button menu
void wVerifyMap::on_tbnHumansTools_clicked()
{
    ui->tbnHumansTools->showMenu();
}

/// \brief Copies a entry from listWidget.
void wVerifyMap::on_lwgTilesAll_itemDoubleClicked(QListWidgetItem *item)
{
    misc.copy(item->text());
}

/// \brief Copies a entry from listWidget.
void wVerifyMap::on_lwgTilesMissing_itemDoubleClicked(QListWidgetItem *item)
{
    misc.copy(item->text());
}

/// \brief Copies a entry from listWidget.
void wVerifyMap::on_lwgTexturesAll_itemDoubleClicked(QListWidgetItem *item)
{
    misc.copy(item->text());
}

/// \brief Copies a entry from listWidget.
void wVerifyMap::on_lwgTexturesMissing_itemDoubleClicked(QListWidgetItem *item)
{
    misc.copy(item->text());
}

/// \brief Copies a entry from listWidget.
void wVerifyMap::on_lwgObjectsAll_itemDoubleClicked(QListWidgetItem *item)
{
    misc.copy(item->text());
}

/// \brief Copies a entry from listWidget.
void wVerifyMap::on_lwgObjectsMissing_itemDoubleClicked(QListWidgetItem *item)
{
    misc.copy(item->text());
}

/// \brief Copies a entry from listWidget.
void wVerifyMap::on_lwgSplinesAll_itemDoubleClicked(QListWidgetItem *item)
{
    misc.copy(item->text());
}

/// \brief Copies a entry from listWidget.
void wVerifyMap::on_lwgSplinesMissing_itemDoubleClicked(QListWidgetItem *item)
{
    misc.copy(item->text());
}

/// \brief Copies a entry from listWidget.
void wVerifyMap::on_lwgVehiclesAll_itemDoubleClicked(QListWidgetItem *item)
{
    misc.copy(item->text());
}

/// \brief Copies a entry from listWidget.
void wVerifyMap::on_lwgVehiclesMissing_itemDoubleClicked(QListWidgetItem *item)
{
    misc.copy(item->text());
}

/// \brief Copies a entry from listWidget.
void wVerifyMap::on_lwgHumansAll_itemDoubleClicked(QListWidgetItem *item)
{
    misc.copy(item->text());
}

/// \brief Copies a entry from listWidget.
void wVerifyMap::on_lwgHumansMissing_itemDoubleClicked(QListWidgetItem *item)
{
    misc.copy(item->text());
}

/// \brief Shows details for tiles
void wVerifyMap::on_btnTilesDetails_clicked()
{
    ui->twgVerfying->setCurrentIndex(1);
}

/// \brief Shows details for tiles
void wVerifyMap::on_btnTexturesDetails_clicked()
{
    ui->twgVerfying->setCurrentIndex(2);
}

/// \brief Shows details for objects
void wVerifyMap::on_btnObjectsDetails_clicked()
{
    ui->twgVerfying->setCurrentIndex(3);
}

/// \brief Shows details for splines
void wVerifyMap::on_btnSplinesDetails_clicked()
{
    ui->twgVerfying->setCurrentIndex(4);
}

/// \brief Shows details for vehicles
void wVerifyMap::on_btnVehiclesDetails_clicked()
{
    ui->twgVerfying->setCurrentIndex(5);
}

/// \brief Shows details for humans
void wVerifyMap::on_btnHumansDetails_clicked()
{
    ui->twgVerfying->setCurrentIndex(6);
}

/// \brief Adjusts the size of the window
void wVerifyMap::on_actionAdjustWindowSize_triggered()
{
    adjustSize();
}

/// \brief Opens bug report module
void wVerifyMap::on_actionSendFeedback_triggered()
{
    OTMiscellaneous::sendFeedback();
}

/// \brief Tool button setup
void wVerifyMap::toolButtonSetup()
{
    qDebug() << "Set tool buttons...";

    // Tiles
    QMenu *tiles = new QMenu(this);
    tiles->addAction(ui->actionTilesSearch);
    tiles->addAction(ui->actionTilesCopy);
    tiles->addAction(ui->actionTilesIgnore);
    ui->tbnTilesTools->setMenu(tiles);

    // Textures
    QMenu *textures = new QMenu(this);
    textures->addAction(ui->actionTexturesSearch);
    textures->addAction(ui->actionTexturesCopy);
    textures->addAction(ui->actionTexturesIgnore);
    ui->tbnTexturesTools->setMenu(textures);

    // Sceneryobjects
    QMenu *objects = new QMenu(this);
    objects->addAction(ui->actionObjectsSearch);
    objects->addAction(ui->actionObjectsCopy);
    objects->addAction(ui->actionObjectsIgnore);
    ui->tbnObjectsTools->setMenu(objects);

    // Splines
    QMenu *splines = new QMenu(this);
    splines->addAction(ui->actionSplinesSearch);
    splines->addAction(ui->actionSplinesCopy);
    splines->addAction(ui->actionSplinesIgnore);
    ui->tbnSplinesTools->setMenu(splines);

    // Vehicles
    QMenu *vehicles = new QMenu(this);
    vehicles->addAction(ui->actionVehiclesSearch);
    vehicles->addAction(ui->actionVehiclesCopy);
    vehicles->addAction(ui->actionVehiclesIgnore);
    ui->tbnVehiclesTools->setMenu(vehicles);

    // Humans
    QMenu *humans = new QMenu(this);
    humans->addAction(ui->actionHumansSearch);
    humans->addAction(ui->actionHumansCopy);
    humans->addAction(ui->actionHumansIgnore);
    ui->tbnHumansTools->setMenu(humans);
}

/// \brief Starts verifying
void wVerifyMap::on_actionStartVerifying_triggered()
{
    on_btnStartVerifying_clicked();
}

/// \brief Selects all items from lwg's and deletes them (and more)
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
}


/// \brief Reloads progress state
void wVerifyMap::reloadProgress()
{
    ui->pgbProgress->setMinimum(0);
    ui->pgbProgress->setMaximum(filehandler.maxProgress);
    ui->pgbProgress->setValue(filehandler.currentProgress);
    ui->statusbar->showMessage(filehandler.progressName);
}

/// \brief Starts / ends the progress
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

    if (state)
        ui->pgbProgress->setVisible(true);
}

/// \brief Starts verifying
void wVerifyMap::on_btnStartVerifying_clicked()
{
    enableIgnoreLabels(true);

    qInfo() << "Start verifying...";

    if (set.read("main", "mainDir").toString() == "")
    {
        if (msg.setMainDirYesNo(this))
        {
            WSETTINGS = new wSettings(this, "mainDirSelection");
            WSETTINGS->show();
        }
        else
            return;
        cutCount = set.read("main", "mainDir").toString().count() + 1;
    }

    ui->btnStartVerifying->setEnabled(false);
    ui->btnStartVerifying->setText(tr("Running..."));
    startEndWatchProgress(true);

    ui->twgVerfying->setCurrentIndex(0);

    if (!QFile::exists(filehandler.getMapPath()))
    {
        QMessageBox::warning(this, tr("Map file doesn't exist", "Note #1"), tr("The selected map file doesn't exist."));
        qWarning() << QString("Could not found map file '%1'!").arg(filehandler.getMapPath());
        return endVerifying();
    }
    selectAllAndClear();

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


    // SCO and SLI (advanced)
    if (ui->cbxAdvancedVerifying->isChecked())
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
    if ((set.read(moduleName, "onlyMapTextures") == "false") || (!set.read(moduleName, "onlyMapTextures").isValid()))
    {
        qDebug() << "Read ALL Textures...";
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
        if ((set.read(moduleName, "onlyMapTextures") == "false") || (!set.read(moduleName, "onlyMapTextures").isValid()))
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

/// \brief Sets detail buttons
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

/// \brief Ends verifying
void wVerifyMap::endVerifying()
{
    startEndWatchProgress(false);

    ui->btnStartVerifying->setEnabled(true);
    ui->btnStartVerifying->setText(tr("Start verifying"));
    return;
}

/// \brief Closes the application
void wVerifyMap::on_actionClose_triggered()
{
    QApplication::quit();
}

/// \brief Shows an getOpenFileName-Dialog and put this path into the map model
void wVerifyMap::on_tbnMapPath_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Select map file..."), set.read("main", "mainDir").toString() + "/maps", tr("OMSI map file") + " (global.cfg)");
    if (path != "")
    {
        filehandler.setMapPath(path);
        qDebug().noquote() << "New map path:" << path;

        // Set view
        ui->ledMapPath->setText(path);
        ui->ledMapName->setText(filehandler.readGlobal("name", this));
        ui->lblPicture->setPixmap(QPixmap(filehandler.getMapPath().remove(QRegularExpression("global.cfg")) + "/picture.jpg"));

        // Check if global.cfg exists:
        QFile global(filehandler.getMapPath());

        if (!global.open(QFile::ReadOnly | QFile::Text))
        {
            msg.errorWhileOpeningOmsi(this, QFileInfo(global).absoluteFilePath());
            return;
        }
    }
    ui->pgbProgress->setVisible(false);
}

/// \brief Clears the view if map path has changed
void wVerifyMap::on_ledMapPath_textChanged(const QString &arg1)
{
    if (arg1 == "")
        ui->btnStartVerifying->setEnabled(false);
    else
        ui->btnStartVerifying->setEnabled(true);

    set.write(moduleName, "mapPath", arg1);
    selectAllAndClear();
}

/// \brief Opens the settings
void wVerifyMap::on_actionSettings_triggered()
{
    WSETTINGS = new wSettings(this);
    WSETTINGS->setWindowModality(Qt::ApplicationModal);
    WSETTINGS->show();
}

/// \brief Opens the ignorelist (UI)
void wVerifyMap::on_actionEditIgnorelist_triggered()
{
    DIGNORELIST = new dIgnoreList();
    DIGNORELIST->show();
}

/// \brief Sets the view to the selected verifying depth
void wVerifyMap::on_cbxAdvancedVerifying_stateChanged(int arg1)
{
    set.write(moduleName, "advVerifying", arg1);

    if (ui->cbxAdvancedVerifying->isChecked())
        ui->cbxOnlyMapTextures->setEnabled(true);
    else
        ui->cbxOnlyMapTextures->setEnabled(false);

    // maybe reset label names
//    bool type;
//    if (arg1 > 0)
//    {
//        type = true;
//    }
//    else
//    {
//        type = false;
//    }

}

/// \brief Copies missing tiles
void wVerifyMap::on_actionTilesCopy_triggered()
{
    if (ui->lwgTilesMissing->currentRow() == -1)
    {
        ui->statusbar->showMessage(tr("No path selected."), 3000);
        return;
    }

    QString copytext = "";
    foreach (QListWidgetItem* current, ui->lwgTilesMissing->selectedItems())
        copytext += current->text() + "\n";

    misc.copy(copytext);
    ui->statusbar->showMessage(tr("Copied!"), 3000);
}

/// \brief Copies missing textures
void wVerifyMap::on_actionTexturesCopy_triggered()
{
    if (ui->lwgTexturesMissing->currentRow() == -1)
    {
        ui->statusbar->showMessage(tr("No path selected."), 3000);
        return;
    }

    QString copytext = "";
    foreach (QListWidgetItem* current, ui->lwgTexturesMissing->selectedItems())
        copytext += current->text() + "\n";

    misc.copy(copytext);
    ui->statusbar->showMessage(tr("Copied!"), 3000);
}

/// \brief Copies missing objects
void wVerifyMap::on_actionObjectsCopy_triggered()
{
    if (ui->lwgObjectsMissing->currentRow() == -1)
    {
        ui->statusbar->showMessage(tr("No path selected."), 3000);
        return;
    }

    QString copytext = "";
    foreach (QListWidgetItem* current, ui->lwgObjectsMissing->selectedItems())
        copytext += current->text() + "\n";

    misc.copy(copytext);
    ui->statusbar->showMessage(tr("Copied!"), 3000);
}

/// \brief Copies missing splines
void wVerifyMap::on_actionSplinesCopy_triggered()
{
    if (ui->lwgSplinesMissing->currentRow() == -1)
    {
        ui->statusbar->showMessage(tr("No path selected."), 3000);
        return;
    }

    QString copytext = "";
    foreach (QListWidgetItem* current, ui->lwgSplinesMissing->selectedItems())
        copytext += current->text() + "\n";

    misc.copy(copytext);
    ui->statusbar->showMessage(tr("Copied!"), 3000);
}

/// \brief Copies missing vehicles
void wVerifyMap::on_actionVehiclesCopy_triggered()
{
    if (ui->lwgVehiclesMissing->currentRow() == -1)
    {
        ui->statusbar->showMessage(tr("No path selected."), 3000);
        return;
    }

    QString copytext = "";
    foreach (QListWidgetItem* current, ui->lwgVehiclesMissing->selectedItems())
        copytext += current->text() + "\n";

    misc.copy(copytext);
    ui->statusbar->showMessage(tr("Copied!"), 3000);
}

/// \brief Copies missing humans
void wVerifyMap::on_actionHumansCopy_triggered()
{
    if (ui->lwgHumansMissing->currentRow() == -1)
        {
            ui->statusbar->showMessage(tr("No path selected."), 3000);
            return;
        }

    QString copytext = "";
    foreach (QListWidgetItem* current, ui->lwgHumansMissing->selectedItems())
        copytext += current->text() + "\n";

    misc.copy(copytext);
    ui->statusbar->showMessage(tr("Copied!"), 3000);
}

/// \brief Search for paths in wContentSearch
void wVerifyMap::on_actionTilesSearch_triggered()
{
    QStringList paths;
    foreach(QListWidgetItem* current, ui->lwgTilesMissing->selectedItems())
        paths << set.read("main", "mainDir").toString() + "/" + current->text();

    if (!paths.empty())
    {
        WCONTENTSEARCH = new wContentSearch(this, paths);
        WCONTENTSEARCH->setWindowModality(Qt::ApplicationModal);
        WCONTENTSEARCH->show();
    }
    else
        ui->statusbar->showMessage(tr("No path selected."), 3000);
}

/// \brief Search for paths in wContentSearch
void wVerifyMap::on_actionTexturesSearch_triggered()
{
    QStringList paths;
    foreach(QListWidgetItem* current, ui->lwgTexturesMissing->selectedItems())
        paths << set.read("main", "mainDir").toString() + "/" + current->text();

    if (!paths.empty())
    {
        WCONTENTSEARCH = new wContentSearch(this, paths);
        WCONTENTSEARCH->setWindowModality(Qt::ApplicationModal);
        WCONTENTSEARCH->show();
    }
    else
        ui->statusbar->showMessage(tr("No path selected."), 3000);
}

/// \brief Search for paths in wContentSearch
void wVerifyMap::on_actionObjectsSearch_triggered()
{
    QStringList paths;
    foreach(QListWidgetItem* current, ui->lwgObjectsMissing->selectedItems())
        paths << set.read("main", "mainDir").toString() + "/" + current->text();

    if (!paths.empty())
    {
        WCONTENTSEARCH = new wContentSearch(this, paths);
        WCONTENTSEARCH->setWindowModality(Qt::ApplicationModal);
        WCONTENTSEARCH->show();
    }
    else
        ui->statusbar->showMessage(tr("No path selected."), 3000);
}

/// \brief Search for paths in wContentSearch
void wVerifyMap::on_actionSplinesSearch_triggered()
{
    QStringList paths;
    foreach(QListWidgetItem* current, ui->lwgSplinesMissing->selectedItems())
        paths << set.read("main", "mainDir").toString() + "/" + current->text();

    if (!paths.empty())
    {
        WCONTENTSEARCH = new wContentSearch(this, paths);
        WCONTENTSEARCH->setWindowModality(Qt::ApplicationModal);
        WCONTENTSEARCH->show();
    }
    else
        ui->statusbar->showMessage(tr("No path selected."), 3000);
}

/// \brief Search for paths in wContentSearch
void wVerifyMap::on_actionVehiclesSearch_triggered()
{
    QStringList paths;
    foreach(QListWidgetItem* current, ui->lwgVehiclesMissing->selectedItems())
        paths << set.read("main", "mainDir").toString() + "/" + current->text();

    if (!paths.empty())
    {
        WCONTENTSEARCH = new wContentSearch(this, paths);
        WCONTENTSEARCH->setWindowModality(Qt::ApplicationModal);
        WCONTENTSEARCH->show();
    }
    else
        ui->statusbar->showMessage(tr("No path selected."), 3000);
}

/// \brief Search for paths in wContentSearch
void wVerifyMap::on_actionHumansSearch_triggered()
{
    QStringList paths;
    foreach(QListWidgetItem* current, ui->lwgHumansMissing->selectedItems())
        paths << set.read("main", "mainDir").toString() + "/" + current->text();

    if (!paths.empty())
    {
        WCONTENTSEARCH = new wContentSearch(this, paths);
        WCONTENTSEARCH->setWindowModality(Qt::ApplicationModal);
        WCONTENTSEARCH->show();
    }
    else
        ui->statusbar->showMessage(tr("No path selected."), 3000);
}

/// \brief Ignores the selected path(s)
void wVerifyMap::on_actionTilesIgnore_triggered()
{
    if (ui->lwgTilesMissing->currentRow() == -1)
    {
        ui->statusbar->showMessage(tr("No path selected."), 3000);
        return;
    }

    for (int i = 0; i < ui->lwgTilesMissing->count(); i++)
    {
        if (ui->lwgTilesMissing->item(i)->isSelected())
        {
            iglF.write(ui->lwgTilesMissing->item(i)->text());
            ui->lblIgnoredTiles->setText(QString::number(ui->lblIgnoredTiles->text().toInt() + 1));
        }
    }
    qDeleteAll(ui->lwgTilesMissing->selectedItems());
}

/// \brief Ignores the selected path(s)
void wVerifyMap::on_actionTexturesIgnore_triggered()
{
    if (ui->lwgTexturesMissing->currentRow() == -1)
    {
        ui->statusbar->showMessage(tr("No path selected."), 3000);
        return;
    }

    for (int i = 0; i < ui->lwgTexturesMissing->count(); i++)
    {
        if (ui->lwgTexturesMissing->item(i)->isSelected())
        {
            iglF.write(ui->lwgTexturesMissing->item(i)->text());
            ui->lblIgnoredTextures->setText(QString::number(ui->lblIgnoredTextures->text().toInt() + 1));
        }
    }
    qDeleteAll(ui->lwgTexturesMissing->selectedItems());
}

/// \brief Ignores the selected path(s)
void wVerifyMap::on_actionObjectsIgnore_triggered()
{
    if (ui->lwgObjectsMissing->currentRow() == -1)
    {
        ui->statusbar->showMessage(tr("No path selected."), 3000);
        return;
    }

    for (int i = 0; i < ui->lwgObjectsMissing->count(); i++)
    {
        if (ui->lwgObjectsMissing->item(i)->isSelected())
        {
            iglF.write(ui->lwgObjectsMissing->item(i)->text());
            ui->lblIgnoredObjects->setText(QString::number(ui->lblIgnoredObjects->text().toInt() + 1));
        }
    }
    qDeleteAll(ui->lwgObjectsMissing->selectedItems());
}

/// \brief Ignores the selected path(s)
void wVerifyMap::on_actionSplinesIgnore_triggered()
{
    if (ui->lwgSplinesMissing->currentRow() == -1)
    {
        ui->statusbar->showMessage(tr("No path selected."), 3000);
        return;
    }

    for (int i = 0; i < ui->lwgSplinesMissing->count(); i++)
    {
        if (ui->lwgSplinesMissing->item(i)->isSelected())
        {
            iglF.write(ui->lwgSplinesMissing->item(i)->text());
            ui->lblIgnoredSplines->setText(QString::number(ui->lblIgnoredSplines->text().toInt() + 1));
        }
    }
    qDeleteAll(ui->lwgSplinesMissing->selectedItems());
}

/// \brief Ignores the selected path(s)
void wVerifyMap::on_actionVehiclesIgnore_triggered()
{
    if (ui->lwgVehiclesMissing->currentRow() == -1)
    {
        ui->statusbar->showMessage(tr("No path selected."), 3000);
        return;
    }

    for (int i = 0; i < ui->lwgVehiclesMissing->count(); i++)
    {
        if (ui->lwgVehiclesMissing->item(i)->isSelected())
        {
            iglF.write(ui->lwgVehiclesMissing->item(i)->text());
            ui->lblIgnoredVehicles->setText(QString::number(ui->lblIgnoredVehicles->text().toInt() + 1));
        }
    }
    qDeleteAll(ui->lwgVehiclesMissing->selectedItems());
}

/// \brief Ignores the selected path(s)
void wVerifyMap::on_actionHumansIgnore_triggered()
{
    if (ui->lwgHumansMissing->currentRow() == -1)
    {
        ui->statusbar->showMessage(tr("No path selected."), 3000);
        return;
    }

    for (int i = 0; i < ui->lwgHumansMissing->count(); i++)
    {
        if (ui->lwgHumansMissing->item(i)->isSelected())
        {
            iglF.write(ui->lwgHumansMissing->item(i)->text());
            ui->lblIgnoredHumans->setText(QString::number(ui->lblIgnoredHumans->text().toInt() + 1));
        }
    }
    qDeleteAll(ui->lwgHumansMissing->selectedItems());
}

void wVerifyMap::on_btnShowSettings_clicked()
{
    if (ui->gbxSettings->isVisible())
    {
        ui->btnShowSettings->setText(tr("Show settings"));
        ui->btnShowSettings->setIcon(QIcon(":/rec/data/icons/iconTabClosed.svg"));
    }
    else
    {
        ui->btnShowSettings->setText(tr("Hide settings"));
        ui->btnShowSettings->setIcon(QIcon(":/rec/data/icons/iconTabOpened.svg"));
    }

    ui->gbxSettings->setVisible(!ui->gbxSettings->isVisible());
}

void wVerifyMap::on_cbxOnlyMapTextures_stateChanged(int arg1)
{
    if (arg1 == 2)
        set.write(moduleName, "onlyMapTextures", true);
    else
        set.write(moduleName, "onlyMapTextures", false);
}

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

void wVerifyMap::ignoreListItems()
{
    QObject* obj = sender();
    QListWidget *lwg = dynamic_cast<QListWidget*>(obj);

    enableIgnoreLabels(false);
    if (lwg->currentRow() == -1)
    {
        ui->statusbar->showMessage(tr("No path selected."), 3000);
        return;
    }

    for (int i = 0; i < lwg->count(); i++)
    {
        if (lwg->item(i)->isSelected())
            iglF.write(lwg->item(i)->text());
    }

    qDeleteAll(lwg->selectedItems());
}

void wVerifyMap::searchListItems()
{
    QObject* obj = sender();
    QListWidget *lwg = dynamic_cast<QListWidget*>(obj);

    QStringList paths;
    foreach(QListWidgetItem* current, lwg->selectedItems())
        paths << set.read("main", "mainDir").toString() + "/" + current->text();

    if (!paths.empty())
    {
        WCONTENTSEARCH = new wContentSearch(this, paths);
        WCONTENTSEARCH->setWindowModality(Qt::ApplicationModal);
        WCONTENTSEARCH->show();
    }
    else
        ui->statusbar->showMessage(tr("No path selected."), 3000);
}

void wVerifyMap::copyListItems()
{
    QObject* obj = sender();
    QListWidget *lwg = dynamic_cast<QListWidget*>(obj);

    if (lwg->currentRow() == -1)
        {
            ui->statusbar->showMessage(tr("No path selected."), 3000);
            return;
        }

    QString copytext = "";
    foreach (QListWidgetItem* current, lwg->selectedItems())
        copytext += current->text() + "\n";

    misc.copy(copytext);
    ui->statusbar->showMessage(tr("Copied!"), 3000);
}
