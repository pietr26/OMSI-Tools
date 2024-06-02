#ifndef WLFCLIENTDISPATCHER_H
#define WLFCLIENTDISPATCHER_H

#include <QMainWindow>
#include <QDockWidget>
#include "OTBackend/OTGlobal.h"
#include "OTModules/OTGeneric/wfeedback.h"
#include "OTModules/OTGeneric/wpreferences.h"
#include "Widgets/wdginformation.h"
#include "Widgets/wdgmanagetrips.h"
#include "Widgets/wdgmanageusers.h"
#include "OTModules/wLFClient/wlogin.h"

#include "OTBackend/LFClientAPIInterface/lfclientapiinterface.h"

namespace Ui {
class wLFClientDispatcher;
}

class wLFClientDispatcher : public QMainWindow
{
    Q_OBJECT

public:
    explicit wLFClientDispatcher(QWidget *parent = nullptr);
    ~wLFClientDispatcher();

private slots:
    void on_actionBackToHome_triggered();

    void on_actionClose_triggered();

    void on_actionPreferences_triggered();

    void on_actionHelp_triggered();

    void on_actionLogin_triggered();

    void handleLogin();

    void on_actionLogout_triggered();

signals:
    void backToHome();

private:
    Ui::wLFClientDispatcher *ui;
    LFClientAPIInterface *api = new LFClientAPIInterface(this);
    wPreferences *WPREFERENCES;
    wLogin *WLOGIN;
    wdgInformation *WDGINFORMATION = new wdgInformation(this, api);
    wdgManageTrips *WDGMANAGETRIPS = new wdgManageTrips(this, api);
    wdgManageUsers *WDGMANAGEUSERS = new wdgManageUsers(this, api);

    OTNetworkConnection nc;

    void createDockWidgets();

};

#endif // WLFCLIENTDISPATCHER_H
