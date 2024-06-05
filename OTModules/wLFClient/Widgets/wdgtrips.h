#ifndef WDGTRIPS_H
#define WDGTRIPS_H

#include <QWidget>
#include <QStandardItemModel>
#include "OTBackend/LFClientAPIInterface/lfclientapiinterface.h"
#include <QTimer>

namespace Ui {
class wdgTrips;
}

class wdgTrips : public QWidget
{
    Q_OBJECT

public:
    explicit wdgTrips(QWidget *parent = nullptr, LFClientAPIInterface *api = nullptr);
    ~wdgTrips();

private slots:
    void on_btnTest_clicked();

private:
    Ui::wdgTrips *ui;

    QStandardItemModel *trips;

    LFClientAPIInterface *api;

    QTimer *timer10s;

    QList<LFCApiTrip> tripData;

signals:
    void tripSelectionChanged(QList<LFCApiTrip> trips, int index);

private slots:
    void reloadUi10s();
    void on_tvwTrips_clicked(const QModelIndex &index);
};

#endif // WDGTRIPS_H
