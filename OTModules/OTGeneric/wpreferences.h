#ifndef WPREFERENCES_H
#define WPREFERENCES_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"
#include "OTModules/OTInternals/wdevtools.h"
#include "OTModules/OTGeneric/wreleasenotes.h"
#include "OTModules/OTGeneric/wfeedback.h"
#include <QStandardItemModel>
#include <QColorDialog>

namespace Ui {
class wPreferences;
}

class wPreferences : public QMainWindow
{
    Q_OBJECT

public:
    explicit wPreferences(QWidget *parent = nullptr, QString openDirect = "");
    ~wPreferences();

private slots:

    void closeEvent (QCloseEvent *event);

    void on_btnDeleteAllBackups_clicked();

    void on_btnOpenBackupFolder_clicked();

    void refreshDiskUsage();

    void on_btnCheckForUpdates_clicked();

    void on_btnResetPreferences_clicked();

    void on_btnCreateDesktopShortcut_clicked();

    void on_btnClose_clicked();

    void on_btnRestart_clicked();

    void on_cobxLanguage_currentIndexChanged(int index);

    void on_btnOmsiPath_clicked();

    void on_sbxAutosaveDuration_valueChanged(int arg1);

    void on_ledAuthor_textChanged(const QString &arg1);

    void on_btnOpenLogfilePath_clicked();

    void on_btnMoreLanguages_clicked();

    void on_cobxAutoUpdateCheck_currentIndexChanged(int index);

    void on_cobxLogfileMode_currentIndexChanged(int index);

    void on_btnThemeMain_clicked();

    void on_btnThemeMainSC_clicked();

    void on_btnThemeDis_clicked();

    void on_btnThemeDisD_clicked();

    void on_btnThemeAcc1_clicked();

    void on_btnThemeAcc2_clicked();

    void on_btnThemeAcc3_clicked();

    void on_btnLoadTheme_clicked();

    void on_btnThemeButton_clicked();

    void on_cbxBackupEnabled_clicked(bool checked);

    void on_actionSendFeedback_triggered();

    void on_cbxAdvancedVerifying_stateChanged(int arg1);

    void on_cbxOnlyMapTextures_stateChanged(int arg1);

    void on_lwgSections_currentRowChanged(int currentRow);

    void on_btnSave_clicked();

    void on_btnUseCustomTheme_clicked();

private:
    Ui::wPreferences *ui;
    OTSettings set;
    OTMessage msg;
    OTMiscellaneous misc;
    OTFileOperations fop;
    OTDiskUsage dUs;
    QTimer *timer;
    wDevTools *WDEVTOOLS;
    wReleaseNotes *WRELEASENOTES;
    wFeedback *WFEEDBACK;

    void reloadThemePreview();

    void loadSettings();

    void saveSettings();

    void modified();

    QString tcMain;
    QString tcMainSC;
    QString tcDis;
    QString tcDisD;
    QString tcAcc1;
    QString tcAcc2;
    QString tcAcc3;
    QString tcButton;
    bool useStandardTheme = false;

    bool isFirstSetup = true;

    bool needRestart = false;
};

#endif // WPREFERENCES_H
