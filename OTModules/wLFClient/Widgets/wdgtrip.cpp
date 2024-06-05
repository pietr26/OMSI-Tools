#include "wdgtrip.h"
#include "ui_wdgtrip.h"

wdgTrip::wdgTrip(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::wdgTrip)
{
    ui->setupUi(this);
}

wdgTrip::~wdgTrip()
{
    delete ui;
}

void wdgTrip::loadUi()
{
    ui->lblLineRoute->setText(QString::number(tripData[currentIndex].line()) + "/" + QString::number(tripData[currentIndex].route()));
    ui->lblTrip->setText(tripData[currentIndex].name());
    ui->lblStopCount->setText(QString::number(tripData[currentIndex].busstops().count()) + " " + tr("stops", "busstops"));

    trip = new QStandardItemModel(tripData[currentIndex].busstops().count(), 4);
    trip->setHeaderData(0, Qt::Horizontal, tr("Time"));
    trip->setHeaderData(1, Qt::Horizontal, tr("Name"));
    trip->setHeaderData(2, Qt::Horizontal, tr("Delay"));
    trip->setHeaderData(3, Qt::Horizontal, tr("Note"));

    for (int i = 0; i < tripData[currentIndex].busstops().count(); i++)
    {
        if (tripData[currentIndex].busstops()[i].status() == LFCApiBusstop::Status::notAssigned) continue;

        trip->setData(trip->index(i, 0), tripData[currentIndex].busstops()[i].time().toString("HH:mm"));
        trip->setData(trip->index(i, 1), tripData[currentIndex].busstops()[i].name());
        trip->setData(trip->index(i, 2), QString::number(tripData[currentIndex].busstops()[i].delay() * 60) + " min");

        if (tripData[currentIndex].busstops()[i].status() != LFCApiBusstop::Status::runsAsPlanned)
        {
            trip->setData(trip->index(i, 3), tr("Stop cancelled"));

            QFont strikeOut;
            strikeOut.setStrikeOut(true);

            trip->item(i, 0)->setFont(strikeOut);
            trip->item(i, 1)->setFont(strikeOut);
            trip->item(i, 2)->setFont(strikeOut);

            trip->item(i, 0)->setForeground(QColor(Qt::red));
            trip->item(i, 1)->setForeground(QColor(Qt::red));
            trip->item(i, 2)->setForeground(QColor(Qt::red));
            trip->item(i, 3)->setForeground(QColor(Qt::red));
        }
    }

    ui->tvwTrip->setModel(trip);
    ui->tvwTrip->resizeColumnsToContents();
}

void wdgTrip::changeTrip(QList<LFCApiTrip> trips, int index)
{
    tripData = trips;
    currentIndex = index;
    loadUi();
}
