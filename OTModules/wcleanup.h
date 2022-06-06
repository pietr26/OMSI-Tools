#ifndef WCLEANUP_H
#define WCLEANUP_H

#include <QMainWindow>
#include <OTModules/OTGeneric/wsettings.h>
#include "OTBackend/OTGlobal.h"
#include "OTBackend/OTOmsiFileHandler.h"
#include <OTModules/OTGeneric/wfeedback.h>

namespace Ui {
class wCleanup;
}

class wCleanup : public QMainWindow
{
    Q_OBJECT

public:
    explicit wCleanup(QWidget *parent = nullptr);
    ~wCleanup();

private slots:

    void on_btnAnalyze_clicked();

    void on_btnStartAction_clicked();

    void on_rbtnMoveToFolder_clicked();

    void on_rbtnDelete_clicked();

    void on_actionAnalyze_triggered();

    void on_actionClose_triggered();

    void on_actionSettings_triggered();

    void on_actionSendFeedback_triggered();

private:
    Ui::wCleanup *ui;
    const QString moduleName = "wCleanup";

    int cutCount = 0;

    OTSettings set;
    OTOMSIFileHandler filehandler;
    wSettings *WSETTINGS;
};

#endif // WCLEANUP_H
