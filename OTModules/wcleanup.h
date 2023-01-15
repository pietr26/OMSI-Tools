#ifndef WCLEANUP_H
#define WCLEANUP_H

#include <QMainWindow>
#include <QtConcurrent>
#include "OTModules/OTGeneric/wsettings.h"
#include "OTBackend/OTGlobal.h"
#include "OTBackend/OTOmsiFileHandler.h"
#include "OTModules/OTGeneric/wfeedback.h"

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

    void on_actionBackToHome_triggered();

signals:
    void backToHome();

private:
    Ui::wCleanup *ui;

    int cutCount = 0;

    OTSettings set;
    OTMessage msg;
    OTOMSIFileHandler filehandler;
    wSettings *WSETTINGS;
};

#endif // WCLEANUP_H
