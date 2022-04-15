#ifndef WSETTINGS_H
#define WSETTINGS_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"
#include "OTModules/OTInternals/wdevtools.h"
#include "OTModules/OTGeneric/wreleasenotes.h"
#include <QStandardItemModel>

namespace Ui {
class wSettings;
}

class wSettings : public QMainWindow
{
    Q_OBJECT

public:
    explicit wSettings(QWidget *parent = nullptr, QString openDirect = "");
    ~wSettings();

private slots:

    void closeEvent (QCloseEvent *event);

    void on_btnDeleteAllBackups_clicked();

    void on_btnOpenBackupFolder_clicked();

    void on_cobxTheme_currentIndexChanged(int index);

    void refreshDiskUsage();

    void on_btnCheckForUpdates_clicked();

    void on_btnResetSettings_clicked();

    void on_btnCreateDesktopShortcut_clicked();

    void on_btnClose_clicked();

    void on_btnRestart_clicked();

    void on_cobxLanguage_currentIndexChanged(int index);

    void on_btnOmsiPath_clicked();

    void on_sbxAutosaveDuration_valueChanged(int arg1);

    void on_cbxConfirmDeletion_clicked();

    void on_ledAuthor_textChanged(const QString &arg1);

    void on_gbxAutosave_clicked();

    void on_btnOpenLogfilePath_clicked();

    void on_btnMoreLanguages_clicked();

    void on_cobxAutoUpdateCheck_currentIndexChanged(int index);

    void on_cobxLogfileMode_currentIndexChanged(int index);

private:
    const QString moduleName = "wSettings";
    Ui::wSettings *ui;
    OTSettings set;
    OTMessage msg;
    OTMiscellaneous misc;
    OTFileOperations fop;
    OTDiskUsage dUs;
    QTimer *timer;
    wDevTools *WDEVTOOLS;
    wReleaseNotes *WRELEASENOTES;

    bool setupFinished = false;

    void setUnsaved(bool visible);
};

#endif // WSETTINGS_H
