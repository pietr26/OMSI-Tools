#include "wglobalprops.h"
#include "ui_wglobalprops.h"

wGlobalProps::wGlobalProps(OCMap::Global globalProps, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wGlobalProps)
{
    qInfo().noquote() << "Starting " + objectName() + "...";
    qDebug() << "Set up UI...";
    ui->setupUi(this);
    //adjustSize();
    qDebug() << "UI set";

    setWindowTitle(tr("Edit global properties"));

    ui->twgGlobalProps->setTabVisible(5, false);

    map.global = globalProps;

    reloadUI();

    qInfo().noquote() << objectName() + " started";
}

// TODO: standardDepot prop in ailist preferences

wGlobalProps::~wGlobalProps()
{
    delete ui;
}

void wGlobalProps::on_btnEditProtectedProps_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Change protected properties"), tr("Changes in protected properties may result in unexpected behavior or bugs in the map editor. Continue?"), QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        ui->twgGlobalProps->setTabVisible(5, true);
        ui->btnEditProtectedProps->setVisible(false);
        ui->twgGlobalProps->setCurrentIndex(5);
    }
}

void wGlobalProps::reloadUI()
{
    setupFinished = false;

    QString mainDir = set.read("main", "mainDir").toString();

    /// GENERAL

    ui->ledFriendlyname->setText(map.global.friendlyname);

    ui->ledName->setText(map.global.name);

    on_btnSyncNames_clicked(map.global.name == map.global.friendlyname);
    ui->btnSyncNames->setChecked(map.global.name == map.global.friendlyname);
    on_ledFriendlyname_textEdited(ui->ledFriendlyname->text());

    ui->pteDescription->setPlainText(map.global.description);

    ui->cbxWorldCoordinates->setChecked(map.global.worldCoodinates);

    ui->cbxDynamicHelpArrows->setChecked(map.global.dynHelpers);

    ui->cbxLeftHandDriving->setChecked(map.global.lht);

    ui->cbxRealrail->setChecked(map.global.realrail);

    currency = filehandler.listMoney();
    ui->cobxCurrency->addItem(tr("(none)"));
    for (int i = 1; i < currency.count(); i++)
    {
        ui->cobxCurrency->addItem(currency[i].first);
        if (currency[i].second.remove(mainDir + "/").replace("/", "\\") == map.global.currency) ui->cobxCurrency->setCurrentIndex(i);
    }

    tickets = filehandler.listTicketpacks();
    ui->cobxTicketpack->addItem(tr("(none)"));
    for (int i = 1; i < tickets.count(); i++)
    {
        ui->cobxTicketpack->addItem(tickets[i].first);
        if (tickets[i].second.remove(mainDir + "/").replace("/", "\\") == map.global.ticketpack) ui->cobxTicketpack->setCurrentIndex(i);
    }

    ui->tieRepairTime->setTime(map.global.repairTime); // TODO: Test bigger as 60

    ui->cuwYears->setValue1(map.global.startYear);
    ui->cuwYears->setValue2(map.global.endYear);

    /// TEXTURES

    ui->cbxBackgroundImage->setChecked(map.global.bgImage.isVisible);
    ui->ledBIPicturePath->setText(map.global.bgImage.picturePath);
    ui->cuwBIPictureSize->setValue1(map.global.bgImage.width);
    ui->cuwBIPictureSize->setValue2(map.global.bgImage.height);
    ui->sbxBIStartWidth->setValue(map.global.bgImage.startWidth);
    ui->sbxBIStartHeight->setValue(map.global.bgImage.startHeight);

    ui->cuwDVTilePosition->setValue1(map.global.standardView.tilePosition.x);
    ui->cuwDVTilePosition->setValue2(map.global.standardView.tilePosition.y);
    ui->cuwDVPosition->setValue1(map.global.standardView.position.x);
    ui->cuwDVPosition->setValue2(map.global.standardView.position.y);
    ui->cuwDVPosition->setValue3(map.global.standardView.position.z);
    ui->cuwDVRotation->setValue1(map.global.standardView.rotAroundX);
    ui->cuwDVRotation->setValue2(map.global.standardView.rotAroundZ);
    ui->sbxDVDistanceFromPosition->setValue(map.global.standardView.distanceFromPosition);

    modelGroundTextures = new QStandardItemModel(map.global.groundTextures.count(), 1);
    modelGroundTextures->setHeaderData(0, Qt::Horizontal, tr("Texture"));
    for (int i = 0; i < map.global.groundTextures.count(); i++) { modelGroundTextures->setData(modelGroundTextures->index(i, 0), map.global.groundTextures[i].mainTex); }
    ui->tvwGroundTextures->setModel(modelGroundTextures);
    ui->tvwGroundTextures->resizeColumnsToContents();
    //if (map.global.groundTextures.count() == 0) ui->cuwGroundTextures->setRemoveEnabled() // TODO

    /// ENV & TRAFFIC

    modelSeasons = new QStandardItemModel(map.global.seasons.count(), 3);
    modelSeasons->setHeaderData(0, Qt::Horizontal, tr("Season"));
    modelSeasons->setHeaderData(1, Qt::Horizontal, tr("Start"));
    modelSeasons->setHeaderData(2, Qt::Horizontal, tr("End"));
    for (int i = 0; i < map.global.seasons.count(); i++)
    {
        modelSeasons->setData(modelSeasons->index(i, 0), map.global.seasons[i].name());
        modelSeasons->setData(modelSeasons->index(i, 1), map.global.seasons[i].start.toString("dd.MM."));
        modelSeasons->setData(modelSeasons->index(i, 2), map.global.seasons[i].end.toString("dd.MM."));
    }
    ui->tvwSeasons->setModel(modelSeasons);
    ui->tvwSeasons->resizeColumnsToContents();

    modelVehicleDensity = new QStandardItemModel(map.global.trafficDensities.count(), 2);
    modelVehicleDensity->setHeaderData(0, Qt::Horizontal, tr("Time"));
    modelVehicleDensity->setHeaderData(1, Qt::Horizontal, tr("Factor"));
    for (int i = 0; i < map.global.trafficDensities.count(); i++)
    {
        modelVehicleDensity->setData(modelVehicleDensity->index(i, 0), map.global.trafficDensities[i].time.toString("HH:mm"));
        modelVehicleDensity->setData(modelVehicleDensity->index(i, 1), map.global.trafficDensities[i].factor);
    }
    ui->tvwTrafficDensities->setModel(modelVehicleDensity);
    ui->tvwTrafficDensities->resizeColumnsToContents();

    modelPassengerDensity = new QStandardItemModel(map.global.passengerDensities.count(), 2);
    modelPassengerDensity->setHeaderData(0, Qt::Horizontal, tr("Time"));
    modelPassengerDensity->setHeaderData(1, Qt::Horizontal, tr("Factor"));
    for (int i = 0; i < map.global.passengerDensities.count(); i++)
    {
        modelPassengerDensity->setData(modelPassengerDensity->index(i, 0), map.global.passengerDensities[i].time.toString("HH:mm"));
        modelPassengerDensity->setData(modelPassengerDensity->index(i, 1), map.global.passengerDensities[i].factor);
    }
    ui->tvwPassengerDensities->setModel(modelPassengerDensity);
    ui->tvwPassengerDensities->resizeColumnsToContents();

    /// ENTRYPOINTS

    modelEntrypoints = new QStandardItemModel(map.global.entrypoints.count(), 2);
    modelEntrypoints->setHeaderData(0, Qt::Horizontal, tr("Name"));
    modelEntrypoints->setHeaderData(1, Qt::Horizontal, tr("Count"));
    for (int i = 0; i < map.global.entrypoints.count(); i++)
    {
        modelEntrypoints->setData(modelEntrypoints->index(i, 0), map.global.entrypoints[i].name);
        modelEntrypoints->setData(modelEntrypoints->index(i, 1), map.global.entrypoints[i].entrypoints.count());
    }
    ui->tvwEntrypoints->setModel(modelEntrypoints);
    ui->tvwEntrypoints->resizeColumnsToContents();

    /// TILES

    modelTiles = new QStandardItemModel(map.global.tiles.count(), 3);
    modelTiles->setHeaderData(0, Qt::Horizontal, tr("x coordinate"));
    modelTiles->setHeaderData(1, Qt::Horizontal, tr("y coordinate"));
    modelTiles->setHeaderData(2, Qt::Horizontal, tr("Associated file exists"));
    for (int i = 0; i < map.global.tiles.count(); i++)
    {
        modelTiles->setData(modelTiles->index(i, 0), map.global.tiles[i].position.x);
        modelTiles->setData(modelTiles->index(i, 1), map.global.tiles[i].position.y);
        modelTiles->setData(modelTiles->index(i, 2), (QFile::exists(map.dir + "/" + map.global.tiles[i].filename)) ? tr("Yes") : tr("No"));
    }
    ui->tvwTiles->setModel(modelTiles);
    ui->tvwTiles->resizeColumnsToContents();

    // PROTECTED PROPS

    ui->sbxVersion->setValue(map.global.version);

    ui->sbxNextIDCode->setValue(map.global.nextIDCode);

    setupFinished = true;
}

