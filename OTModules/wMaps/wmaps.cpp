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

    // Load settings
    setStyleSheet(set.read("main", "theme").toString());

    loadMapList();

    qInfo().noquote() << objectName() + " started";
}

wMaps::~wMaps()
{
    delete ui;
}

void wMaps::on_actionSettings_triggered()
{
    WSETTINGS = new wSettings(this);
    WSETTINGS->show();
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
    wFeedback *WFEEDBACK = new wFeedback(this, OTLinks::wiki::maps);
    WFEEDBACK->show();
}

/// Loads map list
void wMaps::loadMapList()
{
    mapListSetupFinished = false;
    ui->cobxMapName->clear();

    qDebug() << "Reload map list...";
    mapList = filehandler.listMaps();

    qDebug().noquote() << "Map count:" << mapList.size();

    for (int i = 0; i < mapList.size(); i++)
        ui->cobxMapName->addItem(mapList[i].second);

    for (int i = 0; i < mapList.size(); i++)
    {
        if (mapList[i].first == set.read(objectName(), "mapPath").toString())
        {
            ui->cobxMapName->setCurrentIndex(i);
            i = mapList.size();
        }
    }

    mapListSetupFinished = true;
}

/// Sets UI for a new map
void wMaps::on_cobxMapName_currentIndexChanged(int index)
{
    if ((!mapListSetupFinished) || mapList.isEmpty())
        return;

    set.write(objectName(), "mapPath", mapList[index].first);
    filehandler.setMapPath(mapList[index].first);

    // Load whole stuff for UI here...
}
