#include "wdgmanagetrips.h"
#include "ui_wdgmanagetrips.h"

wdgManageTrips::wdgManageTrips(QWidget *parent, LFClientAPIInterface *api)
    : QWidget(parent)
    , ui(new Ui::wdgManageTrips),
    api(api)
{
    ui->setupUi(this);

    timer10s = new QTimer(this);
    connect(timer10s, &QTimer::timeout, this, &wdgManageTrips::reloadUi10s);
    timer10s->start(10000);

    connect(api, &LFClientAPIInterface::loginStatusChanged, this, &wdgManageTrips::reloadUi10s);

    reloadUi10s();
}

wdgManageTrips::~wdgManageTrips()
{
    delete ui;
}

void wdgManageTrips::reloadUi10s()
{
    if (api->isLoggedIn()) {
        // QList<LFCApiNotification> notifications = api->...();

        // ...
    }
}
