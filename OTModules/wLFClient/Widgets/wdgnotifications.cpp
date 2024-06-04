#include "wdgnotifications.h"
#include "ui_wdgnotifications.h"

wdgNotifications::wdgNotifications(QWidget *parent, LFClientAPIInterface *api)
    : QWidget(parent)
    , ui(new Ui::wdgNotifications),
    api(api)
{
    ui->setupUi(this);

    timer10s = new QTimer(this);
    connect(timer10s, &QTimer::timeout, this, &wdgNotifications::reloadUi10s);
    timer10s->start(10000);

    connect(api, &LFClientAPIInterface::loginStatusChanged, this, &wdgNotifications::reloadUi10s);

    ui->scrollingText->setText("Hier geht es nach unten: gg");
    //ui->scrollingText->setText("Dies ist ein Lauftext lol!");
    QFont f;
    f.setPixelSize(30);
    ui->scrollingText->setFont(f);
    ui->scrollingText->setSpeed(100);
    ui->scrollingText->setDirection(ScrollingText::RightDirection);

    reloadUi10s();
}

wdgNotifications::~wdgNotifications()
{
    delete ui;
}

void wdgNotifications::reloadUi10s()
{
    int currentRow = ui->lwgNotifications->currentRow();

    ui->lwgNotifications->clear();
    if(api->isLoggedIn()) {
        QList<LFCApiNotification> notifications = api->getNotifications();

        for (int i = 0; i < notifications.count(); i++)
        {
            wdgNotification *notification = new wdgNotification(notifications[i].afftectedLines(), notifications[i].title(), notifications[i].text(), this);
            notification->adjustSize();
            QListWidgetItem *item = new QListWidgetItem();

            item->setSizeHint(notification->sizeHint());
            ui->lwgNotifications->addItem(item);

            ui->lwgNotifications->setItemWidget(item, notification);
        }

        if (currentRow <= ui->lwgNotifications->count() - 1) ui->lwgNotifications->setCurrentRow(currentRow);
    }
}
