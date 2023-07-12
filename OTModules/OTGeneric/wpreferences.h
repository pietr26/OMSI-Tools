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

    void on_btnThemeBackground_clicked();

    void on_btnThemeFontDisabled_clicked();

    void on_btnThemeBackgroundDisabled_clicked();

    void on_btnThemeBorders_clicked();

    void on_btnThemeAccent_clicked();

    void on_btnThemeFont_clicked();

    void on_btnLoadTheme_clicked();

    void on_btnThemeInputs_clicked();

    void on_cbxBackupEnabled_clicked(bool checked);

    void on_actionSendFeedback_triggered();

    void on_cbxAdvancedVerifying_stateChanged(int arg1);

    void on_cbxOnlyMapTextures_stateChanged(int arg1);

    void on_lwgSections_currentRowChanged(int currentRow);

    void on_btnSave_clicked();

    void on_btnUseCustomTheme_clicked();

    void on_cbxShowNews_stateChanged(int arg1);

    void on_cbxKeepPixelRow_stateChanged(int arg1);

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

    void loadPreferences();

    void savePreferences();

    void modified();

    QString tcBackground;
    QString tcFontDisabled;
    QString tcBackgroundDisabled;
    QString tcBorders;
    QString tcAccent;
    QString tcFont;
    QString tcInputs;
    bool useStandardTheme = false;

    bool isFirstSetup = true;

    bool needRestart = false;
};

#endif // WPREFERENCES_H
