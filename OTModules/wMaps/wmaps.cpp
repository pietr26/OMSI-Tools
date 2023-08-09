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

    setWindowTitle(OTInformation::name + " - " + tr("map editor"));

    // Load prefs
    setStyleSheet(set.read("main", "theme").toString());

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
    backToHome();
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
    currentMap = mapInfo;

    set.write(objectName(), "mapPath", currentMap.second);

    ui->ledCurrentMap->setText(mapInfo.first);

    // picture
    QString picture = currentMap.second + "picture.jpg";
    picture.remove("global.cfg");

    if (QFile(picture).exists())
        ui->lblPicture->setPixmap(QPixmap(picture));
    else
        ui->lblPicture->setPixmap(QPixmap(":/rec/data/icons/iconUnvisible.svg").scaled(185, 140));
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
