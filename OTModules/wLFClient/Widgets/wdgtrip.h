#ifndef WDGTRIP_H
#define WDGTRIP_H

#include <QWidget>
#include <QStandardItemModel>
#include "OTBackend/LFClientAPIInterface/lfclientapiinterface.h"

namespace Ui {
class wdgTrip;
}

class wdgTrip : public QWidget
{
    Q_OBJECT

public:
    explicit wdgTrip(QWidget *parent);
    ~wdgTrip();

private:
    Ui::wdgTrip *ui;

    void loadUi();

    QList<LFCApiTrip> tripData;

    QStandardItemModel *trip;
    int currentIndex;

public slots:
    void changeTrip(QList<LFCApiTrip> trips, int index);
};

#endif // WDGTRIP_H
