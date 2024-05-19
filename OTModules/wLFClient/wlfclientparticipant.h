#ifndef WLFCLIENTPARTICIPANT_H
#define WLFCLIENTPARTICIPANT_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"
#include "OTModules/OTGeneric/wfeedback.h"
#include "OTModules/OTGeneric/wpreferences.h"

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

signals:
    void backToHome();

private:
    Ui::wLFClientParticipant *ui;
    OTSettings set;
    wPreferences *WPREFERENCES;
};

#endif // WLFCLIENTPARTICIPANT_H
