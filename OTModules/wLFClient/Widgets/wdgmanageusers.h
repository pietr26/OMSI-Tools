#ifndef WDGMANAGEUSERS_H
#define WDGMANAGEUSERS_H

#include <QWidget>
#include <QTimer>
#include "OTBackend/LFClientAPIInterface/lfclientapiinterface.h"

namespace Ui {
class wdgManageUsers;
}

class wdgManageUsers : public QWidget
{
    Q_OBJECT

public:
    explicit wdgManageUsers(QWidget *parent = nullptr, LFClientAPIInterface *api = nullptr);
    ~wdgManageUsers();

private:
    Ui::wdgManageUsers *ui;

    LFClientAPIInterface *api;

    void reloadUi10s();

    QTimer *timer10s;
};

#endif // WDGMANAGEUSERS_H
