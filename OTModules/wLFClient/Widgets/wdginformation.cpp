#include "wdginformation.h"
#include "ui_wdginformation.h"

#include "OTBackend/LFClientAPIInterface/lfclientapiinterface.h"

wdgInformation::wdgInformation(QWidget *parent, LFClientAPIInterface *api)
    : QWidget(parent)
    , ui(new Ui::wdgInformation),
    api(api)
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
    QTime omsiTime = QTime::currentTime().addSecs(timeDiff);

    ui->lcdRealTimeHours->display(QTime::currentTime().hour());
    ui->lcdRealTimeMinutes->display(QTime::currentTime().minute());
    ui->lcdRealTimeSeconds->display(QTime::currentTime().second());

    ui->lcdOMSITimeHours->display(omsiTime.hour());
    ui->lcdOMSITimeMinutes->display(omsiTime.minute());
    ui->lcdOMSITimeSeconds->display(omsiTime.second());
}

void wdgInformation::reloadUi5s()
{
    qDebug() << "Getting data...";

    LfCApiGlobalData data = api->getGlobalData();
    timeDiff = data.timeDiff();
    ui->ledMap->setText(data.mapName());
}
