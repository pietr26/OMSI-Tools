#include "wstart.h"
#include "ui_wstart.h"

wStart::wStart(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::wStart)
{
    qInfo().noquote() << "Starting " + objectName() + "...";

    qDebug() << "Set up UI...";
    ui->setupUi(this);
    qDebug() << "UI set";

    // Load settings
    setStyleSheet(set.read("main", "theme").toString());

    setWindowTitle(OTName + " " + OTVersion);

#ifdef QT_DEBUG
    setWindowTitle("[Debug Build] " + windowTitle());
#endif

    // Prepare GUI:
    bool isNoRelease;

    if (OTBuild == OTBuildOptions::Release || OTBuild == OTBuildOptions::EA ||
        OTBuild == OTBuildOptions::Lite || OTBuild == OTBuildOptions::Prerelease)
        isNoRelease = false;
    else
        isNoRelease = true;

    /// Funktioniert so semi. Lambda wird zwar im Thread ausgeführt, dennoch funktioniert das Connect fürn progress nicht (mehr).
    OTDownloader dl;


//    QFutureWatcher<QString> *fWatcher = new QFutureWatcher<QString>();

//    connect(fWatcher, SIGNAL(finished()), this, SLOT(testDLFinished()));

//    QFuture<QString> future = QtConcurrent::run(dl.doDownload(QUrl(dl.doDownload(OTLinks::download).remove("\t")), QDir().tempPath() + "/OMSI-Tools_update.zip"));
//    fWatcher->setFuture(future);

    ui->btnSimulateCrash->setVisible(isNoRelease);
    ui->gbxDevTools->setVisible(isNoRelease);
    ui->actionSendFeedback->setVisible(isNoRelease);

    // Disable unused buttons
    ui->btnVehicles->setVisible(false);
    ui->btnSceneryobjects->setVisible(false);
    ui->btnMoneyTicketpacks->setVisible(false);
    ui->btnScripts->setVisible(false);
    ui->btnMaps->setVisible(false);
    ui->btnTextures->setVisible(false);
    ui->btnSplines->setVisible(false);
    ui->btnBackup->setVisible(false);
    ui->gbxFinishing->setVisible(false);

    if (!QFile("Fbh.unlock").exists())
        ui->gbxFbh->setVisible(false);

    adjustSize();

    ui->actionAbout->setText(tr("About %1").arg(OTName));

    ui->fraUpdate->setVisible(false);

    adjustSize();

    QVariant checkVersion = set.read("main", "autoUpdateCheck");
    QVariant lastAutoUpdateCheck = set.read("main", "lastAutoUpdateCheck").toString();
    bool checkForUpdate = false;

    QDate lastCheck;
    lastCheck.setDate(lastAutoUpdateCheck.toString().remove(4, 4).toInt(),
                      lastAutoUpdateCheck.toString().remove(0, 4).remove(2, 2).toInt(),
                      lastAutoUpdateCheck.toString().remove(0, 6).toInt());

            // On start
    if (checkVersion == 1)
        checkForUpdate = true;

            // If updates enabled, but there's no lastAutoUpdateCheck
    else if (!lastAutoUpdateCheck.isValid() && (checkVersion != 0))
        checkForUpdate = true;

            // Daily
    else if ((checkVersion == 2) &&
             (lastAutoUpdateCheck.toString() != misc.getDate("yyyyMMdd")))
        checkForUpdate = true;

            // Weekly
    else if ((checkVersion == 3) &&
             (QDate::currentDate().toString("yyyyMMdd") >= lastCheck.addDays(7).toString("yyyyMMdd")))
        checkForUpdate = true;

            // Monthly
    else if ((checkVersion == 4) &&
             (QDate::currentDate().toString("yyyyMMdd") >= lastCheck.addMonths(1).toString("yyyyMMdd")))
        checkForUpdate = true;

    if (checkForUpdate)
    {
        update = misc.getUpdateInformation();
        set.write("main", "lastAutoUpdateCheck", misc.getDate("yyyyMMdd"));

        if (update.at(0) == "noUpdates")
            ui->statusbar->showMessage(tr("No updates available."), 30000);
        else if (update.at(0) != "false")
        {
            ui->lblUpdate->setText(tr("New update available") + " •");
            ui->fraUpdate->setVisible(true);
            ui->lblUpdateVersion->setText(update.at(1));

            updateVersion = update.at(1);
        }
    }

    startCounterMsgSender();

    fadeInOutText *facts = new fadeInOutText(OTFacts);
    ui->vlaFacts->addWidget(facts);

    qInfo().noquote() << objectName() + " started";

    //QTimer::singleShot(1, this, SLOT(on_btnVerifyMap_clicked()));
}

void wStart::testDLFinished()
{
    qDebug() << "DOWNLOAD FINISHED.";
}

wStart::~wStart()
{
    qInfo().noquote() << objectName() << "is closing...";
    delete ui;
}

/// Opens the settings
void wStart::on_actionSettings_triggered()
{
    WSETTINGS = new wSettings(this);
    WSETTINGS->setWindowModality(Qt::ApplicationModal);
    WSETTINGS->show();
}