void wGlobalProps::on_btnSave_clicked()
{
    emit returnGlobalProps(map.global);
    close();
}

void wGlobalProps::on_btnBISelectPicturePath_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Select terrain background picture"), (ui->ledBIPicturePath->text().isEmpty()) ? set.read("main", "mainDir").toString() : ui->ledBIPicturePath->text(), OTStrings::textureSuffixes);

    if (!path.isEmpty()) {
        map.global.bgImage.picturePath = path;
        reloadUI();
    }
}

void wGlobalProps::recieveGroundTexture(OCMap::Global::Texture texture, int index)
{
    if (index == -1) map.global.groundTextures.append(texture);
    else map.global.groundTextures.insert(index, texture);

    reloadUI();
}

void wGlobalProps::on_ledFriendlyname_textEdited(const QString &arg1)
{
    map.global.friendlyname = arg1;
    if (ui->btnSyncNames->isChecked()) ui->ledName->setText(arg1);
}

void wGlobalProps::on_ledName_textEdited(const QString &arg1)
{
    map.global.name = arg1;
}

void wGlobalProps::on_btnSyncNames_clicked(bool checked)
{
    if (checked)  ui->ledName->setText(map.global.friendlyname);
    ui->ledName->setEnabled(!checked);
}

void wGlobalProps::on_cuwGroundTextures_addClicked()
{
    WEDITGROUNDTEXTURE = new wEditGroundTexture(OCMap::Global::Texture(), -1, this);
    WEDITGROUNDTEXTURE->show();
}

