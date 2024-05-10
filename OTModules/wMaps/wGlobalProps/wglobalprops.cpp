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

    props = globalProps;

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

    ui->ledFriendlyname->setText(props.friendlyname);

    ui->ledName->setText(props.name);

    on_btnSyncNames_clicked(props.name == props.friendlyname);
    ui->btnSyncNames->setChecked(props.name == props.friendlyname);
    on_ledFriendlyname_textEdited(ui->ledFriendlyname->text());

    ui->pteDescription->setPlainText(props.description);

    ui->cbxWorldCoordinates->setChecked(props.worldCoodinates);

    ui->cbxDynamicHelpArrows->setChecked(props.dynHelpers);

    ui->cbxLeftHandDriving->setChecked(props.lht);

    ui->cbxRealrail->setChecked(props.realrail);

    currency = filehandler.listMoney();
    ui->cobxCurrency->addItem(tr("(none)"));
    for (int i = 1; i < currency.count(); i++)
    {
        ui->cobxCurrency->addItem(currency[i].first);
        if (currency[i].second.remove(mainDir + "/").replace("/", "\\") == props.currency) ui->cobxCurrency->setCurrentIndex(i);
    }

    tickets = filehandler.listTicketpacks();
    ui->cobxTicketpack->addItem(tr("(none)"));
    for (int i = 1; i < tickets.count(); i++)
    {
        ui->cobxTicketpack->addItem(tickets[i].first);
        if (tickets[i].second.remove(mainDir + "/").replace("/", "\\") == props.ticketpack) ui->cobxTicketpack->setCurrentIndex(i);
    }

    ui->tieRepairTime->setTime(props.repairTime); // TODO: Test bigger as 60

    ui->cuwYears->setValue1(props.startYear);
    ui->cuwYears->setValue2(props.endYear);

    /// TEXTURES

    ui->cbxBackgroundImage->setChecked(props.bgImage.isVisible);
    ui->ledBIPicturePath->setText(props.bgImage.picturePath);
    ui->cuwBIPictureSize->setValue1(props.bgImage.width);
    ui->cuwBIPictureSize->setValue2(props.bgImage.height);
    ui->sbxBIStartWidth->setValue(props.bgImage.startWidth);
    ui->sbxBIStartHeight->setValue(props.bgImage.startHeight);

    ui->cuwDVTilePosition->setValue1(props.standardView.tilePosition.x);
    ui->cuwDVTilePosition->setValue2(props.standardView.tilePosition.y);
    ui->cuwDVPosition->setValue1(props.standardView.position.x);
    ui->cuwDVPosition->setValue2(props.standardView.position.y);
    ui->cuwDVPosition->setValue3(props.standardView.position.z);
    ui->cuwDVRotation->setValue1(props.standardView.rotAroundX);
    ui->cuwDVRotation->setValue2(props.standardView.rotAroundZ);
    ui->sbxDVDistanceFromPosition->setValue(props.standardView.distanceFromPosition);

    modelGroundTextures = new QStandardItemModel(props.groundTextures.count(), 1);
    modelGroundTextures->setHeaderData(0, Qt::Horizontal, tr("Texture"));
    for (int i = 0; i < props.groundTextures.count(); i++) { modelGroundTextures->setData(modelGroundTextures->index(i, 0), props.groundTextures[i].mainTex); }
    ui->tvwGroundTextures->setModel(modelGroundTextures);
    ui->tvwGroundTextures->resizeColumnsToContents();
    //if (props.groundTextures.count() == 0) ui->cuwGroundTextures->setRemoveEnabled() // TODO

    /// ENV & TRAFFIC

    modelSeasons = new QStandardItemModel(props.seasons.count(), 3);
    modelSeasons->setHeaderData(0, Qt::Horizontal, tr("Season"));
    modelSeasons->setHeaderData(1, Qt::Horizontal, tr("Start"));
    modelSeasons->setHeaderData(2, Qt::Horizontal, tr("End"));
    for (int i = 0; i < props.seasons.count(); i++)
    {
        modelSeasons->setData(modelSeasons->index(i, 0), props.seasons[i].name());
        modelSeasons->setData(modelSeasons->index(i, 1), props.seasons[i].start.toString("dd.MM."));
        modelSeasons->setData(modelSeasons->index(i, 2), props.seasons[i].end.toString("dd.MM."));
    }
    ui->tvwSeasons->setModel(modelSeasons);
    ui->tvwSeasons->resizeColumnsToContents();

    modelVehicleDensity = new QStandardItemModel(props.trafficDensities.count(), 2);
    modelVehicleDensity->setHeaderData(0, Qt::Horizontal, tr("Time"));
    modelVehicleDensity->setHeaderData(1, Qt::Horizontal, tr("Factor"));
    for (int i = 0; i < props.trafficDensities.count(); i++)
    {
        modelVehicleDensity->setData(modelVehicleDensity->index(i, 0), props.trafficDensities[i].time.toString("HH:mm"));
        modelVehicleDensity->setData(modelVehicleDensity->index(i, 1), props.trafficDensities[i].factor);
    }
    ui->tvwTrafficDensities->setModel(modelVehicleDensity);
    ui->tvwTrafficDensities->resizeColumnsToContents();

    modelPassengerDensity = new QStandardItemModel(props.passengerDensities.count(), 2);
    modelPassengerDensity->setHeaderData(0, Qt::Horizontal, tr("Time"));
    modelPassengerDensity->setHeaderData(1, Qt::Horizontal, tr("Factor"));
    for (int i = 0; i < props.passengerDensities.count(); i++)
    {
        modelPassengerDensity->setData(modelPassengerDensity->index(i, 0), props.passengerDensities[i].time.toString("HH:mm"));
        modelPassengerDensity->setData(modelPassengerDensity->index(i, 1), props.passengerDensities[i].factor);
    }
    ui->tvwPassengerDensities->setModel(modelPassengerDensity);
    ui->tvwPassengerDensities->resizeColumnsToContents();

    /// ENTRYPOINTS

    modelEntrypoints = new QStandardItemModel(props.entrypoints.count(), 2);
    modelEntrypoints->setHeaderData(0, Qt::Horizontal, tr("Name"));
    modelEntrypoints->setHeaderData(1, Qt::Horizontal, tr("Count"));
    for (int i = 0; i < props.entrypoints.count(); i++)
    {
        modelEntrypoints->setData(modelEntrypoints->index(i, 0), props.entrypoints[i].name);
        modelEntrypoints->setData(modelEntrypoints->index(i, 1), props.entrypoints[i].entrypoints.count());
    }
    ui->tvwEntrypoints->setModel(modelEntrypoints);
    ui->tvwEntrypoints->resizeColumnsToContents();

    /// TILES

    modelTiles = new QStandardItemModel(props.tiles.count(), 3);
    modelTiles->setHeaderData(0, Qt::Horizontal, tr("x coordinate"));
    modelTiles->setHeaderData(1, Qt::Horizontal, tr("y coordinate"));
    modelTiles->setHeaderData(2, Qt::Horizontal, tr("Associated file exists"));
    for (int i = 0; i < props.tiles.count(); i++)
    {
        modelTiles->setData(modelTiles->index(i, 0), props.tiles[i].position.x);
        modelTiles->setData(modelTiles->index(i, 1), props.tiles[i].position.y);
        modelTiles->setData(modelTiles->index(i, 2), (QFile::exists(QString(props.filepath).remove("global.cfg") + props.tiles[i].filename)) ? tr("Yes") : tr("No"));
    }
    ui->tvwTiles->setModel(modelTiles);
    ui->tvwTiles->resizeColumnsToContents();

    // PROTECTED PROPS

    ui->sbxVersion->setValue(props.version);

    ui->sbxNextIDCode->setValue(props.nextIDCode);

    setupFinished = true;
}

