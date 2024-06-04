#ifndef WDGMANAGETRIPS_H
#define WDGMANAGETRIPS_H

#include <QWidget>
#include <QTimer>
#include "OTBackend/LFClientAPIInterface/lfclientapiinterface.h"

namespace Ui {
class wdgManageTrips;
}

class wdgManageTrips : public QWidget
{
    Q_OBJECT

public:
    explicit wdgManageTrips(QWidget *parent, LFClientAPIInterface *api);
    ~wdgManageTrips();

private:
    Ui::wdgManageTrips *ui;

    LFClientAPIInterface *api;

    void reloadUi10s();

    QTimer *timer10s;
};

#endif // WDGMANAGETRIPS_H
