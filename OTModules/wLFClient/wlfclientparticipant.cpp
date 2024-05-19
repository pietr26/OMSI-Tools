#include "wlfclientparticipant.h"
#include "ui_wlfclientparticipant.h"

wLFClientParticipant::wLFClientParticipant(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::wLFClientParticipant)
{
    qInfo().noquote() << "Starting " + objectName() + "...";
    qDebug() << "Set up UI...";
    ui->setupUi(this);
    //adjustSize();
    qDebug() << "UI set";

    setWindowTitle(OTInformation::name + " - " + tr("Control center trip") + " - " + tr("participant dashboard"));

    qInfo().noquote() << objectName() + " started";
}

wLFClientParticipant::~wLFClientParticipant()
{
    delete ui;
}

void wLFClientParticipant::on_actionBackToHome_triggered()
{
    close();
    backToHome();
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
