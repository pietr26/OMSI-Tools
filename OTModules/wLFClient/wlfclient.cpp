#include "wlfclient.h"
#include "ui_wlfclient.h"

wLFClient::wLFClient(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::wLFClient)
{
    qInfo().noquote() << "Starting " + objectName() + "...";
    qDebug() << "Set up UI...";
    ui->setupUi(this);
    //adjustSize();
    qDebug() << "UI set";

    setWindowTitle(OTInformation::name + " - " + tr("Control center trip"));

    qInfo().noquote() << objectName() + " started";
}

wLFClient::~wLFClient()
{
    delete ui;
}

void wLFClient::on_actionBackToHome_triggered()
{
    close();
    backToHome();
}

void wLFClient::on_actionClose_triggered()
{
    QApplication::quit();
}

void wLFClient::on_actionPreferences_triggered()
{
    WPREFERENCES = new wPreferences();
    WPREFERENCES->show();
}

void wLFClient::on_actionHelp_triggered()
{
    wFeedback *WFEEDBACK = new wFeedback(this, OTLinks::Wiki::lfClient);
    WFEEDBACK->show();
}
