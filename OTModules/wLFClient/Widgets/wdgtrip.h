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
    explicit wdgTrip(QWidget *parent, LFClientAPIInterface *api);
    ~wdgTrip();

private:
    Ui::wdgTrip *ui;

    QList<LFCApiTrip> tripData;

    LFClientAPIInterface *api;

    QStandardItemModel *trip;
    int currentIndex;

public slots:
    void changeTrip(QList<LFCApiTrip> trips, int index);

private slots:
    void loadUi();
};

#endif // WDGTRIP_H
