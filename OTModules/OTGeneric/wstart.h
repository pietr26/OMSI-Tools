#ifndef WSTART_H
#define WSTART_H

#include <QMainWindow>
#include "OTModules/wCleanup/wcleanup.h"
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

    void on_lblUpdate_linkActivated(const QString &link);

private:
    Ui::wStart *ui;
    wPreferences *WPREFERENCES = nullptr;
    wFonts *WFONTS = nullptr;
    wVerifyMap *WVERIFYMAP = nullptr;
    wDevTools *WDEVTOOLS = nullptr;
    wRegEditor *WREGEDITOR = nullptr;
    wDBPanel *WDBPANEL = nullptr;
    wContentSearch *WCONTENTSEARCH = nullptr;
    wReleaseNotes *WRELEASENOTES = nullptr;
    wAbout *WABOUT = nullptr;
    wFeedback *WFEEDBACK = nullptr;
    wCleanup *WCLEANUP = nullptr;
    wDBCopyrights *WDBCOPYRIGHTS = nullptr;
    wMaps *WMAPS = nullptr;
    wBugDoc *WBUGDOC = nullptr;
    wDBKnownWords *WDBKNOWNWORDS = nullptr;
    OTUpdater *updater = new OTUpdater();

    OTSettings set;
    OTMessage msg;
    OTMiscellaneous misc;
    OTFileOperations fop;
    OTNetworkConnection nc;

    bool checkMainDir();

    /*!
        Shows a module window, comes back to this window when the module is done, and
        lets the module free itself afterwards.

        The modules used to stay alive forever: created without a parent, closed without
        WA_DeleteOnClose, and forgotten as soon as the next click overwrote the pointer.
        Every switch back and forth cost a whole window including its models.

        deleteLater() is safe here: every module emits backToHome from one of its own
        slots, and Qt only carries out a deferred deletion in the event loop the call was
        made from - never in a nested one - so the emitting function has always returned
        by then. The three modules which emit before their close() are fine for the same
        reason.
    */
    template<class T>
    void showModule(T *&window)
    {
        connect(window, &T::backToHome, this, [this, &window]()
        {
            reopen();

            window->deleteLater();
            window = nullptr;
        });

        window->show();
        close();
    }

    void loadMessagesOld();
    void loadMessages();
    void checkForUpdates();

    QString updateVersion;
    QPair<int, QString> updateInformation;
};

#endif // WSTART_H
