#include "wstart.h"
#include "ui_wstart.h"

wStart::wStart(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::wStart)
{
    qInfo().noquote() << "Starting " + moduleName + "...";

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

    ui->btnSimulateCrash->setVisible(isNoRelease);
    ui->gbxDevTools->setVisible(isNoRelease);
    ui->actionSendFeedback->setVisible(isNoRelease);

    if (!QFile("Fbh.unlock").exists())
        ui->gbxFbh->setVisible(false);

    adjustSize();

    ui->actionAbout->setText(tr("About %1").arg(OTName));

    QVariant checkVersion = set.read("main", "autoUpdateCheck");
    QVariant lastAutoUpdateCheck = set.read("main", "lastAutoUpdateCheck").toString();
    bool checkForUpdate = false;

    QDate lastCheck;
    lastCheck.setDate(lastAutoUpdateCheck.toString().remove(4, 4).toInt(),
                      lastAutoUpdateCheck.toString().remove(0, 4).remove(2, 2).toInt(),
                      lastAutoUpdateCheck.toString().remove(0, 6).toInt());

    if (!checkVersion.isValid())
    {
        checkForUpdate = true;
        set.write("main", "autoUpdateCheck", 2);
    }
    else if (checkVersion == 1)
        checkForUpdate = true;
    else if ((!lastAutoUpdateCheck.isValid()) && (checkVersion != 0))
        checkForUpdate = true;
    else if ((checkVersion == 2) && (lastAutoUpdateCheck.toString() != misc.getDate("yyyyMMdd")))
        checkForUpdate = true;
    else if (checkVersion == 3)
    {
        if (QDate::currentDate().toString("yyyyMMdd") >= lastCheck.addDays(7).toString("yyyyMMdd"))
            checkForUpdate = true;
    }
    else if (checkVersion == 4)
    {
        if (QDate::currentDate().toString("yyyyMMdd") >= lastCheck.addMonths(1).toString("yyyyMMdd"))
            checkForUpdate = true;
    }

    if (checkForUpdate)
    {
        QStringList update = misc.checkForUpdate();
        set.write("main", "lastAutoUpdateCheck", misc.getDate("yyyyMMdd"));

        if (update.at(0) == "noUpdates")
            ui->statusbar->showMessage(tr("No updates available."), 30000);
        else if (update.at(0) != "false")
        {
            WCHANGELOG = new wChangelog(this, true);
            WCHANGELOG->setWindowModality(Qt::ApplicationModal);
            WCHANGELOG->show();
        }
    }

    if (!set.read("main", "language").isValid())
    {
        WFIRSTSETUP = new wFirstSetup(this);
        WFIRSTSETUP->setWindowModality(Qt::ApplicationModal);
        WFIRSTSETUP->show();
    }

    qInfo().noquote() << moduleName + " started successfully.";

    //setWindowFlags(Qt::Window | Qt::WindowContextHelpButtonHint | Qt::WindowCloseButtonHint);
}

/*    // Set welcomeText:
 *
    int time = misc.getTime("hh").toInt();
    if (time == 0 || time > 18)
    ui->lblWelcome->setText(tr("Good evening!"));
    else if (time > 1 && time < 9)
    ui->lblWelcome->setText(tr("Good morning!"));
    else if (time > 10 && time < 17)
    ui->lblWelcome->setText(tr("Hello!"));
    else
    ui->lblWelcome->setText("");

    // picture animation
    QPixmap picture(":/rec/data/pictures/test.jpg");
    ui->lblPicture->setPixmap(picture);
    QGraphicsOpacityEffect *fadeIn = new QGraphicsOpacityEffect(ui->lblPicture);
    ui->lblPicture->setGraphicsEffect(fadeIn);

    QPropertyAnimation *animation = new QPropertyAnimation(fadeIn, "opacity");
    animation->setDuration(1500);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->setEasingCurve(QEasingCurve::InOutQuad);
    animation->start(QPropertyAnimation::DeleteWhenStopped);
*/

