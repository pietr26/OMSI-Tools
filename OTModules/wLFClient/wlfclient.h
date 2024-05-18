#ifndef WLFCLIENT_H
#define WLFCLIENT_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"
#include "OTModules/OTGeneric/wfeedback.h"
#include "OTModules/OTGeneric/wpreferences.h"

namespace Ui {
class wLFClient;
}

class wLFClient : public QMainWindow
{
    Q_OBJECT

public:
    explicit wLFClient(QWidget *parent = nullptr);
    ~wLFClient();

private slots:
    void on_actionBackToHome_triggered();

    void on_actionClose_triggered();

    void on_actionPreferences_triggered();

    void on_actionHelp_triggered();

signals:
    void backToHome();

private:
    Ui::wLFClient *ui;
    OTSettings set;
    wPreferences *WPREFERENCES;
};

#endif // WLFCLIENT_H
