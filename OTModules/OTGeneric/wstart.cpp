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

    setWindowTitle(OTInformation::name + " " + OTInformation::versions::currentVersion.first);

#ifdef QT_DEBUG
    setWindowTitle("[Debug Build] " + windowTitle());
#endif

    // Prepare GUI:
    bool isNoRelease;

    if (OTInformation::build == OTBuildOptions::Release || OTInformation::build == OTBuildOptions::EA ||
        OTInformation::build == OTBuildOptions::Lite || OTInformation::build == OTBuildOptions::Prerelease
            || OTInformation::build == OTBuildOptions::Beta)
        isNoRelease = false;
    else
        isNoRelease = true;

//    QFutureWatcher<QString> *fWatcher = new QFutureWatcher<QString>();

//    connect(fWatcher, SIGNAL(finished()), this, SLOT(testDLFinished()));

//    QFuture<QString> future = QtConcurrent::run(dl.doDownload(QUrl(dl.doDownload(OTLinks::download).remove("\t")), QDir().tempPath() + "/OMSI-Tools_update.zip"));
//    fWatcher->setFuture(future);

    ui->btnSimulateCrash->setVisible(isNoRelease);
    ui->gbxDevTools->setVisible(isNoRelease);
    ui->actionSendFeedback->setVisible(isNoRelease);

    if (!QFile("Fbh.unlock").exists())
        ui->gbxFbh->setVisible(false);

    adjustSize();

    ui->actionAbout->setText(tr("About %1").arg(OTInformation::name));

    startCounterMsgSender();

    fadeInOutText *facts = new fadeInOutText(OTStrings::getFunFacts());
    ui->vlaFacts->addWidget(facts);

    ui->dwgMessages->setWindowTitle(tr("News"));
    loadMessages();
    ui->dwgMessages->setVisible(set.read(objectName(), "messagesVisible").toBool());

    QVariant state = set.read(objectName(), "state");
    if (state.isValid())
        restoreState(state.toByteArray());

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

/// Loads the messages
void wStart::loadMessages()
{
    QStringList messages = QString(nc.post(OTLinks::inAppMessages)).split("\n");
    ui->dwgMessages->setWindowTitle(tr("News"));

    if (nc.lastSuccess == -1)
        ui->dwgMessages->setWindowTitle(tr("News - no internet connection"));
    else if (nc.lastSuccess == -2)
    {
        OTInAppMessage messageData;
        messageData.ID = "-1";
        messageData.publicity = 1;
        messageData.versions = QStringList("all");
        messageData.start = QDateTime::currentDateTime();
        messageData.end = QDateTime::currentDateTime().addYears(10);
        messageData.enTitle = "Maintenance work";
        messageData.enShortDescription = "The application server is currently undergoing maintenance";
        messageData.enDescription = "The application server is currently undergoing maintenance. During this time the updater, messages and the other functions with application server connection are not available. Please come back again later.";
        messageData.deTitle = "Wartungsarbeiten";
        messageData.deShortDescription = "Der Anwendungsserver befindet sich derzeit in Wartungsarbeiten";
        messageData.deDescription = "Der Anwendungsserver befindet sich derzeit in Wartungsarbeiten. Während dieser Zeit sind Aktualisierungen, Nachrichten und weitere Funktionen mit Verbindung zum Anwendungsserver nicht verfügbar. Bitte schaue später wieder vorbei.";

        message *widget = new message(messageData, this);
        QListWidgetItem *item = new QListWidgetItem();
        item->setSizeHint(widget->sizeHint());

        ui->lwgMessages->addItem(item);
        ui->lwgMessages->setItemWidget(item, widget);
    }
    else
    {
        for (int i = 0; i < messages.size(); i++)
        {
            if (messages.at(i) == "[news]")
            {
                OTInAppMessage messageData;
                i++; messageData.ID = messages.at(i);
                i++; messageData.publicity = QVariant(messages.at(i)).toInt();
                i++; messageData.start = QDateTime::fromString(messages.at(i), "yyyy-MM-dd HH:mm:ss");
                i++; messageData.end = QDateTime::fromString(messages.at(i), "yyyy-MM-dd HH:mm:ss");
                i++; messageData.slug = messages.at(i);
                i++; messageData.versions = messages.at(i).split("|");
                i++; messageData.enTitle = messages.at(i);
                i++; messageData.enShortDescription = messages.at(i);

                while (messages.at(i) != "[enDescrEnd]")
                {
                    i++; messageData.enDescription += messages.at(i);
                }
                messageData.enDescription.remove("[enDescrEnd]");

                QStringList parseEn = messageData.enDescription.split("\r");

                i++; messageData.deTitle = messages.at(i);
                i++; messageData.deShortDescription = messages.at(i);

                while (messages.at(i) != "[deDescrEnd]")
                {
                    i++; messageData.deDescription += messages.at(i);
                }
                messageData.deDescription.remove("[deDescrEnd]");

                i++; messageData.image = messages.at(i);

                i++; messageData.trashbin = QVariant(messages.at(i)).toInt();
                if (messageData.trashbin) continue;

                if ((OTInformation::build == OTBuildOptions::Dev) || (OTInformation::build == OTBuildOptions::Alpha) || (OTInformation::build == OTBuildOptions::Beta)) {
                    if ((messageData.publicity == 0) || (messageData.publicity == 3))
                        continue;
                }
                else {
                    if ((messageData.publicity == 0) || (messageData.publicity == 2) || (messageData.publicity == 3) || (messageData.publicity == 5))
                        continue;
                }



                /*
                 * 0 = deactivated
                 * 1 = in-App
                 * 2 = in-App (Beta only)
                 * 3 = Website
                 * 4 = in-App + Website
                 * 5 = in-App (Beta only) + Website
                */

                if (!(messageData.versions.contains("all") || messageData.versions.contains(OTInformation::versions::currentVersion.first)))
                    continue;

                if ((QDateTime::currentDateTime().secsTo(messageData.start) <= 0) && (QDateTime::currentDateTime().secsTo(messageData.end) >= 0))
                {
                    message *widget = new message(messageData, this);
                    QListWidgetItem *item = new QListWidgetItem();
                    item->setSizeHint(widget->sizeHint());

                    ui->lwgMessages->addItem(item);
                    ui->lwgMessages->setItemWidget(item, widget);
                }
            }
        }
    }
}