void wGlobalProps::on_btnSave_clicked()
{
    emit returnGlobalProps(props);
    close();
}

void wGlobalProps::on_btnBISelectPicturePath_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Select terrain background picture"), (ui->ledBIPicturePath->text().isEmpty()) ? set.read("main", "mainDir").toString() : ui->ledBIPicturePath->text(), OTStrings::textureSuffixes);

    if (!path.isEmpty()) {
        props.bgImage.picturePath = path;
        reloadUI();
    }
}

void wGlobalProps::recieveGroundTexture(OCMap::Global::Texture texture, int index)
{
    if (index == -1) props.groundTextures.append(texture);
    else props.groundTextures.insert(index, texture);

    reloadUI();
}

void wGlobalProps::on_ledFriendlyname_textEdited(const QString &arg1)
{
    props.friendlyname = arg1;
    if (ui->btnSyncNames->isChecked()) ui->ledName->setText(arg1);
}

void wGlobalProps::on_ledName_textEdited(const QString &arg1)
{
    props.name = arg1;
}

void wGlobalProps::on_btnSyncNames_clicked(bool checked)
{
    if (checked)  ui->ledName->setText(props.friendlyname);
    ui->ledName->setEnabled(!checked);
}

void wGlobalProps::on_cuwGroundTextures_addClicked()
{
    WEDITGROUNDTEXTURE = new wEditGroundTexture(OCMap::Global::Texture(), -1, this);
    WEDITGROUNDTEXTURE->show();
}

