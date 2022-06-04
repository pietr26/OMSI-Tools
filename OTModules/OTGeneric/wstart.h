#ifndef WSTART_H
#define WSTART_H

#include <QMainWindow>
#include <QCloseEvent>
#include <OTModules/wcleanup.h>
#include "OTModules/OTGeneric/wsettings.h"
#include "OTModules/wfonts.h"
#include "OTModules/wVerifyMap/wverifymap.h"
#include "OTBackend/OTGlobal.h"
#include "OTModules/OTGeneric/wabout.h"
#include "OTModules/OTInternals/wdevtools.h"
#include "OTModules/wContentSearch/wcontentsearch.h"
#include "OTModules/wContentSearch/wDBPanel.h"
#include "OTModules/OTGeneric/wreleasenotes.h"
#include "OTModules/wFbh/wfbh.h"
#include "OTModules/OTInternals/wstyletest.h"
#include "OTModules/OTGeneric/wfeedback.h"

QT_BEGIN_NAMESPACE
namespace Ui { class wStart; }
QT_END_NAMESPACE

class wStart : public QMainWindow
{
    Q_OBJECT

public:
    wStart(QWidget *parent = nullptr);
    ~wStart();

private slots:
    void on_actionAbout_triggered();

    void on_actionSettings_triggered();

    void on_actionClose_triggered();

    void on_btnFonts_clicked();

    void on_btnWhereShouldIReleaseMyModification_clicked();

    void on_btnVerifyMap_clicked();

    void on_btnDevTools_clicked();

    void on_btnDBPanel_clicked();

    void on_btnContentSearch_clicked();

    void on_btnSimulateCrash_clicked();

    void reopenTest(QObject*);

    void on_actionSendFeedback_triggered();

    void on_actionManual_triggered();

    void on_btnOpenFbh_clicked();

    void on_btnCreateShortcut_clicked();

    void on_btnStyleTest_clicked();

    void on_btnView_clicked();

    void on_actionSourceCodeOnGitHub_triggered();

    void on_actionReleaseNotes_triggered();

    void on_actionRestart_triggered();

    void on_btnCleanup_clicked();

private:
    const QString moduleName = "wStart";
    Ui::wStart *ui;
    wSettings *WSETTINGS;
    wFonts *WFONTS;
    wVerifyMap *WVERIFYMAP;
    wDevTools *WDEVTOOLS;
    wDBPanel *WDBPANEL;
    wContentSearch *WCONTENTSEARCH;
    wReleaseNotes *WRELEASENOTES;
    wAbout *WABOUT;
    wFbh *WFBH;
    wStyleTest *WSTYLETEST;
    wFeedback *WFEEDBACK;
    wCleanup *WCLEANUP;

    OTSettings set;
    OTMessage msg;
    OTMiscellaneous misc;
    OTFileOperations fop;

    QString updateVersion;
    void startCounterMsgSender();
};

#endif // WSTART_H
