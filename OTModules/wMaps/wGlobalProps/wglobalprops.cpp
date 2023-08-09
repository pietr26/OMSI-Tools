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

    // Load prefs
    setStyleSheet(set.read("main", "theme").toString());

    //ui->twgGlobalProps->setTabVisible(1, false); // TODO: Tab index "Protected props"

    props = globalProps;

    reloadUI();

    qInfo().noquote() << objectName() + " started";
}

// TODO: standartDepot prop in ailist preferences

wGlobalProps::~wGlobalProps()
{
    delete ui;
}

void wGlobalProps::on_btnEditProtectedProps_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Change protected properties"), tr("Changes in protected properties may result in unexpected behavior or bugs in the map editor. Continue?"), QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        //ui->twgGlobalProps->setTabVisible(1, true); // TODO: Tab index "Protected props"
        ui->btnEditProtectedProps->setVisible(false);
        //ui->twgGlobalProps->setCurrentIndex(1); // TODO: Tab index "Protected props"
    }
}

void wGlobalProps::reloadUI()
{
    // TODO
}

void wGlobalProps::saveUI()
{
    // TODO
}

void wGlobalProps::on_btnSave_clicked()
{
    saveUI();
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

void wGlobalProps::on_cbxBackgroundImage_stateChanged(int arg1)
{
    ui->fraBackgroundImage->setEnabled(arg1);
}

void wGlobalProps::on_cbxDefaultView_stateChanged(int arg1)
{
    ui->fraDefaultView->setEnabled(arg1);
}

void wGlobalProps::recieveGroundTexture(OCMap::Global::Texture texture, int index)
{
    if (index == -1) props.groundTextures.append(texture);
    else props.groundTextures.insert(index, texture);

    reloadUI();
}

void wGlobalProps::on_btnGTAdd_clicked()
{
    WEDITGROUNDTEXTURE = new wEditGroundTexture(OCMap::Global::Texture(), -1, this);
    WEDITGROUNDTEXTURE->show();
}

void wGlobalProps::on_btnGTEdit_clicked()
{
    WEDITGROUNDTEXTURE = new wEditGroundTexture(OCMap::Global::Texture(), ui->lwgGroundTextures->currentRow(), this);
    WEDITGROUNDTEXTURE->show();
}

void wGlobalProps::on_btnGTRemove_clicked()
{
    // TODO - pay attention to painted texture files in map folder - maybe rename them by name (means also deleting old files)?
}
