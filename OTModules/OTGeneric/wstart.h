#ifndef WSTART_H
#define WSTART_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QFutureWatcher>
#include "OTModules/wcleanup.h"
#include "OTModules/OTGeneric/wpreferences.h"
#include "OTModules/OTGeneric/wfeedback.h"
#include "OTModules/OTGeneric/wreleasenotes.h"
#include "OTModules/wfonts.h"
#include "OTModules/wVerifyMap/wverifymap.h"
#include "OTBackend/OTGlobal.h"
#include "OTModules/OTGeneric/wabout.h"
#include "OTModules/OTInternals/wdevtools.h"
#include "OTModules/OTInternals/wstyletest.h"
#include "OTModules/OTInternals/wregeditor.h"
#include "OTModules/wContentSearch/wcontentsearch.h"
#include "OTModules/wContentSearch/wDBPanel.h"
#include "OTModules/wFbh/wfbh.h"
#include "OTWidgets/fadeinouttext.h"
#include "OTWidgets/message.h"
#include "OTModules/wFbh/wdbcopyrights.h"
#include "OTModules/wMaps/wmaps.h"
#include "OTModules/wBugDoc/wbugdoc.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

namespace Ui {
class wStart;
}

class wStart : public QMainWindow
{
    Q_OBJECT

public:
    wStart(QWidget *parent = nullptr);
    ~wStart();

private slots:
    void on_actionAbout_triggered();

    void on_actionPreferences_triggered();

    void on_actionClose_triggered();

    void on_btnDBPanel_clicked();

    void reopenTest(QObject*);

    void on_actionSendFeedback_triggered();

    void on_actionManual_triggered();

    void on_actionSourceCodeOnGitHub_triggered();

    void on_actionReleaseNotes_triggered();

    void on_actionRestart_triggered();

    void on_actionCheckForUpdates_triggered();

    void on_btnReloadMessages_clicked();

    void on_lwgMessages_itemDoubleClicked(QListWidgetItem *item);

    void on_btnFbhDBPanel_clicked();

    void on_tbnFonts_clicked();

    void on_tbnContentSearch_clicked();

    void on_tbnCleanup_clicked();

    void on_tbnFbhOpen_clicked();

    void on_tbnFbhCreateShortcut_clicked();

    void reopen() { show(); }

    void on_tbnMaps_clicked();

    void on_tbnMapVerification_clicked();

    void on_actionDashboard_triggered();

    void on_actionApplicationCrashSimulation_triggered();

    void on_actionStyleTest_triggered();

    void on_actionRegEditor_triggered();

    void on_btnBugDoc_clicked();

private:
    Ui::wStart *ui;
    wPreferences *WPREFERENCES;
    wFonts *WFONTS;
    wVerifyMap *WVERIFYMAP;
    wDevTools *WDEVTOOLS;
    wRegEditor *WREGEDITOR;
    wDBPanel *WDBPANEL;
    wContentSearch *WCONTENTSEARCH;
    wReleaseNotes *WRELEASENOTES;
    wAbout *WABOUT;
    wFbh *WFBH;
    wStyleTest *WSTYLETEST;
    wFeedback *WFEEDBACK;
    wCleanup *WCLEANUP;
    wDBCopyrights *WDBCOPYRIGHTS;
    wMaps *WMAPS;
    wBugDoc *WBUGDOC;

    OTSettings set;
    OTMessage msg;
    OTMiscellaneous misc;
    OTFileOperations fop;
    OTNetworkConnection nc;

    void startCounterMsgSender();
    bool checkMainDir();

    void loadMessagesOld();
    void loadMessages();
};

#endif // WSTART_H