void wGlobalProps::on_cuwGroundTextures_editClicked()
{
    WEDITGROUNDTEXTURE = new wEditGroundTexture(map.global.groundTextures[ui->tvwGroundTextures->currentIndex().row()], ui->tvwGroundTextures->currentIndex().row(), this);
    WEDITGROUNDTEXTURE->show();
}

void wGlobalProps::on_cuwGroundTextures_removeClicked()
{
    // TODO - pay attention to painted texture files in map folder - maybe rename them by name (means also deleting old files)?
}

void wGlobalProps::on_btnTest_clicked()
{

}

void wGlobalProps::on_cobxCurrency_currentIndexChanged(int index)
{
    if (setupFinished) map.global.currency = currency[index - 1].second.remove(set.read("main", "mainDir").toString() + "/");
}

void wGlobalProps::on_cobxTicketpack_currentIndexChanged(int index)
{
    if (setupFinished) map.global.ticketpack = tickets[index - 1].second.remove(set.read("main", "mainDir").toString() + "/");
}

void wGlobalProps::on_pteDescription_textChanged()
{
    map.global.description = ui->pteDescription->toPlainText();
}

void wGlobalProps::on_cbxWorldCoordinates_stateChanged(int arg1)
{
    map.global.worldCoodinates = arg1;
}

