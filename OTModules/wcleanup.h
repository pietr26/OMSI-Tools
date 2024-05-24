#ifndef WCLEANUP_H
#define WCLEANUP_H

#include <QMainWindow>
#include <QtConcurrent>
#include "OTModules/OTGeneric/wpreferences.h"
#include "OTBackend/OTGlobal.h"
#include "OTBackend/OTOmsiFileHandler.h"
#include "OTModules/OTGeneric/wfeedback.h"
#include <QListWidgetItem>

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

    void on_actionPreferences_triggered();

    void on_actionSendFeedback_triggered();

    void on_actionBackToHome_triggered();

    void on_lwgObjects_itemDoubleClicked(QListWidgetItem *item);

    void on_lwgSplines_itemDoubleClicked(QListWidgetItem *item);

    void on_lwgVehicles_itemDoubleClicked(QListWidgetItem *item);

    void on_actionBulkMarkInCurrentList_triggered();

signals:
    void backToHome();

private:
    Ui::wCleanup *ui;

    int cutCount = 0;

    OTSettings set;
    OTMessage msg;
    OTOMSIFileHandler filehandler;
    OTMiscellaneous misc;
    wPreferences *WPREFERENCES;
};

#endif // WCLEANUP_H
