#ifndef WPROJECTMANAGEMENT_H
#define WPROJECTMANAGEMENT_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"
#include "OTBackend/OTExternal.h"
#include "OTModules/OTGeneric/wpreferences.h"
#include "OTModules/OTGeneric/wpromptselector.h"
#include "wprojectpreferences.h"
#include "wmanagefiles.h"
#include "remote/wdownloadproject.h"
#include "wcreateproject.h"
#include <QTableWidgetItem>
#include <QStandardItemModel>

namespace Ui {
class wProjectManagement;
}

class wProjectManagement : public QMainWindow
{
    Q_OBJECT

public:
    explicit wProjectManagement(QWidget *parent = nullptr);
    ~wProjectManagement();

private slots:
    void on_actionBackToHome_triggered();

    void on_actionClose_triggered();

    void on_btnPullDownload_clicked();

    void on_btnFetchDownload_clicked();

    void on_btnCommitSave_clicked();

    void on_btnPushUpload_clicked();

    void on_actionOpen_triggered();

    void on_btnStatusGetStatus_clicked();

    void on_btnDe_selectAll_clicked();

    void on_twgStatus_itemClicked(QTableWidgetItem *item);

    void on_twgStatus_itemDoubleClicked(QTableWidgetItem *item);

    void on_actionPreferences_triggered();

    void on_actionProjectSettings_triggered();

    void on_actionNewProject_triggered();

    void on_btnTest_clicked();

    void on_btnManageFiles_clicked();

    void on_actionDownloadProject_triggered();

    void on_actionUploadProject_triggered();

    void on_actionCloseProject_triggered();

public slots:
    void reloadDownloadedProject(QString path);

    void recieveNewProject(QString path);

    void loadProject();

signals:
    void backToHome();

private:
    Ui::wProjectManagement *ui;
    OTSettings set;
    OTGit git;

    wPreferences *WPREFERENCES;
    wProjectPreferences *WPROJECTPREFERENCES;
    wCreateProject *WCREATEPROJECT;
    wPromptSelector *WPROMPTSELECTOR;
    wDownloadProject *WDOWNLOADPROJECT;
    wManageFiles *WMANAGEFILES;

    void selectProjectFolder(bool newProject);

    void getGitStatus();

    QStandardItemModel *modelStatus;

    bool isAllSelected = false;

    QFileSystemWatcher *watcher;

    QTimer *fileWatcherTimer = new QTimer(this);

    void restartFileWatcherTimer();

    void enableUi(bool enable);

    void clearUi();

    bool checkFolderStatus(bool newProject);
};

#endif // WPROJECTMANAGEMENT_H