/// Shows the about message
void wStart::on_actionAbout_triggered()
{
    WABOUT = new wAbout(this);
    WABOUT->setWindowModality(Qt::ApplicationModal);
    WABOUT->show();
}

/// Closes the apllication
void wStart::on_actionClose_triggered()
{
    QApplication::quit();
}

/// Reopen test, coming soon... https://trello.com/c/8nOzgfSg
void wStart::reopenTest(QObject*)
{
    show();
    qDebug() << "Show!";
}

/// Opens the font module
void wStart::on_btnFonts_clicked()
{
    close();
    WFONTS = new wFonts();
    WFONTS->show();
}

/// Opens verify map module
void wStart::on_btnVerifyMap_clicked()
{
    close();
    WVERIFYMAP = new wVerifyMap();
    WVERIFYMAP->show();
}

/// Opens DevTools
void wStart::on_btnDevTools_clicked()
{
    close();
    WDEVTOOLS = new wDevTools();
    WDEVTOOLS->show();
}

/// Opens database panel for content search
void wStart::on_btnDBPanel_clicked()
{
    close();
    WDBPANEL = new wDBPanel();
    WDBPANEL->show();
}

/// Opens content search module
void wStart::on_btnContentSearch_clicked()
{
    close();
    WCONTENTSEARCH = new wContentSearch();
    WCONTENTSEARCH->show();
}

/// Starts crash simulation
void wStart::on_btnSimulateCrash_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Simulate crash"), tr("Should an appliaction crash be simulated?"));

    if (reply == QMessageBox::Yes)
        qFatal("Crash simulation started by user");
}

/// Opens bug report module
void wStart::on_actionSendFeedback_triggered()
{
    WFEEDBACK = new wFeedback(this);
    WFEEDBACK->setWindowModality(Qt::ApplicationModal);
    WFEEDBACK->show();
}

/// Opens the release notes
void wStart::on_actionReleaseNotes_triggered()
{
    WRELEASENOTES = new wReleaseNotes(this);
    WRELEASENOTES->setWindowModality(Qt::ApplicationModal);
    WRELEASENOTES->show();
}

/// Opens the manual, currenty only in german
void wStart::on_actionManual_triggered()
{
    QDesktopServices::openUrl(QUrl("file:///" + QApplication::applicationDirPath() + "/_docs/OMSI-Tools Lite - Handbuch DE.pdf"));
}

/// Opens Filebase-Helper
void wStart::on_btnOpenFbh_clicked()
{
    hide();
    WFBH = new wFbh();
    WFBH->show();
}

/// Creates shortcut for Filebase-Helper
void wStart::on_btnCreateShortcut_clicked()
{
    fop.createShortcut(qApp->applicationFilePath(), QDir().homePath() + QString("/Desktop/Filebase-Helper.lnk"), this);
    QMessageBox::information(this, "Verknüpfung erstellt", "Die gesetzte Verknüpfung muss jedoch noch modifiziert werden. Wie genau das funktioniert, kannst du im entsprechenden Thread im internen OWD-Forum sehen.");
}

/// Opens style test
void wStart::on_btnStyleTest_clicked()
{
    hide();
    WSTYLETEST = new wStyleTest();
    WSTYLETEST->show();
}

/// Opens wReleaseNotes to show latest update notes
void wStart::on_btnView_clicked()
{
    WRELEASENOTES = new wReleaseNotes(this, true, update.at(1));
    WRELEASENOTES->setWindowModality(Qt::ApplicationModal);
    WRELEASENOTES->show();
}

/// Opens link to github repository
void wStart::on_actionSourceCodeOnGitHub_triggered()
{
    QDesktopServices::openUrl(OTLinks::github);
}

/// Restarts application
void wStart::startCounterMsgSender()
{
    unsigned int count = set.read("main", "startCount").toInt();

    if ((count >= 5) && (set.read("main\\startCountMessages", "feedbackForm").toString() != "true"))
    {
        QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Feedback"), tr("You are welcome to give us feedback about %1 so we can improve our software.").arg(OTName), QMessageBox::Yes | QMessageBox::No);
        set.write("main\\startCountMessages", "feedbackForm", true);

        if (reply == QMessageBox::Yes)
            QDesktopServices::openUrl(OTLinks::survey);
    }
}

void wStart::on_actionRestart_triggered()
{
    misc.restart();
}

void wStart::on_btnCleanup_clicked()
{
    close();
    WCLEANUP = new wCleanup();
    WCLEANUP->show();
}

void wStart::on_actionCheckForUpdates_triggered()
{
    QStringList update = misc.getUpdateInformation();

    if (update.at(0) == "noUpdates")
        QMessageBox::information(this, tr("Finshed"), tr("No updates available."));
    else if (update.at(0) != "false")
    {
        WRELEASENOTES = new wReleaseNotes(this, true, update.at(1));
        WRELEASENOTES->setWindowModality(Qt::ApplicationModal);
        WRELEASENOTES->show();
    }
}