wStart::~wStart()
{
    qInfo().noquote() << moduleName << "is closing...";
    delete ui;
}

/// \brief Opens the settings
void wStart::on_actionSettings_triggered()
{
    WSETTINGS = new wSettings(this);
    WSETTINGS->setWindowModality(Qt::ApplicationModal);
    WSETTINGS->show();
}

/// \brief Shows the about message
void wStart::on_actionAbout_triggered()
{
    WABOUT = new wAbout(this);
    WABOUT->setWindowModality(Qt::ApplicationModal);
    WABOUT->show();
}

/// \brief Closes the apllication
void wStart::on_actionClose_triggered()
{
    QApplication::quit();
}

/// \brief Reopen test, coming soon... https://trello.com/c/8nOzgfSg
void wStart::reopenTest(QObject*)
{
    show();
    qDebug() << "Show!";
}

/// \brief Opens the font module
void wStart::on_btnFonts_clicked()
{
    hide();
    WFONTS = new wFonts();
    WFONTS->show();
}

/// \brief Shows "Where should I release my modification" message
void wStart::on_btnWhereShouldIReleaseMyModification_clicked()
{
    QMessageBox::information(this, tr("Where should I release my modification?", "Note #1"), tr("Coming soon!"));
}

/// \brief Opens verify map module
void wStart::on_btnVerifyMap_clicked()
{
    hide();
    WVERIFYMAP = new wVerifyMap();
    WVERIFYMAP->show();
}

/// \brief Opens DevTools
void wStart::on_btnDevTools_clicked()
{
    close();
    WDEVTOOLS = new wDevTools();
    WDEVTOOLS->show();
}

/// \brief Opens database panel for content search
void wStart::on_btnDBPanel_clicked()
{
    close();
    WDBPANEL = new wDBPanel();
    WDBPANEL->show();
}

/// \brief Opens content search module
void wStart::on_btnContentSearch_clicked()
{
    close();
    WCONTENTSEARCH = new wContentSearch();
    WCONTENTSEARCH->show();
}

/// \brief Starts crash simulation
void wStart::on_btnSimulateCrash_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Simulate crash"), tr("Should an appliaction crash be simulated?"));

    if (reply == QMessageBox::Yes)
        qFatal("Crash simulation started by user");
}

/// \brief Opens bug report module
void wStart::on_actionSendFeedback_triggered()
{
    OTMiscellaneous::sendFeedback();
}

/// \brief Opens the changelog
void wStart::on_actionChangelog_triggered()
{
    WCHANGELOG = new wChangelog(this);
    WCHANGELOG->setWindowModality(Qt::ApplicationModal);
    WCHANGELOG->show();
}

/// \brief Opens the manual, currenty only in german
void wStart::on_actionManual_triggered()
{
    QDesktopServices::openUrl(QUrl("file:///" + QApplication::applicationDirPath() + "/_docs/OMSI-Tools Lite - Handbuch DE.pdf"));
}

/// \brief Opens Filebase-Helper
void wStart::on_btnOpenFbh_clicked()
{
    hide();
    WFBH = new wFbh();
    WFBH->show();
}

/// \brief Creates shortcut for Filebase-Helper
void wStart::on_btnCreateShortcut_clicked()
{
    misc.createShortcut(qApp->applicationFilePath(), QDir().homePath() + QString("/Desktop/Filebase-Helper.lnk"), this);
    QMessageBox::information(this, "Info", "Die gesetzte Verknüpfung muss jedoch noch modifiziert werden. Wie genau das funktioniert, kannst du im entsprechenden Thread im internen OWD-Forum sehen.");
}

/// \brief Opens style test
void wStart::on_btnStyleTest_clicked()
{
    hide();
    WSTYLETEST = new wStyleTest();
    WSTYLETEST->show();
}

