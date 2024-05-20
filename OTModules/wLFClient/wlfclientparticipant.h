#ifndef WLFCLIENTPARTICIPANT_H
#define WLFCLIENTPARTICIPANT_H

#include <QMainWindow>
#include <QDockWidget>
#include "OTBackend/OTGlobal.h"
#include "OTModules/OTGeneric/wfeedback.h"
#include "OTModules/OTGeneric/wpreferences.h"
#include "Widgets/wdginformation.h"
#include "Widgets/wdgactions.h"
#include "Widgets/wdgtrips.h"
#include "Widgets/wdgnotifications.h"
#include "OTModules/wLFClient/wlogin.h"

namespace Ui {
class wLFClientParticipant;
}

class wLFClientParticipant : public QMainWindow
{
    Q_OBJECT

public:
    explicit wLFClientParticipant(QWidget *parent = nullptr);
    ~wLFClientParticipant();

private slots:
    void on_actionBackToHome_triggered();

    void on_actionClose_triggered();

    void on_actionPreferences_triggered();

    void on_actionHelp_triggered();

    void on_actionLogin_triggered();

signals:
    void backToHome();

private:
    Ui::wLFClientParticipant *ui;
    OTSettings set;
    wPreferences *WPREFERENCES;
    wLogin *WLOGIN;
    wdgInformation *WDGINFORMATION = new wdgInformation();
    wdgActions *WDGACTIONS = new wdgActions();
    wdgTrips *WDGTRIPS = new wdgTrips();
    wdgNotifications *WDGNOTIFICATIONS = new wdgNotifications();

    OTNetworkConnection nc;

    void createDockWidgets();
};

#endif // WLFCLIENTPARTICIPANT_H
