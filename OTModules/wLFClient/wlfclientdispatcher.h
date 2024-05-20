#ifndef WLFCLIENTDISPATCHER_H
#define WLFCLIENTDISPATCHER_H

#include <QMainWindow>
#include <QDockWidget>
#include "OTBackend/OTGlobal.h"
#include "OTModules/OTGeneric/wfeedback.h"
#include "OTModules/OTGeneric/wpreferences.h"
#include "Widgets/wdginformation.h"
#include "OTModules/wLFClient/wlogin.h"

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

signals:
    void backToHome();

private:
    Ui::wLFClientDispatcher *ui;
    OTSettings set;
    wPreferences *WPREFERENCES;
    wLogin *WLOGIN;
    wdgInformation *WDGINFORMATION = new wdgInformation();

    OTNetworkConnection nc;

    void createDockWidgets();
};

#endif // WLFCLIENTDISPATCHER_H
