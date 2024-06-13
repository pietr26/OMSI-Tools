#ifndef WSTART_H
#define WSTART_H

#include <QMainWindow>
#include "OTModules/wcleanup.h"
#include "OTModules/OTGeneric/wpreferences.h"
#include "OTModules/OTGeneric/wfeedback.h"
#include "OTModules/OTGeneric/wreleasenotes.h"
#include "OTModules/wFonts/wfonts.h"
#include "OTModules/wVerifyMap/wverifymap.h"
#include "OTBackend/OTGlobal.h"
#include "OTModules/OTGeneric/wabout.h"
#include "OTModules/OTInternals/wdevtools.h"
#include "OTModules/OTInternals/wregeditor.h"
#include "OTModules/wContentSearch/wcontentsearch.h"
#include "OTModules/wContentSearch/wDBPanel.h"
#include "OTModules/wFbh/wdbknownwords.h"
#include "OTWidgets/fadeinouttext.h"
#include "OTWidgets/message.h"
#include "OTModules/wFbh/wdbcopyrights.h"
#include "OTModules/wMaps/wmaps.h"
#include "OTModules/wBugDoc/wbugdoc.h"
#include "OTModules/wLFClient/wlfclientparticipant.h"
#include "OTModules/wLFClient/wlfclientdispatcher.h"

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

    void on_actionSendFeedback_triggered();

    void on_actionManual_triggered();

    void on_actionSourceCodeOnGitHub_triggered();

    void on_actionReleaseNotes_triggered();

    void on_actionRestart_triggered();

    void on_actionCheckForUpdates_triggered();

    void on_btnReloadMessages_clicked();

    void on_lwgMessages_itemDoubleClicked(QListWidgetItem *item);

    void on_tbnFonts_clicked();

    void on_tbnContentSearch_clicked();

    void on_tbnCleanup_clicked();

    void reopen() { show(); DiscordGameSDK::clearActivity(); }

    void on_tbnMaps_clicked();

    void on_tbnMapVerification_clicked();

    void on_actionDashboard_triggered();

    void on_actionApplicationCrashSimulation_triggered();

    void on_actionRegEditor_triggered();

    void on_actionBugDoc_triggered();

    void on_actionDBPanelContentSearch_triggered();

    void on_actionDBPanelWDTFbh_triggered();

    void on_actionDBPanelKnownWords_triggered();

    void on_actionDirectXTex_triggered();

    void on_actionRoadmap_triggered();

    void on_actionAboutQt_triggered();

    void on_tbnLFClientDispatcher_clicked();

    void on_tbnLFClientParticipant_clicked();

    void on_lblUpdate_linkActivated(const QString &link);

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
    wFeedback *WFEEDBACK;
    wCleanup *WCLEANUP;
    wDBCopyrights *WDBCOPYRIGHTS;
    wMaps *WMAPS;
    wBugDoc *WBUGDOC;
    wDBKnownWords *WDBKNOWNWORDS;
    wLFClientParticipant *WLFCLIENTPARTICIPANT;
    wLFClientDispatcher *WLFCLIENTDISPATCHER;
    OTUpdater *updater = new OTUpdater();

    OTSettings set;
    OTMessage msg;
    OTMiscellaneous misc;
    OTFileOperations fop;
    OTNetworkConnection nc;

    bool checkMainDir();

    void loadMessagesOld();
    void loadMessages();
    void checkForUpdates();

    QString updateVersion;
    QPair<int, QString> updateInformation;
};

#endif // WSTART_H
