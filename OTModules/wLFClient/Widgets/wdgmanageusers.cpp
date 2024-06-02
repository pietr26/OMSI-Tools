#include "wdgmanageusers.h"
#include "ui_wdgmanageusers.h"

wdgManageUsers::wdgManageUsers(QWidget *parent, LFClientAPIInterface *api)
    : QWidget(parent)
    , ui(new Ui::wdgManageUsers),
    api(api)
{
    ui->setupUi(this);

    timer10s = new QTimer(this);
    connect(timer10s, &QTimer::timeout, this, &wdgManageUsers::reloadUi10s);
    timer10s->start(10000);

    connect(api, &LFClientAPIInterface::loginStatusChanged, this, &wdgManageUsers::reloadUi10s);

    reloadUi10s();
}

wdgManageUsers::~wdgManageUsers()
{
    delete ui;
}

void wdgManageUsers::reloadUi10s()
{
    if (api->isLoggedIn()) {
        // QList<LFCApiNotification> notifications = api->...();

        // ...
    }
}
