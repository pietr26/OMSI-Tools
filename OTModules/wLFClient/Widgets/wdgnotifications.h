#ifndef WDGNOTIFICATIONS_H
#define WDGNOTIFICATIONS_H

#include <QWidget>
#include <QStandardItemModel>
#include <QTimer>
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

    QTimer *timer10s;

    QStandardItemModel *notifications;

private slots:
    void reloadUi10s();
};

#endif // WDGNOTIFICATIONS_H