void wGlobalProps::on_cuwGroundTextures_editClicked()
{
    WEDITGROUNDTEXTURE = new wEditGroundTexture(props.groundTextures[ui->tvwGroundTextures->currentIndex().row()], ui->tvwGroundTextures->currentIndex().row(), this);
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
    if (setupFinished) props.currency = currency[index - 1].second.remove(set.read("main", "mainDir").toString() + "/");
}

void wGlobalProps::on_cobxTicketpack_currentIndexChanged(int index)
{
    if (setupFinished) props.ticketpack = tickets[index - 1].second.remove(set.read("main", "mainDir").toString() + "/");
}

void wGlobalProps::on_pteDescription_textChanged()
{
    props.description = ui->pteDescription->toPlainText();
}

void wGlobalProps::on_cbxWorldCoordinates_stateChanged(int arg1)
{
    props.worldCoodinates = arg1;
}

void wGlobalProps::on_cbxDynamicHelpArrows_stateChanged(int arg1)
{
    props.dynHelpers = arg1;
}

void wGlobalProps::on_cbxLeftHandDriving_stateChanged(int arg1)
{
    props.lht = arg1;
}

void wGlobalProps::on_cbxRealrail_stateChanged(int arg1)
{
    props.realrail = arg1;
}

void wGlobalProps::on_tieRepairTime_userTimeChanged(const QTime &time)
{
    props.repairTime = time;
}

void wGlobalProps::on_cuwYears_name1Changed(float value)
{
    props.startYear = static_cast<int>(value);
}

void wGlobalProps::on_cuwYears_name2Changed(float value)
{
    props.endYear = static_cast<int>(value);
}

void wGlobalProps::on_sbxRealYearOffset_valueChanged(int arg1)
{
    props.realYearOffset = arg1;
}

void wGlobalProps::on_cbxBackgroundImage_stateChanged(int arg1)
{
    props.bgImage.isVisible = arg1;
    ui->fraBackgroundImage->setEnabled(arg1);
}

void wGlobalProps::on_cuwBIPictureSize_name1Changed(float value)
{
    props.bgImage.width = static_cast<int>(value);
}

void wGlobalProps::on_cuwBIPictureSize_name2Changed(float value)
{
    props.bgImage.height = static_cast<int>(value);
}

void wGlobalProps::on_sbxBIStartWidth_valueChanged(int arg1)
{
    props.bgImage.startWidth = arg1;
}

void wGlobalProps::on_sbxBIStartHeight_valueChanged(int arg1)
{
    props.bgImage.startHeight = arg1;
}

void wGlobalProps::on_cuwDVTilePosition_name1Changed(float value)
{
    props.standardView.tilePosition.x = value;
}

void wGlobalProps::on_cuwDVTilePosition_name2Changed(float value)
{
    props.standardView.tilePosition.y = value;
}

void wGlobalProps::on_cuwDVPosition_name1Changed(float value)
{
    props.standardView.position.x = value;
}

void wGlobalProps::on_cuwDVPosition_name2Changed(float value)
{
    props.standardView.position.y = value;
}

void wGlobalProps::on_cuwDVPosition_name3Changed(float value)
{
    props.standardView.position.z = value;
}

void wGlobalProps::on_cuwDVRotation_name1Changed(float value)
{
    props.standardView.rotAroundX = value;
}

void wGlobalProps::on_cuwDVRotation_name2Changed(float value)
{
    props.standardView.rotAroundZ = value;
}

void wGlobalProps::on_sbxDVDistanceFromPosition_valueChanged(int arg1)
{
    props.standardView.distanceFromPosition = arg1;
}

void wGlobalProps::on_cuwSeasons_addClicked()
{
    WEDITSEASON = new wEditSeason(OCMap::Global::Season(), -1, this);
    WEDITSEASON->show();
}


void wGlobalProps::on_cuwSeasons_editClicked()
{
    WEDITSEASON = new wEditSeason(props.seasons[ui->tvwSeasons->currentIndex().row()], ui->tvwGroundTextures->currentIndex().row(), this);
    WEDITSEASON->show();
}


void wGlobalProps::on_cuwSeasons_removeClicked()
{
    props.seasons.removeAt(ui->tvwSeasons->currentIndex().row());
    reloadUI();
}


void wGlobalProps::on_actionClose_triggered()
{
    close();
}

