#ifndef WSTART_H
#define WSTART_H

#include <QMainWindow>
#include "wsettings.h"
#include "OTModules/wfonts.h"
#include "OTModules/wVerifyMap/wverifymap.h"
#include "OTBackend/OTGlobal.h"
#include "OTModules/wVehicleList/wvehiclelist.h"
#include "wabout.h"
#include "OTModules/wdevtools.h"
#include "OTModules/wContentSearch/wcontentsearch.h"
#include "OTModules/wContentSearch/wDBPanel.h"
#include "wchangelog.h"
#include "wfirstsetup.h"
#include <QMessageBox>
#include <QDebug>
#include <QTimer>
#include <QCloseEvent>
#include <OTModules/wFbh/wfbh.h>

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

    void on_btnVehicleList_clicked();

    void on_btnDevTools_clicked();

    void on_btnDBPanel_clicked();

    void on_btnContentSearch_clicked();

    void on_btnSimulateCrash_clicked();

    void reopenTest(QObject*);

    void on_actionSendFeedback_triggered();

    void on_actionChangelog_triggered();

    void on_actionManual_triggered();

    void on_btnOpenFbh_clicked();

    void on_btnCreateShortcut_clicked();

private:
    const QString moduleName = "wStart";
    Ui::wStart *ui;
    wSettings *WSETTINGS;
    wFonts *WFONTS;
    wVerifyMap *WVERIFYMAP;
    wVehicleList *WVEHICLELIST;
    wDevTools *WDEVTOOLS;
    wDBPanel *WDBPANEL;
    wContentSearch *WCONTENTSEARCH;
    wChangelog *WCHANGELOG;
    wFirstSetup *WFIRSTSETUP;
    wAbout *WABOUT;
    wFbh *WFBH;

    OTSettings set;
    OTMessage msg;
    OTMiscellaneous misc;

};

#endif // WSTART_H
