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

    setWindowTitle(OTInformation::name + " " + OTInformation::versions::currentVersion.first);

#ifdef QT_DEBUG
    setWindowTitle("[Debug Build] " + windowTitle());
#endif

    // Prepare GUI:
    if (set.devModeEnabled())
    {
        QMenu *devTools = new QMenu("DevTools", this);
            QMenu *dbpanels = new QMenu("DBPanels", this);
            dbpanels->addAction(ui->actionDBPanelContentSearch);
            dbpanels->addAction(ui->actionDBPanelWDTFbh);
            dbpanels->addAction(ui->actionDBPanelKnownWords);

            devTools->addMenu(dbpanels);

        devTools->addAction(ui->actionDashboard);
        devTools->addAction(ui->actionApplicationCrashSimulation);
        devTools->addAction(ui->actionRegEditor);
        devTools->addAction(ui->actionBugDoc);

        ui->menubar->addMenu(devTools);
    }
    else
    {
        ui->tbnMaps->setVisible(false);
        ui->tbnLFClientParticipant->setVisible(false);
        ui->tbnLFClientDispatcher->setVisible(false);
        ui->gbxControlCenterTrip->setVisible(false);
    }

    ui->lblUpdateInfo->setVisible(false);
    ui->lblUpdateVersion->setVisible(false);
    ui->lblUpdate->setVisible(false);
    ui->statusbar->addPermanentWidget(ui->lblUpdateInfo);
    ui->statusbar->addPermanentWidget(ui->lblUpdateVersion);
    ui->statusbar->addPermanentWidget(ui->lblUpdate);

    adjustSize();

    if (QDate().currentDate().month() == 12)
        ui->lblSeasonIcon->setPixmap(QPixmap(":/rec/data/icons/christmas.svg"));

    ui->dwgMessages->setVisible(set.read(objectName(), "messagesVisible").toBool());
    move(misc.centerPosition(this));

    ui->actionAbout->setText(tr("About %1").arg(OTInformation::name));

    fadeInOutText *facts = new fadeInOutText(OTStrings::getFunFacts());
    ui->vlaFacts->addWidget(facts);

    ui->dwgMessages->setWindowTitle(tr("News"));
    loadMessages();
    ui->dwgMessages->setVisible(set.read(objectName(), "messagesVisible").toBool());

    checkForUpdates(); // and show frame if update is available

    // Restore state (for dockwidget)
    QVariant state = set.read(objectName(), "state");
    if (state.isValid())
        restoreState(state.toByteArray());

    qInfo().noquote() << objectName() + " started";
}

wStart::~wStart()
{
    qInfo().noquote() << objectName() << "is closing...";
    delete ui;
}

void wStart::checkForUpdates()
{
    updateInformation = updater->getUpdateInformation();

    if (updateInformation.first > 0)
    {
        ui->lblUpdateInfo->setVisible(true);
        ui->lblUpdateVersion->setVisible(true);
        ui->lblUpdate->setVisible(true);

        ui->lblUpdateVersion->setText(updateInformation.second);
    }
}

void wStart::loadMessages()
{
    QByteArray messageString = nc.post(OTLinks::inAppMessages);
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
        QJsonArray messages = QJsonDocument::fromJson(messageString).array();

        for (int i = 0; i < messages.size(); i++)
        {
            QJsonObject singleMessage = messages[i].toObject();

            OTInAppMessage messageData;
            messageData.ID = QString::number(singleMessage["ID"].toInt());

            messageData.publicity = singleMessage["publicity"].toInt();

            messageData.start = QDateTime::fromString(singleMessage["startdate"].toString(), "yyyy-MM-dd HH:mm:ss");
            messageData.end = QDateTime::fromString(singleMessage["enddate"].toString(), "yyyy-MM-dd HH:mm:ss");

            messageData.slug = singleMessage["slug"].toString();

            messageData.versions = singleMessage["versions"].toString().split("|");

            messageData.isImportant = singleMessage["is_important"].toBool();
            messageData.isVirutal = singleMessage["is_virtual"].toBool();

            messageData.enTitle = singleMessage["enTitle"].toString();
            messageData.enShortDescription = singleMessage["enShortDescription"].toString();
            messageData.enDescription = singleMessage["enDescription"].toString();

            messageData.deTitle = singleMessage["deTitle"].toString();
            messageData.deShortDescription = singleMessage["deShortDescription"].toString();
            messageData.deDescription = singleMessage["deDescription"].toString();

            messageData.image = singleMessage["image"].toString();

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

                if (messageData.isImportant && !set.read("main\\messages", messageData.ID).toBool()) on_lwgMessages_itemDoubleClicked(item);

                if (messageData.isVirutal) item->setHidden(true);
            }
        }
    }
}

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

void wStart::on_actionPreferences_triggered()
{
    WPREFERENCES = new wPreferences();
    WPREFERENCES->setWindowModality(Qt::ApplicationModal);
    WPREFERENCES->show();
}

void wStart::on_actionAbout_triggered()
{
    WABOUT = new wAbout(this);
    WABOUT->setWindowModality(Qt::ApplicationModal);
    WABOUT->show();
}

void wStart::on_actionClose_triggered()
{
    close();
}

bool wStart::checkMainDir() // TODO: do better. :P
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

