#include "wdgtrips.h"
#include "ui_wdgtrips.h"

wdgTrips::wdgTrips(QWidget *parent, LFClientAPIInterface *api)
    : QWidget(parent)
    , ui(new Ui::wdgTrips),
    api(api)
{
    ui->setupUi(this);

    timer10s = new QTimer(this);
    connect(timer10s, &QTimer::timeout, this, &wdgTrips::reloadUi10s);
    timer10s->start(10000);

    reloadUi10s();
    connect(api, &LFClientAPIInterface::loginStatusChanged, this, &wdgTrips::reloadUi10s);
}

wdgTrips::~wdgTrips()
{
    delete ui;
}

void wdgTrips::on_btnTest_clicked()
{
    if (api->isLoggedIn())
    {
        LFCApiSpeakRequest test = api->getMySpeakRequest();
        qInfo() << "test";
    }
}

void wdgTrips::reloadUi10s()
{
    if (api->isLoggedIn())
    {
        tripData = api->getMyTrips();

        trips = new QStandardItemModel(tripData.count(), 6);
        trips->setHeaderData(0, Qt::Horizontal, tr("Vehicle"));
        trips->setHeaderData(1, Qt::Horizontal, tr("Line"));
        trips->setHeaderData(2, Qt::Horizontal, tr("Route"));
        trips->setHeaderData(3, Qt::Horizontal, tr("Trip"));
        trips->setHeaderData(4, Qt::Horizontal, tr("From"));
        trips->setHeaderData(5, Qt::Horizontal, tr("To"));

        for (int i = 0; i < tripData.count(); i++)
        {
            trips->setData(trips->index(i, 0), api->getVehicle(tripData[i].vehicle()).number());
            trips->setData(trips->index(i, 1), tripData[i].line());
            trips->setData(trips->index(i, 2), tripData[i].route());
            trips->setData(trips->index(i, 3), tripData[i].name());
            trips->setData(trips->index(i, 4), (tripData[i].busstops().isEmpty() ? "---" : tripData[i].busstops().first().name() + " (" + tripData[i].busstops().first().time().toString("HH:mm") + ")"));
            trips->setData(trips->index(i, 5), (tripData[i].busstops().isEmpty() ? "---" : tripData[i].busstops().last().name() + " (" + tripData[i].busstops().last().time().toString("HH:mm") + ")"));
        }

        // QFont font;
        // font.setStrikeOut(true);
        // font.setItalic(true);
        // trips->item(2, 0)->setFont(font);
        // trips->item(2, 1)->setFont(font);
        // trips->item(2, 2)->setFont(font);

        ui->tvwTrips->setModel(trips);
        ui->tvwTrips->resizeColumnsToContents();
        connect(ui->tvwTrips->selectionModel(), &QItemSelectionModel::selectionChanged, this, &wdgTrips::onCurrentTripChanged);
    }
    else
    {
        trips = new QStandardItemModel();
        ui->tvwTrips->setModel(trips);
    }
}

void wdgTrips::onCurrentTripChanged() {
    emit tripSelectionChanged(tripData, ui->tvwTrips->selectionModel()->currentIndex().row());
}