/// Reloads the message system
void wStart::on_btnReloadMessages_clicked()
{
    ui->lwgMessages->clear();
    loadMessages();
}

void wStart::on_lwgMessages_itemDoubleClicked(QListWidgetItem *item)
{
    message *msg = qobject_cast<message*>(ui->lwgMessages->itemWidget(item));
    msg->showDescription();
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
    close();
}

/// Reopen test, coming soon... https://trello.com/c/8nOzgfSg
void wStart::reopenTest(QObject*)
{
    show();
    qDebug() << "Show!";
}

bool wStart::checkMainDir()
{
checkMainDir:
    if (set.read("main", "mainDir").toString().isEmpty())
    {
        if (msg.setMainDir(this))
        {
            set.write("main", "mainDir", set.getOmsiPath(this));
            goto checkMainDir;
        }
        else
            return false;
    }

    return true;
}

/// Opens DevTools
void wStart::on_btnDevTools_clicked()
{
    WDEVTOOLS = new wDevTools();
    WDEVTOOLS->show();
    close();
}

/// Opens database panel for content search
void wStart::on_btnDBPanel_clicked()
{
    WDBPANEL = new wDBPanel();
    connect(WDBPANEL, &wDBPanel::backToHome, this, &wStart::reopen);
    WDBPANEL->show();
    close();
}

/// Starts crash simulation
void wStart::on_btnSimulateCrash_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Application crash simulation", "Press 'Yes' to start to simulate an applicaiton crash.");

    if (reply == QMessageBox::Yes)
        qFatal("Application crash simulation started by user");
}

/// Opens bug report module
void wStart::on_actionSendFeedback_triggered()
{
    WFEEDBACK = new wFeedback(this, OTLinks::wiki::general);
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
    QDesktopServices::openUrl(QUrl("file:///" + QApplication::applicationDirPath() + "/_docs/OMSI-Tools - Handbuch DE.pdf"));
}

