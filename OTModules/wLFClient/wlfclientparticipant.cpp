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

    centralWidget()->setVisible(false);
    createDockWidgets();

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
    dock->setWidget(WDGINFORMATION);
    addDockWidget(Qt::LeftDockWidgetArea, dock);

    dock = new QDockWidget(tr("Actions"), this);
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    dock->setWidget(WDGACTIONS);
    addDockWidget(Qt::LeftDockWidgetArea, dock);

    dock = new QDockWidget(tr("Trips"), this);
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    dock->setWidget(WDGTRIPS);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    dock = new QDockWidget(tr("Notifications"), this);
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    dock->setWidget(WDGNOTIFICATIONS);
    addDockWidget(Qt::TopDockWidgetArea, dock);
}

void wLFClientParticipant::on_actionLogin_triggered()
{
    WLOGIN = new wLogin();
    WLOGIN->show();
}