void wStart::on_actionSendFeedback_triggered()
{
    WFEEDBACK = new wFeedback(this, OTLinks::Wiki::general);
    WFEEDBACK->setWindowModality(Qt::ApplicationModal);
    WFEEDBACK->show();
}

void wStart::on_actionReleaseNotes_triggered()
{
    WRELEASENOTES = new wReleaseNotes(this);
    WRELEASENOTES->setWindowModality(Qt::ApplicationModal);
    WRELEASENOTES->show();
}

void wStart::on_actionManual_triggered()
{
    QDesktopServices::openUrl(QUrl("file:///" + QApplication::applicationDirPath() + "/_docs"));
}

void wStart::on_actionSourceCodeOnGitHub_triggered()
{
    QDesktopServices::openUrl(OTLinks::GitHub::main);
}

void wStart::on_actionRestart_triggered()
{
    misc.restart();
}

void wStart::on_actionCheckForUpdates_triggered()
{
    on_actionPreferences_triggered();
    qApp->processEvents();
    WPREFERENCES->on_btnCheckForUpdates_clicked();
}

void wStart::on_tbnFonts_clicked()
{
    WFONTS = new wFonts();
    connect(WFONTS, &wFonts::backToHome, this, &wStart::reopen);
    WFONTS->show();
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

void wStart::on_tbnMaps_clicked()
{
    if (!checkMainDir())
        return;

    WMAPS = new wMaps();
    connect(WMAPS, &wMaps::backToHome, this, &wStart::reopen);
    WMAPS->show();
    close();
}

void wStart::on_tbnMapVerification_clicked()
{
    if (!checkMainDir())
        return;

    WVERIFYMAP = new wVerifyMap();
    connect(WVERIFYMAP, &wVerifyMap::backToHome, this, &wStart::reopen);
    WVERIFYMAP->show();
    close();
}

void wStart::on_actionDashboard_triggered()
{
    WDEVTOOLS = new wDevTools();
    connect(WDEVTOOLS, &wDevTools::backToHome, this, &wStart::reopen);
    WDEVTOOLS->show();
    close();
}

void wStart::on_actionApplicationCrashSimulation_triggered()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Application crash simulation", "Press 'Yes' to start to simulate an applicaiton crash.");

    if (reply == QMessageBox::Yes) qFatal("Application crash simulation started by user");
}

void wStart::on_actionRegEditor_triggered()
{
    WREGEDITOR = new wRegEditor();
    connect(WREGEDITOR, &wRegEditor::backToHome, this, &wStart::reopen);
    WREGEDITOR->show();
    close();
}

void wStart::on_actionDBPanelContentSearch_triggered()
{
    WDBPANEL = new wDBPanel();
    connect(WDBPANEL, &wDBPanel::backToHome, this, &wStart::reopen);
    WDBPANEL->show();
    close();
}

void wStart::on_actionBugDoc_triggered()
{
    WBUGDOC = new wBugDoc();
    connect(WBUGDOC, &wBugDoc::backToHome, this, &wStart::reopen);
    WBUGDOC->show();
    close();
}

void wStart::on_actionDBPanelWDTFbh_triggered()
{
    WDBCOPYRIGHTS = new wDBCopyrights();
    connect(WDBCOPYRIGHTS, &wDBCopyrights::backToHome, this, &wStart::reopen);
    WDBCOPYRIGHTS->show();
    close();
}

void wStart::on_actionDBPanelKnownWords_triggered()
{
    WDBKNOWNWORDS = new wDBKnownWords();
    connect(WDBKNOWNWORDS, &wDBKnownWords::backToHome, this, &wStart::reopen);
    WDBKNOWNWORDS->show();
    close();
}

void wStart::on_actionDirectXTex_triggered()
{
    QMessageBox::about(this, tr("DirectXTex License"), "MIT License\n\nCopyright (c) Microsoft Corporation.\n\nPermission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the \"Software\"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sellcopies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in allcopies or substantial portions of the Software.\n\nTHE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THEAUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE");
}

void wStart::on_actionRoadmap_triggered() { QDesktopServices::openUrl(OTLinks::roadmap); }

void wStart::on_actionAboutQt_triggered() { QMessageBox::aboutQt(this); }

void wStart::on_tbnLFClientDispatcher_clicked()
{
    WLFCLIENTDISPATCHER = new wLFClientDispatcher();
    connect(WLFCLIENTDISPATCHER, &wLFClientDispatcher::backToHome, this, &wStart::reopen);
    WLFCLIENTDISPATCHER->show();
    close();
}

void wStart::on_tbnLFClientParticipant_clicked()
{
    WLFCLIENTPARTICIPANT = new wLFClientParticipant();
    connect(WLFCLIENTPARTICIPANT, &wLFClientParticipant::backToHome, this, &wStart::reopen);
    WLFCLIENTPARTICIPANT->show();
    close();
}

void wStart::on_lblUpdate_linkActivated(const QString &link)
{
    Q_UNUSED(link);
    WRELEASENOTES = new wReleaseNotes(this, true, updateInformation.second, (updateInformation.first == 2) ? true : false);
    WRELEASENOTES->setWindowModality(Qt::ApplicationModal);
    WRELEASENOTES->show();
}