/// Opens style test
void wStart::on_btnStyleTest_clicked()
{
    WSTYLETEST = new wStyleTest();
    connect(WSTYLETEST, &wStyleTest::backToHome, this, &wStart::reopen);
    WSTYLETEST->show();
    close();
}

/// Opens link to github repository
void wStart::on_actionSourceCodeOnGitHub_triggered()
{
    QDesktopServices::openUrl(OTLinks::github);
}

/// Shows a message at a defined start count
void wStart::startCounterMsgSender()
{
    //unsigned int count = set.read("main", "startCount").toInt();

//    if ((count >= 5) && (set.read("main\\startCountMessages", "feedbackForm").toString() != "true"))
//    {
//        QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Feedback"), tr("You are welcome to give us feedback about %1 so we can improve our software.").arg(OTInformation::name), QMessageBox::Yes | QMessageBox::No);
//        set.write("main\\startCountMessages", "feedbackForm", true);

//        if (reply == QMessageBox::Yes)
//            QDesktopServices::openUrl(OTLinks::survey);
//    }
}

/// Restarts application
void wStart::on_actionRestart_triggered()
{
    misc.restart();
}

/// Checks for updates
void wStart::on_actionCheckForUpdates_triggered()
{
    QStringList update = misc.getUpdateInformation();

    if (update.at(0) == "503")
        QMessageBox::information(this, tr("Maintenance"), OTStrings::serverMaintenance());
    else if (update.at(0) == "noUpdates")
        QMessageBox::information(this, tr("Finshed"), tr("No updates available."));
    else if (update.at(0) != "false")
    {
        WRELEASENOTES = new wReleaseNotes(this, true, update.at(1));
        WRELEASENOTES->setWindowModality(Qt::ApplicationModal);
        WRELEASENOTES->show();
    }
}

void wStart::on_actionShowHideMessageDock_triggered()
{
    ui->dwgMessages->setVisible(!ui->dwgMessages->isVisible());
    set.write(objectName(), "messagesVisible", ui->dwgMessages->isVisible());
    adjustSize();
    move(misc.centerPosition(this));
}

void wStart::on_dwgMessages_dockLocationChanged(const Qt::DockWidgetArea &area)
{
    Q_UNUSED(area);
    set.write(objectName(), "state", saveState());
}

void wStart::on_btnFbhDBPanel_clicked()
{
    WDBCOPYRIGHTS = new wDBCopyrights();
    connect(WDBCOPYRIGHTS, &wDBCopyrights::backToHome, this, &wStart::reopen);
    WDBCOPYRIGHTS->show();
    close();
}

void wStart::on_tbnFonts_clicked()
{
    WFONTS = new wFonts();
    connect(WFONTS, &wFonts::backToHome, this, &wStart::reopen);
    WFONTS->show();
    close();
}

void wStart::on_tbnMapVerifycation_clicked()
{
    if (!checkMainDir())
        return;

    WVERIFYMAP = new wVerifyMap();
    connect(WVERIFYMAP, &wVerifyMap::backToHome, this, &wStart::reopen);
    WVERIFYMAP->show();
    close();
}

void wStart::on_tbnContentSearch_clicked()
{
    WCONTENTSEARCH = new wContentSearch();
    connect(WCONTENTSEARCH, &wContentSearch::backToHome, this, &wStart::reopen);
    WCONTENTSEARCH->show();
    close();
}

void wStart::on_tbnCleanup_clicked()
{
    if (!checkMainDir())
        return;

    WCLEANUP = new wCleanup();
    connect(WCLEANUP, &wCleanup::backToHome, this, &wStart::reopen);
    WCLEANUP->show();
    close();
}

void wStart::on_tbnFbhOpen_clicked()
{
    WFBH = new wFbh();
    WFBH->show();
    close();
}

void wStart::on_tbnFbhCreateShortcut_clicked()
{
    fop.createShortcut(qApp->applicationFilePath(), QDir().homePath() + QString("/Desktop/Filebase-Helper.lnk"), this);
    QMessageBox::information(this, "Verknüpfung erstellt", "Die gesetzte Verknüpfung muss jedoch noch modifiziert werden. Wie genau das funktioniert, kannst du im entsprechenden Thread im internen OWD-Forum sehen.");
}
