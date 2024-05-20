#include "wdgnotifications.h"
#include "ui_wdgnotifications.h"

wdgNotifications::wdgNotifications(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::wdgNotifications)
{
    ui->setupUi(this);
}

wdgNotifications::~wdgNotifications()
{
    delete ui;
}
