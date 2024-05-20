#include "wdgtrips.h"
#include "ui_wdgtrips.h"

wdgTrips::wdgTrips(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::wdgTrips)
{
    ui->setupUi(this);

    trips = new QStandardItemModel(3, 3);
    trips->setHeaderData(0, Qt::Horizontal, tr("Code"));
    trips->setHeaderData(1, Qt::Horizontal, tr("From"));
    trips->setHeaderData(2, Qt::Horizontal, tr("To"));

    trips->setData(trips->index(0, 0), "76/1-1");
    trips->setData(trips->index(0, 1), "[04:07] Nordspitze");
    trips->setData(trips->index(0, 2), "[04:??] Bauernhof");

    trips->setData(trips->index(1, 0), "76/1-1");
    trips->setData(trips->index(1, 1), "[04:22] Bauernhof");
    trips->setData(trips->index(1, 2), "[04:??] Nordspitze");

    trips->setData(trips->index(2, 0), "76/1-1");
    trips->setData(trips->index(2, 1), "[04:37] Nordspitze");
    trips->setData(trips->index(2, 2), "[04:??] Bauernhof");

    QFont font;
    font.setStrikeOut(true);
    font.setItalic(true);
    trips->item(2, 0)->setFont(font);
    trips->item(2, 1)->setFont(font);
    trips->item(2, 2)->setFont(font);

    ui->tvwTrips->setModel(trips);
    ui->tvwTrips->resizeColumnsToContents();
}

wdgTrips::~wdgTrips()
{
    delete ui;
}
