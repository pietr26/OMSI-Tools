#ifndef WVERIFYMAP_H
#define WVERIFYMAP_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"
#include "OTBackend/OTOmsiFileHandler.h"
#include "OTWidgets/verifymaptools.h"
#include "OTModules/OTGeneric/wpreferences.h"
#include "OTModules/OTGeneric/wfeedback.h"
#include <QFileDialog>
#include <QProgressDialog>
#include <QTimer>
#include <QClipboard>
#include <QtConcurrent>
#include <QFuture>
#include <QListWidgetItem>
#include <QMouseEvent>
#include <QDrag>

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

    void on_btnTilesDetails_clicked();

    void on_btnObjectsDetails_clicked();

    void on_btnSplinesDetails_clicked();

    void on_btnVehiclesDetails_clicked();

    void on_btnHumansDetails_clicked();

    void on_actionStartVerifying_triggered();

    void on_btnTexturesDetails_clicked();

    void on_actionAdjustWindowSize_triggered();

    void reloadProgress();

    void on_lwgTilesAll_itemDoubleClicked(QListWidgetItem *item);

    void on_lwgTilesMissing_itemDoubleClicked(QListWidgetItem *item);

    void on_lwgTexturesAll_itemDoubleClicked(QListWidgetItem *item);

    void on_lwgTexturesMissing_itemDoubleClicked(QListWidgetItem *item);

    void on_lwgObjectsAll_itemDoubleClicked(QListWidgetItem *item);

    void on_lwgObjectsMissing_itemDoubleClicked(QListWidgetItem *item);

    void on_lwgSplinesAll_itemDoubleClicked(QListWidgetItem *item);

    void on_lwgSplinesMissing_itemDoubleClicked(QListWidgetItem *item);

    void on_lwgVehiclesAll_itemDoubleClicked(QListWidgetItem *item);

    void on_lwgVehiclesMissing_itemDoubleClicked(QListWidgetItem *item);

    void on_lwgHumansAll_itemDoubleClicked(QListWidgetItem *item);

    void on_lwgHumansMissing_itemDoubleClicked(QListWidgetItem *item);

    void on_actionSendFeedback_triggered();

    void on_cobxMapName_currentIndexChanged(int index);

    void on_btnReloadMaps_clicked();

    void on_actionBackToHome_triggered();

    void on_btnVerificationPreferences_clicked();

signals:
    void backToHome();

private:
    Ui::wVerifyMap *ui;
    OTMessage msg;
    OTSettings set;
    OTMiscellaneous misc;
    wContentSearch *WCONTENTSEARCH;
    wPreferences *WPREFERENCES;

    int cutCount = set.read("main", "mainDir").toString().size() + 1;

    void selectAllAndClear();

    OTOMSIFileHandler filehandler;

    QTimer *watchProgress = new QTimer();

    void startEndWatchProgress(bool state);

    void endVerifying();

    void setDetailButtons();

    void loadMapList();

    QList<QPair<QString, QString>> mapList;

    bool mapListSetupFinished = false;
    void enableView(bool enable);
};

#endif // WVERIFYMAP_H
