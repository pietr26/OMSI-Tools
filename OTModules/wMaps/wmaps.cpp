#include "wmaps.h"
#include "ui_wmaps.h"

wMaps::wMaps(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wMaps)
{
    qInfo().noquote() << "Starting " + objectName() + "...";
    qDebug() << "Set up UI...";
    ui->setupUi(this);
    //adjustSize();
    qDebug() << "UI set";

    setWindowTitle(OTInformation::name + " - " + tr("Map editor"));

    DiscordGameSDK::setModule(tr("Map editor"));
    DiscordGameSDK::setStatus(tr("Editing a map's properties"));
    DiscordGameSDK::setStart();
    DiscordGameSDK::setIcon("wmaps");
    DiscordGameSDK::update();

    enableUi(false);

    qInfo().noquote() << objectName() + " started";
}

wMaps::~wMaps()
{
    delete ui;
}

void wMaps::on_actionPreferences_triggered()
{
    WPREFERENCES = new wPreferences();
    WPREFERENCES->show();
}

void wMaps::on_actionBackToHome_triggered()
{
    close();
    emit backToHome();
}

void wMaps::on_actionClose_triggered()
{
    QApplication::quit();
}

void wMaps::on_actionSendFeedback_triggered()
{
    wFeedback *WFEEDBACK = new wFeedback(this, OTLinks::Wiki::maps);
    WFEEDBACK->show();
}

void wMaps::on_btnLoadMap_clicked()
{
    on_actionLoadMap_triggered();
}

void wMaps::recieveSelectedMap(QPair<QString, QString> mapInfo)
{
    enableUi(true);
    currentMap = mapInfo;

    set.write(objectName(), "mapPath", currentMap.second);

    ui->ledCurrentMap->setText(mapInfo.first);

    // picture
    QString picture = currentMap.second.remove("global.cfg") + "picture.jpg";

    if (QFile(picture).exists())
        ui->lblPicture->setPixmap(QPixmap(picture));
    else
        ui->lblPicture->setPixmap(QIcon::fromTheme(QIcon::ThemeIcon::CameraPhoto).pixmap(128, 128));

    map.global = OCMap::Global();
    map.dir = currentMap.second.remove("/global.cfg");
    map.global.read();
}

void wMaps::on_actionLoadMap_triggered()
{
    WMAPSELECTION = new wMapSelection(this, set.read(objectName(), "mapPath").toString());
    connect(WMAPSELECTION, &wMapSelection::returnMapInfo, this, &wMaps::recieveSelectedMap);
    WMAPSELECTION->show();
}

void wMaps::on_btnEditPropterties_clicked()
{
    WGLOBALPROPS = new wGlobalProps(map.global, this);
    connect(WGLOBALPROPS, &wGlobalProps::returnGlobalProps, this, &wMaps::recieveGlobalProps);
    WGLOBALPROPS->show();
}

void wMaps::recieveGlobalProps(OCMap::Global globalProps)
{
    map.global = globalProps;
}

void wMaps::on_btnSave_clicked()
{
    ui->statusbar->showMessage(tr("Save global.cfg..."));
    ui->centralwidget->setEnabled(false);
    qApp->processEvents();
    map.global.write();
    ui->centralwidget->setEnabled(true);
    ui->statusbar->showMessage(tr("global.cfg saved!"));
}

void wMaps::on_btnPlaceObjects_clicked()
{
    WPLACEOBJECTS = new wPlaceObjects(map.global, this);
    connect(WPLACEOBJECTS, &wPlaceObjects::returnGlobalProps, this, &wMaps::recieveGlobalProps);
    WPLACEOBJECTS->show();
}

void wMaps::enableUi(bool enable)
{
    ui->btnEditPropterties->setEnabled(enable);
    ui->btnPlaceObjects->setEnabled(enable);
}

