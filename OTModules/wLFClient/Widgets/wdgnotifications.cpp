#include "wdgnotifications.h"
#include "ui_wdgnotifications.h"

wdgNotifications::wdgNotifications(QWidget *parent, LFClientAPIInterface *api)
    : QWidget(parent)
    , ui(new Ui::wdgNotifications)
{
    ui->setupUi(this);

    timer10s = new QTimer(this);
    connect(timer10s, &QTimer::timeout, this, &wdgNotifications::reloadUi10s);
    timer10s->start(10000);

    reloadUi10s();
}

wdgNotifications::~wdgNotifications()
{
    delete ui;
}

void wdgNotifications::reloadUi10s()
{
    int currentRow = ui->lwgNotifications->currentRow();
    qInfo() << "currentRow:" << currentRow;
    qInfo() << "count:" << ui->lwgNotifications->count();
    //LFCApiGlobalData notifications = api->getNotifications();

    //for (int i = 0; i < notifications.count(); i++) ...

    wdgNotification *notification = new wdgNotification(QStringList() << "76", "Straßensperrung Nordspitze", "Vom X zum Y ist der Bereich vor der Haltestelle \"Nordspitze, Bahnhof\" (Ri. Einsteindorf) gesperrt.\n\nFür alle betroffenen Linien ist eine Ersatzhaltestelle in der Nebenstraße aufgestellt.\n\nEs entstehen keine Verspätungen", this);
    notification->adjustSize();
    QListWidgetItem *item = new QListWidgetItem();

    item->setSizeHint(notification->sizeHint());
    ui->lwgNotifications->addItem(item);

    ui->lwgNotifications->setItemWidget(item, notification);

    if (currentRow <= ui->lwgNotifications->count() - 1) ui->lwgNotifications->setCurrentRow(currentRow);
}
