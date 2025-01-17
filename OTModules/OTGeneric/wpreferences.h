#ifndef WPREFERENCES_H
#define WPREFERENCES_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"
#include "OTModules/OTInternals/wdevtools.h"
#include "OTModules/OTGeneric/wreleasenotes.h"
#include "OTModules/OTGeneric/wfeedback.h"
#include <QStandardItemModel>
#include <QListWidgetItem>

namespace Ui {
class wPreferences;
}

class wPreferences : public QMainWindow
{
    Q_OBJECT

public:
    explicit wPreferences(QWidget *parent = nullptr, QString openDirect = "");
    ~wPreferences();

public slots:
    void on_btnCheckForUpdates_clicked();

private slots:

    void closeEvent (QCloseEvent *event);

    void on_btnDeleteAllBackups_clicked();

    void on_btnOpenBackupFolder_clicked();

    void refreshDiskUsage();

    void on_btnResetPreferences_clicked();

    void on_btnClose_clicked();

    void on_cobxLanguage_currentIndexChanged(int index);

    void on_btnOmsiPath_clicked();

    void on_sbxAutosaveDuration_valueChanged(int arg1);

    void on_ledAuthor_textChanged(const QString &arg1);

    void on_btnOpenLogfilePath_clicked();

    void on_btnMoreLanguages_clicked();

    void on_cobxLogfileMode_currentIndexChanged(int index);

    void on_cbxBackupEnabled_clicked(bool checked);

    void on_actionSendFeedback_triggered();

    void on_cbxAdvancedVerifying_stateChanged(int arg1);

    void on_btnSave_clicked();

    void on_cbxShowNews_stateChanged(int arg1);

    void on_cbxKeepPixelRow_stateChanged(int arg1);

    void on_btnDevToolsPrefs_clicked();

    void on_lwgSections_currentRowChanged(int currentRow);

    void on_actionClose_triggered();

    void on_cobxTheme_currentIndexChanged(int index);

    void on_cbxDiscordGameSDK_stateChanged(int arg1);

private:
    Ui::wPreferences *ui;
    OTSettings set;
    OTMessage msg;
    OTMiscellaneous misc;
    OTUpdater *updater = new OTUpdater();
    OTFileOperations fop;
    OTDiskUsage dUs;
    OTStrings strings;
    QTimer *timer;
    wDevTools *WDEVTOOLS;
    wReleaseNotes *WRELEASENOTES;
    wFeedback *WFEEDBACK;

    void reloadThemePreview();

    void loadPreferences();

    void savePreferences();

    void modified();

    bool setupFinished = false;

    bool needRestart = false;

    QList<QPair<QString, QString>> themes = strings.getThemeNames();
};

#endif // WPREFERENCES_H
