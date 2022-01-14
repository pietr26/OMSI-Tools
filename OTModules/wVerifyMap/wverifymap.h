#ifndef WVERIFYMAP_H
#define WVERIFYMAP_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"
#include "OTBackend/OTOmsiFileHandler.h"
#include "wsettings.h"
#include "dignorelist.h"
#include "OTModules/wContentSearch/wcontentsearch.h"
#include <QFileDialog>
#include <QProgressDialog>
#include <QTimer>
#include <QClipboard>
#include <QDebug>
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

    void on_tbnMapPath_clicked();

    void on_ledMapPath_textChanged(const QString &arg1);

    void on_btnStartVerifying_clicked();

    void on_actionSettings_triggered();

    void on_btnTilesDetails_clicked();

    void on_btnObjectsDetails_clicked();

    void on_btnSplinesDetails_clicked();

    void on_btnVehiclesDetails_clicked();

    void on_btnHumansDetails_clicked();

    void on_actionEditIgnorelist_triggered();

    void on_actionStartVerifying_triggered();

    void on_btnTexturesDetails_clicked();

    void on_actionAdjustWindowSize_triggered();

    void reloadProgress();

    void on_cbxAdvancedVerifying_stateChanged(int arg1);

    void on_actionTilesCopy_triggered();

    void on_actionTexturesCopy_triggered();

    void on_actionObjectsCopy_triggered();

    void on_actionSplinesCopy_triggered();

    void on_actionVehiclesCopy_triggered();

    void on_actionHumansCopy_triggered();

    void on_tbnTilesTools_clicked();

    void on_tbnTexturesTools_clicked();

    void on_tbnObjectsTools_clicked();

    void on_tbnSplinesTools_clicked();

    void on_tbnVehiclesTools_clicked();

    void on_tbnHumansTools_clicked();

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

    void on_actionTilesSearch_triggered();

    void on_actionTexturesSearch_triggered();

    void on_actionObjectsSearch_triggered();

    void on_actionSplinesSearch_triggered();

    void on_actionVehiclesSearch_triggered();

    void on_actionHumansSearch_triggered();

    void on_actionTilesIgnore_triggered();

    void on_actionTexturesIgnore_triggered();

    void on_actionObjectsIgnore_triggered();

    void on_actionSplinesIgnore_triggered();

    void on_actionVehiclesIgnore_triggered();

    void on_actionHumansIgnore_triggered();

    void on_btnShowSettings_clicked();

    void on_cbxOnlyMapTextures_stateChanged(int arg1);

    void ignoreListItems();

    void searchListItems();

    void copyListItems();

private:
    const QString moduleName = "wVerifyMap";
    Ui::wVerifyMap *ui;
    OTMessage msg;
    OTSettings set;
    OTMiscellaneous misc;
    wSettings *WSETTINGS;
    wContentSearch *WCONTENTSEARCH;
    int cutCount = set.read("main", "mainDir").toString().count() + 1;

    void selectAllAndClear();

    void getIgnoreList();

    void toolButtonSetup();

    void writeIgnoreList(QString line);

    QStringList checkForIgnored(QStringList checkList, int &ignoreCount);

    dIgnoreList *DIGNORELIST;

    int ignoredTiles = 0;
    int ignoredTextures = 0;
    int ignoredObjects = 0;
    int ignoredSplines = 0;
    int ignoredHumans = 0;
    int ignoredVehicles = 0;
    QStringList ignoreList;
    OTOMSIFileHandler filehandler;

    QTimer *watchProgress = new QTimer();

    void startEndWatchProgress(bool state);

    void endVerifying();

    void setDetailButtons();

    void copy(QString copytext);

    void enableIgnoreLabels(bool enable);

};

#endif // WVERIFYMAP_H