void wGlobalProps::on_cbxDynamicHelpArrows_stateChanged(int arg1)
{
    map.global.dynHelpers = arg1;
}

void wGlobalProps::on_cbxLeftHandDriving_stateChanged(int arg1)
{
    map.global.lht = arg1;
}

void wGlobalProps::on_cbxRealrail_stateChanged(int arg1)
{
    map.global.realrail = arg1;
}

void wGlobalProps::on_tieRepairTime_userTimeChanged(const QTime &time)
{
    map.global.repairTime = time;
}

void wGlobalProps::on_cuwYears_name1Changed(float value)
{
    map.global.startYear = static_cast<int>(value);
}

void wGlobalProps::on_cuwYears_name2Changed(float value)
{
    map.global.endYear = static_cast<int>(value);
}

void wGlobalProps::on_sbxRealYearOffset_valueChanged(int arg1)
{
    map.global.realYearOffset = arg1;
}

void wGlobalProps::on_cbxBackgroundImage_stateChanged(int arg1)
{
    map.global.bgImage.isVisible = arg1;
    ui->fraBackgroundImage->setEnabled(arg1);
}

void wGlobalProps::on_cuwBIPictureSize_name1Changed(float value)
{
    map.global.bgImage.width = static_cast<int>(value);
}

void wGlobalProps::on_cuwBIPictureSize_name2Changed(float value)
{
    map.global.bgImage.height = static_cast<int>(value);
}

void wGlobalProps::on_sbxBIStartWidth_valueChanged(int arg1)
{
    map.global.bgImage.startWidth = arg1;
}

void wGlobalProps::on_sbxBIStartHeight_valueChanged(int arg1)
{
    map.global.bgImage.startHeight = arg1;
}

void wGlobalProps::on_cuwDVTilePosition_name1Changed(float value)
{
    map.global.standardView.tilePosition.x = value;
}

void wGlobalProps::on_cuwDVTilePosition_name2Changed(float value)
{
    map.global.standardView.tilePosition.y = value;
}

void wGlobalProps::on_cuwDVPosition_name1Changed(float value)
{
    map.global.standardView.position.x = value;
}

void wGlobalProps::on_cuwDVPosition_name2Changed(float value)
{
    map.global.standardView.position.y = value;
}

void wGlobalProps::on_cuwDVPosition_name3Changed(float value)
{
    map.global.standardView.position.z = value;
}

void wGlobalProps::on_cuwDVRotation_name1Changed(float value)
{
    map.global.standardView.rotAroundX = value;
}

void wGlobalProps::on_cuwDVRotation_name2Changed(float value)
{
    map.global.standardView.rotAroundZ = value;
}

void wGlobalProps::on_sbxDVDistanceFromPosition_valueChanged(int arg1)
{
    map.global.standardView.distanceFromPosition = arg1;
}

void wGlobalProps::on_cuwSeasons_addClicked()
{
    WEDITSEASON = new wEditSeason(OCMap::Global::Season(), -1, this);
    WEDITSEASON->show();
}


void wGlobalProps::on_cuwSeasons_editClicked()
{
    WEDITSEASON = new wEditSeason(map.global.seasons[ui->tvwSeasons->currentIndex().row()], ui->tvwGroundTextures->currentIndex().row(), this);
    WEDITSEASON->show();
}


void wGlobalProps::on_cuwSeasons_removeClicked()
{
    map.global.seasons.removeAt(ui->tvwSeasons->currentIndex().row());
    reloadUI();
}


void wGlobalProps::on_actionClose_triggered()
{
    close();
}

