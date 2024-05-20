#include "wdginformation.h"
#include "ui_wdginformation.h"

wdgInformation::wdgInformation(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::wdgInformation)
{
    ui->setupUi(this);

    timer1s = new QTimer(this);
    connect(timer1s, &QTimer::timeout, this, &wdgInformation::reloadUi1s);
    timer1s->start(1000);

    timer5s = new QTimer(this);
    connect(timer5s, &QTimer::timeout, this, &wdgInformation::reloadUi5s);
    timer5s->start(5000);

    reloadUi1s();
    reloadUi5s();
}

wdgInformation::~wdgInformation()
{
    delete ui;
}

void wdgInformation::reloadUi1s()
{
    ui->lcdRealTimeHours->display(QTime::currentTime().hour());
    ui->lcdRealTimeMinutes->display(QTime::currentTime().minute());
    ui->lcdRealTimeSeconds->display(QTime::currentTime().second());
}

void wdgInformation::reloadUi5s()
{
    qDebug() << "Getting data...";

    QByteArray bytearray = nc.get(OTLinks::lfClient::apiPath + "?name=map");
    ui->ledMap->setText(QJsonDocument::fromJson(bytearray).object().value("map").toString());
}
