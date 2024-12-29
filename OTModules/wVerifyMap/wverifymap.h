#ifndef WVERIFYMAP_H
#define WVERIFYMAP_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"
#include "OTBackend/OTOmsiFileHandler.h"
#include "OTModules/OTGeneric/wpreferences.h"
#include "OTModules/OTGeneric/wfeedback.h"
#include "OTBackend/OCC/OCFS.h"
#include "OTBackend/OTMapScanner.h"
#include <QListWidgetItem>
#include <QKeySequence>
#include <QKeyEvent>

namespace Ui {
class wVerifyMap;
}

class wVerifyMap : public QMainWindow
{
    Q_OBJECT

public:
    explicit wVerifyMap(QWidget *parent = nullptr);
    ~wVerifyMap();

private slots:

    void on_actionClose_triggered();

    void on_btnStartVerifying_clicked();

    void on_actionPreferences_triggered();

    void on_actionStartVerifying_triggered();

    void on_actionSendFeedback_triggered();

    void on_cobxMapName_currentIndexChanged(int index);

    void on_btnReloadMaps_clicked();

    void on_actionBackToHome_triggered();

    void on_btnVerificationPreferences_clicked();

    void onScannerFinished();
    void onCheckerFinished();
    void onStatusUpdate(int, QString);

signals:
    void backToHome();

private:
    Ui::wVerifyMap *ui;
    OTMessage msg;
    OTSettings set;
    wPreferences *WPREFERENCES;

    int cutCount = set.read("main", "mainDir").toString().size() + 1;

    void selectAllAndClear();

    OTOMSIFileHandler filehandler;

    void loadMapList();

    QList<QPair<QString, QString>> mapList;

    bool mapListSetupFinished = false;
    void enableView(bool enable);

    OTMapScanner *_scanner;
    OTMapChecker *_checker;

};

#endif // WVERIFYMAP_H
