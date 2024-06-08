#include "wlfclientparticipant.h"
#include "ui_wlfclientparticipant.h"

#include <QMessageBox>

wLFClientParticipant::wLFClientParticipant(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::wLFClientParticipant),
    WLOGIN(new wLogin(this))
{
    qInfo().noquote() << "Starting " + objectName() + "...";
    qDebug() << "Set up UI...";
    ui->setupUi(this);
    //adjustSize();
    qDebug() << "UI set";

    setWindowTitle(OTInformation::name + " - " + tr("Control center trip") + " - " + tr("participant dashboard"));

    DiscordGameSDK::setModule(tr("Control center trip"));
    DiscordGameSDK::setStatus(tr("Driving"));
    DiscordGameSDK::start();
    DiscordGameSDK::setIcon("wlfclientparticipant");
    DiscordGameSDK::update();

    centralWidget()->setVisible(false);
    createDockWidgets();

    if (OTInformation::build == OTBuildOptions::Dev) ui->statusbar->addPermanentWidget(ui->lblRequests);
    else ui->lblRequests->setVisible(false);

    connect(api, &LFClientAPIInterface::requestRequested, this, &wLFClientParticipant::recieveRequestCount);

    connect(WLOGIN, &wLogin::accepted, this, &wLFClientParticipant::handleLogin);

    qInfo().noquote() << objectName() + " started";
}

wLFClientParticipant::~wLFClientParticipant()
{
    delete ui;
}

void wLFClientParticipant::on_actionBackToHome_triggered()
{
    close();
    emit backToHome();
}

void wLFClientParticipant::on_actionClose_triggered()
{
    QApplication::quit();
}

void wLFClientParticipant::on_actionPreferences_triggered()
{
    WPREFERENCES = new wPreferences();
    WPREFERENCES->show();
}

void wLFClientParticipant::on_actionHelp_triggered()
{
    wFeedback *WFEEDBACK = new wFeedback(this, OTLinks::Wiki::lfClient);
    WFEEDBACK->show();
}

void wLFClientParticipant::createDockWidgets()
{
    disconnect(WDGTRIP, nullptr, nullptr, nullptr);

    QDockWidget *dockInformation = new QDockWidget(tr("Information", "plural form"), this);
    dockInformation->setAllowedAreas(Qt::AllDockWidgetAreas);
    dockInformation->setFeatures(dockInformation->features() & ~QDockWidget::DockWidgetClosable);
    dockInformation->setWidget(WDGINFORMATION);
    addDockWidget(Qt::LeftDockWidgetArea, dockInformation);

    QDockWidget *dockActions = new QDockWidget(tr("Actions"), this);
    dockActions->setAllowedAreas(Qt::AllDockWidgetAreas);
    dockActions->setFeatures(dockActions->features() & ~QDockWidget::DockWidgetClosable);
    dockActions->setWidget(WDGACTIONS);
    addDockWidget(Qt::LeftDockWidgetArea, dockActions);

    QDockWidget *dockNotifications = new QDockWidget(tr("Notifications"), this);
    dockNotifications->setFeatures(dockNotifications->features() & ~QDockWidget::DockWidgetClosable);
    dockNotifications->setAllowedAreas(Qt::AllDockWidgetAreas);
    dockNotifications->setWidget(WDGNOTIFICATIONS);
    addDockWidget(Qt::TopDockWidgetArea, dockNotifications);

    QDockWidget *dockTrip = new QDockWidget(tr("Trip"), this);
    dockTrip->setFeatures(dockTrip->features() & ~QDockWidget::DockWidgetClosable);
    dockTrip->setAllowedAreas(Qt::AllDockWidgetAreas);
    dockTrip->setWidget(WDGTRIP);
    addDockWidget(Qt::RightDockWidgetArea, dockTrip);

    QDockWidget *dockTrips = new QDockWidget(tr("Trips"), this);
    dockTrips->setAllowedAreas(Qt::AllDockWidgetAreas);
    dockTrips->setFeatures(dockTrips->features() & ~QDockWidget::DockWidgetClosable);
    dockTrips->setWidget(WDGTRIPS);
    addDockWidget(Qt::RightDockWidgetArea, dockTrips);

    connect(WDGTRIPS, &wdgTrips::tripSelectionChanged, WDGTRIP, &wdgTrip::changeTrip);

    splitDockWidget(dockNotifications, dockTrip, Qt::Horizontal);
}

void wLFClientParticipant::recieveRequestCount()
{
    requestCount++;
    ui->lblRequests->setText("Requests: " + QString::number(requestCount));
}

void wLFClientParticipant::on_actionLogin_triggered()
{
    WLOGIN->show();
}

void wLFClientParticipant::handleLogin() {
    WLOGIN->setDisabled(true);
    const QString username = WLOGIN->username();
    const QString password = WLOGIN->password();
    bool ok = api->login(username, password);
    WLOGIN->setEnabled(true);
    if(ok) {
        WLOGIN->close();
        ui->actionLogin ->setVisible(false);
        ui->actionLogout->setVisible(true);
    }
    else {
        QString errorDescription;
        switch(api->errorType()) {
            case LFClientAPIInterface::LoginUserNotFound:  errorDescription = LFClientAPIInterface::userNotFoundDescription;  break;
            case LFClientAPIInterface::LoginWrongPassword: errorDescription = LFClientAPIInterface::wrongPasswordDescription; break;
            default: tr("unkown error"); break;
        }

        QMessageBox::warning(WLOGIN, tr("Login failed"), errorDescription, QMessageBox::Ok);
    }
}

void wLFClientParticipant::on_actionLogout_triggered() {
    QMessageBox::StandardButton msg = QMessageBox::warning(this, tr("Logout"), tr("Do you really want to log out?"), QMessageBox::Yes | QMessageBox::No);
    if(msg != QMessageBox::Yes)
        return;

    api->logout();
    ui->actionLogin ->setVisible(true);
    ui->actionLogout->setVisible(false);
}

