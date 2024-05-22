#ifndef WDGNOTIFICATIONS_H
#define WDGNOTIFICATIONS_H

#include <QWidget>
#include <QStandardItemModel>
#include "OTBackend/OTGlobal.h"

#include "OTBackend/LFClientAPIInterface/lfclientapiinterface.h"
#include "OTModules/wLFClient/Widgets/wdgnotification.h"

namespace Ui {
class wdgNotifications;
}

class wdgNotifications : public QWidget
{
    Q_OBJECT

public:
    explicit wdgNotifications(QWidget *parent = nullptr, LFClientAPIInterface *api = nullptr);
    ~wdgNotifications();

private:
    Ui::wdgNotifications *ui;

    LFClientAPIInterface *api;

    void reloadUi10s();

    QTimer *timer10s;

    QStandardItemModel *notifications;
};

#endif // WDGNOTIFICATIONS_H
