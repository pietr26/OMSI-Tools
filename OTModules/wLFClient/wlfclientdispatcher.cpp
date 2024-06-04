#include "wlfclientdispatcher.h"
#include "ui_wlfclientdispatcher.h"

#include <QMessageBox>

wLFClientDispatcher::wLFClientDispatcher(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::wLFClientDispatcher),
    WLOGIN(new wLogin(this))
{
    qInfo().noquote() << "Starting " + objectName() + "...";
    qDebug() << "Set up UI...";
    ui->setupUi(this);
    //adjustSize();
    qDebug() << "UI set";

    setWindowTitle(OTInformation::name + " - " + tr("Control center trip") + " - " + tr("dispatcher dashboard"));

    centralWidget()->setVisible(false);
    createDockWidgets();

    if (OTInformation::build == OTBuildOptions::Dev) ui->statusbar->addPermanentWidget(ui->lblRequests);
    else ui->lblRequests->setVisible(false);

    connect(api, &LFClientAPIInterface::requestRequested, this, &wLFClientDispatcher::recieveRequestCount);

    connect(WLOGIN, &wLogin::accepted, this, &wLFClientDispatcher::handleLogin);

    qInfo().noquote() << objectName() + " started";
}

wLFClientDispatcher::~wLFClientDispatcher()
{
    delete ui;
}

void wLFClientDispatcher::on_actionBackToHome_triggered()
{
    close();
    emit backToHome();
}

void wLFClientDispatcher::on_actionClose_triggered()
{
    QApplication::quit();
}

void wLFClientDispatcher::on_actionPreferences_triggered()
{
    WPREFERENCES = new wPreferences();
    WPREFERENCES->show();
}

void wLFClientDispatcher::on_actionHelp_triggered()
{
    wFeedback *WFEEDBACK = new wFeedback(this, OTLinks::Wiki::lfClient);
    WFEEDBACK->show();
}

void wLFClientDispatcher::createDockWidgets()
{
    QDockWidget *dock = new QDockWidget(tr("Information", "plural form"), this);
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    dock->setFeatures(dock->features() & ~QDockWidget::DockWidgetClosable);
    dock->setWidget(WDGINFORMATION);
    addDockWidget(Qt::LeftDockWidgetArea, dock);

    dock = new QDockWidget(tr("Users"), this);
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    dock->setFeatures(dock->features() & ~QDockWidget::DockWidgetClosable);
    dock->setWidget(WDGMANAGEUSERS);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    dock = new QDockWidget(tr("Manage trips"), this);
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    dock->setFeatures(dock->features() & ~QDockWidget::DockWidgetClosable);
    dock->setWidget(WDGMANAGETRIPS);
    addDockWidget(Qt::BottomDockWidgetArea, dock);
}

void wLFClientDispatcher::recieveRequestCount()
{
    requestCount++;
    ui->lblRequests->setText("Requests: " + QString::number(requestCount));
}

void wLFClientDispatcher::on_actionLogin_triggered()
{
    WLOGIN->show();
}

void wLFClientDispatcher::handleLogin() {
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

void wLFClientDispatcher::on_actionLogout_triggered() {
    QMessageBox::StandardButton msg = QMessageBox::warning(this, tr("Logout"), tr("Do you really want to log out?"), QMessageBox::Yes | QMessageBox::No);
    if(msg != QMessageBox::Yes)
        return;

    api->logout();
    ui->actionLogin ->setVisible(true);
    ui->actionLogout->setVisible(false);
}

