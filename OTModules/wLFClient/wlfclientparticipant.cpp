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
    QDockWidget *dock = new QDockWidget(tr("Information", "plural form"), this);
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    dock->setFeatures(dock->features() & ~QDockWidget::DockWidgetClosable);
    dock->setWidget(WDGINFORMATION);
    addDockWidget(Qt::LeftDockWidgetArea, dock);

    dock = new QDockWidget(tr("Actions"), this);
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    dock->setFeatures(dock->features() & ~QDockWidget::DockWidgetClosable);
    dock->setWidget(WDGACTIONS);
    addDockWidget(Qt::LeftDockWidgetArea, dock);

    dock = new QDockWidget(tr("Trips"), this);
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    dock->setFeatures(dock->features() & ~QDockWidget::DockWidgetClosable);
    dock->setWidget(WDGTRIPS);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    dock = new QDockWidget(tr("Notifications"), this);
    dock->setFeatures(dock->features() & ~QDockWidget::DockWidgetClosable);
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    dock->setWidget(WDGNOTIFICATIONS);
    addDockWidget(Qt::TopDockWidgetArea, dock);
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

